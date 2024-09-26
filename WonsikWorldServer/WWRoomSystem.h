#pragma once
#include "RoomSystem/RoomSystem.h"
class WWRoomSystem : public RoomSystem
{
private:
	class WonsikWorldServer* _wwServer = nullptr;
	virtual void OnRegisterToLeave(SessionInfo sessionInfo) {};
	virtual void OnLeaveRoomSystem(SessionInfo sessionInfo) override;
	virtual bool CheckCanLeaveSystem(SessionInfo sessionInfo) override;
public:
	WWRoomSystem(class WonsikWorldServer* pServer);
};