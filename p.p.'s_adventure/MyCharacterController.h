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
//��Entity��Mesh������Mesh�еĶ����󶨵����ƶ����ȶ�����

class MyCharacterController
{
private:
	//��ɫ��Entity
	MyGameSceneEntity* pCharEntity;
	SkinnedMesh*		pCharMesh;


	//��ɫ��Ӧ��PxController
	PxController* pCharacterCtl;//��controller������������������

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

