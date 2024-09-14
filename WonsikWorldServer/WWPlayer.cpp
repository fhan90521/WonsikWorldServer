#include "WWPlayer.h"
#include "WWEnum.h"
#include "DebugTool/Log.h"
#include "GridSystem.h"
void WWPlayer::SetNickName(const WString& nickName)
{
    _nickName = nickName;
}
const WString& WWPlayer::GetNickNameRef()
{
    return _nickName;
}
WString WWPlayer::GetNickName()
{
    return _nickName;
}
WWVector2D WWPlayer::GetLocation()
{
    return _location;
}

void WWPlayer::SetLocation(const WWVector2D& location)
{
    _location = location;
}

void WWPlayer::SetMoveSpeed(float speed)
{
    _moveSpeed = speed;
}

void WWPlayer::SetDestinations(const List<WWVector2D>& destinations)
{
    _destinations = destinations;
}

void WWPlayer::SetDestinations(const Vector<WWVector2D>& destinations)
{
    _destinations.clear();
    _destinations.insert(_destinations.end(), destinations.begin(), destinations.end());
}

bool WWPlayer::GetDestination(WWVector2D& destination)
{
    if (_destinations.empty())
    {
        return false;
    }
    destination= _destinations.front();
    return true;
}

bool WWPlayer::GetDestinations(Vector<WWVector2D>& destinations)
{
    if (_destinations.empty())
    {
        return false;
    }
    destinations.push_back(_location);
    destinations.insert(destinations.end(), _destinations.begin(), _destinations.end());
    return true;
}

void WWPlayer::Stop()
{
    _destinations.clear();
}

bool WWPlayer::IsMoving()
{
    return _destinations.empty() == false;
}

void WWPlayer::SetSessionInfo(SessionInfo sessionInfo)
{
    _sessionInfo = sessionInfo;
}

SessionInfo WWPlayer::GetSessionInfo()
{
    return _sessionInfo;
}

void WWPlayer::SetPlayerID(LONG64 id)
{
    _playerID = id;
}

LONG64 WWPlayer::GetPlayerID()
{
    return _playerID;
}

void WWPlayer::SetDirVec(const WWVector2D& dirVec)
{
    _dirVec = dirVec;
}

void WWPlayer::SetDirVec(float x, float y)
{
    _dirVec._x = x;
    _dirVec._y = y;
}

WWVector2D WWPlayer::GetDirVec()
{
    return _dirVec;
}


void WWPlayer::Tick(float deltaTime)
{
    Move(deltaTime);
}

void WWPlayer::Move(float deltaTime)
{
    if (_destinations.size()>0)
    { 

        WWVector2D& destination = _destinations.front();
        WWVector2D dirVec = destination - _location;
        float distance = dirVec.Length();
        //도착했는지 확인
        if (GridSystem::IsSameGrid(destination._x, destination._y, _location._x, _location._y) == true && distance < CLOSE_DISTANCE )
        {
            _destinations.pop_front();
        }

        if (_destinations.size() > 0)
        {
            destination = _destinations.front();
            dirVec = destination - _location;
            dirVec.Normalize();
            _dirVec = dirVec;
            _location._x += _dirVec._x * _moveSpeed * deltaTime;
            _location._y += _dirVec._y * _moveSpeed * deltaTime;
            _location._x = max(0, _location._x);
            _location._x = min(MAP_WIDTH - 1, _location._x);
            _location._y = max(0, _location._y);
            _location._y = min(MAP_HEIGHT - 1, _location._y);
        }
    }
}

void WWPlayer::SetSectorPosition(int iX, int iY)
{
    _sectorPosition.first = iX;
    _sectorPosition.second = iY;
}

std::pair<int, int> WWPlayer::GetSectorPosition()
{
    return _sectorPosition;
}

int WWPlayer::GetSectorX()
{
    return _sectorPosition.first;
}

int WWPlayer::GetSectorY()
{
    return _sectorPosition.second;
}
