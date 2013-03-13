#include "StdAfx.h"
#include "MyGameVertexBuffer.h"


MyGameVertexBuffer::MyGameVertexBuffer(  )
{
}


MyGameVertexBuffer::~MyGameVertexBuffer(void)
{
	vertexBuffer->Release();
}
