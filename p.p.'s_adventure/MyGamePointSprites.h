#pragma once
#include <d3d9.h>
#include "MyGameSceneManager.h"
#include <vector>
using std::vector;
class MyGamePointSprites
{
	struct POINTVERTEX
	{
		float x, y, z;
		D3DCOLOR color;
	};
	IDirect3DVertexBuffer9* vb;
private:
	IDirect3DDevice9* device;
	vector< MyGameSceneEntity* > spriteEntityList; 
public:

	MyGamePointSprites(int num = 1);
	~MyGamePointSprites(void);

	void render(MyGameSceneManager* sceneMgr);
	void createPointSprite( MyGameSceneEntity* ent );
};

