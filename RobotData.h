/*
 * RobotData.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef ROBOTDATA_H_
#define ROBOTDATA_H_

#include <cstring>
#include <map>

#include "core/GRCJsonData.h"
#include "core/GRCString.h"
#include "Defines.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

class RobotData: public GRCJsonData
{
public:
	struct DATA
	{
		GRCString name;
		int type = 0;
		unsigned char movepowers[(int)WAROIDDIRECTION::TOTAL];
		WAROIDWEAPON weapon = WAROIDWEAPON::UNKNOWN;
		GRCString weaponSoundFilename;

		DATA()
		{
			bzero(movepowers, sizeof(movepowers));
		}

		bool isValid() const
		{
			return name.isEmpty() == false && type >= 0 && weapon != WAROIDWEAPON::UNKNOWN;
		}
	};
	using MapData = std::map<int, DATA*>;

public:
	RobotData();
	virtual ~RobotData();

public:
	bool load();

	const DATA* find(int id) const;
	const DATA* find(GRCCSTR name) const;

protected:
	virtual bool onLoad(const RAPIDJSON_NAMESPACE::Value& data) override;

private:
	bool setMovePowers(DATA* data, const RAPIDJSON_NAMESPACE::Value& value);

private:
	MapData m_datas;
};

#endif /* ROBOTDATA_H_ */
