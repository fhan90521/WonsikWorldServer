#pragma once
#include "Network/Session.h"
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
	SessionType sessionType;
	int roomID;
	class WWPlayer* wwPlayer;
	~WWSession();
};