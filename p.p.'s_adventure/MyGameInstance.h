#pragma once
#include "MyGame3DDevice.h"
#include "MyGameSettings.h"
#include <RendererModules/Direct3D9/CEGUIDirect3D9Renderer.h>
#include <CEGUI.h>
#include "MyGameLevel.h"
typedef struct Vertex{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texCoord;
}Vertex;


class MyGameInstance
{
private:
	
	MyGameSettings config;

	MyGame3DDevice* pMyDevice;

	MyGameWindow*	pMyWindow;

	MyGameLevel* pLevel;
public:
	void RunLevel( MyGameLevel* level );
	void UpdateLevel( MyGameLevel* level, MSG msg );
	static  MyGameInstance* GetMyGameInstance(HINSTANCE _hInstance,
												int _nCmdShow);

	static	MyGameInstance* GetMyGameInstance();

	virtual void	InitGame();

	bool	MainLoop();
	
	MyGameInstance(void);
	virtual ~MyGameInstance(void);
	void	SetInstanceAndCmdLine( HINSTANCE _hInstance, int _nCmdShow )
	{
		pMyWindow->SetInstance( _hInstance );
		pMyWindow->SetCmdShow( _nCmdShow );
	}

protected:

	//pointer to the MyGameInstance object
	static MyGameInstance*	pMyGameInstance;
};
