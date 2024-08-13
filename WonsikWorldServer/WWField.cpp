#include "WWField.h"
#include "WonsikWorldServer.h"
#include "WWEnum.h"
#include "WWSession.h"
#include "WWPlayer.h"
#include "dXdY.h"
#include "Room.h"
#include "MathUtil.h"
#include "MapSource.h"
#include <cmath>
#include <format>
WWField::WWField(WonsikWorldServer* pServer):Room(pServer->GetCompletionPortHandle()),_gridSystem(MAP_HEIGHT/GRID_CELL_SIZE,MAP_WIDTH / GRID_CELL_SIZE, GRID_CELL_SIZE )
{
    _wwServer = pServer;
    _sectorMapHeight = MAP_HEIGHT / SECTOR_SIZE;
    _sectorMapWidth = MAP_WIDTH / SECTOR_SIZE;
    _sectorMap = new List<WWPlayer*>*[_sectorMapHeight+2];
    for (int i = 0; i < _sectorMapHeight+2; i++)
    {
        _sectorMap[i] = new List<WWPlayer*>[_sectorMapWidth+2];
    }
}

WWField::~WWField()
{
    for (int i = 0; i < _sectorMapHeight+2; i++)
    {
        delete[] _sectorMap[i];
    }
    delete[] _sectorMap;
}

void WWField::PrintFieldStatus()
{
	std::cout << std::format(R"(
--Field {} Status
PlayerNum: {} ,UpdateTps: {}, SectorUpdateTps: {}
)",GetRoomID(), _playerMap.size(), GetUpdateCnt(), _sectorUpdateCnt.load());
	_sectorUpdateCnt = 0;
}

void WWField::Update(float deltaTime)
{
	
	for (const auto& tempPair : _playerMap)
	{
		WWPlayer* wwPlayer=tempPair.second;
		wwPlayer->Tick(deltaTime);
		if (CheckSectorUpdate(wwPlayer) == true)
		{
			_sectorUpdateCnt++;
			UpdateSectorAround(wwPlayer);
		}
	}
}

void WWField::OnEnter(SessionInfo sessionInfo)
{
  SharedPtr<WWSession> wwSession = _wwServer->GetWWSession(sessionInfo);
  if (wwSession && wwSession->wwPlayer)
  {
	  int beforeRoomID=wwSession->roomID;
	  wwSession->roomID = GetRoomID();
	  _wwServer->ChangeMap_SC(wwSession->sessionInfo, beforeRoomID, wwSession->roomID);
	  
	  //g_cacheTracer.trace(pNewPlayer, "player");
	  WWPlayer* newPlayer = wwSession->wwPlayer;

	  //���� ���� ���ã��
	  std::pair<float, float> newLocation;
	  bool bFindNoneObstacle = false;
	  for (int i = 0; i < 10; i++)
	  {
		  newLocation.first = (rand() % (MAP_WIDTH));
		  newLocation.second = (rand() % (MAP_HEIGHT));
		  if (_gridSystem.IsObstacleByFloat(newLocation) == false)
		  {
			  bFindNoneObstacle = true;
			  break;
		  }
	  }
	  if (bFindNoneObstacle == false)
	  {
		  _gridSystem.GetNotObstacleLocation(newLocation);
	  }
		
	  //Debug��
	  //newLocation.first = 50;
	  //newLocation.second = 50;

	  //player Init
	  newPlayer->SetLocation(newLocation);
	  newPlayer->SetDirVec(DEFAULT_DIRX,DEFAULT_DIRY);
	  newPlayer->SetSectorPosition(newLocation.first / SECTOR_SIZE + 1, newLocation.second / SECTOR_SIZE + 1);
	  newPlayer->Stop();

	  //Log::LogOnConsole(Log::ERROR_LEVEL, "newPlayer ID:%d, roomID %d", newPlayer->GetPlayerID(), GetRoomID());
	  //����ڿ� ���� ���� �÷��̾�� ������Ʈ
	  _wwServer->CreateMyCharacter_SC(newPlayer->GetSessionInfo(), GetRoomID(), DEFAULT_DIRX, DEFAULT_DIRY, newLocation.first, newLocation.second);

	  SectorAround sectorAround;
	  GetSectorAround(newPlayer, sectorAround);


	  for (int i = 0; i < sectorAround.cnt; i++)
	  {

		  for (WWPlayer* pPlayer : _sectorMap[sectorAround.around[i].second][sectorAround.around[i].first])
		  {
			  if (pPlayer->GetPlayerID() == newPlayer->GetPlayerID())
			  {
				  continue;
			  }
			  //accept�� ������� �ٸ� player ������
			  auto pPlayerDirVec = pPlayer->GetDirVec();
			  auto pPlayerLocation = pPlayer->GetLocation();
			  _wwServer->CreateOtherCharacter_SC(newPlayer->GetSessionInfo(), GetRoomID(), pPlayer->GetPlayerID(), pPlayer->GetNickNameRef(), pPlayerDirVec.first, pPlayerDirVec.second, pPlayerLocation.first, pPlayerLocation.second);
			  if (pPlayer->IsMoving()==true)
			  {
				  Vector<float> pPlayerDestinationsX;
				  Vector<float> pPlayerDestinationsY;
				  pPlayer->GetDestinations(pPlayerDestinationsX, pPlayerDestinationsY);
				  _wwServer->MoveOtherCharacter_SC(newPlayer->GetSessionInfo(), GetRoomID(), pPlayer->GetPlayerID(), pPlayerDestinationsX, pPlayerDestinationsY);
			  }
			  //���������� ���ο� ĳ���� ������
			  _wwServer->CreateOtherCharacter_SC(pPlayer->GetSessionInfo(), GetRoomID(), newPlayer->GetPlayerID(), pPlayer->GetNickNameRef(), DEFAULT_DIRX, DEFAULT_DIRY, newLocation.first, newLocation.second);
		  }

	  }

	  _playerMap[sessionInfo.Id()] = newPlayer;
	  _sectorMap[newPlayer->GetSectorY()][newPlayer->GetSectorX()].push_back(newPlayer);
  }
  else
  {
	  _wwServer->Disconnect(sessionInfo);
  }
}

