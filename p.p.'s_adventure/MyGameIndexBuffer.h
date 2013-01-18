#pragma once
#include <d3d9.h>
class MyGameIndexBuffer
{
private:
	IDirect3DIndexBuffer9* pIndexBuffer;

	static IDirect3DDevice9* pD9Device;
public:
	MyGameIndexBuffer( unsigned int _indicesNum );
	~MyGameIndexBuffer(void);

	void SetIndexBuffer( void* _pSrcIndexBuffer, unsigned int _size );

	static void SetIndexBufferDevice( IDirect3DDevice9* _pD9Device )
	{ pD9Device = _pD9Device;	}

	IDirect3DIndexBuffer9* GetIndexBuffer()
	{ return pIndexBuffer;}
};

