#pragma once
#include <d3dx9.h>
#include "MyGameScene.h"
#include "MyGameSceneBillboard.h"
#include "SkinnedMesh.h"
#include "MyGame3DEffect.h"
#include "MyGameMusic.h"
#include "MyGameCamera.h"
class Scene1
{
private:
	//IDirect3DVertexBuffer9* rectBuffer;
	//IDirect3DVertexDeclaration9* vDecl;
	//IDirect3DDevice9* pDevice;
	//D3DXMATRIX viewMat;
	//D3DXMATRIX perspectiveMat;
	D3DXMATRIX lightViewPersMat;
	D3DXMATRIX idMat;
	//IDirect3DTexture9* _tex;
	//ID3DXEffect* pTransformEffect;
	MyGame3DEffect* pGenShadowMapEffect;
	MyGameSceneBillboard* testBoard;
	SkinnedMesh* loli;
	MyGameSceneEntity* loliEnt;
	MyGame3DEffect* pLoliEffect;
	//ID3DXMesh*
	//MyGameSceneNode* loliParentNode;
	MyGameSceneNode* sceneRoot;
	MyGameMesh* sword;
	MyGameSceneEntity* swordEnt;
	MyGameMesh* plane;

	MyGameMesh* boxMesh;
	MyGameSceneEntity* boxEnt;

	MyGameSceneEntity* planeEnt;
	MyGame3DEffect* pPlatformEffect;
	MyGameSceneNode* boxParentNode;

	IDirect3DSurface9* pScreenRenderSurface;
	IDirect3DTexture9* pPass1RenderTarget;
	IDirect3DSurface9* pPass1RenderTargetSurface;

	MyGameSceneManager* sceneMgr;

	MyGameCamera* cam;

	MyGameMusic bgm;

	float stepLength;
public:
	Scene1(void);
	virtual ~Scene1(void);
	void Render();

	void UpdatePhysx();
};

