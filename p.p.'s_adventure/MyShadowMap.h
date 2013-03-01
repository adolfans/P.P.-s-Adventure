#pragma once
#include <d3dx9.h>
class MyShadowMap
{
private:
	::IDirect3DTexture9* shadowMap;
	IDirect3DSurface9*	shadowMapSurface;
	IDirect3DSurface9*	screenSurface;
	IDirect3DDevice9*  pDevice;
public:
	MyShadowMap(unsigned int width, unsigned int height);
	~MyShadowMap(void);

	void start();
	void end();
	IDirect3DTexture9* getShadowMap();
};

