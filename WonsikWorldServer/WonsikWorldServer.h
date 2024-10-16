#pragma once
#include "Network/IOCPServer.h"
#include "WonsikWorldServerProxy.h"
#include "WonsikWorldServerStub.h"
#include "Container/MyStlContainer.h"
#include "Lock/LockGuard.h"
#include "RoomSystem/RoomSystem.h"
#include "WWEnum.h"
#include <thread>
#include "Profiler/PerformanceMonitor.h"
class WonsikWorldServer : public IOCPServer, public WonsikWorldServerProxy, private WonsikWorldServerStub
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
	virtual void OnRecv(SessionInfo sessionInfo,int roomID, CRecvBuffer& buf) override;

private:
	std::atomic<int> _moveCharacterCnt = 0;
	std::atomic<int> _enterGameCnt = 0;
	std::atomic<int> _changeMapCnt = 0;
	std::atomic<int> _sendChatMessageCnt = 0;

	virtual void ProcMoveMyCharacter_CS(SessionInfo sessionInfo,int roomID, short mapID, WWVector2D& destination) override;
	virtual void ProcHeartBeat_CS(SessionInfo sessionInfo, int roomID);
	virtual void ProcEnterGame_CS(SessionInfo sessionInfo, int roomID, WString& nickName) override;
	virtual void ProcChangeMap_CS(SessionInfo sessionInfo, int roomID, short beforeMapID, short afterMapID) override;
	virtual void ProcSendChatMessage_CS(SessionInfo sessionInfo, int roomID, short mapID, WString& chatMessage) override;
private:
	SharedPtr<class WWRoomSystem> _wwRoomSystem;
	SharedPtr<class WWLobby> _lobby;
	Array<SharedPtr<class WWField>,3> _fields;
	Array<int, 3> _roomIDs = { ROOM_ID_LOBBY,ROOM_ID_FIELD1,ROOM_ID_FIELD2 };
	USE_LOCK;
	HashMap<SessionInfo::ID, SharedPtr<struct WWSession>> _wwSessions;
private:
	void CreateWWSession(SessionInfo sessionInfo);
	int GetWWSessionCnt();
public:
	SharedPtr<struct WWSession> GetWWSession(SessionInfo sessionInfo);
	void DeleteWWSession(SessionInfo sessionInfo);

};