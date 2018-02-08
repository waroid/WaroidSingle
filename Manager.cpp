/*
 * Manager.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "Manager.h"

#include <unistd.h>

#include "common/GRCSoundWorker.h"
#include "communication/GRCCommunicator.h"
#include "core/GRCCore.h"
#include "core/GRCCoreUtil.h"
#include "core/GRCMutex.h"
#include "Defines.h"
#include "UserSessionDefines.h"
#include "ControlBoardSessionDefines.h"

GRCMutex Manager::s_mutex;
RobotData Manager::s_robotData;
GRCTcpListenerT<UserSession> Manager::s_userListener("UserListener", 1, WAROID_USER_ROBOT_MAX_PACKET_SIZE);
GRCSerialOpenerT<ControlBoardSession> Manager::s_controlBoardOpener("ControlBoardOpener", 1, sizeof(WAROIDCONTROLBOARD::PACKET));

bool Manager::start(const char* robotTypeName)
{
	GRC_CHECK_RETFALSE(GRCSoundWorker::add(BOOT_SOUND_FILENAME, false, 1));
	GRCSoundWorker::startPlay(BOOT_SOUND_FILENAME);
	while (GRCSoundWorker::isPlaying())
	{
		GRCCoreUtil::sleep(0.3);
	}

	GRC_CHECK_RETFALSE(s_robotData.load());

	const RobotData::DATA* data = s_robotData.find(robotTypeName);
	GRC_CHECK_RETFALSE(data);
	//GRC_CHECK_RETFALSE(GRCSoundWorker::add(m_robotData->attackedsoundfilename, false, 1));
	GRCSoundWorker::playTts("I'm %s.", *data->name);

	GRC_CHECK_RETFALSE(s_controlBoardOpener.open(CONTROL_BOARD_DEVICE, CONTROL_BOARD_BAUD));
	GRC_CHECK_RETFALSE(s_userListener.listen(USER_ROBOT_PORT));

	GlobalData::UpdateOwner(data);

	s_mutex.wait();

	return true;
}

void Manager::stop()
{
	s_mutex.signal();

	s_userListener.stop();
	s_controlBoardOpener.stop();
#ifdef __RPI__
#else
	system("killall socat");
#endif
}
