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

public:
	MyGameSceneManager(void);
	~MyGameSceneManager(void);

	static MyGameSceneNode* CreateSceneNode( const char* name )
	{ return new MyGameSceneNode( name ); }//Yooooooooooo~

	static void SetViewProjMat( D3DXMATRIX& _vMat, D3DXMATRIX& _pMat );

	static D3DXMATRIX& getViewMat(){ return vMat; }
	static D3DXMATRIX& getProjMat(){ return pMat;}
	static D3DXMATRIX  getViewProjCombinedMat();
};
}