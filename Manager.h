/*
 * Manager.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include "communication/GRCSerialOpener.h"
#include "communication/GRCTcpConnector.h"
#include "communication/GRCTcpListener.h"
#include "ControlBoardSession.h"
#include "GlobalData.h"
#include "RobotData.h"
#include "UserSession.h"

class Manager
{
public:
	static const RobotData& GetRobotData()
	{
		return s_robotData;
	}

	static GRCTcpListenerT<UserSession>& GetUserListener()
	{
		return s_userListener;
	}

	static GRCSerialOpenerT<ControlBoardSession>& GetControlBoardOpener()
	{
		return s_controlBoardOpener;
	}

	static bool start(const char* robotTypeName);
	static void stop();

private:
	static GRCMutex s_mutex;
	static RobotData s_robotData;
	static GRCTcpListenerT<UserSession> s_userListener;
	static GRCSerialOpenerT<ControlBoardSession> s_controlBoardOpener;
};

#endif /* MANAGER_H_ */
