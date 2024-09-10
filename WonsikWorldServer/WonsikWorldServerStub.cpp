#include "WonsikWorldServerStub.h"
#include "IOCPServer.h"
#include "Log.h"
#include "WWVector2D.h"
bool WonsikWorldServerStub::PacketProcEnterGame_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
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
	ProcEnterGame_CS(sessionInfo, roomID, nickName);
	return true;
}

bool WonsikWorldServerStub::PacketProcEnterGame_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
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
	ProcEnterGame_SC(sessionInfo, roomID, enterGameResult, playerID);
	return true;
}

bool WonsikWorldServerStub::PacketProcCreateMyCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
{
	short mapID;
	WWVector2D dirVec;
	WWVector2D location;
	try
	{
		buf >> mapID >> dirVec >> location;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcCreateMyCharacter_SC error\n");
		return false;
	}
	ProcCreateMyCharacter_SC(sessionInfo, roomID, mapID, dirVec, location);
	return true;
}

bool WonsikWorldServerStub::PacketProcCreateOtherCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
{
	short mapID;
	LONG64 playerID;
	WString nickName;
	WWVector2D dirVec;
	WWVector2D location;
	try
	{
		buf >> mapID >> playerID >> nickName >> dirVec >> location;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcCreateOtherCharacter_SC error\n");
		return false;
	}
	ProcCreateOtherCharacter_SC(sessionInfo, roomID, mapID, playerID, nickName, dirVec, location);
	return true;
}

bool WonsikWorldServerStub::PacketProcDeleteCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
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
	ProcDeleteCharacter_SC(sessionInfo, roomID, mapID, playerID);
	return true;
}

bool WonsikWorldServerStub::PacketProcChangeMap_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
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
	ProcChangeMap_CS(sessionInfo, roomID, beforeMapID, afterMapID);
	return true;
}

bool WonsikWorldServerStub::PacketProcChangeMap_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
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
	ProcChangeMap_SC(sessionInfo, roomID, beforeMapID, afterMapID);
	return true;
}

bool WonsikWorldServerStub::PacketProcSendChatMessage_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
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
	ProcSendChatMessage_CS(sessionInfo, roomID, mapID, chatMessage);
	return true;
}

bool WonsikWorldServerStub::PacketProcSendChatMessage_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
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
	ProcSendChatMessage_SC(sessionInfo, roomID, mapID, playerID, chatMessage);
	return true;
}

bool WonsikWorldServerStub::PacketProcMoveMyCharacter_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
{
	short mapID;
	WWVector2D destination;
	try
	{
		buf >> mapID >> destination;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcMoveMyCharacter_CS error\n");
		return false;
	}
	ProcMoveMyCharacter_CS(sessionInfo, roomID, mapID, destination);
	return true;
}

bool WonsikWorldServerStub::PacketProcMoveMyCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
{
	short mapID;
	Vector<WWVector2D> destinations;
	try
	{
		buf >> mapID >> destinations;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcMoveMyCharacter_SC error\n");
		return false;
	}
	ProcMoveMyCharacter_SC(sessionInfo, roomID, mapID, destinations);
	return true;
}

bool WonsikWorldServerStub::PacketProcMoveOtherCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
{
	short mapID;
	LONG64 playerID;
	Vector<WWVector2D> destinations;
	try
	{
		buf >> mapID >> playerID >> destinations;
	}
	catch(int useSize)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "PacketProcMoveOtherCharacter_SC error\n");
		return false;
	}
	ProcMoveOtherCharacter_SC(sessionInfo, roomID, mapID, playerID, destinations);
	return true;
}

bool WonsikWorldServerStub::PacketProcHeartBeat_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf)
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
	ProcHeartBeat_CS(sessionInfo, roomID);
	return true;
}

bool WonsikWorldServerStub::PacketProc(SessionInfo sessionInfo,int roomID, CRecvBuffer& buf)
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
		return PacketProcEnterGame_CS(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_EnterGame_SC:
	{
		return PacketProcEnterGame_SC(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_CreateMyCharacter_SC:
	{
		return PacketProcCreateMyCharacter_SC(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_CreateOtherCharacter_SC:
	{
		return PacketProcCreateOtherCharacter_SC(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_DeleteCharacter_SC:
	{
		return PacketProcDeleteCharacter_SC(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_ChangeMap_CS:
	{
		return PacketProcChangeMap_CS(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_ChangeMap_SC:
	{
		return PacketProcChangeMap_SC(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_SendChatMessage_CS:
	{
		return PacketProcSendChatMessage_CS(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_SendChatMessage_SC:
	{
		return PacketProcSendChatMessage_SC(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_MoveMyCharacter_CS:
	{
		return PacketProcMoveMyCharacter_CS(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_MoveMyCharacter_SC:
	{
		return PacketProcMoveMyCharacter_SC(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_MoveOtherCharacter_SC:
	{
		return PacketProcMoveOtherCharacter_SC(sessionInfo,roomID, buf);
		break;
	}
	case PKT_TYPE_HeartBeat_CS:
	{
		return PacketProcHeartBeat_CS(sessionInfo,roomID, buf);
		break;
	}
	default:
	{
		return false;
	}
	}
}
