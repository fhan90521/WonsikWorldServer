#include "WWLobby.h"
#include "WWSession.h"
#include "WonsikWorldServer.h"
#include "WWEnum.h"
#include "MakeShared.h"
#include "WWPlayer.h"
#include <memory>
#include "RoomSystem.h"
#include "MyNew.h"
#include <format>
WWLobby::WWLobby(WonsikWorldServer* pServer): Room(pServer->GetCompletionPortHandle())
{
	_wwServer=pServer;
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
  
}

void WWLobby::OnEnter(SessionInfo sessionInfo)
{
	auto wwSession=_wwServer->GetWWSession(sessionInfo);
	if (wwSession)
	{
		_guests[sessionInfo.Id()] = wwSession;
	}
}

int WWLobby::RequestEnter(SessionInfo sessionInfo)
{
    return ENTER_SUCCESS;
}

void WWLobby::OnLeave(SessionInfo sessionInfo)
{
	_guests.erase(sessionInfo.Id());
}

void WWLobby::OnLeaveRoomSystem(SessionInfo sessionInfo)
{
	OnLeave(sessionInfo);
	_wwServer->DeleteWWSession(sessionInfo);
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
