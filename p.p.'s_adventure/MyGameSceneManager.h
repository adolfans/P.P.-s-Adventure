#pragma once
//#include "MyGameSceneNode.h"
#include "MyGameScene.h"
#include "MyGameSceneNode.h"
#include "d3dx9.h"
namespace MyGameScene{
class MyGameSceneManager
{
private:
	static D3DXMATRIX vMat;
	static D3DXMATRIX pMat;
	static D3DXMATRIX lightViewProjMat;
	static IDirect3DTexture9* shadowMap;
public:
	MyGameSceneManager(void);
	~MyGameSceneManager(void);

	static MyGameSceneNode* CreateSceneNode( const char* name )
	{ return new MyGameSceneNode( name ); }//Yooooooooooo~

	static void SetViewProjMat( D3DXMATRIX& _vMat, D3DXMATRIX& _pMat );
	static void setLightViewProjMat( D3DXMATRIX& lightMat )
	{ lightViewProjMat = lightMat; }
	static void SetShadowMap( IDirect3DTexture9* _tex );
	static IDirect3DTexture9* GetShadowMap();

	static D3DXMATRIX& getViewMat(){ return vMat; }
	static D3DXMATRIX& getProjMat(){ return pMat;}
	static D3DXMATRIX  getViewProjCombinedMat();

	static D3DXMATRIX&  getLightViewProjMat(){ return lightViewProjMat;}

};
}