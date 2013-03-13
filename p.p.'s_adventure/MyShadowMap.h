//已废弃，不再使用，替换解决方案为MyRenderTargetTexture
#pragma once
#include <d3dx9.h>
class MyShadowMap
{
private:
	::IDirect3DTexture9* shadowMap;
	IDirect3DSurface9*	shadowMapSurface;
	IDirect3DDevice9*  pDevice;
public:
	MyShadowMap(unsigned int width, unsigned int height);
	~MyShadowMap(void);

	void start();
	void end();
	IDirect3DTexture9* getShadowMap();
};

