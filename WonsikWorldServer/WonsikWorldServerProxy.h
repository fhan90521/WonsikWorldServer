#pragma once
#include "Network/Session.h"
#include "Container/MyStlContainer.h"
class WWVector2D;
class WonsikWorldServerProxy
{
private:
	class IOCPServer* _pServer;
public:
	void EnterGame_CS(SessionInfo sessionInfo, const WString& nickName, bool bDisconnect = false);
	void EnterGame_CS(const List<SessionInfo>& sessionInfoList, const WString& nickName, bool bDisconnect = false);

	void EnterGame_SC(SessionInfo sessionInfo, short enterGameResult, LONG64 playerID, bool bDisconnect = false);
	void EnterGame_SC(const List<SessionInfo>& sessionInfoList, short enterGameResult, LONG64 playerID, bool bDisconnect = false);

	void CreateMyCharacter_SC(SessionInfo sessionInfo, short mapID, const WWVector2D& dirVec, const WWVector2D& location, bool bDisconnect = false);
	void CreateMyCharacter_SC(const List<SessionInfo>& sessionInfoList, short mapID, const WWVector2D& dirVec, const WWVector2D& location, bool bDisconnect = false);

	void CreateOtherCharacter_SC(SessionInfo sessionInfo, short mapID, LONG64 playerID, const WString& nickName, const WWVector2D& dirVec, const WWVector2D& location, bool bDisconnect = false);
	void CreateOtherCharacter_SC(const List<SessionInfo>& sessionInfoList, short mapID, LONG64 playerID, const WString& nickName, const WWVector2D& dirVec, const WWVector2D& location, bool bDisconnect = false);

	void DeleteCharacter_SC(SessionInfo sessionInfo, short mapID, LONG64 playerID, bool bDisconnect = false);
	void DeleteCharacter_SC(const List<SessionInfo>& sessionInfoList, short mapID, LONG64 playerID, bool bDisconnect = false);

	void ChangeMap_CS(SessionInfo sessionInfo, short beforeMapID, short afterMapID, bool bDisconnect = false);
	void ChangeMap_CS(const List<SessionInfo>& sessionInfoList, short beforeMapID, short afterMapID, bool bDisconnect = false);

	void ChangeMap_SC(SessionInfo sessionInfo, short beforeMapID, short afterMapID, bool bDisconnect = false);
	void ChangeMap_SC(const List<SessionInfo>& sessionInfoList, short beforeMapID, short afterMapID, bool bDisconnect = false);

	void SendChatMessage_CS(SessionInfo sessionInfo, short mapID, const WString& chatMessage, bool bDisconnect = false);
	void SendChatMessage_CS(const List<SessionInfo>& sessionInfoList, short mapID, const WString& chatMessage, bool bDisconnect = false);

	void SendChatMessage_SC(SessionInfo sessionInfo, short mapID, LONG64 playerID, const WString& chatMessage, bool bDisconnect = false);
	void SendChatMessage_SC(const List<SessionInfo>& sessionInfoList, short mapID, LONG64 playerID, const WString& chatMessage, bool bDisconnect = false);

	void MoveMyCharacter_CS(SessionInfo sessionInfo, short mapID, const WWVector2D& destination, bool bDisconnect = false);
	void MoveMyCharacter_CS(const List<SessionInfo>& sessionInfoList, short mapID, const WWVector2D& destination, bool bDisconnect = false);

	void MoveMyCharacter_SC(SessionInfo sessionInfo, short mapID, const Vector<WWVector2D>& destinations, bool bDisconnect = false);
	void MoveMyCharacter_SC(const List<SessionInfo>& sessionInfoList, short mapID, const Vector<WWVector2D>& destinations, bool bDisconnect = false);

	void MoveOtherCharacter_SC(SessionInfo sessionInfo, short mapID, LONG64 playerID, const Vector<WWVector2D>& destinations, bool bDisconnect = false);
	void MoveOtherCharacter_SC(const List<SessionInfo>& sessionInfoList, short mapID, LONG64 playerID, const Vector<WWVector2D>& destinations, bool bDisconnect = false);

	void HeartBeat_CS(SessionInfo sessionInfo, bool bDisconnect = false);
	void HeartBeat_CS(const List<SessionInfo>& sessionInfoList, bool bDisconnect = false);

	WonsikWorldServerProxy(class IOCPServer* pServer)
	{
		_pServer = pServer;
	}
};
