#pragma once
#include <d3d9.h>
class MyRenderTargetTexture
{
private:
	IDirect3DDevice9* device;
	IDirect3DTexture9* texture;
	IDirect3DSurface9* surface;
public:
	enum TextureMode
	{
		Gray,
		ARGB
	};

	MyRenderTargetTexture(TextureMode mode);
	~MyRenderTargetTexture(void);
	void setRenderTarget( int level );
	void clear();
	IDirect3DTexture9* getTexture()
	{	return texture; }
};