int WWField::RequestEnter(SessionInfo sessionInfo)
{
    return ENTER_SUCCESS;
}

void WWField::OnLeave(SessionInfo sessionInfo)
{
	auto iter=_playerMap.find(sessionInfo.Id());
	if (iter != _playerMap.end())
	{
		WWPlayer* pPlayer = iter->second;
		_playerMap.erase(iter);
		_sectorMap[pPlayer->GetSectorY()][pPlayer->GetSectorX()].remove(pPlayer);

		//Log::LogOnConsole(Log::ERROR_LEVEL, "removePlayer ID:%d, roomID %d", pPlayer->GetPlayerID(), GetRoomID());
		SectorAround sectorAround;
		GetSectorAround(pPlayer, sectorAround);

		for (int i = 0; i < sectorAround.cnt; i++)
		{
			for (WWPlayer* pAroundPlayer : _sectorMap[sectorAround.around[i].second][sectorAround.around[i].first])
			{
				_wwServer->DeleteCharacter_SC(pAroundPlayer->GetSessionInfo(),GetRoomID(), pPlayer->GetPlayerID());
			}
		}
	}
	
}

void WWField::OnLeaveRoomSystem(SessionInfo sessionInfo)
{
	OnLeave(sessionInfo);
	_wwServer->DeleteWWSession(sessionInfo);
}

void WWField::GetSectorAround(WWPlayer* wwPlayer, SectorAround& sectorAround)
{
	int index = 0;
	int sectorX = wwPlayer->GetSectorX();
	int sectorY = wwPlayer->GetSectorY();
	for (int dY = -1; dY <= 1; dY++)
	{
		for (int dX = -1; dX <= 1; dX++)
		{
			sectorAround.around[index].first = sectorX + dX;
			sectorAround.around[index].second = sectorY + dY;
			index++;
		}
	}
	sectorAround.cnt = 9;
}

void WWField::GetSectorAround(int x, int y, SectorAround& sectorAround)
{

	int index = 0;
	for (int dY = -1; dY <= 1; dY++)
	{
		for (int dX = -1; dX <= 1; dX++)
		{
			sectorAround.around[index].first = x + dX;
			sectorAround.around[index].second = y + dY;
			index++;
		}
	}
	sectorAround.cnt = 9;
}

