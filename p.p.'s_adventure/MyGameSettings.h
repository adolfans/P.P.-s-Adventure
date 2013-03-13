/*
load settings from a lua script
*/
#pragma once

#include <string>
using std::string;

#include "MyGameWindow.h"
#include "MyGameLua.h"
class MyGameSettings
{

private:

	int wWidth,wHeight;

	string wTittle;

	int gVersion;

	/*MyGameSettings(void);*/

	void setWTittle( const char* _tittle );

public:

	MyGameSettings(string _scriptName);

	void SetLoadedSettingsToInst(MyGameWindow*	_pGameWindow);
	
	~MyGameSettings(void);
};

