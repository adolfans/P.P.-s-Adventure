
#include "stdafx.h"
#include "MyGameIndexBuffer.h"
#include "MyGameFunctions.h"

IDirect3DDevice9* MyGameIndexBuffer::pD9Device = 0;
MyGameIndexBuffer::MyGameIndexBuffer( unsigned int _indicesNum )
{
	HR( pD9Device->CreateIndexBuffer( _indicesNum*sizeof(WORD), D3DUSAGE_WRITEONLY,
											D3DFMT_INDEX16,
											D3DPOOL_MANAGED,
											&pIndexBuffer,
											0 ) );
}


MyGameIndexBuffer::~MyGameIndexBuffer(void)
{
	pIndexBuffer->Release();
}

void MyGameIndexBuffer::SetIndexBuffer( void* _pSrcIndexBuffer, unsigned int _size )// i dont know if i could choose a better name
{
	void* _pDest;
	pIndexBuffer->Lock( 0, _size, &_pDest, 0 ); 
	memcpy( _pDest, _pSrcIndexBuffer, _size );
	pIndexBuffer->Unlock();
}
