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
	float x, y, z;
	IDirect3DDevice9* device;
	vector< MyGameSceneEntity* > spriteEntityList; 
	POINTVERTEX			vert;
public:

	MyGamePointSprites(int num = 1);
	~MyGamePointSprites(void);

	void render(MyGameSceneManager* sceneMgr);
	void createPointSprite( MyGameSceneEntity* ent );

	void setPosition( float x, float y, float z )
		{
				device = MyGame3DDevice::GetSingleton()->GetDevice();
	
				POINTVERTEX* vertices;
				vb->Lock( 0, sizeof( POINTVERTEX ), (void**)&vertices, 0 );

				vertices->x = x;
				vertices->y = y;
				vertices->z = 0;
			
				vert.x = x;
				vert.y = y;
				vert.z = 0.0f;
				vertices->color = 0xffffffff;
	
				vb->Unlock();
	}
};

