#include "StdAfx.h"
#include "MyCharacterController.h"


MyCharacterController::MyCharacterController(PxPhysics& sdk,
											PxScene* pScene,
											PxControllerManager* pManager,
											float height,
											SkinnedMesh* pMesh,
											MyGameSceneEntity* pEnt)
											:
			pCharMesh(pMesh),pCharEntity( pEnt )
{
	PxMaterial* mMaterial = sdk.createMaterial( 0.5, 0.5, 0.5 );
	PxCapsuleControllerDesc desc;
	desc.height = height;		//½ÇÉ«¸ß¶È
	desc.radius = 1.0f;
	desc.density				= 10.0f;
	desc.scaleCoeff			= 0.899998f;
	desc.material				= mMaterial;
	desc.position				= PxExtendedVec3( 0.0f, 0.0f, 0.0f );
	desc.slopeLimit			= 0.0f;
	desc.stepOffset			= 0.05f;
	desc.invisibleWallHeight	= 0.0f;
	desc.maxJumpHeight		= 10.0f;
	desc.callback				= 0;
	desc.behaviorCallback		= 0;
	desc.upDirection = PxVec3( 0.0f, 1.0f, 0.0f );
	desc.slopeLimit = cosf( 0.25 *PxPi);

	this->pCharacterCtl = pManager->createController( sdk, pScene, desc );
}


MyCharacterController::~MyCharacterController(void)
{
}

void MyCharacterController::move( float x, float y, float z )
{

}

void MyCharacterController::bindAnimToMove( const char* aniName )
{
	moveAni = aniName;
}

void MyCharacterController::playAnim( const char* aniName )
{
	currentAni = aniName;
	pCharMesh->setAnimation( string( aniName ));
	
}