void WWField::GetUpdateSectorAround(WWPlayer* wwPlayer, SectorAround& removeSector, SectorAround& addSector)
{
	auto location = wwPlayer->GetLocation();
	int currentSectorX = location.first/ SECTOR_SIZE + 1;
	int currentSectorY = location.second / SECTOR_SIZE + 1;
	int prevSectorX = wwPlayer->GetSectorX();
	int prevSectorY = wwPlayer->GetSectorY();
	int dX = currentSectorX - prevSectorX;
	int dY = currentSectorY - prevSectorY;

	if (dX == 1 && dY == 0)
	{
		//RR
		GetUpdateSectorByDir(prevSectorX, prevSectorY, removeSector, removeRR, NON_DIAGONOL_DELTA_SECTOR);
		GetUpdateSectorByDir(currentSectorX, currentSectorY, addSector, addRR, NON_DIAGONOL_DELTA_SECTOR);
	}
	else if (dX == 1 && dY == -1)
	{
		//RD
		GetUpdateSectorByDir(prevSectorX, prevSectorY,removeSector, removeRD, DIAGONOL_DELTA_SECTOR);
		GetUpdateSectorByDir(currentSectorX, currentSectorY, addSector, addRD, DIAGONOL_DELTA_SECTOR);
	}
	else if (dX == 0 && dY == -1)
	{
		//DD
		GetUpdateSectorByDir(prevSectorX, prevSectorY, removeSector, removeDD, NON_DIAGONOL_DELTA_SECTOR);
		GetUpdateSectorByDir(currentSectorX, currentSectorY, addSector, addDD, NON_DIAGONOL_DELTA_SECTOR);
	}
	else if (dX == -1 && dY == -1)
	{
		//LD
		GetUpdateSectorByDir(prevSectorX, prevSectorY, removeSector, removeLD, DIAGONOL_DELTA_SECTOR);
		GetUpdateSectorByDir(currentSectorX, currentSectorY, addSector, addLD, DIAGONOL_DELTA_SECTOR);
	}
	else if (dX == -1 && dY == 0)
	{
		//LL
		GetUpdateSectorByDir(prevSectorX, prevSectorY, removeSector, removeLL, NON_DIAGONOL_DELTA_SECTOR);
		GetUpdateSectorByDir(currentSectorX, currentSectorY, addSector, addLL, NON_DIAGONOL_DELTA_SECTOR);
	}
	else if (dX == -1 && dY == 1)
	{
		//LU
		GetUpdateSectorByDir(prevSectorX, prevSectorY, removeSector, removeLU, DIAGONOL_DELTA_SECTOR);
		GetUpdateSectorByDir(currentSectorX, currentSectorY, addSector, addLU, DIAGONOL_DELTA_SECTOR);
	}
	else if (dX == 0 && dY == 1)
	{
		//UU
		GetUpdateSectorByDir(prevSectorX, prevSectorY, removeSector, removeUU, NON_DIAGONOL_DELTA_SECTOR);
		GetUpdateSectorByDir(currentSectorX, currentSectorY, addSector, addUU, NON_DIAGONOL_DELTA_SECTOR);
	}
	else if (dX == 1 && dY == 1)
	{
		//RU
		GetUpdateSectorByDir(prevSectorX, prevSectorY, removeSector, removeRU, DIAGONOL_DELTA_SECTOR);
		GetUpdateSectorByDir(currentSectorX, currentSectorY, addSector, addRU, DIAGONOL_DELTA_SECTOR);
	}
	else
	{
		
		if (abs(dX) > 2 || abs(dY) > 2)
		{
			//���͸� 3ĭ�̻� �پ����� ��ġ�°� ����	
			GetSectorAround(wwPlayer, removeSector);
			GetSectorAround(currentSectorX, currentSectorY, addSector);
		}
		else
		{
			//���͸� ��ĭ �پ����� �׳� �������鼭 ���� üũ
			GetUpdateSectorByAllCheck(prevSectorX, prevSectorY, currentSectorX, currentSectorY, removeSector, addSector);
		}
	}

	_sectorMap[prevSectorY][prevSectorX].remove(wwPlayer);
	_sectorMap[currentSectorY][currentSectorX].push_back(wwPlayer);
	wwPlayer->SetSectorPosition(currentSectorX, currentSectorY);
}

void WWField::GetUpdateSectorByDir(int x, int y, SectorAround& sectorAround, const int* dxdyIndexArray, int arrayLen)
{
	for (int i = 0; i < arrayLen; i++)
	{
		sectorAround.around[i].first = x + dXdY[dxdyIndexArray[i]][0];
		sectorAround.around[i].second = y + dXdY[dxdyIndexArray[i]][1];
	}
	sectorAround.cnt = arrayLen;
}

