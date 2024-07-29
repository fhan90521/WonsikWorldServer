#pragma once
#include "IOCPServer.h"
#include "WonsikWorldServerProxy.h"
#include "WonsikWorldServerStub.h"
#include "MyStlContainer.h"
#include "LockGuard.h"
#include "RoomSystem.h"
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
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
	virtual void ProcMoveMyCharacter_CS(SessionInfo sessionInfo, short mapID, float destinationX, float destinationY) override;
	virtual void ProcHeartBeat_CS(SessionInfo sessionInfo);
	virtual void ProcEnterGame_CS(SessionInfo sessionInfo, WString& nickName) override;
	virtual void ProcChangeMap_CS(SessionInfo sessionInfo, short beforeMapID, short afterMapID) override;
	virtual void ProcSendChatMessage_CS(SessionInfo sessionInfo, short mapID, WString& chatMessage) override;	
private:
	RoomSystem _wwRoomSystem;
	SharedPtr<class WWLobby> _lobby;
	Array<SharedPtr<class WWField>,3> _fields;
	USE_LOCK;
	HashMap<SessionInfo::ID, SharedPtr<struct WWSession>> _wwSessions;
private:
	HANDLE _hShutDownEvent;
	std::jthread* _checkRecvTimeThread;
	void CheckLastRecvTime();
	void CreateWWSession(SessionInfo sessionInfo);
public:
	SharedPtr<struct WWSession> GetWWSession(SessionInfo sessionInfo);
	void DeleteWWSession(SessionInfo sessionInfo);
};