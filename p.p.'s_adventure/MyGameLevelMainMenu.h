#pragma once
#include "mygamelevel.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <CEGUI.h>
	using namespace CEGUI;
class MyGameLevelMainMenu :
	public MyGameLevel
{
private:
	
	ID3DXEffect* pBloomEffect;
	ID3DXEffect* pTransformEffect;
	IDirect3DVertexBuffer9* pVertexBuffer;
	IDirect3DTexture9* pBgTexture;
	IDirect3DSurface9* pScreenRenderSurface;
	IDirect3DTexture9* pPass1RenderTarget;
	IDirect3DSurface9* pPass1RenderTargetSurface;
	
	
	IDirect3DVertexDeclaration9* pMyGameVertexDecl;

	CEGUI::Event::Connection myconn;
	Window* myRoot;
public:
	MyGameLevelMainMenu(void);
	~MyGameLevelMainMenu(void);
	virtual void InitLevel(){}
	void Update( MSG msg ){;}
	virtual void Render();

	bool OnStart( const CEGUI::EventArgs& );

	bool Button1Clicked(const CEGUI::EventArgs&);
	bool Button2Clicked(const CEGUI::EventArgs&);
	bool Button3Clicked(const CEGUI::EventArgs&);
	bool Button4Clicked(const CEGUI::EventArgs&);
	bool Button5Clicked(const CEGUI::EventArgs&);

};

