#include "StdAfx.h"
#include "SkinnedMesh.h"
#include "MyGameMesh.h"
#include "MyGameSceneManager.h"
#include "MyGame3DDevice.h"
#include <PxToolkit.h>
#include <exception>
using std::runtime_error;

MyGameMesh::MyGameMesh()
	:pDXMesh(0), adjBuffer(0), extraTexture(0)//,pVerBuffer(0),pVerDecl(0), tex(0)
{
	//nodes.push_back( MyGameSceneManager::CreateSceneNode( name ));
}


MyGameMesh::~MyGameMesh(void)
{
	IRelease(adjBuffer);
	IRelease(pDXMesh);
	/*		for( int i = 0; i < Mtrls.size(); i++ )
		{
			//pDevice->SetMaterial( &Mtrls[i] );
			//pDevice->SetTexture( 0, Textures[i] );
			Textures[i]->Release();
		}
		*/
	for( vector< IDirect3DTexture9* >::iterator _itr = Textures.begin();
		_itr != Textures.end();
		++ _itr )
		IRelease((*_itr));

	//IRelease(pVerBuffer);
	//IRelease(pVerDecl);
	//IRelease( tex );
	IRelease( extraTexture );
}

void MyGameMesh::loadMeshFromXFile( const char* fileName )
{
	if( pDXMesh )
		throw runtime_error( "此Entity已经加载了XFile" );
	IDirect3DDevice9* pDevice = ::MyGame3DDevice::GetSingleton()->GetDevice();
	
	ID3DXBuffer* mtrlBuffer = 0;

	DWORD numMtrls = 0;

	HR( D3DXLoadMeshFromXA( fileName,
						D3DXMESH_MANAGED,
						pDevice,
						&adjBuffer,
						&mtrlBuffer,
						0,
						&numMtrls,
						&pDXMesh) );

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		
		for( int i = 0; i < numMtrls; i++ )
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			Mtrls.push_back( mtrls[i].MatD3D );
			
			if( mtrls[i].pTextureFilename != 0 )
			{
				IDirect3DTexture9* tex = 0;
				HR(D3DXCreateTextureFromFileA(
					pDevice,
					mtrls[i].pTextureFilename,
					&tex ));

				Textures.push_back( tex );
			}else
			{
				Textures.push_back( 0 );
			}
		}

		mtrlBuffer->Release();
	}
}
//getNode可以被MyGameSceneNode里的GetNodeByName替换掉
/*
MyGameSceneNode* MyGameMesh::getNode()
{
	return pNode;
}
*/
/*
void MyGameMesh::Draw()
{
	D3DXMATRIX idMat;
	D3DXMatrixIdentity( &idMat );
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	pDevice->SetTransform( D3DTS_VIEW, &MyGameSceneManager::getViewProjCombinedMat() );
	pDevice->SetTransform( D3DTS_PROJECTION, &idMat );
	for( vector<MyGameSceneNode* >::iterator _itr = nodes.begin();
		_itr != nodes.end();
		++ _itr )
	{
		pDevice->SetTransform( D3DTS_WORLD, &(*_itr)->getCombinedMatrix() );
		for( int i = 0; i < Mtrls.size(); i++ )
		{
			pDevice->SetMaterial( &Mtrls[i] );
			pDevice->SetTexture( 0, Textures[i] );
			this->pDXMesh->DrawSubset( i );
		}
	}
}
*/
/*
void MyGameMesh::addNode( const char* name )
{
	nodes.push_back( MyGameSceneManager::CreateSceneNode( name ));
}
*/
void MyGameMesh::createPlaneXZ( float width, float height )
{
	//float _x = width/2; float _z = height/2;
	//float vB[8*6] =
	//{
	//	-_x, 0.0f, _z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	//	_x,  0.0f, _z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	//	-_x, 0.0f,-_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	//	-_x, 0.0f, -_z,0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	//	_x, 0.0f,  _z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	//	_x, 0.0f, -_z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	//};
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	//IDirect3DVertexBuffer9* pVerBuffer;
	//HR(pDevice->CreateVertexBuffer( sizeof(float)*8*6, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &pVerBuffer, 0 ));
	//float* dest;

	//pVerBuffer->Lock( 0, sizeof(float)*8*6, (void**)&dest, 0 );
	//memcpy( dest, vB, sizeof(float)*8*6 );
	//pVerBuffer->Unlock();

	//HR( pDevice->CreateIndexBuffer( sizeof(unsigned short )* 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &this->pIndicesBuffer, 0 ));

	//unsigned short indices[] = { 0, 1, 2, 3, 4,5 };

	//void* inDest;

	//pIndicesBuffer->Lock( 0, sizeof(unsigned short), &inDest, 0 );

	//memcpy( inDest, indices, sizeof( indices ) );

	//pIndicesBuffer->Unlock();

	//D3DVERTEXELEMENT9 decl[] = {
	//	{
	//		0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0
	//	},
	//	{
	//		0, 12, D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,0
	//		//0, 12, D3DDECLTYPE_D3DCOLOR ,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,0
	//	},
	//	{
	//		0, 24, D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0
	//	},
	//	D3DDECL_END()
	//};
	//	D3DVERTEXELEMENT9 decl[] = {
	//	{
	//		0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0
	//	},
	//	{
	//		0, 12, D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,0
	//	},
	//	D3DDECL_END()
	//};
	//HR( pDevice->CreateVertexDeclaration( decl, &pVerDecl ) );

	float _x = width/2; float _z = height/2;
	float vB[8*6] =
	{
		-_x, 0.0f, _z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		_x,  0.0f, _z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-_x, 0.0f,-_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-_x, 0.0f, -_z,0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		_x, 0.0f,  _z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		_x, 0.0f, -_z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	};
	//IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	//IDirect3DVertexBuffer9* pVerBuffer;
	//HR(pDevice->CreateVertexBuffer( sizeof(float)*8*6, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &pVerBuffer, 0 ));
	//float* dest;

	//pVerBuffer->Lock( 0, sizeof(float)*8*6, (void**)&dest, 0 );
	//memcpy( dest, vB, sizeof(float)*8*6 );
	//pVerBuffer->Unlock();

	D3DVERTEXELEMENT9 decl[] = {
		{
			0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0
		},
		{
			0, 12, D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,0
			//0, 12, D3DDECLTYPE_D3DCOLOR ,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,0
		},
		{
			0, 24, D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0
		},
		D3DDECL_END()
	};
	////HR( pDevice->CreateVertexDeclaration( decl, &pVerDecl ) );

	D3DXCreateMesh( 2, 6, D3DXMESH_MANAGED/* | D3DXMESH_32BIT加上这个参数会导致一些奇怪的问题*/, decl, pDevice, &this->pDXMesh );

	void* pdata;

	this->pDXMesh->LockVertexBuffer( 0, &pdata );

	memcpy( pdata, vB, sizeof( vB ) );

	this->pDXMesh->UnlockVertexBuffer();

	//unlock

	this->pDXMesh->LockIndexBuffer( 0, &pdata );
	//lockIndexBuffer
	WORD idb[] = { 0, 1, 2, 3, 4, 5 };

	memcpy( pdata, idb, sizeof(idb));

	pDXMesh->UnlockIndexBuffer();

	
	DWORD* attributeBuffer; 

	pDXMesh->LockAttributeBuffer( 0, &attributeBuffer );

	attributeBuffer[0] = 0;
	attributeBuffer[1] = 0;

	pDXMesh->UnlockAttributeBuffer();


}

