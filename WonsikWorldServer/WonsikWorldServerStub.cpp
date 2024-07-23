#include  "WonsikWorldServerStub.h"
 #include "IOCPServer.h"
#include <iostream>
#include "Log.h"
using namespace std;
bool WonsikWorldServerStub::PacketProcEnterGame_CS(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcEnterGame_CS( sessionInfo , nickName);
	return true;
}
bool WonsikWorldServerStub::PacketProcEnterGame_SC(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcEnterGame_SC( sessionInfo , enterGameResult, playerID);
	return true;
}
bool WonsikWorldServerStub::PacketProcCreateMyCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcCreateMyCharacter_SC( sessionInfo , mapID, dirX, dirY, locationX, locationY);
	return true;
}
bool WonsikWorldServerStub::PacketProcCreateOtherCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcCreateOtherCharacter_SC( sessionInfo , mapID, playerID, nickName, dirX, dirY, locationX, locationY);
	return true;
}
bool WonsikWorldServerStub::PacketProcDeleteCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcDeleteCharacter_SC( sessionInfo , mapID, playerID);
	return true;
}
bool WonsikWorldServerStub::PacketProcChangeMap_CS(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcChangeMap_CS( sessionInfo , beforeMapID, afterMapID);
	return true;
}
bool WonsikWorldServerStub::PacketProcChangeMap_SC(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcChangeMap_SC( sessionInfo , beforeMapID, afterMapID);
	return true;
}
bool WonsikWorldServerStub::PacketProcSendChatMessage_CS(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcSendChatMessage_CS( sessionInfo , mapID, chatMessage);
	return true;
}
bool WonsikWorldServerStub::PacketProcSendChatMessage_SC(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcSendChatMessage_SC( sessionInfo , mapID, playerID, chatMessage);
	return true;
}
bool WonsikWorldServerStub::PacketProcMoveMyCharacter_CS(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcMoveMyCharacter_CS( sessionInfo , mapID, destinationX, destinationY);
	return true;
}
bool WonsikWorldServerStub::PacketProcMoveMyCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcMoveMyCharacter_SC( sessionInfo , mapID, destinationsX, destinationsY);
	return true;
}
bool WonsikWorldServerStub::PacketProcMoveOtherCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcMoveOtherCharacter_SC( sessionInfo , mapID, playerID, destinationsX, destinationsY);
	return true;
}
bool WonsikWorldServerStub::PacketProcHeartBeat_CS(SessionInfo sessionInfo, CRecvBuffer& buf)
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
	ProcHeartBeat_CS( sessionInfo );
	return true;
}

bool WonsikWorldServerStub::PacketProc(SessionInfo sessionInfo, CRecvBuffer& buf)
{
	short packetType;
	try
	{
		buf>>packetType;
	}
	catch(int remainSize)
	{
		 return false;
	}
	switch(packetType)
	{
	case PKT_TYPE_EnterGame_CS:
	{
		return PacketProcEnterGame_CS(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_EnterGame_SC:
	{
		return PacketProcEnterGame_SC(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_CreateMyCharacter_SC:
	{
		return PacketProcCreateMyCharacter_SC(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_CreateOtherCharacter_SC:
	{
		return PacketProcCreateOtherCharacter_SC(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_DeleteCharacter_SC:
	{
		return PacketProcDeleteCharacter_SC(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_ChangeMap_CS:
	{
		return PacketProcChangeMap_CS(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_ChangeMap_SC:
	{
		return PacketProcChangeMap_SC(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_SendChatMessage_CS:
	{
		return PacketProcSendChatMessage_CS(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_SendChatMessage_SC:
	{
		return PacketProcSendChatMessage_SC(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_MoveMyCharacter_CS:
	{
		return PacketProcMoveMyCharacter_CS(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_MoveMyCharacter_SC:
	{
		return PacketProcMoveMyCharacter_SC(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_MoveOtherCharacter_SC:
	{
		return PacketProcMoveOtherCharacter_SC(sessionInfo,buf);
		break;
	}
	case PKT_TYPE_HeartBeat_CS:
	{
		return PacketProcHeartBeat_CS(sessionInfo,buf);
		break;
	}
	default:
	{
		Log::LogOnFile(Log::DEBUG_LEVEL,"Packet Type not exist error\n");
		return false;
		break;
	}
	}
}
