#include "StdAfx.h"
#include "MyGameHelperGraphics.h"
#include "MyGame3DDevice.h"
#include "MyGameSceneEntity.h"
#include "MyGameSceneManager.h"
IDirect3DVertexBuffer9*		MyGameHelperGraphics::boxVertexBuffer	= NULL;
IDirect3DVertexBuffer9*		MyGameHelperGraphics::axisVertexBuffer	= NULL;
IDirect3DIndexBuffer9*		MyGameHelperGraphics::boxIndexBuffer	= NULL;
DWORD						MyGameHelperGraphics::vertexFVF			= D3DFVF_XYZ | D3DFVF_DIFFUSE;
unsigned int				MyGameHelperGraphics::boxVerticeNum		= 8;
unsigned int				MyGameHelperGraphics::boxIndiceNum		= 12 * 2;//12条线

MyGameScene::MyGameSceneNode*	MyGameHelperGraphics::axisNode		= NULL;
MyGameScene::MyGameSceneEntity*	MyGameHelperGraphics::boxEntity		= NULL;

MyGameHelperGraphics::vertex	MyGameHelperGraphics::transformedAxis[4];

IDirect3DTexture9*			MyGameHelperGraphics::xTexture			= NULL;
IDirect3DTexture9*			MyGameHelperGraphics::yTexture			= NULL;
IDirect3DTexture9*			MyGameHelperGraphics::zTexture			= NULL;

MyGameHelperGraphics::MyGameHelperGraphics(void)
{
}


MyGameHelperGraphics::~MyGameHelperGraphics(void)
{
}

void MyGameHelperGraphics::initialize()
{
	IDirect3DDevice9* device= MyGame3DDevice::GetSingleton()->GetDevice();

	HR( device->CreateVertexBuffer(	sizeof( vertex ) * boxVerticeNum,
								0,
								vertexFVF,
								D3DPOOL_MANAGED,
								&boxVertexBuffer,
								NULL ) );
	{
	vertex*	boxDest;

	boxVertexBuffer->Lock( 0, sizeof( vertex ) * boxVerticeNum, (void**)&boxDest, 0 );

	vertex vertices[] = 
	{
		{ -0.5,  0.5, -0.5, D3DCOLOR_ARGB( 255, 255, 0, 0 )   },//0
		{  0.5,  0.5, -0.5, D3DCOLOR_ARGB( 255, 255, 0, 0 )   }, //1
		{  0.5, -0.5, -0.5, D3DCOLOR_ARGB( 255, 255, 0, 0 )   }, //2
		{ -0.5, -0.5, -0.5, D3DCOLOR_ARGB( 255, 255, 0, 0 )   }, //3
		{ -0.5,  0.5,  0.5, D3DCOLOR_ARGB( 255, 255, 0, 0 )   },
		{  0.5,  0.5,  0.5, D3DCOLOR_ARGB( 255, 255, 0, 0 )   },	//5
		{  0.5, -0.5,  0.5, D3DCOLOR_ARGB( 255, 255, 0, 0 )   },	//6
		{ -0.5, -0.5,  0.5, D3DCOLOR_ARGB( 255, 255, 0, 0 )  }	//7
	};

	memcpy( boxDest, vertices, sizeof( vertices ) );

	boxVertexBuffer->Unlock();
	}
	HR( device->CreateIndexBuffer( sizeof(WORD)*boxIndiceNum,
								0,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&boxIndexBuffer,
								0 ) );

	{
	WORD*	indicesDest;

	boxIndexBuffer->Lock( 0, sizeof(WORD)*boxIndiceNum, (void**)&indicesDest, 0 );

	WORD	indices[] =
	{
		0, 1, 
		1, 2,
		2, 3,
		0, 3,
		0, 4,
		1, 5, 
		2, 6,
		3, 7,
		4, 5,
		5, 6,
		6, 7,
		7, 4
	};

	memcpy( indicesDest, indices, sizeof( indices ) );

	boxIndexBuffer->Unlock();

	}
	HR( device->CreateVertexBuffer( 6 * sizeof( vertex ),
									0,
									D3DFMT_INDEX16,
									D3DPOOL_MANAGED,
									&axisVertexBuffer,
									0 ) );
	{
		vertex* destVertices;

		axisVertexBuffer->Lock( 0, sizeof(vertex)*6, (void**)&destVertices, 0 );

		vertex axisVertices[] = 
		{
			{  0,  0,  0, 0xff00ff00 },//0
			{  0,  1,  0, 0xff00ff00 }, //1
			{  0,  0,  0, 0xffff0000 }, //2
			{  1,  0,  0, 0xffff0000 }, //x
			{  0,  0,  0, 0xff0000ff },
			{  0,  0,  1, 0xff0000ff }
		};

		memcpy( destVertices, axisVertices, sizeof(vertex)*6 );

		axisVertexBuffer->Unlock();

	}


	HR( D3DXCreateTextureFromFileA( device,
								"x.png",
								&xTexture ) );

	HR( D3DXCreateTextureFromFileA( device,
		"y.png",&yTexture ) );

	HR( D3DXCreateTextureFromFileA( device,
									"z.png",
									&zTexture ) );

	transformedAxis[0].color = 0xffffffff;
	transformedAxis[1].color = 0xffffffff;
	transformedAxis[2].color = 0xffffffff;
	transformedAxis[3].color = 0xffffffff;

}
void TranslateVector( float x, float y, float z, const D3DXMATRIX& mat, float& outX, float& outY, float& outZ )
{
	outX = x * mat._11 + y * mat._21 + z * mat._31;
	outY = x * mat._12 + y * mat._22 + z * mat._32;
	outZ = x * mat._13 + y * mat._23 + z * mat._33;
	//outZ = _x * mat._14 + _y * mat._24 + _z * mat._34;

}

