#pragma once
#include "Room.h"
#include "MyStlContainer.h"
#include "GridSystem.h"
#include "WWEnum.h"
#include "Sector.h"
#include <atomic>
class WWField :public Room
{
public:
	WWField(class WonsikWorldServer* pServer);
	virtual ~WWField();
	std::atomic<int> _sectorUpdateCnt = 0;
	void PrintFieldStatus();
private:
	//Update���� ���������� ���鼭 move�� ���Ѿ������� PLAYER_MAP�̳� SET�� �ʿ��ϴ�.
	HashMap<SessionInfo::ID, class WWPlayer*> _playerMap;
	GridSystem _gridSystem;
	class WonsikWorldServer* _wwServer;
	
	virtual void Update(float deltaTime) override;
	virtual void OnEnter(SessionInfo sessionInfo) override;
	virtual int RequestEnter(SessionInfo sessionInfo) override;
	virtual void OnLeave(SessionInfo sessionInfo) override;
	virtual void OnLeaveRoomSystem(SessionInfo sessionInfo) override;
private:
	//Sector ó��
	int _sectorMapHeight;
	int _sectorMapWidth;
	List<class WWPlayer*>** _sectorMap;
	void GetSectorAround(class WWPlayer* wwPlayer, SECTOR_AROUND& sectorAround);
	void GetSectorAround(int x,int y, SECTOR_AROUND& sectorAround);
	void GetUpdateSectorAround(class WWPlayer* wwPlayer, SECTOR_AROUND& removeSector, SECTOR_AROUND& addSector);
	void GetUpdateSectorByDir(int x, int y, SECTOR_AROUND& sectorAround, const int* dxdyIndexArray, int arrayLen);
	void GetUpdateSectorByAllCheck(int beforeSectorX, int beforeSectorY, int afterSectorX, int afterSectorY, SECTOR_AROUND& removeSector, SECTOR_AROUND& addSector);
	bool CheckSectorUpdate(class WWPlayer* wwPlayer);
	void GetSessionInfoInAroundSector(List<SessionInfo>& sessionInfoListInAroundSector, WWPlayer* wwPlayer, bool bIncludeSelf);
	void UpdateSectorAround(class WWPlayer* wwPlayer);
public:
	void SendChatMessage(SharedPtr<struct WWSession> wwSession, WString chatMessage);
	void SetCharacterDestination(SharedPtr<struct WWSession> wwSession, float destinationX, float destinationY);
	void ChangeField(SharedPtr<struct WWSession> wwSession, int afterRoomID);
	void InitMap(const int MapResource[10][10]);
};