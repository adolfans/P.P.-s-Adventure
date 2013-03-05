////////////////////////
//要画Mesh，请用Entity
////////////////////////

#pragma once
#include <d3dx9.h>
#include "MyGameScene.h"
#include <vector>
#include "MyGame3DEffect.h"
using std::vector;


#ifdef free
#undef free
#endif
#ifdef realloc
#undef realloc
#endif
#include <PxPhysicsAPI.h>
using namespace physx;
//using namespace PxToolkit;
class MyGameMeshManager;
class MyGameMesh
{
	friend class MyGameMeshManager;
protected:
private:
	ID3DXMesh* pDXMesh;
	ID3DXBuffer* adjBuffer;
	std::vector<D3DMATERIAL9> Mtrls;
	std::vector<IDirect3DTexture9* > Textures;
	//vector<MyGameSceneNode*> nodes;
	
	//IDirect3DVertexBuffer9* pVerBuffer;
	//IDirect3DVertexDeclaration9* pVerDecl;
	//IDirect3DIndexBuffer9*	pIndicesBuffer;
	//IDirect3DTexture9* tex;

	IDirect3DTexture9* extraTexture; //临时的额外贴图，用于多重纹理中的编号1的那一层
	D3DXVECTOR3 minPoint, maxPoint;//bounding box

	IDirect3DDevice9* pDevice;
protected:
	MyGameMesh();
	virtual ~MyGameMesh(void);
	void setID3DXMesh( ID3DXMesh* mesh )
	{
		pDXMesh = mesh;
	}

	void addMtrls( const D3DMATERIAL9& mtrl, const char* name )
	{
		Mtrls.push_back( mtrl );
		IDirect3DTexture9* texture;
		if( name )
		{
			D3DXCreateTextureFromFileA( pDevice, name, &texture );
			Textures.push_back( texture );
		}else
		{
			Textures.push_back( 0 );
		}
	}

	void generateBoundingBox()
	{
		void* firstPos;
		pDXMesh->LockVertexBuffer( 0, &firstPos );
		HR( D3DXComputeBoundingBox( 
							(D3DXVECTOR3*)firstPos,
							pDXMesh->GetNumVertices(),
							D3DXGetFVFVertexSize( pDXMesh->GetFVF() ),
							&minPoint,
							&maxPoint) );

		pDXMesh->UnlockVertexBuffer();
	}
public:

	virtual void loadMeshFromXFile( const char* fileName );
	virtual void createPlaneXZ( float width, float height  );
	virtual void createTexture( const char* fileName, unsigned int num = 0 );
	virtual void createExtraTexture( const char* fileName );
	virtual void createGridFromBmp( const char* fileName );
	physx::PxTriangleMesh* generatePxTriangleMesh();
	MyGameSceneNode* getNode();
	//virtual void Draw();

	//virtual void addNode( const char* name );

	//!!!should be called between BeginPass and EndPass!!!!!!!!!!
	virtual void render( MyGame3DEffect* pEffect );

	virtual void getBoundingBox( D3DXVECTOR3& _minPoint, D3DXVECTOR3& _maxPoint )
	{
		_minPoint = this->minPoint;
		_maxPoint = this->maxPoint;
	}

	virtual void prepare();

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