void WWField::GetUpdateSectorByAllCheck(int beforeSectorX, int beforeSectorY, int afterSectorX, int afterSectorY, SectorAround& removeSector, SectorAround& addSector)
{
	using std::pair;
	Vector<pair<int,int>> beforeAroundSectors;
	Vector<pair<int,int>> afterAroundSectors;
	for (int iY = -1; iY <= 1; iY++)
	{
		for (int iX = -1; iX <= 1; iX++)
		{
			beforeAroundSectors.push_back(pair<int, int>(beforeSectorX + iX, beforeSectorY + iY));
			afterAroundSectors.push_back(pair<int, int>(afterSectorX + iX, afterSectorY + iY));
		}
	}

	for (int comparingIndex = 0; comparingIndex < beforeAroundSectors.size(); comparingIndex++)
	{
		bool IsBeforeComparingSectorRemoveSector = true;
		bool IsAfterComparingSectorAddSector = true;
		pair<int, int> beforeComparingSector = beforeAroundSectors[comparingIndex];
		pair<int, int> afterComparingSector = afterAroundSectors[comparingIndex];
		for (int i = 0; i < beforeAroundSectors.size(); i++)
		{
			if (beforeComparingSector.first == afterAroundSectors[i].first && beforeComparingSector.second == afterAroundSectors[i].second)
			{
				IsBeforeComparingSectorRemoveSector = false;
			}

			if (afterComparingSector.first == beforeAroundSectors[i].first && afterComparingSector.second == beforeAroundSectors[i].second)
			{
				IsAfterComparingSectorAddSector = false;
			}
			//if (IsBeforeComparingSectorRemoveSector == false && IsAfterComparingSectorAddSector == false)
			//{
			//	//��ĭ�̻� �پ��� ���� ��κ� �Ȱɸ�
			//	break;
			//}
		}
		if (IsBeforeComparingSectorRemoveSector == true)
		{
			removeSector.around[removeSector.cnt++] = beforeComparingSector;
		}
		if (IsAfterComparingSectorAddSector == true)
		{
			addSector.around[addSector.cnt++] = afterComparingSector;
		}

	}
}

void WWField::GetSessionInfoInAroundSector(List<SessionInfo>& sessionInfoListInAroundSector, WWPlayer* wwPlayer, bool bIncludeSelf)
{
	SectorAround sectorAround;
	GetSectorAround(wwPlayer, sectorAround);
	for (int i = 0; i < sectorAround.cnt; i++)
	{
		for (WWPlayer* aroundPlayer : _sectorMap[sectorAround.around[i].second][sectorAround.around[i].first])
		{
			if (bIncludeSelf == true)
			{
				sessionInfoListInAroundSector.push_back(aroundPlayer->GetSessionInfo());
			}
			else
			{
				if (aroundPlayer->GetPlayerID() != wwPlayer->GetPlayerID())
				{
					sessionInfoListInAroundSector.push_back(aroundPlayer->GetSessionInfo());
				}
			}
		}
	}
}

bool WWField::CheckSectorUpdate(WWPlayer* wwPlayer)
{
	auto location = wwPlayer->GetLocation();
	int currentSectorX = location.first / SECTOR_SIZE + 1;
	int currentSectorY = location.second / SECTOR_SIZE + 1;
	if (currentSectorX != wwPlayer->GetSectorX() || currentSectorY != wwPlayer->GetSectorY())
	{
		return true;
	}
	return false;
}

