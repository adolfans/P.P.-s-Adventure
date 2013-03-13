#include "StdAfx.h"
#include "MyGameSettings.h"
#include "MyGameFunctions.h"


MyGameSettings::~MyGameSettings(void)
{
}

MyGameSettings::MyGameSettings(string _scriptName)
	:wWidth(0), wHeight(0), /*wTittle(0), */gVersion(9)
{
	
	MyGameLua _config;

	_config.LoadAScriptFile( _scriptName );
	
	_config.PushGlobalByName("gversion");
	_config.PushGlobalByName("width");
	_config.PushGlobalByName("height");
	_config.PushGlobalByName("tittle");
	
	const char* tittle = _config.GetStringFromStack(-1);
	
	this->setWTittle(tittle);
	
	wHeight = _config.GetIntFromStack(-2);
	
	wWidth	= _config.GetIntFromStack(-3);
	
	gVersion = _config.GetIntFromStack(-4);
	
	_config.CleanStack();

}

void MyGameSettings::SetLoadedSettingsToInst(MyGameWindow* _pGameWindow)
{
	_pGameWindow->SetHeightClient(this->wHeight);
	_pGameWindow->SetWidthClient(this->wWidth);
	_pGameWindow->SetWindowTittle(this->wTittle.c_str());
}

void MyGameSettings::setWTittle(const char* _tittle)
{
	this->wTittle = _tittle;
}