#pragma once
#include "MyCharacterController.h"
#ifdef free
#undef free
#endif
#ifdef realloc
#undef realloc
#endif
#include <PxPhysicsAPI.h>
class MyPlayerRole
{
private:
	MyCharacterController* con;
	int			attackTimes;

	float		lastAttackTime;
	float			stepLength;
	int			attack;
public:
	MyPlayerRole(PxScene* pScene,
											PxControllerManager* pManager,
											float height,
											SkinnedMesh* pMesh,
											MyGameSceneEntity* pEnt);
	~MyPlayerRole(void);

	void update( MSG msg );
};

