#include "WWLobby.h"
#include "WWSession.h"
#include "WonsikWorldServer.h"
#include "WWEnum.h"
#include "WWPlayer.h"
#include <memory>
#include <format>
#include <iostream>
WWLobby::WWLobby(WonsikWorldServer* pServer): Room(pServer->GetCompletionPortHandle())
{
	_wwServer=pServer;
	_lastCheckTime = GetTickCount64();
}

void WWLobby::PrintLobbyStatus()
{
	std::cout << std::format(R"(
--LobbyStatus
SessionNum: {}       UpdateTps: {}
)", GetSessionCnt(), GetUpdateCnt());

}

void WWLobby::Update(float deltaTime)
{
	CheckLastRecvTime();
}

void WWLobby::OnEnter(SessionInfo sessionInfo)
{
	auto wwSession=_wwServer->GetWWSession(sessionInfo);
	if (wwSession)
	{
		_guests[sessionInfo.Id()] = wwSession;
	}
}

bool WWLobby::CheckCanLeave(SessionInfo sessionInfo)
{
	int i = rand()%4;
	return i==0;
}

void WWLobby::OnLeave(SessionInfo sessionInfo)
{
	_guests.erase(sessionInfo.Id());
}

void WWLobby::CheckLastRecvTime()
{
	ULONG64 curTime = GetCurTime();
	if (curTime - _lastCheckTime < LAST_RECV_TIME_OUT)
	{
		return;
	}
	_lastCheckTime = curTime;
	for (auto& temp : _guests)
	{
		auto& wwSession = temp.second;
		ULONG64 lastRecvTime = _wwServer->GetLastRecvTime(wwSession->sessionInfo);
		if (curTime - lastRecvTime > LAST_RECV_TIME_OUT)
		{
			_wwServer->Disconnect(wwSession->sessionInfo);
		}
	}

	for (auto& temp : _players)
	{
		auto& wwSession = temp.second;
		ULONG64 lastRecvTime = _wwServer->GetLastRecvTime(wwSession->sessionInfo);
		if (curTime - lastRecvTime > LAST_RECV_TIME_OUT)
		{
			_wwServer->Disconnect(wwSession->sessionInfo);
		}
	}
}

void WWLobby::EnterGame(SessionInfo sessionInfo, WString& nickName)
{
	auto iter = _guests.find(sessionInfo.Id());
	if (iter == _guests.end())
	{
		_wwServer->Disconnect(sessionInfo);
		Log::LogOnFile(Log::SYSTEM_LEVEL, "EnterGame guest not found error");
		return;
	}

	auto& wwSession = iter->second;
	if (wwSession->sessionType != SessionType::GUEST)
	{
		_wwServer->Disconnect(wwSession->sessionInfo);
		return;
	}

	if (_namesOnPlay.find(nickName) == _namesOnPlay.end())
	{
		//player 생성
		wwSession->sessionType = SessionType::PLAYER;
		WWPlayer* newPlayer = New<WWPlayer>();
		wwSession->wwPlayer = newPlayer;
		newPlayer->SetMoveSpeed(MOVE_SPEED);
		newPlayer->SetNickName(nickName);
		newPlayer->SetSessionInfo(wwSession->sessionInfo);
		newPlayer->SetPlayerID(_newPlayerID++);
		
		_namesOnPlay.insert(nickName);
		_players[sessionInfo.Id()] = wwSession;

		_wwServer->EnterGame_SC(wwSession->sessionInfo, ENTER_GAME_SUCCESS, newPlayer->GetPlayerID());
		
		if (ChangeRoom(wwSession->sessionInfo, ROOM_ID_FIELD1)==false)
		{
			_wwServer->Disconnect(wwSession->sessionInfo);
		}
	}
	else
	{
		//닉네임 중복
		_wwServer->EnterGame_SC(wwSession->sessionInfo, NAME_DUPLICATION,0);
	}
}

void WWLobby::LeaveGame(SessionInfo sessionInfo)
{
	auto iter = _players.find(sessionInfo.Id());
	if (iter == _players.end())
	{
		return;
	}
	auto& wwSession = iter->second;
	if (wwSession)
	{
		_namesOnPlay.erase(wwSession->wwPlayer->GetNickNameRef());
	}
	_players.erase(iter);
}
