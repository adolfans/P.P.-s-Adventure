#pragma once
#include <d3dx9.h>
#include <vector>
using std::vector;
#include "MyGameSceneManager.h"
class MyGameSceneBillboardSet
{
	struct billboardVertex
	{
		float x, y, z;
		D3DCOLOR color;
		float u, v;
		static const DWORD fvf = D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
	};
private:
	IDirect3DDevice9*				device;
	IDirect3DVertexBuffer9*			vertexBuffer;
	IDirect3DTexture9*				texture;
	vector< MyGameSceneNode* >	entityList;
	int								vertexBufferSize;

	static const unsigned int		verticesNumPerRect = 3;
	static billboardVertex			vertices[verticesNumPerRect];
	static void multiplyMatrix( float& x, float& y, float& z, D3DXMATRIX& matrix );
	static void multiplyMatrix( struct billboardVertex& in, D3DXMATRIX& matrix, struct billboardVertex& out );

	static int compareTwoRects( const void* first, const void* second );
	//static void vertexMultyplyMatrix( struct billboardVertex& newBillboard,
	//									const D3DXMATRIX& matrix );
public:
	MyGameSceneBillboardSet( int num, string textureFileName );
	~MyGameSceneBillboardSet(void);

	void addEntity( MyGameSceneNode* ent );
	
	void draw( MyGameSceneManager* sceneMgr );
};

