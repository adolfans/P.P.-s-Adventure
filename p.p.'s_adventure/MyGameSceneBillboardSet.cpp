#include "StdAfx.h"
#include "MyGameSceneBillboardSet.h"
#include "MyGameSceneManager.h"
#include "MyGame3DDevice.h"

#include "MyGameSceneNode.h"
//const unsigned int MyGameSceneBillboardSet::verticesNumPerRect;不用定义了？
MyGameSceneBillboardSet::billboardVertex MyGameSceneBillboardSet::vertices[MyGameSceneBillboardSet::verticesNumPerRect] = 
{
/*	{-1.0f, 1.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f},
	{ 1.0f, 1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f},
	{-1.0f,-1.0f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f},
	{-1.0f,-1.0f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f},
	{ 1.0f, 1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f},
	{ 1.0f,-1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f}*/
	{-3.0f,-1.0f, 0.0f, 0xFFFFFFFF,-1.0f, 1.0f},	//使用一个等腰直角三角形来绘制billboard，比前者减少了计算量
	{ 0.0f, 1.5f, 0.0f, 0xFFFFFFFF, 0.5f,-0.5f},
	{ 3.0f,-1.0f, 0.0f, 0xFFFFFFFF, 2.0f, 1.0f}
};

MyGameSceneBillboardSet::MyGameSceneBillboardSet( int num, string textureFileName )
{
	device = MyGame3DDevice::GetSingleton()->GetDevice();
	vertexBufferSize = sizeof( billboardVertex ) * num * verticesNumPerRect;
	HR( device->CreateVertexBuffer(
		vertexBufferSize,
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
		billboardVertex::fvf,
		D3DPOOL_DEFAULT,
		&vertexBuffer,
		0 ) );

	HR( D3DXCreateTextureFromFileA( device, textureFileName.c_str(), &texture ) );
}

//-1,  1, 0
//1,   1, 0
//-1, -1, 0
//-1, -1, 0
//1,   1, 0
//1,  -1, 0
MyGameSceneBillboardSet::~MyGameSceneBillboardSet(void)
{
	IRelease( vertexBuffer );
	IRelease( texture );
}

void MyGameSceneBillboardSet::addEntity( MyGameSceneNode* ent )
{
	entityList.push_back( ent );
}

int MyGameSceneBillboardSet::compareTwoRects( const void* first, const void* second )
{
	//如果first > second  
	void* _first = const_cast<void*>( first );
	void* _second = const_cast<void*>( second );

	MyGameSceneBillboardSet::billboardVertex* firstVer = static_cast< MyGameSceneBillboardSet::billboardVertex* >( _first );
	MyGameSceneBillboardSet::billboardVertex* secondVer = static_cast< MyGameSceneBillboardSet::billboardVertex* >( _second );
	if( firstVer->z < secondVer->z )
		return 1;
	else//反之
		return -1;
}

void MyGameSceneBillboardSet::draw( MyGameSceneManager* sceneMgr )
{
	D3DXMATRIX idMat;
	D3DXMatrixIdentity( &idMat );
	device->SetTransform( D3DTS_WORLD, &idMat);
	device->SetTransform( D3DTS_PROJECTION, &idMat );
	device->SetTransform( D3DTS_VIEW, &idMat );

	D3DXMATRIX viewMat = sceneMgr->getViewMat();
	D3DXMATRIX projMat = sceneMgr->getProjMat();
	D3DXMATRIX rotationMat;
	D3DXMatrixTranspose( &rotationMat, &viewMat );
	rotationMat._14 = 0;
	rotationMat._24 = 0;
	rotationMat._34 = 0;
	rotationMat._41 = 0;
	rotationMat._42 = 0;
	rotationMat._43 = 0;
	rotationMat._44 = 1;
	int j = 0;

	MyGameSceneBillboardSet::billboardVertex* transformedVertexBuffer = new MyGameSceneBillboardSet::billboardVertex[entityList.size()*verticesNumPerRect];
	for( auto itr = entityList.begin();
		itr != entityList.end();
		++ itr )
	{
		D3DXMATRIX com = (*itr)->getCombinedMatrix();
		//只需要combined matrix中的坐标信息
		float x = com._41;
		float y = com._42;
		float z = com._43;
		//com._41 = 0; com._42 = 0; com._43 = 0;
		//com *= rotationMat;
		//com._41 = x; com._42 = y; com._43 = z;
		
		D3DXMATRIX trans = rotationMat;
		trans._41 = x;
		trans._42 = y;
		trans._43 = z;

		D3DXMATRIX finalMat = /*com*/ trans * viewMat * projMat;
		//(*itr)->setRotateMatrix( rotationMat );//←是否要移到别处？这种方法大错特错，完全没有考虑到父坐标系也在旋转的情况
		
		MyGameSceneBillboardSet::billboardVertex transformedVertices[verticesNumPerRect];
		for( int i = 0; i != verticesNumPerRect; ++ i )
		{
			this->multiplyMatrix( vertices[i], finalMat, transformedVertices[i] );//变换
		}
		memcpy(// transformedVertexBuffer + j * sizeof( billboardVertex ) * verticesNumPerRect,
				&transformedVertexBuffer[j*verticesNumPerRect],
				transformedVertices,
				sizeof( billboardVertex ) * verticesNumPerRect );

		++ j;
	}
	qsort( transformedVertexBuffer,
		entityList.size(),
		sizeof( MyGameSceneBillboardSet::billboardVertex ) * verticesNumPerRect,
		MyGameSceneBillboardSet::compareTwoRects );

	MyGameSceneBillboardSet::billboardVertex* targetVertexBuffer;
	this->vertexBuffer->Lock( 0, this->vertexBufferSize, (void**)&targetVertexBuffer, 0 );
	memcpy( targetVertexBuffer, transformedVertexBuffer, sizeof( MyGameSceneBillboardSet::billboardVertex ) * verticesNumPerRect* entityList.size() );
	this->vertexBuffer->Unlock();
	delete[] transformedVertexBuffer;
	device->SetFVF( MyGameSceneBillboardSet::billboardVertex::fvf );
	device->SetStreamSource( 0, vertexBuffer, 0, sizeof( MyGameSceneBillboardSet::billboardVertex ) );
	device->SetTexture( 0, texture );
	device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, j * verticesNumPerRect / 3);	//因为每个矩形对应2个三角形 
	
}


