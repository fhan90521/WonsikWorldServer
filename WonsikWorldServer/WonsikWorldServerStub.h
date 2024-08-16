#pragma once
#include "Session.h"
#include "CRecvBuffer.h"
#include "MyStlContainer.h"
#include "WonsikWorldPKT_TYPE.h"
class WWVector2D;
class WonsikWorldServerStub
{
public:
	bool PacketProcEnterGame_CS(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcEnterGame_CS(SessionInfo sessionInfo, WString& nickName) {}

	bool PacketProcEnterGame_SC(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcEnterGame_SC(SessionInfo sessionInfo, short enterGameResult, LONG64 playerID) {}

	bool PacketProcCreateMyCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcCreateMyCharacter_SC(SessionInfo sessionInfo, short mapID, WWVector2D& dirVec, WWVector2D& location) {}

	bool PacketProcCreateOtherCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcCreateOtherCharacter_SC(SessionInfo sessionInfo, short mapID, LONG64 playerID, WString& nickName, WWVector2D& dirVec, WWVector2D& location) {}

	bool PacketProcDeleteCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcDeleteCharacter_SC(SessionInfo sessionInfo, short mapID, LONG64 playerID) {}

	bool PacketProcChangeMap_CS(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcChangeMap_CS(SessionInfo sessionInfo, short beforeMapID, short afterMapID) {}

	bool PacketProcChangeMap_SC(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcChangeMap_SC(SessionInfo sessionInfo, short beforeMapID, short afterMapID) {}

	bool PacketProcSendChatMessage_CS(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcSendChatMessage_CS(SessionInfo sessionInfo, short mapID, WString& chatMessage) {}

	bool PacketProcSendChatMessage_SC(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcSendChatMessage_SC(SessionInfo sessionInfo, short mapID, LONG64 playerID, WString& chatMessage) {}

	bool PacketProcMoveMyCharacter_CS(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcMoveMyCharacter_CS(SessionInfo sessionInfo, short mapID, WWVector2D& destination) {}

	bool PacketProcMoveMyCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcMoveMyCharacter_SC(SessionInfo sessionInfo, short mapID, Vector<WWVector2D>& destinations) {}

	bool PacketProcMoveOtherCharacter_SC(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcMoveOtherCharacter_SC(SessionInfo sessionInfo, short mapID, LONG64 playerID, Vector<WWVector2D>& destinations) {}

	bool PacketProcHeartBeat_CS(SessionInfo sessionInfo, CRecvBuffer& buf);
	virtual void ProcHeartBeat_CS(SessionInfo sessionInfo) {}

	bool PacketProc(SessionInfo sessionInfo, CRecvBuffer& buf);
};