void WWField::UpdateSectorAround(WWPlayer* wwPlayer)
{
	SectorAround removeSector;
	SectorAround addSector;
	GetUpdateSectorAround(wwPlayer, removeSector, addSector);

	for (int i = 0; i < removeSector.cnt; i++)
	{
		for (WWPlayer* pRemovePlayer : _sectorMap[removeSector.around[i].second][removeSector.around[i].first])
		{
			Log::LogOnConsole(Log::DEBUG_LEVEL, "wwPlayerId :%d, wwPlayerX: %f wwPlayerY: %f, removePlayerID: %d, removeX : %f removeY: %f\n", wwPlayer->GetPlayerID(), wwPlayer->GetLocation().first, wwPlayer->GetLocation().second, pRemovePlayer->GetPlayerID(), pRemovePlayer->GetLocation().first, pRemovePlayer->GetLocation().second);
			if (pRemovePlayer->GetPlayerID() != wwPlayer->GetPlayerID())
			{
				_wwServer->DeleteCharacter_SC(wwPlayer->GetSessionInfo(), GetRoomID(), pRemovePlayer->GetPlayerID());
				_wwServer->DeleteCharacter_SC(pRemovePlayer->GetSessionInfo(), GetRoomID(), wwPlayer->GetPlayerID());
			}
		}
	}

	auto myDirVec = wwPlayer->GetDirVec();
	auto myLocation = wwPlayer->GetLocation();
	Vector<float> myDestinationXs;
	Vector<float> myDestinationYs;
	if (wwPlayer->IsMoving()==true)
	{
		wwPlayer->GetDestinations(myDestinationXs, myDestinationYs);
	}
	
	for (int i = 0; i < addSector.cnt; i++)
	{
		for (WWPlayer* pAddPlayer : _sectorMap[addSector.around[i].second][addSector.around[i].first])
		{
			if (wwPlayer->GetPlayerID() == pAddPlayer->GetPlayerID())
			{
				continue;
			}
			auto otherDirVec = pAddPlayer->GetDirVec();
			auto otherLocation = pAddPlayer->GetLocation();
			_wwServer->CreateOtherCharacter_SC(wwPlayer->GetSessionInfo(), GetRoomID(),pAddPlayer->GetPlayerID(), pAddPlayer->GetNickNameRef(), otherDirVec.first, otherDirVec.second, otherLocation.first, otherLocation.second);
			if (pAddPlayer->IsMoving())
			{
				Vector<float> otherDestinationXs;
				Vector<float> otherDestinationYs;
				pAddPlayer->GetDestinations(otherDestinationXs, otherDestinationYs);
				_wwServer->MoveOtherCharacter_SC(wwPlayer->GetSessionInfo(), GetRoomID(), pAddPlayer->GetPlayerID(), otherDestinationXs, otherDestinationYs);
			}

			_wwServer->CreateOtherCharacter_SC(pAddPlayer->GetSessionInfo(), GetRoomID(),wwPlayer->GetPlayerID(), wwPlayer->GetNickNameRef(), myDirVec.first, myDirVec.second,myLocation.first,myLocation.second);
			if (wwPlayer->IsMoving() == true)
			{
				_wwServer->MoveOtherCharacter_SC(pAddPlayer->GetSessionInfo(), GetRoomID(), wwPlayer->GetPlayerID(), myDestinationXs, myDestinationYs);
			}

			Log::LogOnConsole(Log::DEBUG_LEVEL, "wwPlayerId :%d, wwPlayerX: %f wwPlayerY: %f, addPlayerID: %d, pAddPlayer : %f pAddPlayer: %f\n", wwPlayer->GetPlayerID(), wwPlayer->GetLocation().first, wwPlayer->GetLocation().second, pAddPlayer->GetPlayerID(), pAddPlayer->GetLocation().first, pAddPlayer->GetLocation().second);

		}
	}
}

void WWField::InitMap(const int MapResource[10][10])
{
	for (int iY = 0; iY < 10; iY++)
	{
		for (int iX = 0; iX < 10; iX++)
		{
			if (MapResource[iY][iX] > 0)
			{
				for (int YPlus = 0; YPlus <= 80; YPlus += 20)
				{
					for (int XPlus = 0; XPlus <= 80; XPlus += 20)
					{
						_gridSystem.SetObstacle(iX*2+XPlus,iY*2+ YPlus,true);
						_gridSystem.SetObstacle(iX*2+1+ XPlus, iY*2+ YPlus, true);
						_gridSystem.SetObstacle(iX*2+ XPlus, iY*2+1+ YPlus, true);
						_gridSystem.SetObstacle(iX*2+1+ XPlus, iY*2+1+ YPlus, true);
					}
				}
			}
		}
	}
}

void WWField::SendChatMessage(SharedPtr<struct WWSession>& wwSession, WString& chatMessage)
{
	List<SessionInfo> sessionInfoListInAroundSector;
	GetSessionInfoInAroundSector(sessionInfoListInAroundSector, wwSession->wwPlayer, false);
	_wwServer->SendChatMessage_SC(sessionInfoListInAroundSector, GetRoomID(), wwSession->wwPlayer->GetPlayerID(), chatMessage);
}

