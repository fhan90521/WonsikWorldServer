#include "WWRoomSystem.h"
#include "WonsikWorldServer.h"
void WWRoomSystem::OnLeaveByChangingRoomSession(SessionInfo sessionInfo)
{
	_wwServer->DeleteWWSession(sessionInfo);
}
WWRoomSystem::WWRoomSystem(WonsikWorldServer* pServer):RoomSystem(pServer)
{
	_wwServer = pServer;
}
