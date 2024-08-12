#pragma once
#include "MyStlContainer.h"
#include "Sector.h"
#include "Session.h"
class WWPlayer
{
private:
	WString _nickName;
	LONG64 _playerID;
	SessionInfo _sessionInfo;
	float _moveSpeed;
	std::pair<float, float> _dirVec;
	std::pair<float, float> _location;
	List<std::pair<float, float>> _destinations;
public:
	void SetNickName(const WString& nickName);
	const WString& GetNickNameRef();
	WString GetNickName();
	std::pair<float, float> GetLocation();
	void SetLocation(const std::pair<float, float>& location);
	void SetMoveSpeed(float speed);
	void SetDestinations(const List<std::pair<float, float>>& destinations,bool bIgnoreFirst);
	bool GetDestination(std::pair<float, float>& destination);
	bool GetDestinations(Vector<float>& destinationXs, Vector<float>& destinationYs);
	void Stop();
	bool IsMoving();
	void SetSessionInfo(SessionInfo sessionInfo);
	SessionInfo GetSessionInfo();
	void SetPlayerID(LONG64 id);
	LONG64 GetPlayerID();
	void SetDirVec(const std::pair<float, float>& dirVec);
	void SetDirVec(float x, float y);
	std::pair<float, float> GetDirVec();

	void Tick(float deltaTime);
	void Move(float deltaTime);
//섹터처리
public:
	SECTOR_POS sector;
};