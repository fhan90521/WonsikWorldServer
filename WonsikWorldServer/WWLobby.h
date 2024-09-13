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
	virtual void OnLeaveRoomSystem(SessionInfo sessionInfo,bool bEnterCompleted) override;
private:
	LONG64 _newPlayerID = 1;
	HashMap<SessionInfo::ID,SharedPtr<struct WWSession>> _guests;
	HashMap<SessionInfo::ID, SharedPtr<struct WWSession>> _players;
	HashSet<WString> _namesOnPlay;
	ULONG64 _lastCheckTime;
	void CheckLastRecvTime();
public:
	void EnterGame(SessionInfo sessionInfo,WString nickName);
	void LeaveGame(SessionInfo sessionInfo);

};