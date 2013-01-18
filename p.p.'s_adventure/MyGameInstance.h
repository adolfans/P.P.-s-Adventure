#pragma once
//#pragma comment (lib, "lua5.1.lib")
/*
#ifdef DX9
#include <d3d9.h>
#include <D3dx9math.h>
#pragma comment (lib, "D3D9.lib")
#pragma comment (lib, "d3dx9.lib")
#endif
#ifdef DX11
#include <D3D11.h>
#include <D3DX11.h>
#include <DXGI.h>
#endif
*/
#include "MyGame3DDevice.h"
#include "MyGameSettings.h"/*
#include "MyGameUISprite.h"
#include "MyGameUISpriteMain.h"
#include "MyGameUIText.h"
#include "MyGameMessage.h"
#include "MyGameLevel.h"
#include "MyGameMessageBox.h"
#include "MyGameNotify.h"*/
#include <RendererModules/Direct3D9/CEGUIDirect3D9Renderer.h>
#include <CEGUI.h>
#include "MyGameLevel.h"
typedef struct Vertex{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texCoord;
//	float u, v;
}Vertex;


class MyGameInstance
{
private:
	
	MyGameSettings config;

	MyGame3DDevice* pMyDevice;

	MyGameWindow*	pMyWindow;

	MyGameLevel* pLevel;


	//HRESULT UpdateScene_tmp2();

	//initialize the MyGameInstance object
	/*static	MyGameInstance*	InitMyGameInstance(
								HINSTANCE _hInstance,
								char* _winTittle,
								int _Width,
								int _Height,
								int _nCmdShow);*/

public:
	void RunLevel( MyGameLevel* level );
	void UpdateLevel( MyGameLevel* level );
	static  MyGameInstance* GetMyGameInstance(HINSTANCE _hInstance,
												int _nCmdShow);

	static	MyGameInstance* GetMyGameInstance();

	virtual void	InitGame();

	bool	MainLoop();
	

//	void	SetUpdateFunc( HRESULT (*Func)());
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
