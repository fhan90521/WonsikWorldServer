#include "WonsikWorldServer.h"
#include "Log.h"
#include "MakeShared.h"
#include "WWLobby.h"
#include "WWSession.h"
#include "WWPlayer.h"
#include "WWField.h"
#include "MapSource.h"
#include "WWRoomSystem.h"
#include <iostream>
#include <format>
WonsikWorldServer::WonsikWorldServer():WonsikWorldServerProxy(this),IOCPServer("WWServerSet.Json")
{
	_wwRoomSystem = new WWRoomSystem(this);
	_lobby = MakeShared<WWLobby>(this);
	_fields[ROOM_ID_FIELD1] = MakeShared<WWField>(this);
	_fields[ROOM_ID_FIELD2] = MakeShared<WWField>(this);
	
	bool retRegisterRoom = _wwRoomSystem->RegisterRoom(_lobby);
	if (retRegisterRoom==false|| _lobby->GetRoomID() != ROOM_ID_LOBBY)
	{
		Log::LogOnFile(Log::SYSTEM_LEVEL, "Lobby Room ID not match");
		DebugBreak();
	}
	
	for (int i = 1; i < _fields.size(); i++)
	{
		retRegisterRoom = _wwRoomSystem->RegisterRoom(_fields[i]);
		if (retRegisterRoom==false|| _fields[i]->GetRoomID() != _roomIDs[i])
		{
			Log::LogOnFile(Log::SYSTEM_LEVEL, "FIELD%d Room ID not match",i);
			DebugBreak();
		}
	}

	_fields[ROOM_ID_FIELD1]->InitMap(map1);
	_fields[ROOM_ID_FIELD2]->InitMap(map2);

	//_monitor.AddInterface("0.0.0.0");
}

WonsikWorldServer::~WonsikWorldServer()
{
	_wwRoomSystem->DeregisterRoom(_lobby);
	_wwRoomSystem->DeregisterRoom(_fields[1]);
	_wwRoomSystem->DeregisterRoom(_fields[2]);
	delete _wwRoomSystem;

	CloseServer();
}

void WonsikWorldServer::Run()
{
	IOCPRun();
}

void WonsikWorldServer::PrintServerStatus()
{
	std::cout << std::format(R"(
-------------------------------------
SessionNum: {}	Accept Tps: {}	RecvMessageTps: {}	SendMessageTps: {}	SessionCntInRoomSystem: {}

MoveCharacterJobTps: {}	EnterGameJobTps: {}	ChangeMapJobTps: {}	SendChatMessageJobTps: {}
)", GetWWSessionCnt(), GetAcceptCnt(), GetRecvCnt(), GetSendCnt(),_wwRoomSystem->GetSessionCntInRoomSystem(), _moveCharacterCnt.load(), _enterGameCnt.load(), _changeMapCnt.load(), _sendChatMessageCnt.load());
	
	_moveCharacterCnt = 0;
	_enterGameCnt = 0;
	_changeMapCnt = 0;
	_sendChatMessageCnt = 0;
	_lobby->PrintLobbyStatus();
	_fields[ROOM_ID_FIELD1]->PrintFieldStatus();
	_fields[ROOM_ID_FIELD2]->PrintFieldStatus();
	_monitor.PrintMonitorData();
}

bool WonsikWorldServer::OnAcceptRequest(const char* ip, USHORT port)
{
	Log::LogOnConsole(Log::DEBUG_LEVEL, "connect ip: %s", ip);
	return true;
}

void WonsikWorldServer::OnAccept(SessionInfo sessionInfo)
{
	CreateWWSession(sessionInfo);
}

void WonsikWorldServer::OnDisconnect(SessionInfo sessionInfo)
{
	_lobby->DoAsync(&WWLobby::LeaveGame, sessionInfo);
	bool ret = _wwRoomSystem->LeaveRoomSystem(sessionInfo);
	if (ret == false)
	{
		DeleteWWSession(sessionInfo);
	}
}

