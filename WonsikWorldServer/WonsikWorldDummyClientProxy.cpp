#include "WonsikWorldDummyClientProxy.h"
#include "WonsikWorldPKT_TYPE.h"
void WonsikWorldDummyClientProxy::EnterGame_CS(WString& nickName, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_EnterGame_CS << nickName;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::EnterGame_SC(short enterGameResult, LONG64 playerID, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_EnterGame_SC << enterGameResult << playerID;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::CreateMyCharacter_SC(short mapID, float dirX, float dirY, float locationX, float locationY, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_CreateMyCharacter_SC << mapID << dirX << dirY << locationX << locationY;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::CreateOtherCharacter_SC(short mapID, LONG64 playerID, WString& nickName, float dirX, float dirY, float locationX, float locationY, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_CreateOtherCharacter_SC << mapID << playerID << nickName << dirX << dirY << locationX << locationY;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::DeleteCharacter_SC(short mapID, LONG64 playerID, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_DeleteCharacter_SC << mapID << playerID;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::ChangeMap_CS(short beforeMapID, short afterMapID, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_ChangeMap_CS << beforeMapID << afterMapID;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::ChangeMap_SC(short beforeMapID, short afterMapID, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_ChangeMap_SC << beforeMapID << afterMapID;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::SendChatMessage_CS(short mapID, WString& chatMessage, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_SendChatMessage_CS << mapID << chatMessage;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::SendChatMessage_SC(short mapID, LONG64 playerID, WString& chatMessage, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_SendChatMessage_SC << mapID << playerID << chatMessage;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::MoveMyCharacter_CS(short mapID, float destinationX, float destinationY, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_MoveMyCharacter_CS << mapID << destinationX << destinationY;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::MoveMyCharacter_SC(short mapID, Vector<float>& destinationsX, Vector<float>& destinationsY, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_MoveMyCharacter_SC << mapID << destinationsX << destinationsY;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::MoveOtherCharacter_SC(short mapID, LONG64 playerID, Vector<float>& destinationsX, Vector<float>& destinationsY, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_MoveOtherCharacter_SC << mapID << playerID << destinationsX << destinationsY;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
void WonsikWorldDummyClientProxy::HeartBeat_CS(, bool bDisconnect)
{
	CSendBuffer* pBuf = CSendBuffer::Alloc();
	pBuf->IncrementRefCnt();
	try
	{
		*pBuf << PKT_TYPE_HeartBeat_CS;
	}
	catch(int useSize)
	{
	}
	_pClient->Unicast(pBuf, bDisconnect);
	pBuf->DecrementRefCnt();
}
