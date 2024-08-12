#pragma once
#include "Room.h"
#include "MyStlContainer.h"
class WWLobby:public Room
{
public:
	WWLobby(class WonsikWorldServer* pServer);
	virtual ~WWLobby() {};
	void PrintLobbyStatus();
private:
	class WonsikWorldServer* _wwServer;
	virtual void Update(float deltaTime) override;
	virtual void OnEnter(SessionInfo sessionInfo) override;
	virtual int RequestEnter(SessionInfo sessionInfo) override;
	virtual void OnLeave(SessionInfo sessionInfo) override;
	virtual void OnLeaveRoomSystem(SessionInfo sessionInfo) override;
private:
	LONG64 _newPlayerID = 1;
	HashSet<WString> _namesOnPlay;
public:
	void EnterGame(SharedPtr<struct WWSession>& wwSession,WString& nickName);
	void LeaveGame(SharedPtr<struct WWSession>& wwSession);

};