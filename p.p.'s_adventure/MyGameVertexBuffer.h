#pragma once
#include <d3d9.h>
class MyGameVertexBuffer
{
private:
	IDirect3DVertexBuffer9* vertexBuffer;
	unsigned	int			bufferSize;
public:
	MyGameVertexBuffer( /*unsigned int verticeSize*/ );
	~MyGameVertexBuffer(void);
	void EnableVertexBuffer( IDirect3DVertexBuffer9* _buffer )
	{	vertexBuffer = _buffer;	}
	void SetBufferSize( int _size )
	{	bufferSize = _size; }
	IDirect3DVertexBuffer9* GetVertexBuffer()
	{	return vertexBuffer;
	}
	unsigned int GetBufferSize()
	{
		return bufferSize;
	}
	void SetVertexBuffer( IDirect3DVertexBuffer9* _vertexBuffer )
	{ this->vertexBuffer = _vertexBuffer; }
};

