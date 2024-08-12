// WonsikWorldServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "WonsikWorldServer.h"
#include "MyStlContainer.h"
#include "MakeShared.h"
#include "CCrashDump.h"
CCrashDump gCrashDump;
int main()
{
	WonsikWorldServer* pServer = new WonsikWorldServer;
	pServer->Run();
	while (pServer->_bShutdown == false)
	{
		if (pServer->ServerControl() == false)
		{
			pServer->PrintServerStatus();
		}
		Sleep(1000);
	}
	delete pServer;
	return 0;
}