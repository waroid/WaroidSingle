/*
 * Defines.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: mirime
 */

#include "Defines.h"

#define WAROIDDIRECTIONELSEIFRETURN(e,n)	else if (strcasecmp(#e, n) == 0) return WAROIDDIRECTION::e;
WAROIDDIRECTION GetWaroidDirection(GRCCSTR name)
{
	if (name == nullptr)
		return WAROIDDIRECTION::NONE;

	WAROIDDIRECTIONELSEIFRETURN(FORWARD, name)
	WAROIDDIRECTIONELSEIFRETURN(BACKWARD, name)
	WAROIDDIRECTIONELSEIFRETURN(LEFT, name)
	WAROIDDIRECTIONELSEIFRETURN(RIGHT, name)
	WAROIDDIRECTIONELSEIFRETURN(LEFT_FORWARD, name)
	WAROIDDIRECTIONELSEIFRETURN(RIGHT_FORWARD, name)
	WAROIDDIRECTIONELSEIFRETURN(LEFT_BACKWARD, name)
	WAROIDDIRECTIONELSEIFRETURN(RIGHT_BACKWARD, name)
	WAROIDDIRECTIONELSEIFRETURN(LEFT_TURN, name)
	WAROIDDIRECTIONELSEIFRETURN(RIGHT_TURN, name)

	return WAROIDDIRECTION::NONE;
}
bool IsValidWaroidDirection(WAROIDDIRECTION direction)
{
	int dir = (int)direction;
	return dir >= (int)WAROIDDIRECTION::NONE && dir < (int)WAROIDDIRECTION::TOTAL;
}

bool IsValidWaroidSpeed(WAROIDSPEED speed)
{
	int s = (int)speed;
	return s >= (int)WAROIDSPEED::NONE && s < (int)WAROIDSPEED::TOTAL;
}

#define WAROIDWEAPONELSEIFRETURN(e,n)	else if (strcasecmp(#e, n) == 0) return WAROIDWEAPON::e;
WAROIDWEAPON GetWaroidWeapon(GRCCSTR name)
{
	if (name == nullptr)
		return WAROIDWEAPON::UNKNOWN;

	WAROIDWEAPONELSEIFRETURN(UNKNOWN, name)
	WAROIDWEAPONELSEIFRETURN(GATLING, name)
	WAROIDWEAPONELSEIFRETURN(CANNON, name)
	WAROIDWEAPONELSEIFRETURN(LASER, name)

	return WAROIDWEAPON::UNKNOWN;
}
