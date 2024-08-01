#include "WonsikWorldClientStub.h"
#include "IOCPServer.h"
#include "Log.h"
using namespace std;
bool WonsikWorldClientStub::PacketProcEnterGame_CS(CRecvBuffer& buf)
{
	WString nickName;
	try
	{
		buf >> nickName;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcEnterGame_CS error\n");
		return false;
	}
	ProcEnterGame_CS(, nickName);
	return true;
}

bool WonsikWorldClientStub::PacketProcEnterGame_SC(CRecvBuffer& buf)
{
	short enterGameResult;
	LONG64 playerID;
	try
	{
		buf >> enterGameResult >> playerID;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcEnterGame_SC error\n");
		return false;
	}
	ProcEnterGame_SC(, enterGameResult, playerID);
	return true;
}

bool WonsikWorldClientStub::PacketProcCreateMyCharacter_SC(CRecvBuffer& buf)
{
	short mapID;
	float dirX;
	float dirY;
	float locationX;
	float locationY;
	try
	{
		buf >> mapID >> dirX >> dirY >> locationX >> locationY;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcCreateMyCharacter_SC error\n");
		return false;
	}
	ProcCreateMyCharacter_SC(, mapID, dirX, dirY, locationX, locationY);
	return true;
}

bool WonsikWorldClientStub::PacketProcCreateOtherCharacter_SC(CRecvBuffer& buf)
{
	short mapID;
	LONG64 playerID;
	WString nickName;
	float dirX;
	float dirY;
	float locationX;
	float locationY;
	try
	{
		buf >> mapID >> playerID >> nickName >> dirX >> dirY >> locationX >> locationY;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcCreateOtherCharacter_SC error\n");
		return false;
	}
	ProcCreateOtherCharacter_SC(, mapID, playerID, nickName, dirX, dirY, locationX, locationY);
	return true;
}

bool WonsikWorldClientStub::PacketProcDeleteCharacter_SC(CRecvBuffer& buf)
{
	short mapID;
	LONG64 playerID;
	try
	{
		buf >> mapID >> playerID;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcDeleteCharacter_SC error\n");
		return false;
	}
	ProcDeleteCharacter_SC(, mapID, playerID);
	return true;
}

bool WonsikWorldClientStub::PacketProcChangeMap_CS(CRecvBuffer& buf)
{
	short beforeMapID;
	short afterMapID;
	try
	{
		buf >> beforeMapID >> afterMapID;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcChangeMap_CS error\n");
		return false;
	}
	ProcChangeMap_CS(, beforeMapID, afterMapID);
	return true;
}

bool WonsikWorldClientStub::PacketProcChangeMap_SC(CRecvBuffer& buf)
{
	short beforeMapID;
	short afterMapID;
	try
	{
		buf >> beforeMapID >> afterMapID;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcChangeMap_SC error\n");
		return false;
	}
	ProcChangeMap_SC(, beforeMapID, afterMapID);
	return true;
}

bool WonsikWorldClientStub::PacketProcSendChatMessage_CS(CRecvBuffer& buf)
{
	short mapID;
	WString chatMessage;
	try
	{
		buf >> mapID >> chatMessage;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcSendChatMessage_CS error\n");
		return false;
	}
	ProcSendChatMessage_CS(, mapID, chatMessage);
	return true;
}

bool WonsikWorldClientStub::PacketProcSendChatMessage_SC(CRecvBuffer& buf)
{
	short mapID;
	LONG64 playerID;
	WString chatMessage;
	try
	{
		buf >> mapID >> playerID >> chatMessage;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcSendChatMessage_SC error\n");
		return false;
	}
	ProcSendChatMessage_SC(, mapID, playerID, chatMessage);
	return true;
}

bool WonsikWorldClientStub::PacketProcMoveMyCharacter_CS(CRecvBuffer& buf)
{
	short mapID;
	float destinationX;
	float destinationY;
	try
	{
		buf >> mapID >> destinationX >> destinationY;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcMoveMyCharacter_CS error\n");
		return false;
	}
	ProcMoveMyCharacter_CS(, mapID, destinationX, destinationY);
	return true;
}

bool WonsikWorldClientStub::PacketProcMoveMyCharacter_SC(CRecvBuffer& buf)
{
	short mapID;
	Vector<float> destinationsX;
	Vector<float> destinationsY;
	try
	{
		buf >> mapID >> destinationsX >> destinationsY;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcMoveMyCharacter_SC error\n");
		return false;
	}
	ProcMoveMyCharacter_SC(, mapID, destinationsX, destinationsY);
	return true;
}

bool WonsikWorldClientStub::PacketProcMoveOtherCharacter_SC(CRecvBuffer& buf)
{
	short mapID;
	LONG64 playerID;
	Vector<float> destinationsX;
	Vector<float> destinationsY;
	try
	{
		buf >> mapID >> playerID >> destinationsX >> destinationsY;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcMoveOtherCharacter_SC error\n");
		return false;
	}
	ProcMoveOtherCharacter_SC(, mapID, playerID, destinationsX, destinationsY);
	return true;
}

bool WonsikWorldClientStub::PacketProcHeartBeat_CS(CRecvBuffer& buf)
{
	try
	{
		buf;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcHeartBeat_CS error\n");
		return false;
	}
	ProcHeartBeat_CS();
	return true;
}

bool WonsikWorldClientStub::PacketProc(CRecvBuffer& buf)
{
	short packetType;
	try
	{
		buf >> packetType;
	}
	catch(int remainSize)
	{
		return false;
	}
	switch(packetType)
	{
	case PKT_TYPE_EnterGame_CS:
	{
		return PacketProcEnterGame_CS(buf);
		break;
	}
	case PKT_TYPE_EnterGame_SC:
	{
		return PacketProcEnterGame_SC(buf);
		break;
	}
	case PKT_TYPE_CreateMyCharacter_SC:
	{
		return PacketProcCreateMyCharacter_SC(buf);
		break;
	}
	case PKT_TYPE_CreateOtherCharacter_SC:
	{
		return PacketProcCreateOtherCharacter_SC(buf);
		break;
	}
	case PKT_TYPE_DeleteCharacter_SC:
	{
		return PacketProcDeleteCharacter_SC(buf);
		break;
	}
	case PKT_TYPE_ChangeMap_CS:
	{
		return PacketProcChangeMap_CS(buf);
		break;
	}
	case PKT_TYPE_ChangeMap_SC:
	{
		return PacketProcChangeMap_SC(buf);
		break;
	}
	case PKT_TYPE_SendChatMessage_CS:
	{
		return PacketProcSendChatMessage_CS(buf);
		break;
	}
	case PKT_TYPE_SendChatMessage_SC:
	{
		return PacketProcSendChatMessage_SC(buf);
		break;
	}
	case PKT_TYPE_MoveMyCharacter_CS:
	{
		return PacketProcMoveMyCharacter_CS(buf);
		break;
	}
	case PKT_TYPE_MoveMyCharacter_SC:
	{
		return PacketProcMoveMyCharacter_SC(buf);
		break;
	}
	case PKT_TYPE_MoveOtherCharacter_SC:
	{
		return PacketProcMoveOtherCharacter_SC(buf);
		break;
	}
	case PKT_TYPE_HeartBeat_CS:
	{
		return PacketProcHeartBeat_CS(buf);
		break;
	}
	default:
	{
		return false;
	}
	}
}
