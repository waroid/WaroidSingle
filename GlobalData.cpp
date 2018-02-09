/*
 * GlobalData.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: mirime
 */

#include "GlobalData.h"

#include "common/GRCSoundWorker.h"
#include "core/GRCCore.h"
#include "core/GRCMath.h"
#include "core/GRCString.h"

namespace GLOBL_DATA
{
	const int maxQuality = 3;
	const int fpses[maxQuality] = { 25, 25, 25 };
	const int bitRates[maxQuality] = { 15000000, 10000000, 5000000 };
}
using namespace GLOBL_DATA;

const RobotData::DATA* GlobalData::s_owner = nullptr;
GRCString s_weaponSoundFilename;
GRCBoolean GlobalData::s_login(false);
float GlobalData::s_yaw = 0;
int GlobalData::s_battery = 0;

const char* GlobalData::GetRobotName()
{
	return s_owner->name;
}

int GlobalData::GetRobotType()
{
	return s_owner->type;
}

unsigned char GlobalData::GetMovePower(WAROIDDIRECTION direction, WAROIDSPEED speed)
{
	switch (speed)
	{
		case WAROIDSPEED::SLOW:
			return GRCMath::Clamp<unsigned char>(s_owner->movepowers[(int)direction] / 2, MIN_MOVE_POWER, MAX_MOVE_POWER);
		case WAROIDSPEED::DEFAULT:
			return GRCMath::Clamp<unsigned char>(s_owner->movepowers[(int)direction], MIN_MOVE_POWER, MAX_MOVE_POWER);
		case WAROIDSPEED::FAST:
			return GRCMath::Clamp<unsigned char>(s_owner->movepowers[(int)direction] * 2, MIN_MOVE_POWER, MAX_MOVE_POWER);
		default:
			return 0;
	}
}

bool GlobalData::IsRepeatWeapon()
{
	return s_owner->weapon == WAROIDWEAPON::GATLING || s_owner->weapon == WAROIDWEAPON::LASER;
}

const char* GlobalData::GetWeaponSoundFilename()
{
	return s_owner->weaponSoundFilename;
}

int GlobalData::GetCameraFps(int quality)
{
	if (quality >= maxQuality) return fpses[0];

	return fpses[quality];
}

int GlobalData::GetCameraBitRate(int quality)
{
	if (quality >= maxQuality) return bitRates[0];

	return bitRates[quality];
}

void GlobalData::UpdateOwner(const RobotData::DATA* robotData)
{
	s_owner = robotData;
}

void GlobalData::UpdateYaw(char hi, char low)
{
	s_yaw = (static_cast<float>(hi) * 256 + low) * 0.1f;
}

void GlobalData::UpdateBattery(char hi, char low)
{
	s_battery = static_cast<int>(hi) * 256 + low;
}