void MyGameMesh::render( MyGame3DEffect* pEffect )
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	//if( pVerBuffer )
	//{
	//	pDevice->SetVertexDeclaration(this->pVerDecl);
	//	pEffect->setTextureByName( tex, MyGame3DEffect::TEXTURE );
	//	//pEffect->setTexture( 0, tex );
	//	pEffect->CommitChanges();
	//	HR(pDevice->SetStreamSource( 0, this->pVerBuffer, 0, sizeof(float)*8 ) );
	//	HR(pDevice->SetIndices( this->pIndicesBuffer ));
	//	HR( pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2 ) );
	//	//HR(pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ));	
	//}else if( this->pDXMesh )
	{
		for( int i = 0; i < Mtrls.size(); i++ )
		{
			//pDevice->SetMaterial( &Mtrls[i] );
			//pDevice->SetTexture( 0, Textures[i] );
			pEffect->setDiffuse( D3DXVECTOR3(Mtrls[i].Diffuse.r, Mtrls[i].Diffuse.g, Mtrls[i].Diffuse.b) );
			pEffect->setAmbient( D3DXVECTOR3(Mtrls[i].Ambient.r, Mtrls[i].Ambient.g, Mtrls[i].Ambient.b) );
			pEffect->setSpecular( D3DXVECTOR3(Mtrls[i].Specular.r, Mtrls[i].Specular.g, Mtrls[i].Specular.b) );
			//pEffect->setTextureByName( Textures[i], MyGame3DEffect::TEXTURE );
			pEffect->setTexture( 0, Textures[i] );
			pEffect->setTexture( 1, extraTexture );
			pEffect->CommitChanges();
			this->pDXMesh->DrawSubset( i );
		}
	}
}