void TranslatePoint( float x, float y, float z, const D3DXMATRIX& mat, float& outX, float& outY, float& outZ )
{
	outX = x * mat._11 + y * mat._21 + z * mat._31 + 1.0 * mat._41;
	outY = x * mat._12 + y * mat._22 + z * mat._32 + 1.0 * mat._42;
	outZ = x * mat._13 + y * mat._23 + z * mat._33 + 1.0 * mat._43;
	float w = x * mat._14 + y * mat._24 + z * mat._34 + 1.0 * mat._44;
	outX /= w;
	outY /= w;
	outZ /= w;
}

void MyGameHelperGraphics::draw( MyGameScene::MyGameSceneManager* sceneMgr )
{
	IDirect3DDevice9* device= MyGame3DDevice::GetSingleton()->GetDevice();
	
	if( boxEntity || axisNode )
	{
		device->SetTransform( D3DTS_PROJECTION, &sceneMgr->getProjMat() );
		device->SetTransform( D3DTS_VIEW, &sceneMgr->getViewMat() );
        device->SetRenderState( D3DRS_LIGHTING, false );
        //device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
        device->SetTexture(0 ,  NULL);
        device->SetFVF( vertexFVF );
        device->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 ); // 关闭blend
	}

	if( boxEntity )
	{
		D3DXVECTOR3 minPoint, maxPoint;
		boxEntity->getBoundingBox( minPoint, maxPoint );
		D3DXMATRIX scaleMat;
		D3DXMatrixScaling( &scaleMat, maxPoint.x - minPoint.x,
									maxPoint.y - minPoint.y,
									maxPoint.z - minPoint.z );
		D3DXMATRIX transMat;
		D3DXMatrixTranslation( &transMat, ( maxPoint.x + minPoint.x ) / 2.0f,
											( maxPoint.y + minPoint.y ) / 2.0f,
											( maxPoint.z + minPoint.z ) / 2.0f );

        D3DXMATRIX combinedTranslation = scaleMat * transMat * boxEntity->getNode()->getCombinedMatrix();
		device->SetTransform( D3DTS_WORLD, &combinedTranslation );
		device->SetIndices( boxIndexBuffer );
		device->SetStreamSource( 0, boxVertexBuffer, 0, sizeof(vertex) );
		HR(device->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, boxVerticeNum, 0, boxIndiceNum/2 ) );
	}

	if( axisNode )
	{
		float x[3], y[3], z[3];
		

		D3DXMATRIX idMat;
		D3DXMatrixIdentity( &idMat );
		D3DXMATRIX moveMat( idMat );
		moveMat._41 = axisNode->getCombinedMatrix()._41;
		moveMat._42 = axisNode->getCombinedMatrix()._42;
		moveMat._43 = axisNode->getCombinedMatrix()._43;

		device->SetTransform( D3DTS_PROJECTION, &idMat );
		device->SetTransform( D3DTS_VIEW, &idMat );
		device->SetTransform( D3DTS_WORLD, &idMat );
		D3DXMATRIX comMat = moveMat * sceneMgr->getViewProjCombinedMat();//

		for( int i = 0; i != 3; ++ i )
		{
			if( i == 0 )
			{
				TranslateVector( 1.0, 0.0, 0.0, axisNode->getCombinedMatrix(), x[i], y[i], z[i] );
			}else if( i == 1 )
			{
				TranslateVector( 0.0, 1.0, 0.0, axisNode->getCombinedMatrix(), x[i], y[i], z[i] );
			}else if( i == 2 )
			{
				TranslateVector( 0.0, 0.0, 1.0, axisNode->getCombinedMatrix(), x[i], y[i], z[i] );
			}
			float len = sqrt( x[i] * x[i] + y[i] * y[i] + z[i] * z[i] );

			x[i] /=len;
			y[i] /=len;
			z[i] /=len;
			x[i] *=5.0f;
			y[i] *=5.0f;
			z[i] *=5.0f;

			TranslatePoint( x[i], y[i], z[i], comMat, x[i], y[i], z[i] );
			transformedAxis[i+1].x = x[i];
			transformedAxis[i+1].y = y[i];
			transformedAxis[i+1].z = 0.0f;
		}
		
		float x0, y0, z0;

		TranslatePoint( 0.0f, 0.0f, 0.0f, comMat, x0, y0, z0 );

		transformedAxis[0].x = x0; transformedAxis[0].y = y0; transformedAxis[0].z = 0.0f;

		{
			vertex* destVertices;

			axisVertexBuffer->Lock( 0, sizeof(vertex)*6, (void**)&destVertices, 0 );

			destVertices[0].x = x0; destVertices[0].y = y0; destVertices[0].z = z0;
			destVertices[1].x = x[1]; destVertices[1].y = y[1]; destVertices[1].z = z[1];
			destVertices[2].x = x0; destVertices[2].y = y0; destVertices[2].z = z0;
			destVertices[3].x = x[0]; destVertices[3].y = y[0]; destVertices[3].z = z[0];
			destVertices[4].x = x0; destVertices[4].y = y0; destVertices[4].z = z0;
			destVertices[5].x = x[2]; destVertices[5].y = y[2]; destVertices[5].z = z[2];

			axisVertexBuffer->Unlock();

		}

		device->SetStreamSource( 0, axisVertexBuffer, 0, sizeof( vertex ) );
		device->DrawPrimitive( D3DPT_LINELIST, 0, 3 );
	}

}

