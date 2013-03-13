#include "StdAfx.h"
#include "MyRenderTargetTexture.h"
#include "MyGame3DDevice.h"

MyRenderTargetTexture::MyRenderTargetTexture(MyRenderTargetTexture::TextureMode mode)
{
	device = MyGame3DDevice::GetSingleton()->GetDevice();
	D3DFORMAT format;
	switch( mode )
	{
	case Gray:
		format = D3DFMT_R16F;	//TODO:检测硬件是否支持该格式，另有R32F
		break;
	case ARGB:
		format = D3DFMT_A8R8G8B8;
		break;
	default:
		format = D3DFMT_A8R8G8B8;
	}
	device->CreateTexture( MyGame3DDevice::GetWidth(),
							MyGame3DDevice::GetHeight(),
							1,			//level,贴图的表面层数（？）
							D3DUSAGE_RENDERTARGET,
							format,
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
	//device->SetRenderState( D3DRS_ZENABLE, true );
	//device->SetRenderState( D3DRS_ZWRITEENABLE, true );
	device->SetRenderTarget( level, surface );
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0);
	
}

void MyRenderTargetTexture::clear()
{
	device->SetRenderTarget( 0, surface );
	device->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0 );
	MyGame3DDevice::GetSingleton()->restoreScreenRenderTarget();
}