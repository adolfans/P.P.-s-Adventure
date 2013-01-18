#include "StdAfx.h"
#include "MyGameVertexBuffer.h"


MyGameVertexBuffer::MyGameVertexBuffer( /*unsigned int verticeSize*/ )
{
	/*pD3D9InstanceDevice->CreateVertexBuffer(
												verticeSize,
												D3DUSAGE_WRITEONLY,
												0,
												D3DPOOL_MANAGED,
												&pVertexBuffer,
												0 ) );*/
}


MyGameVertexBuffer::~MyGameVertexBuffer(void)
{
	vertexBuffer->Release();
}
