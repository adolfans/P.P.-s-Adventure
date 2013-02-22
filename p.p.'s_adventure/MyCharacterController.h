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
	MyGameSceneEntity*	pCharEntity;
	SkinnedMesh*		pCharMesh;


	//角色对应的PxController
	PxController*		pCharacterCtl;
	
	string currentAni;
	string moveAni;


	static const PxF32		minDist;

public:
	MyCharacterController(//PxPhysics& sdk,
											PxScene* pScene,
											PxControllerManager* pManager,
											float height,
											SkinnedMesh* pMesh,
											MyGameSceneEntity* pEnt);
	~MyCharacterController(void);

	//void setPosition( float x, float y, float z );
	void move( float x, float y, float z, PxF32 elapsedTime ) const;
	void rotateX( float degree ) const;
	void scale( float x, float y, float z ) const;
	void bindAnimToMove( const char* aniName );
	void playAnim( const char* aniName ) const;
	bool ifCurrentAnimEnd() const;
};

