#include "WWLobby.h"
#include "WWSession.h"
#include "WonsikWorldServer.h"
#include "WWEnum.h"
#include "MakeShared.h"
#include "WWPlayer.h"
#include <memory>
#include "RoomSystem.h"
#include "MyNew.h"
WWLobby::WWLobby(WonsikWorldServer* pServer): Room(pServer)
{
	_wwServer=pServer;
}

void WWLobby::PrintLobbyStatus()
{
	std::cout << std::format(R"(
--LobbyStatus
SessionNum: {}                                 
UpdateTps: {}
)", GetSessionCnt(), GetUpdateCnt());

}

void WWLobby::Update(float deltaTime)
{
  
}

void WWLobby::OnEnter(SessionInfo sessionInfo)
{
}

int WWLobby::RequestEnter(SessionInfo sessionInfo)
{
    return ENTER_SUCCESS;
}

void WWLobby::OnLeave(SessionInfo sessionInfo)
{
}

void WWLobby::OnLeaveRoomSystem(SessionInfo sessionInfo)
{
	_wwServer->DeleteWWSession(sessionInfo);
}


void WWLobby::EnterGame(SharedPtr<WWSession> wwSession, WString nickName)
{
	if (wwSession->sessionType != SessionType::GUEST)
	{
		_wwServer->Disconnect(wwSession->sessionInfo);
		return;
	}

	if (_namesOnPlay.find(nickName) == _namesOnPlay.end())
	{
		_namesOnPlay.insert(nickName);
		//player 생성
		wwSession->sessionType = SessionType::PLAYER;
		WWPlayer* newPlayer = New<WWPlayer>();
		wwSession->wwPlayer = newPlayer;
		newPlayer->SetMoveSpeed(MOVE_SPEED);
		newPlayer->_nickName=nickName;
		newPlayer->SetSessionInfo(wwSession->sessionInfo);
		newPlayer->SetPlayerID(_newPlayerID++);
		
		short result = ENTER_GAME_SUCCESS;
		_wwServer->EnterGame_SC(wwSession->sessionInfo, result, newPlayer->GetPlayerID());
		
		
		int beforeRoomID = wwSession->roomID;
		int afterRoomID = ROOM_ID_FIELD1;
		ChangeRoom(wwSession->sessionInfo,beforeRoomID, afterRoomID);
		if (beforeRoomID != ROOM_ID_LOBBY || afterRoomID != ROOM_ID_FIELD1)
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

void WWLobby::LeaveGame(WString nickName)
{
	_namesOnPlay.erase(nickName);
}
