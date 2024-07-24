#include "WWPlayer.h"
#include "MathUtil.h"
#include "WWEnum.h"
#include "Log.h"
std::pair<float, float> WWPlayer::GetLocation()
{
    return _location;
}

void WWPlayer::SetLocation(const std::pair<float, float>& location)
{
    _location = location;
}

void WWPlayer::SetMoveSpeed(float speed)
{
    _moveSpeed = speed;
}

void WWPlayer::SetDestinations(const List<std::pair<float, float>>& destinations, bool bIgnoreFirst)
{
    if (destinations.empty())
    {
        return;
    }

    //������ ����
    if (bIgnoreFirst)
    {
        //pathPoint[0] ù��°�� ������ġ�� �����ϴ°��
        _destinations.clear();
        auto iterInput = destinations.begin();
        iterInput++;
        for (; iterInput != destinations.end(); iterInput++)
        {
            _destinations.push_back(*iterInput);
        }
    }
    else
    {
        _destinations = destinations;
    }
}

bool WWPlayer::GetDestination(std::pair<float, float>& destination)
{
    if (_destinations.empty())
    {
        return false;
    }
    destination= _destinations.front();
    return true;
}

bool WWPlayer::GetDestinations(Vector<float>& destinationXs, Vector<float>& destinationYs)
{
    if (_destinations.empty())
    {
        return false;
    }
    for (auto& destination : _destinations)
    {
        destinationXs.push_back(destination.first);
        destinationYs.push_back(destination.second);
    }
    return true;
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

void WWPlayer::SetDirVec(const std::pair<float, float>& dirVec)
{
    _dirVec = dirVec;
}

void WWPlayer::SetDirVec(float x, float y)
{
    _dirVec.first = x;
    _dirVec.second = y;
}

std::pair<float, float> WWPlayer::GetDirVec()
{
    return _dirVec;
}


void WWPlayer::Move(float deltaTime)
{
    if (_destinations.size()>0)
    { 
        std::pair<float, float>& destination = _destinations.front();
        //�ϴ� dirVec�� �̵�
        std::pair<float, float> newDirVec; 
        newDirVec.first = destination.first - _location.first;
        newDirVec.second = destination.second - _location.second;
        Normalize(newDirVec);
        _dirVec = newDirVec;

        _location.first += _dirVec.first * _moveSpeed * deltaTime;
        _location.second += _dirVec.second * _moveSpeed * deltaTime;
   
        _location.first = max(0, _location.first);
        _location.first = min(MAP_WIDTH - 1, _location.first);
        _location.second = max(0, _location.second);
        _location.second = min(MAP_HEIGHT - 1, _location.second);

        //Log::LogOnConsole(Log::DEBUG_LEVEL, "locationX : %f locationY : %f, dirVecX: %f, dirVecY: %f, deltaTime: %f\n", _location.first, _location.second,_dirVec.first,_dirVec.second, deltaTime);
      
   
        float distance = GetDistanceBetweenTwoPoint(destination.first, destination.second, _location.first, _location.second);
        //Log::LogOnConsole(Log::DEBUG_LEVEL, "distance: %f\n", distance);
        //�����ߴ��� Ȯ��
        if (IsSameGrid(destination.first, destination.second, _location.first, _location.second, GRID_CELL_SIZE) == true &&
            GetDistanceBetweenTwoPoint(destination.first, destination.second, _location.first, _location.second) < CLOSE_DISTANCE)
        {
            
            _destinations.pop_front();
        }

    }
}