/*
 * UserSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "UserSession.h"

#include <cstdio>
#include <cstdlib>

#include "common/GRCSoundWorker.h"
#include "communication/GRCCommunicator.h"
#include "communication/GRCSerialOpener.h"
#include "communication/GRCSockAddr.h"
#include "core/GRCCoreUtil.h"
#include "core/GRCObject.h"
#include "ControlBoardSession.h"
#include "Defines.h"
#include "GlobalData.h"
#include "Manager.h"
#include "UserSessionDefines.h"

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(HEARTBEAT_2)
{
	recvHeartbeat(rpacket->serverTick);

	WAROIDUSERROBOT::HEARTBEAT_3 spacket;
	spacket.clientTick = rpacket->clientTick;
	sendPacket(&spacket);
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_LOGIN)
{
	auto eclose = [this](const char* reason)
	{
		close(reason);
	};

	GRC_CHECK_FUNC_RETURN(GlobalData::Login().update(true), eclose("already logined."));

	GRCSoundWorker::playTts("get on %s", GlobalData::GetRobotName());
	Manager::GetControlBoardOpener().getFirstOpenedSession()->blinkLed(0.2, 0.2, 5);
	pthread_create(&m_sendInfoThread, nullptr, sendInfoWorker, this);

	WAROIDUSERROBOT::U_R_LOGIN_ACK spacket;
	spacket.perror = WAROIDUSERROBOT::PERROR::SUCCESS;
	spacket.type = GlobalData::GetRobotType();
	sendPacket(&spacket);
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_CAMERA)
{
	auto eclose = [this](const char* reason)
	{
		close(reason);
	};

	GRC_CHECK_FUNC_RETURN(GlobalData::Login(), eclose("not login."));

	system("killall nc");
	system("killall raspivid");
	if (rpacket->onoff == WAROIDONOFF::ON)
	{
		int fps = GlobalData::GetCameraFps(rpacket->quality);
		int bitRate = GlobalData::GetCameraBitRate(rpacket->quality);

		char command[256] = { 0 };
		sprintf(command, "raspivid -o - -t 0 -w 1280 -h 720 -fps %d -b %d -vf -n | nc %s %d &", fps, bitRate, m_remoteSockAddr.getIp(), CAMERA_USER_PORT);
#ifdef __RPI__
		system(command);
#endif
		GRC_INFO("opened camera. system=%s", command);
	}
	else
	{
		GRC_INFO("closed camera");
	}

	GRCSoundWorker::playTts("%s camera", rpacket->onoff == WAROIDONOFF::ON ? "open" : "close");
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_MOVE)
{
	auto eclose = [this](const char* reason)
	{
		close(reason);
	};

	GRC_CHECK_FUNC_RETURN(GlobalData::Login(), eclose("not login."));

	GRC_CHECK_RETURN(IsValidWaroidDirection(rpacket->direction));
	GRC_CHECK_RETURN(IsValidWaroidSpeed(rpacket->speed));

	//send serial
	Manager::GetControlBoardOpener().getFirstOpenedSession()->sendMove(rpacket->direction, rpacket->speed);

	GRC_DEV("[%s]move. dir=%d speed=%d", getObjName(), rpacket->direction, rpacket->speed);
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_FIRE)
{
	auto eclose = [this](const char* reason)
	{
		close(reason);
	};

	GRC_CHECK_FUNC_RETURN(GlobalData::Login(), eclose("not login."));

	if (rpacket->onoff == WAROIDONOFF::ON)
	{
		Manager::GetControlBoardOpener().getFirstOpenedSession()->sendFire(true);
		if (GlobalData::IsRepeatWeapon() == false)
		{
			GRCCoreUtil::sleep(0.1);
			Manager::GetControlBoardOpener().getFirstOpenedSession()->sendFire(false);
		}

		GRCSoundWorker::startPlay(GlobalData::GetWeaponSoundFilename());
	}
	else
	{
		Manager::GetControlBoardOpener().getFirstOpenedSession()->sendFire(false);

		if (GlobalData::IsRepeatWeapon())
		{
			GRCSoundWorker::endPlay(GlobalData::GetWeaponSoundFilename());
		}
	}

	GRC_DEV("[%s]fire. on=%d", getObjName(), rpacket->onoff);
}

UserSession::UserSession(size_t maxPacketSize)
		: GRCAcceptSession(maxPacketSize)
{
	// TODO Auto-generated constructor stub

}

UserSession::~UserSession()
{
	// TODO Auto-generated destructor stub
}

void UserSession::onOpen()
{
	GRCAcceptSession::onOpen();

	//todo
}

void UserSession::onClose()
{
	system("killall nc");
#ifdef __RPI__
	system("killall raspivid");
#endif

	//stop all
	auto* controlBoardSession = Manager::GetControlBoardOpener().getFirstOpenedSession();
	if (controlBoardSession) controlBoardSession->sendStopAll();

	if (GlobalData::Login().update(false))
	{
		GRCSoundWorker::playTts("alight from %s", GlobalData::GetRobotName());
	}

	GRCAcceptSession::onClose();
}

int UserSession::onParsing(const char* data, int size)
{
	GRC_CHECK_RETMINUS(data);

	if (size < WAROIDUSERROBOT::HEADER::GetHeaderSize()) return 0;

	const WAROIDUSERROBOT::HEADER* header = reinterpret_cast<const WAROIDUSERROBOT::HEADER*>(data);
	int packetSize = header->GetPacketSize();
	if (size < packetSize) return 0;

	GRC_CHECKV_RETMINUS(packetSize<=WAROID_USER_ROBOT_MAX_PACKET_SIZE, "over packet size. packetsize=%d", packetSize);

	return packetSize;
}

void UserSession::onPacket(const char* packet, int size)
{
	const WAROIDUSERROBOT::HEADER* urh = reinterpret_cast<const WAROIDUSERROBOT::HEADER*>(packet);

	switch (urh->GetCommand())
	{
		WAROID_USER_SESSION_COMMAND_CASE_LOG(3, HEARTBEAT_2, urh)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_LOGIN, urh)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_CAMERA, urh)
		WAROID_USER_SESSION_COMMAND_CASE_LOG(3, U_R_MOVE, urh)
		WAROID_USER_SESSION_COMMAND_CASE_LOG(3, U_R_FIRE, urh)
		default:
			GRC_ERR("invalid packet. cmd=WAROIDUSERROBOT::%d", urh->GetCommand());
			break;
	}
}

void UserSession::sendPacket(const WAROIDUSERROBOT::HEADER* header)
{
	send(header, header->GetPacketSize());
}

void UserSession::onSendingInfo()
{
	WAROIDUSERROBOT::R_U_INFO spacket;
	float yaw = spacket.yaw = 0.0f;
	int battery = spacket.battery = 0;

	while (GlobalData::Login())
	{
		yaw = GlobalData::GetYaw();
		battery = GlobalData::GetBattery();

		if (yaw != spacket.yaw || battery != spacket.battery)
		{
			spacket.yaw = yaw;
			spacket.battery = battery;
			sendPacket(&spacket);

			GRC_INFO_COUNT(3, "[%s]sending info. yaw=%f battery=%d", getObjName(), yaw, battery);
		}

		GRCCoreUtil::sleep(0.1);
	}
}

void* UserSession::sendInfoWorker(void* param)
{
	UserSession* session = (UserSession*)param;

	GRC_INFO("[%s]start send info thread(0x%x)", session->getObjName(), pthread_self());
	session->onSendingInfo();
	GRC_INFO("[%s]stop send info thread(0x%x)", session->getObjName(), pthread_self());

	return nullptr;
}
