////////////////////////
//Òª»­Mesh£¬ÇëÓÃEntity
////////////////////////

#pragma once
#include <d3dx9.h>
#include "MyGameScene.h"
#include <vector>
#include "MyGame3DEffect.h"
using std::vector;
class MyGameMeshManager;
class MyGameMesh
{
	friend class MyGameMeshManager;
private:
	ID3DXMesh* pDXMesh;
	ID3DXBuffer* adjBuffer;
	std::vector<D3DMATERIAL9> Mtrls;
	std::vector<IDirect3DTexture9* > Textures;
	//vector<MyGameSceneNode*> nodes;
	
	IDirect3DVertexBuffer9* pVerBuffer;
	IDirect3DVertexDeclaration9* pVerDecl;

	IDirect3DTexture9* tex;
protected:
	MyGameMesh();
	virtual ~MyGameMesh(void);
public:

	virtual void loadMeshFromXFile( const char* fileName );
	virtual void createPlaneXZ(float width, float height  );
	virtual void createTexture( const char* fileName );
	virtual void createGridFromBmp( const char* fileName );
	MyGameSceneNode* getNode();
	//virtual void Draw();

	//virtual void addNode( const char* name );

	//!!!should be called between BeginPass and EndPass!!!!!!!!!!
	virtual void render( MyGame3DEffect* pEffect );

	//static MyGameMesh* createMyGameMesh( meshType type );

};

class MyGameMeshManager
{
	static vector<MyGameMesh*> meshVec;
public:
	enum meshType{
		MESH,
		SKINNED
	};
	static MyGameMesh* createMyGameMesh( meshType type );
	static void destroyAllMeshes();
};