#include "StdAfx.h"
#include "MyGameMesh.h"
#include "MyGameSceneManager.h"
#include "MyGame3DDevice.h"
#include <exception>
using std::runtime_error;
using namespace MyGameScene;
namespace MyGameScene{
MyGameMesh::MyGameMesh()
	:pDXMesh(0), adjBuffer(0),pVerBuffer(0),pVerDecl(0)
{
	//nodes.push_back( MyGameSceneManager::CreateSceneNode( name ));
}


MyGameMesh::~MyGameMesh(void)
{
	IRelease(adjBuffer);
	IRelease(pDXMesh);
	
	IRelease(pVerBuffer);
	IRelease(pVerDecl);
}

void MyGameMesh::loadMeshFromXFile( const char* fileName )
{
	if( pDXMesh )
		throw runtime_error( "��Entity�Ѿ�������XFile" );
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
	}

	mtrlBuffer->Release();
}
//getNode���Ա�MyGameSceneNode���GetNodeByName�滻��
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
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	HR(pDevice->CreateVertexBuffer( sizeof(float)*8*6, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &pVerBuffer, 0 ));
	float* dest;

	pVerBuffer->Lock( 0, sizeof(float)*8*6, (void**)&dest, 0 );
	memcpy( dest, vB, sizeof(float)*8*6 );
	pVerBuffer->Unlock();

		D3DVERTEXELEMENT9 decl[] = {
		{
			0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0
		},
		{
			0, 12, D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,0
		},
		{
			0, 24, D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0
		},
		D3DDECL_END()
	};
	//	D3DVERTEXELEMENT9 decl[] = {
	//	{
	//		0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0
	//	},
	//	{
	//		0, 12, D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,0
	//	},
	//	D3DDECL_END()
	//};
	HR( pDevice->CreateVertexDeclaration( decl, &pVerDecl ) );
}

void MyGameMesh::render( MyGame3DEffect* pEffect )
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	if( pVerBuffer )
	{
		pDevice->SetVertexDeclaration(this->pVerDecl);
		HR(pDevice->SetStreamSource( 0, this->pVerBuffer, 0, sizeof(float)*8 ) );
		HR(pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ));	
	}else if( this->pDXMesh )
	{
		for( int i = 0; i < Mtrls.size(); i++ )
		{
			//pDevice->SetMaterial( &Mtrls[i] );
			//pDevice->SetTexture( 0, Textures[i] );
			pEffect->SetTextureByName( Textures[i], TEXTURE );
			this->pDXMesh->DrawSubset( i );
		}
	}
}
}