void MyGameHelperGraphics::cleanup()
{
	IRelease( xTexture );
	IRelease( yTexture );
	IRelease( zTexture );
	IRelease( boxVertexBuffer );
	IRelease( axisVertexBuffer );
	IRelease( boxIndexBuffer );
}

void MyGameHelperGraphics::enableBox( MyGameScene::MyGameSceneEntity* entity )
{
	boxEntity = entity;
	axisNode = entity->getNode();
}

void MyGameHelperGraphics::disableBox()
{
	boxEntity = NULL;
}

void MyGameHelperGraphics::enableAxis( MyGameScene::MyGameSceneNode* node )
{
	axisNode = node;
}

void MyGameHelperGraphics::disableAxis()
{
	axisNode = NULL;
}

void MyGameHelperGraphics::drawAxisText()
{
	
	IDirect3DDevice9* device= MyGame3DDevice::GetSingleton()->GetDevice();
	
	
	D3DXMATRIX idMat;
	D3DXMatrixIdentity( &idMat );

	device->SetTransform( D3DTS_PROJECTION, &idMat );
	device->SetTransform( D3DTS_VIEW, &idMat );
	device->SetTransform( D3DTS_WORLD, &idMat );


    device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ); // 开启Blend
    //device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
    device->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
    device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    device->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

    device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE) ;
    device->SetRenderState(D3DRS_POINTSCALEENABLE, true) ;
	float pointSize = 0.025f;//000;
    device->SetRenderState( D3DRS_POINTSIZE, *((DWORD*)&pointSize )); // 点大小


    device->SetRenderState( D3DRS_LIGHTING, FALSE );
	device->SetRenderState( D3DRS_ZENABLE, false );
	device->SetRenderState( D3DRS_ZWRITEENABLE, false );

	/////////////////device->SetStreamSource( 0, vb, 0, sizeof( POINTVERTEX ) );
	device->SetFVF( vertexFVF );
	device->SetTexture( 0, xTexture );
	device->DrawPrimitiveUP( D3DPT_POINTLIST, 1, &transformedAxis[1], sizeof( vertex ) );
	device->SetTexture( 0, yTexture );
	device->DrawPrimitiveUP( D3DPT_POINTLIST, 1, &transformedAxis[2], sizeof( vertex ) );
	device->SetTexture( 0, zTexture );
	device->DrawPrimitiveUP( D3DPT_POINTLIST, 1, &transformedAxis[3], sizeof( vertex ) );
	
}

void MyGameHelperGraphics::getAxisPosition( int num, float& x, float& y )
{
	switch( num )
	{
	case 1:		//x坐标
		x = transformedAxis[1].x;
		y = transformedAxis[1].y;
		break;
	case 2:
		x = transformedAxis[2].x;
		y = transformedAxis[2].y;
		break;
	case 3:
		x = transformedAxis[3].x;
		y = transformedAxis[3].y;
        break;
	default:
		x = transformedAxis[0].x;
		y = transformedAxis[0].y;

	}

}

void MyGameHelperGraphics::activeAxis( int i )
{
	vertex* destVertices;

	axisVertexBuffer->Lock( 0, sizeof(vertex)*6, (void**)&destVertices, 0 );

	destVertices[0].color = 0xFF00FF00;
	destVertices[1].color = 0xFF00FF00;
	
	destVertices[2].color = 0xffff0000;
	destVertices[3].color = 0xffff0000;

	destVertices[4].color = 0xFF0000ff;
	destVertices[5].color = 0xFF0000ff;

	switch( i ){
	case 1://X
		destVertices[2].color = 0xFFFFFF00;
		destVertices[3].color = 0xFFFFFF00;
		break;
	case 2://y
		destVertices[0].color = 0xFFFFFF00;
		destVertices[1].color = 0xFFFFFF00;
		break;
	case 3:
		destVertices[4].color = 0xFFFFFF00;
		destVertices[5].color = 0xFFFFFF00;
		break;
	default:
		break;

	}

	axisVertexBuffer->Unlock();
}
