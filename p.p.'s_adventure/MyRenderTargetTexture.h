#pragma once
#include <d3d9.h>
class MyRenderTargetTexture
{
private:
	IDirect3DTexture9* texture;
	IDirect3DSurface9* surface;
public:
	MyRenderTargetTexture(void);
	~MyRenderTargetTexture(void);
	void setRenderTarget( int level );
	IDirect3DTexture9* getTexture()
	{	return texture; }
};

