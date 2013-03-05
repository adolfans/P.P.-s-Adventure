#include "StdAfx.h"
#include "MyRenderTargetTexture.h"
#include "MyGame3DDevice.h"

MyRenderTargetTexture::MyRenderTargetTexture(void)
{
	IDirect3DDevice9* device = MyGame3DDevice::GetSingleton()->GetDevice();

	device->CreateTexture( MyGame3DDevice::GetWidth(),
							MyGame3DDevice::GetHeight(),
							0,			//level,贴图的表面层数
							D3DUSAGE_RENDERTARGET,
							D3DFMT_A8R8G8B8,
							D3DPOOL_DEFAULT,
							&texture,
							0 );

	texture->GetSurfaceLevel( 0, &surface );
}


MyRenderTargetTexture::~MyRenderTargetTexture(void)
{
	D3DXSaveTextureToFile(L"测试第一个target.png", D3DXIFF_PNG, texture, 0 );
	IRelease( texture );
	IRelease( surface );
}


void MyRenderTargetTexture::setRenderTarget( int level )
{
	IDirect3DDevice9* device = MyGame3DDevice::GetSingleton()->GetDevice();
	device->SetRenderTarget( level, surface );
}