void WonsikWorldServer::OnRecv(SessionInfo sessionInfo,int roomID, CRecvBuffer& buf)
{
	if (PacketProc(sessionInfo, roomID, buf) == false)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "OnRecv Error\n");
		Disconnect(sessionInfo);
	}
}

SharedPtr<WWSession> WonsikWorldServer::GetWWSession(SessionInfo sessionInfo)
{
	SHARED_LOCK;
	auto iter = _wwSessions.find(sessionInfo.Id());
	if (iter != _wwSessions.end())
	{
		return iter->second;
	}
	else
	{
		return SharedPtr<WWSession>();
	}
}

int WonsikWorldServer::GetWWSessionCnt()
{
	int ret = 0;
	{
		SHARED_LOCK;
		ret = _wwSessions.size();
	}
	return ret;
}


void WonsikWorldServer::CreateWWSession(SessionInfo sessionInfo)
{
	auto newWWSession = MakeShared<WWSession>();
	newWWSession->sessionInfo = sessionInfo;
	newWWSession->roomID = ROOM_ID_LOBBY;
	newWWSession->sessionType = SessionType::GUEST;
	
	{
		EXCLUSIVE_LOCK;
		_wwSessions[sessionInfo.Id()] = newWWSession;
	}

	bool enterResult = _wwRoomSystem->EnterRoomSystem(sessionInfo, ROOM_ID_LOBBY);
	if (enterResult == false)
	{
		Disconnect(sessionInfo);
	}
}

void WonsikWorldServer::DeleteWWSession(SessionInfo sessionInfo)
{
	{
		EXCLUSIVE_LOCK;
		_wwSessions.erase(sessionInfo.Id());
	}
}

void WonsikWorldServer::ProcEnterGame_CS(SessionInfo sessionInfo, int roomID, WString& nickName)
{
	if (roomID == ROOM_ID_LOBBY)
	{
		_enterGameCnt++;
		_lobby->DoAsync(&WWLobby::EnterGame, sessionInfo, nickName);
	}
	else
	{
		Disconnect(sessionInfo);
		Log::LogOnFile(Log::DEBUG_LEVEL, "ProcEnterGame_CS roomid Error\n");
	}
}

void WonsikWorldServer::ProcChangeMap_CS(SessionInfo sessionInfo, int roomID, short beforeMapID, short afterMapID)
{
	int beforeRoomID = beforeMapID;
	int afterRoomID = afterMapID;
	if (roomID == beforeRoomID && beforeRoomID>ROOM_ID_LOBBY)
	{
		_changeMapCnt++;
		_fields[roomID]->DoAsync(&WWField::ChangeField, sessionInfo, afterRoomID);
	}
	else
	{
		Disconnect(sessionInfo);
		Log::LogOnFile(Log::DEBUG_LEVEL, "ProcChangeMap_CS roomid Error\n");
	}
}

void WonsikWorldServer::ProcSendChatMessage_CS(SessionInfo sessionInfo, int roomID, short mapID, WString& chatMessage)
{
	if (roomID == mapID && roomID > ROOM_ID_LOBBY)
	{
		_sendChatMessageCnt++;
		_fields[roomID]->DoAsync(&WWField::SendChatMessage, sessionInfo, chatMessage);
	}
	else
	{
		Disconnect(sessionInfo);
		Log::LogOnFile(Log::DEBUG_LEVEL, "ProcSendChatMessage_CS roomid Error\n");
	}

}

void WonsikWorldServer::ProcMoveMyCharacter_CS(SessionInfo sessionInfo, int roomID, short mapID, WWVector2D& destination)
{
	if (roomID == mapID && roomID > ROOM_ID_LOBBY)
	{
		_moveCharacterCnt++;
		_fields[roomID]->DoAsync(&WWField::SetCharacterDestination, sessionInfo, destination);
	}
	else
	{
		Disconnect(sessionInfo);
		Log::LogOnFile(Log::DEBUG_LEVEL, "ProcMoveMyCharacter_CS roomid Error\n");
	}
}

void WonsikWorldServer::ProcHeartBeat_CS(SessionInfo sessionInfo, int roomID )
{
}