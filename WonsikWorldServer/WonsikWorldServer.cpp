#include "WonsikWorldServer.h"
#include "Log.h"
#include "WWEnum.h"
#include "MakeShared.h"
#include "WWLobby.h"
#include "WWSession.h"
#include "WWPlayer.h"
#include "WWField.h"
#include "MapSource.h"
#include <iostream>
#include <format>
WonsikWorldServer::WonsikWorldServer():WonsikWorldServerProxy(this),IOCPServer("WWServerSet.Json")
{
	_lobby = MakeShared<WWLobby>(this);
	_fields[ROOM_ID_FIELD1] = MakeShared<WWField>(this);
	_fields[ROOM_ID_FIELD2] = MakeShared<WWField>(this);
	_wwRoomSystem.RegisterRoom(_lobby);
	_wwRoomSystem.RegisterRoom(_fields[ROOM_ID_FIELD1]);
	_wwRoomSystem.RegisterRoom(_fields[ROOM_ID_FIELD2]);
	_fields[ROOM_ID_FIELD1]->InitMap(map1);
	_fields[ROOM_ID_FIELD2]->InitMap(map2);

	if (LAST_RECV_TIME_OUT != 0)
	{
		_hShutDownEvent = CreateEvent(NULL, TRUE, false, NULL);
		_checkRecvTimeThread = new std::jthread(&WonsikWorldServer::CheckLastRecvTime, this);
	}
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


	CloseServer();
	_wwRoomSystem.DeregisterRoom(ROOM_ID_LOBBY);
	_wwRoomSystem.DeregisterRoom(ROOM_ID_FIELD1);
	_wwRoomSystem.DeregisterRoom(ROOM_ID_FIELD2);
}

void WonsikWorldServer::Run()
{
	IOCPRun();
}

void WonsikWorldServer::PrintServerStatus()
{
	std::cout << std::format(R"(
-------------------------------------
SessionNum: {}                                 
Accept Tps: {}
RecvMessageTps: {}
SendMessageTps: {}

)", GetConnectingSessionCnt(), GetAcceptCnt(),GetRecvCnt(), GetSendCnt());

	_monitor.PrintMonitorData();
	_lobby->PrintLobbyStatus();
	_fields[ROOM_ID_FIELD1]->PrintFieldStatus();
	_fields[ROOM_ID_FIELD2]->PrintFieldStatus();
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
	if (wwSession && wwSession->wwPlayer)
	{
		_lobby->TryDoSync(&WWLobby::LeaveGame, wwSession->wwPlayer->_nickName);
	}
	_wwRoomSystem.LeaveRoomSystem(sessionInfo);	
}

void WonsikWorldServer::OnRecv(SessionInfo sessionInfo, CRecvBuffer& buf)
{
	if (PacketProc(sessionInfo, buf) == false)
	{
		Log::LogOnFile(Log::SYSTEM_LEVEL, "OnRecv Error\n");
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
	auto iter = _wwSessions.find(sessionInfo.id);
	if (iter != _wwSessions.end())
	{
		return iter->second;
	}
	else
	{
		return SharedPtr<WWSession>();
	}
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
	
	bool enterResult = _wwRoomSystem.EnterRoomSystem(sessionInfo, ROOM_ID_LOBBY);
	if(enterResult == true)
	{
		EXCLUSIVE_LOCK;
		_wwSessions[sessionInfo.id] = MakeShared<WWSession>();
		_wwSessions[sessionInfo.id]->sessionInfo = sessionInfo;
		_wwSessions[sessionInfo.id]->roomID = ROOM_ID_LOBBY;
		_wwSessions[sessionInfo.id]->lastRecvTime = GetTickCount64();
		_wwSessions[sessionInfo.id]->sessionType = SessionType::GUEST;
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
		_wwSessions.erase(sessionInfo.id);
	}
}

void WonsikWorldServer::ProcEnterGame_CS(SessionInfo sessionInfo, WString& nickName)
{
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo.id);
	if (wwSession && wwSession->roomID == ROOM_ID_LOBBY)
	{
		_lobby->TryDoSync(&WWLobby::EnterGame, wwSession, nickName);
	}
	else
	{
		Disconnect(sessionInfo);
	}
}

void WonsikWorldServer::ProcChangeMap_CS(SessionInfo sessionInfo, short beforeMapID, short afterMapID)
{
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo.id);
	int beforeRoomID = beforeMapID;
	int afterRoomID = afterMapID;
	if (wwSession && wwSession->roomID == beforeRoomID && beforeRoomID>ROOM_ID_LOBBY)
	{
		_fields[beforeRoomID]->TryDoSync(&WWField::ChangeField, wwSession, beforeRoomID, afterRoomID);
	}
	else
	{
		Disconnect(sessionInfo);
	}
}

void WonsikWorldServer::ProcSendChatMessage_CS(SessionInfo sessionInfo, short mapID, WString& chatMessage)
{
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo.id);
	int roomID = mapID;
	if (wwSession && wwSession->roomID == roomID && roomID > ROOM_ID_LOBBY)
	{
		_fields[wwSession->roomID]->TryDoSync(&WWField::SendChatMessage, wwSession, chatMessage);
	}
	else
	{
		Disconnect(sessionInfo);
	}

}

void WonsikWorldServer::ProcMoveMyCharacter_CS(SessionInfo sessionInfo, short mapID, float destinationX, float destinationY)
{
	SharedPtr<WWSession> wwSession = GetWWSession(sessionInfo.id);
	int roomID = mapID;
	if (wwSession && wwSession->roomID == roomID && roomID > ROOM_ID_LOBBY)
	{
		_fields[wwSession->roomID]->TryDoSync(&WWField::SetCharacterDestination, wwSession, destinationX, destinationY);
	}
	else
	{
		Disconnect(sessionInfo);
	}
}

void WonsikWorldServer::ProcHeartBeat_CS(SessionInfo sessionInfo)
{
}