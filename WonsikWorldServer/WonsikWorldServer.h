#pragma once
#include "IOCPServer.h"
#include "WonsikWorldServerProxy.h"
#include "WonsikWorldServerStub.h"
#include "MyStlContainer.h"
#include "LockGuard.h"
#include "RoomSystem.h"
#include "WWEnum.h"
#include <thread>
#include "PerformanceMonitor.h"
class WonsikWorldServer : public IOCPServer,public WonsikWorldServerProxy, public WonsikWorldServerStub
{
private:
	PerformanceMonitor _monitor;
public:
	WonsikWorldServer();
	~WonsikWorldServer();
	virtual void Run() override;
	void PrintServerStatus();
private:

	virtual bool OnAcceptRequest(const char* ip, USHORT port) override;
	virtual void OnAccept(SessionInfo sessionInfo) override;
	virtual void OnDisconnect(SessionInfo sessionInfo) override;
	virtual void OnRecv(SessionInfo sessionInfo, CRecvBuffer& buf) override;

private:
	std::atomic<int> _moveCharacterCnt = 0;
	std::atomic<int> _enterGameCnt = 0;
	std::atomic<int> _changeMapCnt = 0;
	std::atomic<int> _sendChatMessageCnt = 0;

	virtual void ProcMoveMyCharacter_CS(SessionInfo sessionInfo, short mapID, float destinationX, float destinationY) override;
	virtual void ProcHeartBeat_CS(SessionInfo sessionInfo);
	virtual void ProcEnterGame_CS(SessionInfo sessionInfo, WString& nickName) override;
	virtual void ProcChangeMap_CS(SessionInfo sessionInfo, short beforeMapID, short afterMapID) override;
	virtual void ProcSendChatMessage_CS(SessionInfo sessionInfo, short mapID, WString& chatMessage) override;	
private:
	class WWRoomSystem* _wwRoomSystem;
	SharedPtr<class WWLobby> _lobby;
	Array<SharedPtr<class WWField>,3> _fields;
	Array<int, 3> _roomIDs = { ROOM_ID_LOBBY,ROOM_ID_FIELD1,ROOM_ID_FIELD2 };
	USE_LOCK;
	HashMap<SessionInfo::ID, SharedPtr<struct WWSession>> _wwSessions;
private:
	HANDLE _hShutDownEvent;
	std::thread* _checkRecvTimeThread;
	void CheckLastRecvTime();
	void CreateWWSession(SessionInfo sessionInfo);
	int GetWWSessionCnt();
public:
	SharedPtr<struct WWSession> GetWWSession(SessionInfo sessionInfo);
	void DeleteWWSession(SessionInfo sessionInfo);

};