#include "StdAfx.h"
#include "MyShadowMap.h"
#include "MyGame3DDevice.h"

MyShadowMap::MyShadowMap(unsigned int width, unsigned int height)
	:pDevice( MyGame3DDevice::GetSingleton()->GetDevice() )
{
	//IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	HR( pDevice->CreateTexture( MyGame3DDevice::GetWidth(),
											MyGame3DDevice::GetHeight(),
											1,
											D3DUSAGE_RENDERTARGET,
											//D3DFMT_A8R8G8B8,
											//D3DFMT_R32F,
											D3DFMT_R16F,
											D3DPOOL_DEFAULT,
											&shadowMap,
											0 ) );

	HR( shadowMap->GetSurfaceLevel( 0, &this->shadowMapSurface ));

}


MyShadowMap::~MyShadowMap(void)
{
	IRelease( shadowMap );
	IRelease( shadowMapSurface );
}

void MyShadowMap::start()
{
	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
	
	HR( pDevice->SetRenderTarget( 0, this->shadowMapSurface ) );
			
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0);
	
}

void MyShadowMap::end()
{
	pDevice->EndScene();
	MyGame3DDevice::GetSingleton()->restoreScreenRenderTarget();
	pDevice->BeginScene();
}

IDirect3DTexture9* MyShadowMap::getShadowMap()
{
	return this->shadowMap;
}