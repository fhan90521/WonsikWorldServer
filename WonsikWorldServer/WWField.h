#pragma once
#include "RoomSystem/Room.h"
#include "Container/MyStlContainer.h"
#include "GridSystem.h"
#include "WWEnum.h"
#include "WWVector2D.h"
#include <atomic>
class WWField :public Room
{
private:
	HashMap<SessionInfo::ID, SharedPtr<struct WWSession>> _players;
	GridSystem _gridSystem;
	class WonsikWorldServer* _wwServer;

	virtual void Update(float deltaTime) override;
	virtual void OnEnter(SessionInfo sessionInfo) override;
	virtual int RequestEnter(SessionInfo sessionInfo) override;
	virtual void OnLeave(SessionInfo sessionInfo) override;
	virtual void OnLeaveRoomSystem(SessionInfo sessionInfo, bool bEnterCompleted) override;
public:
	WWField(class WonsikWorldServer* pServer);
	virtual ~WWField();
	std::atomic<int> _sectorUpdateCnt = 0;
	void PrintFieldStatus();
	void InitMap(const int MapResource[10][10]);

private:
	//Sector ó��
	struct SectorAround
	{
		int cnt = 0;
		std::pair<int, int> around[9];
	};
	int _sectorMapHeight;
	int _sectorMapWidth;
	List<class WWPlayer*>** _sectorMap;
	void GetSectorAround(class WWPlayer* wwPlayer, SectorAround& sectorAround);
	void GetSectorAround(int x,int y, SectorAround& sectorAround);
	void GetUpdateSectorAround(class WWPlayer* wwPlayer, SectorAround& removeSector, SectorAround& addSector);
	void GetUpdateSectorByDir(int x, int y, SectorAround& sectorAround, const int* dxdyIndexArray, int arrayLen);
	void GetUpdateSectorByAllCheck(int beforeSectorX, int beforeSectorY, int afterSectorX, int afterSectorY, SectorAround& removeSector, SectorAround& addSector);
	bool CheckSectorUpdate(class WWPlayer* wwPlayer);
	void GetSessionInfoInAroundSector(List<SessionInfo>& sessionInfoListInAroundSector, WWPlayer* wwPlayer, bool bIncludeSelf);
	void UpdateSectorAround(class WWPlayer* wwPlayer);
public:
	//Job
	void SendChatMessage(SessionInfo sessionInfo, WString chatMessage);
	void SetCharacterDestination(SessionInfo sessionInfo, WWVector2D destination);
	void ChangeField(SessionInfo sessionInfo, int afterRoomID);
};