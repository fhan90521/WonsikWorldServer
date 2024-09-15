#include "WWRoomSystem.h"
#include "WonsikWorldServer.h"
void WWRoomSystem::OnLeaveRoomSystem(SessionInfo sessionInfo)
{
	_wwServer->DeleteWWSession(sessionInfo);
}
bool WWRoomSystem::CheckCanLeaveSystem(SessionInfo sessionInfo)
{
	int i = rand() % 5;
	return i==0;
}
WWRoomSystem::WWRoomSystem(WonsikWorldServer* pServer) :RoomSystem(pServer)
{
	_wwServer = pServer;
}
