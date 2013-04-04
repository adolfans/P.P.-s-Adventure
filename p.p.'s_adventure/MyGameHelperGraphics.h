#pragma once
#include <d3d9.h>
#include "MyGameScene.h"
class MyGameHelperGraphics
{
private:
	struct vertex
	{
		float x, y, z;
		D3DCOLOR color;
	};
	static vertex transformedAxis[4];
	static DWORD vertexFVF;
	static unsigned int boxVerticeNum;
	static unsigned int boxIndiceNum;
	static IDirect3DVertexBuffer9*	boxVertexBuffer;
	static IDirect3DVertexBuffer9*	axisVertexBuffer;
	static IDirect3DIndexBuffer9*	boxIndexBuffer;

	static MyGameScene::MyGameSceneNode*	axisNode;
	static MyGameScene::MyGameSceneEntity*	boxEntity;

	static vertex							axisText[3];
	static IDirect3DTexture9*				xTexture;
	static IDirect3DTexture9*				yTexture;
	static IDirect3DTexture9*				zTexture;
public:
	MyGameHelperGraphics(void);
	~MyGameHelperGraphics(void);

	static void initialize();
	static void cleanup();
	static void draw( MyGameScene::MyGameSceneManager* sceneMgr );

	/*因为被当作UI来绘制，所以最后调用*/
	static void drawAxisText();

	static void enableBox( MyGameScene::MyGameSceneEntity* entity );
	static void disableBox();
	static void enableAxis( MyGameScene::MyGameSceneNode* node );
	static void disableAxis();

	static void getAxisPosition( int num, float& x, float& y );

	static void activeAxis( int  );
};

