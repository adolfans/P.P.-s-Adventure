#pragma once
#include <d3dx9.h>
#include "MyGame3DDevice.h"
#include <vector>
using std::vector;
#include <map>
using std::map;
#include "MyGameScene.h"
#include "MyGameMesh.h"

namespace MyGameScene{

class MyGameSceneBillboard
{
private:
	D3DXMATRIX moveMat;
	D3DXMATRIX scaleMat;
	
	D3DXMATRIX worldMat;

	IDirect3DTexture9* pTexture;

	map<unsigned int, IDirect3DTexture9* > texList;

	MyGameSceneBillboard(void);
	
	float vertices[6*5];

	//static D3DXMATRIX viewMatrix;
	MyGameSceneManager* sceneManager;
	MyGameSceneNode* node;
	static vector<MyGameSceneBillboard*> list;
	static IDirect3DVertexBuffer9* vb;
public:
	~MyGameSceneBillboard(void);
	void SetPosition( float _x, float _y, float _z );
	void SetSize( float _width, float _height );
	void CreateTextureFromFile( unsigned int texId, const char* _fileName );
	void EnableTextureFromId( unsigned int texId );

	//static void SetViewMatrix( const D3DXMATRIX* pMat );
	static MyGameSceneBillboard* CreateBillboard( MyGameSceneManager* sMgr );
	static void AttachBillboardToScene( MyGameSceneBillboard* );
	static void DrawAllBillboards();
	static void DestroyAllBillboards();

	MyGameSceneNode* getNode();
};
}