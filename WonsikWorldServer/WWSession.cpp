#include "WWSession.h"
#include "WWPlayer.h"
#include "Memory/MyNew.h"
WWSession::~WWSession()
{
	if (sessionType == SessionType::PLAYER)
	{
		Delete<WWPlayer>(wwPlayer);
	}
}
