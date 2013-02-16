#pragma once
#include "MyGameSceneEntity.h"
#ifdef free
#undef free
#endif
#ifdef realloc
#undef realloc
#endif
#include <PxPhysicsAPI.h>
#include "SkinnedMesh.h"
using namespace MyGameScene;
using namespace physx;
//绑定Entity和Mesh，并将Mesh中的动画绑定到“移动”等动作。

class MyCharacterController
{
private:
	//角色的Entity
	MyGameSceneEntity* pCharEntity;
	SkinnedMesh*		pCharMesh;


	//角色对应的PxController
	PxController* pCharacterCtl;//由controller来管理对象的生命周期

	string moveAni;

	string currentAni;
public:
	MyCharacterController(PxPhysics& sdk,
											PxScene* pScene,
											PxControllerManager* pManager,
											float height,
											SkinnedMesh* pMesh,
											MyGameSceneEntity* pEnt);
	~MyCharacterController(void);

	//void setPosition( float x, float y, float z );
	void move( float x, float y, float z );
	void rotateX( float degree );
	void scale( float x, float y, float z );
	void bindAnimToMove( const char* aniName );
	void playAnim( const char* aniName );
	bool ifCurrentAnimEnd();
};

