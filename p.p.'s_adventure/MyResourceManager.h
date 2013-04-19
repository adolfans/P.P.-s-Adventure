#pragma once
#include <string>
#include <d3d9.h>
using std::string;
class MyResourceManager
{
	static string textureFolder;
	//static string fbxFileFolder;
public:
	MyResourceManager(void);
	~MyResourceManager(void);

	static IDirect3DTexture9*	loadTexture( const string& fileName );
};

