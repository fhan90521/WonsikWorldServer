#pragma once
#include "MyStlContainer.h"
#include "Session.h"
#include "WWVector2D.h"
class WWPlayer
{
private:
	WString _nickName;
	LONG64 _playerID;
	SessionInfo _sessionInfo;
	float _moveSpeed;
	WWVector2D _dirVec;
	WWVector2D _location;
	List<WWVector2D> _destinations;
public:
	void SetNickName(const WString& nickName);
	const WString& GetNickNameRef();
	WString GetNickName();
	WWVector2D GetLocation();
	void SetLocation(const WWVector2D& location);
	void SetMoveSpeed(float speed);
	void SetDestinations(const List<WWVector2D>& destinations);
	void SetDestinations(const Vector<WWVector2D>& destinations);
	bool GetDestination(WWVector2D& destination);
	bool GetDestinations(Vector<WWVector2D>& destinations);
	void Stop();
	bool IsMoving();
	void SetSessionInfo(SessionInfo sessionInfo);
	SessionInfo GetSessionInfo();
	void SetPlayerID(LONG64 id);
	LONG64 GetPlayerID();
	void SetDirVec(const WWVector2D& dirVec);
	void SetDirVec(float x, float y);
	WWVector2D GetDirVec();

	void Tick(float deltaTime);
	void Move(float deltaTime);
//섹터처리
private:
	std::pair<int,int> _sectorPosition;
public:
	void SetSectorPosition(int iX, int iY);
	std::pair<int, int> GetSectorPosition();
	int GetSectorX();
	int GetSectorY();
};