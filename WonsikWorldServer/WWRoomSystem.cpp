#include "WWRoomSystem.h"
#include "WonsikWorldServer.h"
void WWRoomSystem::OnLeaveByChangingRoomSession(SessionInfo sessionInfo)
{
	_wwServer->DeleteWWSession(sessionInfo);
}
void WWRoomSystem::OnError(SessionInfo sessionInfo, RoomError error)
{
	_wwServer->Disconnect(sessionInfo);
}
WWRoomSystem::WWRoomSystem(WonsikWorldServer* pServer) :RoomSystem(pServer)
{
	_wwServer = pServer;
}
