// testMyWorld1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include "MyGameInstance.h"
#include "MyGameSettings.h"
#include "MyGameLevelMainMenu.h"
#include "MyGameFunctions.h"
//#include "MyGameUISpriteMain.h"
#ifdef _DEBUG
#include <vld.h>
#endif
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	setlocale(LC_ALL, "zh_CN");
	//_CrtDumpMemoryLeaks();
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );  
	//_CrtSetBreakAlloc(189);
	MyGameInstance* newGame = MyGameInstance::GetMyGameInstance(hInstance, nCmdShow);
	newGame->InitGame();
	MyGameLevelMainMenu *level1 = new MyGameLevelMainMenu;
	level1->InitLevel();
	newGame->RunLevel(level1);
	//delete level1;
	//newGame->MainLoop();
	delete newGame;

}

