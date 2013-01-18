////////////////////////
//Òª»­Mesh£¬ÇëÓÃEntity
////////////////////////

#pragma once
#include <d3dx9.h>
#include "MyGameScene.h"
#include <vector>
#include "MyGame3DEffect.h"
using std::vector;
namespace MyGameScene{
class MyGameMesh
{
private:
	ID3DXMesh* pDXMesh;
	ID3DXBuffer* adjBuffer;
	std::vector<D3DMATERIAL9> Mtrls;
	std::vector<IDirect3DTexture9* > Textures;
	//vector<MyGameSceneNode*> nodes;
	
	IDirect3DVertexBuffer9* pVerBuffer;
	IDirect3DVertexDeclaration9* pVerDecl;

public:
	MyGameMesh();
	~MyGameMesh(void);

	virtual void loadMeshFromXFile( const char* fileName );
	virtual void createPlaneXZ(float width, float height  );
	MyGameSceneNode* getNode();
	//virtual void Draw();

	//virtual void addNode( const char* name );

	//!!!should be called between BeginPass and EndPass!!!!!!!!!!
	virtual void render( MyGame3DEffect* pEffect );
};
}