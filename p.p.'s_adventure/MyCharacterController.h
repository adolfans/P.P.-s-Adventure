#pragma once
#include "MyGameSceneEntity.h"
#include "SkinnedMesh.h"
#include <queue>
using std::queue;
#ifdef free
#undef free
#endif
#ifdef realloc
#undef realloc
#endif
#include <PxPhysicsAPI.h>
using namespace MyGameScene;
using namespace physx;
//��Entity��Mesh������Mesh�еĶ����󶨵����ƶ����ȶ�����

class MyCharacterController
{
private:
	//��ɫ��Entity
	MyGameSceneEntity*	pCharEntity;
	SkinnedMesh*		pCharMesh;


	//��ɫ��Ӧ��PxController
	PxController*		pCharacterCtl;
	
	string currentAni;
	string moveAni;

	PxScene*			phxScene;

	static const PxF32		minDist;

	float				currentDirection;
	bool				animating;//�����ƶ�֮��Ķ����Ĳ�����״̬

	queue<string>		animQueue;
public:
	MyCharacterController(//PxPhysics& sdk,
											PxScene* pScene,
											PxControllerManager* pManager,
											float height,
											SkinnedMesh* pMesh,
											MyGameSceneEntity* pEnt);
	~MyCharacterController(void);

	//void setPosition( float x, float y, float z );
	void move( float x, float y, float z, PxF32 elapsedTime );
	void rotateX( float angle ) const;
	void scale( float x, float y, float z ) const;
	void bindAnimToMove( const char* aniName );
	void playAnim( const string& aniName );
	bool ifCurrentAnimEnd() const{ return pCharMesh->ifAnimEnded()&&!animQueue.size(); }
	D3DVECTOR getPosition();
	void changeDirection( float angle ) const
	{	this->pCharEntity->getNode()->setRotationAngleY( angle );}
	void update();

	void addAnimToQueue( const string& animName );
};

