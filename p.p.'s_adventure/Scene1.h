#pragma once
#include <d3dx9.h>
#include <CEGUIWindow.h>
using namespace CEGUI;

#include "MyGameScene.h"
#include "MyGameSceneBillboard.h"
#include "SkinnedMesh.h"
#include "MyGame3DEffect.h"
#include "MyGameMusic.h"
#include "MyGameCamera.h"
#include "MyCharacterController.h"
#include "MyPlayerRole.h"
#include "MyRenderTargetTexture.h"
#include "MyGamePointSprites.h"
#include "MyGameSceneBillboardSet.h"
class Scene1
{
private:
	MyGameMusic		bgm;
	MyGamePointSprites sprites;

	D3DXMATRIX idMat;
	MyGame3DEffect* pGenShadowMapEffect;
	MyGameSceneBillboard* testBoard;
	MyGameSceneBillboardSet* testBoard1;
	SkinnedMesh* loli;
	MyGameSceneEntity* loliEnt;
	//MyGame3DEffect* pLoliEffect;
	MyGameSceneNode* sceneRoot;
	MyGameMesh* sword;
	MyGameSceneEntity* swordEnt;
	MyGameMesh* plane;

	MyGameMesh* boxMesh;
	MyGameSceneEntity* boxEnt;

	MyGameSceneEntity* planeEnt;
	MyGame3DEffect* pPlatformEffect;
	MyGame3DEffect* mirrorEffect;
	MyGameSceneNode* boxParentNode;

	MyGameSceneManager* sceneMgr;

	MyGameCamera*	cam;

	MyGame3DEffect*		waterEffect;
	D3DXHANDLE		waterSpeedHandle;

	MyGameCamera*	lightCam;

	//float			stepLength;

	MyPlayerRole*	role1;

	CEGUI::Scrollbar* waterSpeedController;
	CEGUI::Scrollbar* bgmVolume;

	CEGUI::Checkbox*	shadowCheckBox;
	CEGUI::Checkbox*	reflectionCheckBox;

	Window*				fpsBoard;

	CEGUI::Scrollbar*	lightAngle;

	MyRenderTargetTexture*	mirrorTexture;
	MyRenderTargetTexture*	shadowMap;
public:
	Scene1(void);
	virtual ~Scene1(void);
	void Render();

	void Update(MSG msg);

	bool OnShadowChanged( const CEGUI::EventArgs& e );
	bool OnLightAngleChanged( const CEGUI::EventArgs& e );
	bool OnWaterReflectionChanged( const CEGUI::EventArgs& e );
	//void UpdatePhysx();
};

