/*
 * UserSessionDefines.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef USERSESSIONEFINES_H_
#define USERSESSIONEFINES_H_

#include "Defines.h"

//////////
//
#define WAROID_USER_ROBOT_MAX_PACKET_SIZE	50

namespace WAROIDUSERROBOT
{
	////////////////////
	//	error
	namespace PERROR
	{
		enum ETYPE
		{
			UNKNOWN = 0,
			//-----
			SUCCESS,
			EXISTS_OWNER,
			//-----
			TOTAL
		};
	}

	namespace COMMAND
	{
		enum ETYPE
		{
			INVALID = -1,
			//-----
			HEARTBEAT_1 = 1,
			HEARTBEAT_2,
			HEARTBEAT_3,

			U_R_LOGIN,
			U_R_LOGIN_ACK,
			U_R_CAMERA,
			U_R_MOVE,
			U_R_FIRE,

			R_U_INFO,
		};
	}
}

#pragma pack(1)
namespace WAROIDUSERROBOT
{
	////////////////////
	//	packet header
	class HEADER
	{
	public:
		HEADER(COMMAND::ETYPE command, int packetSize)
				: m_packetSize(static_cast<unsigned short>(packetSize)), m_command(static_cast<char>(command))
		{
		}

	public:
		COMMAND::ETYPE GetCommand() const
		{
			return static_cast<COMMAND::ETYPE>(m_command);
		}

		int GetPacketSize() const
		{
			return m_packetSize;
		}

	private:
		unsigned short m_packetSize;
		char m_command;

	public:
		static int GetHeaderSize()
		{
			return sizeof(HEADER);
		}
	};

#define WAROID_USER_ROBOT_PACKET_STRUCT_START(cmd)	struct cmd: public HEADER{cmd():HEADER(COMMAND::cmd, sizeof(*this)){}
#define WAROID_USER_ROBOT_PACKET_STRUCT_END			}

	////////////////////
	//	packet
	WAROID_USER_ROBOT_PACKET_STRUCT_START (HEARTBEAT_1)
		unsigned int serverTick = 0;
	WAROID_USER_ROBOT_PACKET_STRUCT_END;

	WAROID_USER_ROBOT_PACKET_STRUCT_START (HEARTBEAT_2)
		unsigned int serverTick = 0;
		unsigned int clientTick = 0;
	WAROID_USER_ROBOT_PACKET_STRUCT_END;

	WAROID_USER_ROBOT_PACKET_STRUCT_START (HEARTBEAT_3)
		unsigned int clientTick = 0;
	WAROID_USER_ROBOT_PACKET_STRUCT_END;

	WAROID_USER_ROBOT_PACKET_STRUCT_START (U_R_LOGIN)
	WAROID_USER_ROBOT_PACKET_STRUCT_END;

	WAROID_USER_ROBOT_PACKET_STRUCT_START (U_R_LOGIN_ACK)
		PERROR::ETYPE perror = PERROR::UNKNOWN;
		int type = 0;
	WAROID_USER_ROBOT_PACKET_STRUCT_END;

	WAROID_USER_ROBOT_PACKET_STRUCT_START (U_R_CAMERA)
		unsigned char on = 0;
		unsigned char quality = 0;
	WAROID_USER_ROBOT_PACKET_STRUCT_END;

	WAROID_USER_ROBOT_PACKET_STRUCT_START (U_R_MOVE)
		WAROIDDIRECTION::ETYPE dir = WAROIDDIRECTION::NONE;
		WAROIDSPEED::ETYPE speed = WAROIDSPEED::NONE;
	WAROID_USER_ROBOT_PACKET_STRUCT_END;

	WAROID_USER_ROBOT_PACKET_STRUCT_START (U_R_FIRE)
		unsigned char on = 0;
	WAROID_USER_ROBOT_PACKET_STRUCT_END;

	WAROID_USER_ROBOT_PACKET_STRUCT_START (R_U_INFO)
		float yaw = 0.0f;
		int battery = 0;
	WAROID_USER_ROBOT_PACKET_STRUCT_END;
}
#pragma pack()

#endif /* USERSESSIONEFINES_H_ */