//void MyGameSceneBillboardSet::draw( MyGameSceneManager* sceneMgr )
//{
//	D3DXMATRIX idMat;
//	D3DXMatrixIdentity( &idMat );
//	device->SetTransform( D3DTS_WORLD, &idMat);
//	device->SetTransform( D3DTS_PROJECTION, &idMat );
//	device->SetTransform( D3DTS_VIEW, &idMat );
//
//	D3DXMATRIX viewMat = sceneMgr->getViewMat();
//	D3DXMATRIX projMat = sceneMgr->getProjMat();
//	D3DXMATRIX rotationMat;
//	D3DXMatrixTranspose( &rotationMat, &viewMat );
//	rotationMat._14 = 0;
//	rotationMat._24 = 0;
//	rotationMat._34 = 0;
//	rotationMat._41 = 0;
//	rotationMat._42 = 0;
//	rotationMat._43 = 0;
//	rotationMat._44 = 1;
//	int j = 0;
//
//	MyGameSceneBillboardSet::billboardVertex* transformedVertexBuffer = new MyGameSceneBillboardSet::billboardVertex[entityList.size()*verticesNumPerRect];
//	for( auto itr = entityList.begin();
//		itr != entityList.end();
//		++ itr )
//	{
//		D3DXMATRIX com = (*itr)->getCombinedMatrix();
//		D3DXMATRIX finalMat = com * viewMat * projMat;
//		(*itr)->setRotateMatrix( rotationMat );//←是否要移到别处？
//		
//		MyGameSceneBillboardSet::billboardVertex transformedVertices[verticesNumPerRect];
//		for( int i = 0; i != verticesNumPerRect; ++ i )
//		{
//			this->multiplyMatrix( vertices[i], finalMat, transformedVertices[i] );//变换
//		}
//		memcpy(// transformedVertexBuffer + j * sizeof( billboardVertex ) * verticesNumPerRect,
//				&transformedVertexBuffer[j*verticesNumPerRect],
//				transformedVertices,
//				sizeof( billboardVertex ) * verticesNumPerRect );
//
//		++ j;
//	}
//	qsort( transformedVertexBuffer,
//		entityList.size(),
//		sizeof( MyGameSceneBillboardSet::billboardVertex ) * verticesNumPerRect,
//		MyGameSceneBillboardSet::compareTwoRects );
//
//	
//	device->SetFVF( MyGameSceneBillboardSet::billboardVertex::fvf );
//	device->SetTexture( 0, texture );
//	device->DrawPrimitiveUP( D3DPT_TRIANGLELIST, j * verticesNumPerRect / 3, transformedVertexBuffer,  sizeof( MyGameSceneBillboardSet::billboardVertex ) );
//	
//	delete[] transformedVertexBuffer;
//}
void MyGameSceneBillboardSet::multiplyMatrix( float& x, float& y, float& z, D3DXMATRIX& matrix )
{
	//[ x, y, z, 1] * MATRIX = ?

	float nX = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + matrix.m[3][0];
	float nY = y * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + matrix.m[3][1];
	float nZ = y * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + matrix.m[3][2];
	float nW = y * matrix.m[0][3] + y * matrix.m[1][3] + z * matrix.m[2][3] + matrix.m[3][3];

	x = nX / nW;
	y = nY / nW;
	z = nZ / nW;
}

void MyGameSceneBillboardSet::multiplyMatrix(  struct MyGameSceneBillboardSet::billboardVertex& in, D3DXMATRIX& matrix, struct MyGameSceneBillboardSet::billboardVertex& out )
{
	float x = in.x;
	float y = in.y;
	float z = in.z;

	float nX = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + matrix.m[3][0];
	float nY = y * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + matrix.m[3][1];
	float nZ = y * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + matrix.m[3][2];
	float nW = y * matrix.m[0][3] + y * matrix.m[1][3] + z * matrix.m[2][3] + matrix.m[3][3];

	out = in;
	
	out.color = 0xffffffff;

	out.x = nX/nW;
	out.y = nY/nW;
	out.z = nZ/nW;

	out.v = in.v;
	out.u = in.u;
	
}