#pragma once
#include "Session.h"
#include <memory>
#include <atomic>
enum class SessionType
{
	GUEST,
	PLAYER
};

struct WWSession
{
	SessionInfo sessionInfo;
	ULONG64 lastRecvTime;
	SessionType sessionType;
	int roomID;
	class WWPlayer* wwPlayer;
	virtual ~WWSession();
};