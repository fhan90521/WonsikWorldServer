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
	
	int lobbyRoomID = _wwRoomSystem->RegisterRoom(_lobby);
	if (lobbyRoomID != ROOM_ID_LOBBY)
	{
		Log::LogOnFile(Log::SYSTEM_LEVEL, "Lobby Room ID not match");
		DebugBreak();
	}
	
	for (int i = 1; i < _fields.size(); i++)
	{
		int fieldRoomID = _wwRoomSystem->RegisterRoom(_fields[i]);
		if (fieldRoomID != _roomIDs[i])
		{
			Log::LogOnFile(Log::SYSTEM_LEVEL, "FIELD%d Room ID not match",i);
			DebugBreak();
		}
	}

	_fields[ROOM_ID_FIELD1]->InitMap(map1);
	_fields[ROOM_ID_FIELD2]->InitMap(map2);

	if (LAST_RECV_TIME_OUT != 0)
	{
		_hShutDownEvent = CreateEvent(NULL, TRUE, false, NULL);
		_checkRecvTimeThread = new std::thread(&WonsikWorldServer::CheckLastRecvTime, this);
	}
	
	//_monitor.AddInterface("0.0.0.0");
}

WonsikWorldServer::~WonsikWorldServer()
{
	if (LAST_RECV_TIME_OUT != 0)
	{
		SetEvent(_hShutDownEvent);
		_checkRecvTimeThread->join();
		delete _checkRecvTimeThread;
		CloseHandle(_hShutDownEvent);
	}
	_wwRoomSystem->DeregisterRoom(ROOM_ID_LOBBY);
	_wwRoomSystem->DeregisterRoom(ROOM_ID_FIELD1);
	_wwRoomSystem->DeregisterRoom(ROOM_ID_FIELD2);
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
SessionNum: {}		Accept Tps: {}      RecvMessageTps: {}   SendMessageTps: {}

MoveCharacterJobTps: {}    EnterGameJobTps: {}    ChangeMapJobTps: {}     SendChatMessageJobTps: {}
)", GetWWSessionCnt(), GetAcceptCnt(), GetRecvCnt(), GetSendCnt(), _moveCharacterCnt.load(), _enterGameCnt.load(), _changeMapCnt.load(), _sendChatMessageCnt.load());
	
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
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo);
	if (wwSession)
	{
		_lobby->DoAsync(&WWLobby::LeaveGame,wwSession);
	}
	_wwRoomSystem->LeaveRoomSystem(sessionInfo);	
}

void WonsikWorldServer::OnRecv(SessionInfo sessionInfo, CRecvBuffer& buf)
{
	if (PacketProc(sessionInfo, buf) == false)
	{
		Log::LogOnFile(Log::DEBUG_LEVEL, "OnRecv Error\n");
		Disconnect(sessionInfo);
	}
	else
	{
		//RECV1회제한 -> lastRecvTime을 고치고 있는 스레드가 하나라는 것을 보장가능->구조체에 락걸지 않고 lastRecvTime에 쓰기가능
		SharedPtr<WWSession> wwSession=GetWWSession(sessionInfo);
		if (wwSession)
		{
			wwSession->lastRecvTime = GetTickCount64();
		}
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

void WonsikWorldServer::CheckLastRecvTime()
{
	while (1)
	{
		
		DWORD retWait = WaitForSingleObject(_hShutDownEvent, LAST_RECV_TIME_OUT/2);
		if (retWait == WAIT_OBJECT_0)
		{
			break;
		}
		
		{
			SHARED_LOCK;
			ULONG64 currentTime = GetTickCount64();
			for (auto& pairTemp : _wwSessions)
			{
				SharedPtr<WWSession>& wwSession = pairTemp.second;
				if (currentTime - wwSession->lastRecvTime > LAST_RECV_TIME_OUT)
				{
					Disconnect(wwSession->sessionInfo);
				}

			}
		}
	}
}

void WonsikWorldServer::CreateWWSession(SessionInfo sessionInfo)
{
	
	bool enterResult = _wwRoomSystem->EnterRoomSystem(sessionInfo, ROOM_ID_LOBBY);
	if(enterResult == true)
	{
		EXCLUSIVE_LOCK;
		_wwSessions[sessionInfo.Id()] = MakeShared<WWSession>();
		_wwSessions[sessionInfo.Id()]->sessionInfo = sessionInfo;
		_wwSessions[sessionInfo.Id()]->roomID = ROOM_ID_LOBBY;
		_wwSessions[sessionInfo.Id()]->lastRecvTime = GetTickCount64();
		_wwSessions[sessionInfo.Id()]->sessionType = SessionType::GUEST;
	}
	else
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

void WonsikWorldServer::ProcEnterGame_CS(SessionInfo sessionInfo, WString& nickName)
{
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo.Id());
	if (wwSession && wwSession->roomID == ROOM_ID_LOBBY)
	{
		_enterGameCnt++;
		_lobby->DoAsync(&WWLobby::EnterGame, wwSession, nickName);
	}
	else
	{
		Disconnect(sessionInfo);
		Log::LogOnFile(Log::DEBUG_LEVEL, "ProcEnterGame_CS roomid Error\n");
	}
}

void WonsikWorldServer::ProcChangeMap_CS(SessionInfo sessionInfo, short beforeMapID, short afterMapID)
{
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo.Id());
	int beforeRoomID = beforeMapID;
	int afterRoomID = afterMapID;
	if (wwSession && wwSession->roomID == beforeRoomID && beforeRoomID>ROOM_ID_LOBBY)
	{
		_changeMapCnt++;
		_fields[beforeRoomID]->DoAsync(&WWField::ChangeField, wwSession, afterRoomID);
	}
	else
	{
		Disconnect(sessionInfo);
		Log::LogOnFile(Log::DEBUG_LEVEL, "ProcChangeMap_CS roomid Error\n");
	}
}

void WonsikWorldServer::ProcSendChatMessage_CS(SessionInfo sessionInfo, short mapID, WString& chatMessage)
{
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo.Id());
	int roomID = mapID;
	if (wwSession && wwSession->roomID == roomID && roomID > ROOM_ID_LOBBY)
	{
		_sendChatMessageCnt++;
		_fields[wwSession->roomID]->DoAsync(&WWField::SendChatMessage, wwSession, chatMessage);
	}
	else
	{
		Disconnect(sessionInfo);
		Log::LogOnFile(Log::DEBUG_LEVEL, "ProcSendChatMessage_CS roomid Error\n");
	}

}

void WonsikWorldServer::ProcMoveMyCharacter_CS(SessionInfo sessionInfo, short mapID, float destinationX, float destinationY)
{
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo.Id());
	int roomID = mapID;
	if (wwSession && wwSession->roomID == roomID && roomID > ROOM_ID_LOBBY)
	{
		_moveCharacterCnt++;
		_fields[wwSession->roomID]->DoAsync(&WWField::SetCharacterDestination, wwSession, destinationX, destinationY);
	}
	else
	{
		Disconnect(sessionInfo);
		Log::LogOnFile(Log::DEBUG_LEVEL, "ProcMoveMyCharacter_CS roomid Error\n");
	}
}

void WonsikWorldServer::ProcHeartBeat_CS(SessionInfo sessionInfo)
{
}