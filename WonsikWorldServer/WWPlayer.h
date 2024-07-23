#pragma once
#include "MyStlContainer.h"
#include "Sector.h"
#include "Session.h"
class WWPlayer
{
private:
	LONG64 _playerID;
	SessionInfo _sessionInfo;
	float _moveSpeed;
	std::pair<float, float> _dirVec;
	std::pair<float, float> _location;
	List<std::pair<float, float>> _destinations;
public:
	std::pair<float, float> GetLocation();
	void SetLocation(const std::pair<float, float>& location);
	void SetMoveSpeed(float speed);
	void SetDestinations(const List<std::pair<float, float>>& destinations,bool bIgnoreFirst);
	bool GetDestination(std::pair<float, float>& destination);
	bool GetDestinations(Vector<float>& destinationXs, Vector<float>& destinationYs);
	bool IsMoving();
	void SetSessionInfo(SessionInfo sessionInfo);
	SessionInfo GetSessionInfo();
	void SetPlayerID(LONG64 id);
	LONG64 GetPlayerID();
	void SetDirVec(const std::pair<float, float>& dirVec);
	void SetDirVec(float x, float y);
	std::pair<float, float> GetDirVec();

	void Move(float deltaTime);
public:
	//복사를 줄이기 위해
	WString _nickName;
//섹터처리
public:
	SECTOR_POS sector;
};