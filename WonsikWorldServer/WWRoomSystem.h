#pragma once
#include "RoomSystem.h"
class WWRoomSystem : public RoomSystem
{
private:
	class WonsikWorldServer* _wwServer = nullptr;
	virtual void OnLeaveByChangingRoomSession(SessionInfo sessionInfo) override;
public:
	WWRoomSystem(class WonsikWorldServer* pServer);
};