void MyGameMesh::createTexture( const char* fileName, unsigned int num )
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	IDirect3DTexture9* texture;
	HR( D3DXCreateTextureFromFileA( pDevice, fileName, &texture) );
	D3DMATERIAL9 mtrl;
	memset( &mtrl, 0, sizeof(D3DMATERIAL9) );
	mtrl.Diffuse.r = 1.0f;
	mtrl.Diffuse.g = 0.0f;
	mtrl.Diffuse.b = 0.0f;
	mtrl.Diffuse.a = 1.0f;
	
	if ( Textures.size() > num )	//如果其中有编号为num的材质
	{
		IRelease(Textures[num]);
		//Mtrls[num] = mtrl;		//如果材质中有信息的话，就不替换了
		Textures[num] = texture;
	}else{
		Mtrls.push_back( mtrl );
		Textures.push_back( texture );
	}
}

void MyGameMesh::createExtraTexture( const char* fileName )
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	
	HR( D3DXCreateTextureFromFileA( pDevice, fileName, &this->extraTexture) );
	
}
struct bmpHeader{
	unsigned int	bfSize;		//4 bytes
	unsigned int	bfReserved12;	//4 bytes
	unsigned int	bfOffBits;	//4 bytes
	unsigned int	biSize;		//4 bytes
	unsigned int	Width;		//4 bytes
	int	Height;		//4 bytes
	char biPlanes[4];
	unsigned int	biCompression;	//4 bytes
	unsigned int	biSizeImage;	//4 bytes
	unsigned int	biXPelsPerMeter;
	unsigned int	biYPelsPerMeter;
	unsigned int	biClrUsed;
	unsigned int	biClrImportant;
};
void MyGameMesh::createGridFromBmp( const char* fileName )
{
	//TODO: 这货是完成还是废弃，这是一个问题
	FILE* pFile = fopen( fileName, "rb" );
	char bm[] = "  \n";	//3个
	fread( bm, 1, 2, pFile );
	struct bmpHeader header;

	fread( &header, sizeof( bmpHeader), 1, pFile );


}

vector<MyGameMesh*> MyGameMeshManager::meshVec;

MyGameMesh* MyGameMeshManager::createMyGameMesh( meshType type )
{
	MyGameMesh* pNewMesh;
	switch( type )
	{
	case MESH:
		pNewMesh = new MyGameMesh;
		break;
	case SKINNED:
		pNewMesh = new SkinnedMesh;
		break;
	default:
		pNewMesh = new MyGameMesh;
	}
	if( !pNewMesh )
		throw runtime_error("创建mesh失败" );
	meshVec.push_back( pNewMesh );
	return pNewMesh;
}

void MyGameMeshManager::destroyAllMeshes()
{
	for( auto _itr = meshVec.begin();		//测试auto
		_itr != meshVec.end();
		++ _itr )
		delete (*_itr);

	meshVec.clear();
}

physx::PxTriangleMesh* MyGameMesh::generatePxTriangleMesh(  )
{
	PxTriangleMeshDesc meshDesc;
	//GetNumBytesPerVertex();
	ID3DXMesh* newMesh;
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	this->pDXMesh->CloneMeshFVF( D3DXMESH_32BIT|D3DXMESH_DYNAMIC, D3DFVF_XYZ, pDevice, &newMesh );
	
	float* srcVertices;
	newMesh->LockVertexBuffer( 0, (void**)&srcVertices );
	float* vertices = new float[ newMesh->GetNumVertices()*3 ];//一个点有x, y, z三个数值，所以乘以三
	memcpy( vertices, srcVertices, sizeof(float)* newMesh->GetNumVertices()*3 );
	newMesh->UnlockVertexBuffer();

	meshDesc.points.count = newMesh->GetNumVertices();
	meshDesc.points.stride = sizeof(float)*3;
	meshDesc.points.data	= vertices;

	unsigned int* destIndices = new unsigned int[ newMesh->GetNumFaces() * 3 ];//一个面有三个点，所以乘以三
	unsigned int* srcIndices;
	newMesh->LockIndexBuffer( 0, (void**)&srcIndices );
	memcpy( destIndices, srcIndices, sizeof( unsigned int ) * newMesh->GetNumFaces() * 3 );
	newMesh->UnlockIndexBuffer();

	meshDesc.triangles.count = newMesh->GetNumFaces();
	meshDesc.triangles.stride = 3 * sizeof( unsigned int );
	meshDesc.triangles.data = destIndices;

	PxToolkit::MemoryOutputStream writeBuffer;
	
	MyGame3DDevice* pdevice = MyGame3DDevice::GetSingleton();
	PxCooking* cooking = pdevice->getPhysXCooking();
	PxPhysics* physics = pdevice->getPhysX();

	bool status = (*cooking).cookTriangleMesh(meshDesc, writeBuffer);
	
	if(!status)
		//return NULL;
		throw runtime_error( "cooktriangleMesh failed" );

	PxToolkit::MemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	//return physics.createTriangleMesh(readBuffer);

	delete[] vertices;
	delete[] destIndices;

	newMesh->Release();

	return physics->createTriangleMesh( readBuffer );
}