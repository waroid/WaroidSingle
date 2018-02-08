/*
 * GlobalData.h
 *
 *  Created on: Oct 16, 2017
 *      Author: mirime
 */

#ifndef GLOBADATA_H_
#define GLOBADATA_H_

#include "core/GRCBoolean.h"
#include "Defines.h"
#include "RobotData.h"

class GlobalData
{
public:
	static const char* GetRobotName();
	static int GetRobotType();
	static unsigned char GetMovePower(WAROIDDIRECTION::ETYPE dir, WAROIDSPEED::ETYPE speed);
	static bool IsRepeatWeapon();
	static const char* GetWeaponSoundFilename();

	static GRCBoolean& Login()
	{
		return s_login;
	}
	static float GetYaw()
	{
		return s_yaw;
	}
	static int GetBattery()
	{
		return s_battery;
	}

	static int GetCameraFps(unsigned char quality);
	static int GetCameraBitRate(unsigned char quality);

	static void UpdateOwner(const RobotData::DATA* robotData);
	static void UpdateYaw(char hi, char low);
	static void UpdateBattery(char hi, char low);

private:
	static const RobotData::DATA* s_owner;
	static GRCString s_weaponSoundFilename;
	static GRCBoolean s_login;
	static float s_yaw;
	static int s_battery;
};

#endif /* GLOBADATA_H_ */
