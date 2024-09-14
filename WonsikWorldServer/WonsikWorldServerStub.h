#pragma once
#include "Network/Session.h"
#include "Buffer/CRecvBuffer.h"
#include "Container/MyStlContainer.h"
#include "WonsikWorldPKT_TYPE.h"
class WWVector2D;
class WonsikWorldServerStub
{
public:
	bool PacketProcEnterGame_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcEnterGame_CS(SessionInfo sessionInfo, int roomID, WString& nickName) {}

	bool PacketProcEnterGame_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcEnterGame_SC(SessionInfo sessionInfo, int roomID, short enterGameResult, LONG64 playerID) {}

	bool PacketProcCreateMyCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcCreateMyCharacter_SC(SessionInfo sessionInfo, int roomID, short mapID, WWVector2D& dirVec, WWVector2D& location) {}

	bool PacketProcCreateOtherCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcCreateOtherCharacter_SC(SessionInfo sessionInfo, int roomID, short mapID, LONG64 playerID, WString& nickName, WWVector2D& dirVec, WWVector2D& location) {}

	bool PacketProcDeleteCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcDeleteCharacter_SC(SessionInfo sessionInfo, int roomID, short mapID, LONG64 playerID) {}

	bool PacketProcChangeMap_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcChangeMap_CS(SessionInfo sessionInfo, int roomID, short beforeMapID, short afterMapID) {}

	bool PacketProcChangeMap_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcChangeMap_SC(SessionInfo sessionInfo, int roomID, short beforeMapID, short afterMapID) {}

	bool PacketProcSendChatMessage_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcSendChatMessage_CS(SessionInfo sessionInfo, int roomID, short mapID, WString& chatMessage) {}

	bool PacketProcSendChatMessage_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcSendChatMessage_SC(SessionInfo sessionInfo, int roomID, short mapID, LONG64 playerID, WString& chatMessage) {}

	bool PacketProcMoveMyCharacter_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcMoveMyCharacter_CS(SessionInfo sessionInfo, int roomID, short mapID, WWVector2D& destination) {}

	bool PacketProcMoveMyCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcMoveMyCharacter_SC(SessionInfo sessionInfo, int roomID, short mapID, Vector<WWVector2D>& destinations) {}

	bool PacketProcMoveOtherCharacter_SC(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcMoveOtherCharacter_SC(SessionInfo sessionInfo, int roomID, short mapID, LONG64 playerID, Vector<WWVector2D>& destinations) {}

	bool PacketProcHeartBeat_CS(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
	virtual void ProcHeartBeat_CS(SessionInfo sessionInfo, int roomID) {}

	bool PacketProc(SessionInfo sessionInfo, int roomID, CRecvBuffer& buf);
};