void WWField::SetCharacterDestination(SharedPtr<WWSession>& wwSession, float destinationX, float destinationY)
{
	//destination üũ�Ŀ��ʿ�� ����ȯ
	destinationX = max(0, destinationX);
	destinationX = min(MAP_WIDTH - 1, destinationX);
	destinationY = max(0, destinationY);
	destinationY = min(MAP_HEIGHT - 1, destinationY);
	
	

	//������ �Ÿ����� üũ
	std::pair<float, float> playerLocation = wwSession->wwPlayer->GetLocation();
	//Log::LogOnConsole(Log::DEBUG_LEVEL, "serverlocationX: %f serverLocationY: %f destinationX : %f destinationY: %f\n", playerLocation.first, playerLocation.second, destinationX, destinationY);
	
	float distanceToDest = GetDistanceBetweenTwoPoint(playerLocation.first, playerLocation.second, destinationX, destinationY);
	if (distanceToDest > STRANGE_DISTANCE_TO_DEST)
	{
		_wwServer->Disconnect(wwSession->sessionInfo);
		return;
	}

	//��ã��
	List<std::pair<float, float>> pathPoints;
	Vector<float> destinationXs;
	Vector<float> destinationYs;

	if (_gridSystem.FindPath(playerLocation.first, playerLocation.second, destinationX, destinationY, pathPoints) == true)
	{
		//�÷��̾��� ������ ���� pathPoints[0] �� ��� �����̾����Ƿ� �տ� �ϳ��� �����ؼ� �÷��̾��� ������ ����
		wwSession->wwPlayer->SetDestinations(pathPoints,true);
		
		//��Ŷ����
		for (auto& pathPoint : pathPoints)
		{
			destinationXs.push_back(pathPoint.first);
			destinationYs.push_back(pathPoint.second);
		}
		List<SessionInfo> sessionInfoListInAroundSector;
		GetSessionInfoInAroundSector(sessionInfoListInAroundSector, wwSession->wwPlayer, false);
		_wwServer->MoveMyCharacter_SC(wwSession->sessionInfo, GetRoomID(), destinationXs, destinationYs);
		_wwServer->MoveOtherCharacter_SC(sessionInfoListInAroundSector, GetRoomID(), wwSession->wwPlayer->GetPlayerID(), destinationXs, destinationYs);

	}
	else
	{
		// ���߰� �����̴� ���������� ����� �� ���� �ڵ�
		// ���� ��ã���� ������ �������� ���� ���� ��ã�Ҵٴ� ��Ŷ�� ������ �ϴ°�? �׳� �÷��̾ Ŭ���Ѱ��� �̵����ϴ� ���̶�°� �˾Ƽ� �ν����� ������
		// Ŭ���̾�Ʈ�� ���߰� ��ã�� ��û�� �ϰ� ������ ���� ��ã���� ���߰ԵǸ� obstacle�� �׸��忡 ���߰� �Ǵ� ���� ���� ���ڿ������� ����� �����Ͼ�� �Ѵ� ���������� 
		// ���� �� ã���� ���� �ƹ��ϵ� �Ͼ�� �ʰ���.
		//if (pathPoints.size() == 1)
		//{
		//	//�������� obstacle�� ������ ��
		//	wwSession->wwPlayer->SetLocation(pathPoints.front());
		//	destinationXs.push_back(pathPoints.front().first);
		//	destinationYs.push_back(pathPoints.front().second);
		//	destinationXs.push_back(pathPoints.front().first);
		//	destinationYs.push_back(pathPoints.front().second);
		//	List<SessionInfo> sessionInfoListInAroundSector;
		//	GetSessionInfoInAroundSector(sessionInfoListInAroundSector, wwSession->wwPlayer, false);
		//	_wwServer->MoveMyCharacter_SC(wwSession->sessionInfo, GetRoomID(), destinationXs, destinationYs);
		//	_wwServer->MoveOtherCharacter_SC(sessionInfoListInAroundSector, GetRoomID(), wwSession->wwPlayer->GetPlayerID(), destinationXs, destinationYs);
		//}
		//else
		//{
		//	
		//	//wwSession->wwPlayer->Stop();
		//}
	}
}

void WWField::ChangeField(SharedPtr<WWSession>& wwSession, int afterMapID)
{
	if (ChangeRoom(wwSession->sessionInfo, afterMapID) == false)
	{
		_wwServer->Disconnect(wwSession->sessionInfo);
	}
}

