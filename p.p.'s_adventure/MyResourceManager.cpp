#include "StdAfx.h"
#include "MyResourceManager.h"
#include <d3dx9.h>
#include "MyGame3DDevice.h"
string MyResourceManager::textureFolder = "textures/";
MyResourceManager::MyResourceManager(void)
{
}


MyResourceManager::~MyResourceManager(void)
{
}

IDirect3DTexture9*	MyResourceManager::loadTexture( const string& fileName )
{
	static IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	string relativeFileName = textureFolder + fileName;
	IDirect3DTexture9*	texture;
	HR(D3DXCreateTextureFromFileA(
					pDevice,
					relativeFileName.c_str(),
					&texture ));
	return texture;
}
