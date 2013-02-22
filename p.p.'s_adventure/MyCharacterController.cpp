#include "StdAfx.h"
#include "MyCharacterController.h"
#include "MyGame3DDevice.h"

const PxF32 MyCharacterController::minDist = 0.001;

MyCharacterController::MyCharacterController(//PxPhysics& sdk,
											PxScene* pScene,
											PxControllerManager* pManager,
											float height,
											SkinnedMesh* pMesh,
											MyGameSceneEntity* pEnt)
											:
			pCharMesh(pMesh),pCharEntity( pEnt )
{
	PxMaterial* mMaterial		= MyGame3DDevice::GetSingleton()->getPhysX()->createMaterial( 0.5, 0.5, 0.5 );//sdk.createMaterial( 0.5, 0.5, 0.5 );
	PxCapsuleControllerDesc desc;
	desc.height					= height;		//½ÇÉ«¸ß¶È
	desc.radius					= 1.0f;
	desc.density				= 10.0f;
	desc.scaleCoeff				= 0.899998f;
	desc.material				= mMaterial;
	desc.position				= PxExtendedVec3( 0.0f, 0.0f, 0.0f );
	desc.slopeLimit				= 0.0f;
	desc.stepOffset				= 0.05f;
	desc.invisibleWallHeight	= 0.0f;
	desc.maxJumpHeight			= 10.0f;
	desc.callback				= 0;
	desc.behaviorCallback		= 0;
	desc.upDirection = PxVec3( 0.0f, 1.0f, 0.0f );
	desc.slopeLimit = cosf( static_cast<float>(0.25*PxPi));

	this->pCharacterCtl = pManager->createController( *MyGame3DDevice::GetSingleton()->getPhysX(), pScene, desc );
}


MyCharacterController::~MyCharacterController(void)
{
	this->pCharacterCtl->release();
}

void MyCharacterController::move( float x, float y, float z, PxF32 elapsedTime ) const
{

}

void MyCharacterController::bindAnimToMove( const char* aniName )
{
	moveAni = aniName;
}

void MyCharacterController::playAnim( const char* aniName ) const
{
	//currentAni = string( aniName);
	pCharMesh->setAnimation( string( aniName ));
	
}