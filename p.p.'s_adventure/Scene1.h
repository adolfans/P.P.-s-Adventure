#pragma once
#include <d3dx9.h>
#include "MyGameScene.h"
#include "MyGameSceneBillboard.h"
#include "SkinnedMesh.h"
#include "MyGame3DEffect.h"
#include "MyGameMusic.h"
#include "MyGameCamera.h"
#include "MyShadowMap.h"
#include "MyCharacterController.h"
class Scene1
{
private:
	//D3DXMATRIX lightViewPersMat;
	D3DXMATRIX idMat;
	MyGame3DEffect* pGenShadowMapEffect;
	MyGameSceneBillboard* testBoard;
	SkinnedMesh* loli;
	MyGameSceneEntity* loliEnt;
	MyGame3DEffect* pLoliEffect;
	MyGameSceneNode* sceneRoot;
	MyGameMesh* sword;
	MyGameSceneEntity* swordEnt;
	MyGameMesh* plane;

	MyGameMesh* boxMesh;
	MyGameSceneEntity* boxEnt;

	MyGameSceneEntity* planeEnt;
	MyGame3DEffect* pPlatformEffect;
	MyGameSceneNode* boxParentNode;

	MyGameSceneManager* sceneMgr;

	MyGameCamera* cam;

	MyGameMusic bgm;

	MyShadowMap* shadowMap;

	MyCharacterController* con;

	MyGame3DEffect*		waterEffect;
	D3DXHANDLE waterSpeedHandle;

	MyGameCamera* lightCam;

	float stepLength;
public:
	Scene1(void);
	virtual ~Scene1(void);
	void Render();

	void UpdatePhysx();
};

