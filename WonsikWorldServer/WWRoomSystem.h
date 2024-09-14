#pragma once
#include "RoomSystem/RoomSystem.h"
class WWRoomSystem : public RoomSystem
{
private:
	class WonsikWorldServer* _wwServer = nullptr;
	virtual void OnLeaveByChangingRoomSession(SessionInfo sessionInfo) override;
	virtual void OnError(SessionInfo sessionInfo, RoomError error) override;
public:
	WWRoomSystem(class WonsikWorldServer* pServer);
};