#include "StdAfx.h"
#include "MyGameSceneBillboard.h"
#include "MyGameSceneManager.h"
#include <sstream>

namespace MyGameScene{
vector<MyGameSceneBillboard*> MyGameSceneBillboard::list;
IDirect3DVertexBuffer9*	MyGameSceneBillboard::vb = 0;
//D3DXMATRIX MyGameSceneBillboard::viewMatrix;
MyGameSceneBillboard::MyGameSceneBillboard(void)
	:pTexture(0)
{
	D3DXMatrixIdentity( &moveMat );
	D3DXMatrixIdentity( &scaleMat );

}


MyGameSceneBillboard::~MyGameSceneBillboard(void)
{
	for( map<unsigned int, IDirect3DTexture9* >::iterator _itr = texList.begin();
		_itr != texList.end();
		 ++ _itr )
	{
		IRelease(_itr->second);
	}
}

MyGameSceneBillboard* MyGameSceneBillboard::CreateBillboard( MyGameSceneManager* sMgr )
{
	if( vb == 0 )
	{
		IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
		float vertices[6*5] =
		{
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,-1.0f, 0.0f, 1.0f, 1.0f
		};


	/*	for( int i = 0; i < 48; ++i )
			vertices[i]*=0.5f;*/
		HR(pDevice->CreateVertexBuffer( sizeof(float)*30, D3DUSAGE_WRITEONLY, /*D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1*/0,D3DPOOL_MANAGED, &vb, 0 ) );

		float *pVb;
		vb->Lock( 0, sizeof(float)*30, (void**)&pVb, 0 );
		memcpy( pVb, vertices, sizeof(float)*30 );
		vb->Unlock();

	}
	MyGameSceneBillboard* _b = new MyGameSceneBillboard();
	_b->sceneManager = sMgr;
	static int billboardnum = 1;
	std::stringstream ss;
	ss << "billboard" << billboardnum;
	string name = ss.str();
	++billboardnum;
	_b->node = sMgr->CreateSceneNode( name.c_str() );
	return _b;
}

MyGameSceneNode* MyGameSceneBillboard::getNode()
{
	return node;
}

void MyGameSceneBillboard::AttachBillboardToScene( MyGameSceneBillboard* _b )
{
	list.push_back( _b );
}

void MyGameSceneBillboard::DestroyAllBillboards()
{
	list.clear();

	IRelease(vb);
}

void MyGameSceneBillboard::CreateTextureFromFile(unsigned int texId, const char* _fileName )
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	IDirect3DTexture9* _pTex;
	D3DXCreateTextureFromFileA( pDevice, _fileName, &_pTex ); 

	if( texList.count( texId ) )//如果已经有了
	{
		this->texList[texId]->Release();
	}

	texList[texId] = _pTex;
}

void MyGameSceneBillboard::EnableTextureFromId( unsigned int texId )
{
	if( texList.count(texId) )
	{
		pTexture = texList[texId];

	}else
	{
		::MessageBoxA( 0, "texId not found ", 0, 0 );
	}

}

void MyGameSceneBillboard::SetPosition( float _x, float _y, float _z )
{
	D3DXMatrixTranslation( &moveMat, _x, _y, _z );
	D3DXMATRIX _transpose;
	D3DXMATRIX viewMatrix = sceneManager->getViewMat();
	::D3DXMatrixTranspose( &_transpose, &viewMatrix );
	_transpose._14 = 0;
	_transpose._24 = 0;
	_transpose._34 = 0;
	_transpose._41 = 0;
	_transpose._42 = 0;
	_transpose._43 = 0;
	_transpose._44 = 1;
	D3DXMatrixMultiply( &worldMat, &scaleMat, &_transpose );
	D3DXMatrixMultiply( &worldMat, &worldMat, &moveMat ); 
}

void MyGameSceneBillboard::SetSize( float _width, float _height )
{
	this->node->scale( _width, _height, 0.0f );
}

void MyGameSceneBillboard::DrawAllBillboards()
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	pDevice->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 );

	pDevice->SetRenderState( D3DRS_LIGHTING, false );

	D3DXMATRIX idMat;
	D3DXMatrixIdentity( &idMat );

	pDevice->SetTransform( D3DTS_WORLD, &idMat);
	pDevice->SetTransform( D3DTS_PROJECTION, &idMat );
	pDevice->SetTransform( D3DTS_VIEW, &idMat );

	//pDevice->SetTransform( D3DTS_VIEW, &viewMatrix );

	float h = (float)MyGame3DDevice::GetHeight();
	float w = (float)MyGame3DDevice::GetWidth();
	float pi = 3.1415926536f;	//囧
	D3DXMATRIX perspectiveMat;
	D3DXMatrixPerspectiveFovLH( &perspectiveMat, pi*0.5F, w/h, 5.0f, 2000.0f );

	//pDevice->SetTransform( D3DTS_PROJECTION, &perspectiveMat );

	pDevice->SetStreamSource( 0, vb, 0, sizeof(float)*5 );
	for( vector<MyGameSceneBillboard*>::iterator _itr = list.begin(); _itr != list.end(); ++ _itr )
	{
		//D3DXMATRIX wvp = (*_itr)->worldMat*(*_itr)->sceneManager->getViewMat()*(*_itr)->sceneManager->getProjMat();//perspectiveMat;
		D3DXMATRIX viewMatrix = (*_itr)->sceneManager->getViewMat();
		D3DXMATRIX _transpose;
		::D3DXMatrixTranspose( &_transpose, &viewMatrix );
		_transpose._14 = 0;
		_transpose._24 = 0;
		_transpose._34 = 0;
		_transpose._41 = 0;
		_transpose._42 = 0;
		_transpose._43 = 0;
		_transpose._44 = 1;
		(*_itr)->getNode()->setRotateMatrix( _transpose );
		//上面的工作直到下一帧才会应用
		//下面的工作是获得上一帧的worldmat;
		D3DXMATRIX worldmat = (*_itr)->getNode()->getCombinedMatrix();
		D3DXMATRIX wvp = worldmat//worldmat里只是scale的矩阵吧
			*(*_itr)->sceneManager->getViewMat()
			*(*_itr)->sceneManager->getProjMat();//perspectiveMat;
		
		pDevice->SetTexture( 0, (*_itr)->pTexture );
		//pDevice->SetTransform( D3DTS_WORLD, &( (*_itr)->worldMat) );
		pDevice->SetTransform( D3DTS_WORLD, &wvp );//合并了world、viewMatrix、proj之后应用到world上
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ); 
	}

	//D3DXMATRIX idMat;
	D3DXMatrixIdentity( &idMat );

	pDevice->SetTransform( D3DTS_WORLD, &idMat);
	pDevice->SetTransform( D3DTS_PROJECTION, &idMat );
	pDevice->SetTransform( D3DTS_VIEW, &idMat );

}
}