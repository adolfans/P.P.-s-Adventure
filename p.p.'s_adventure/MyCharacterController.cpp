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
			pCharMesh(pMesh),pCharEntity( pEnt ),phxScene(pScene)
{
	PxMaterial* mMaterial		= MyGame3DDevice::GetSingleton()->getPhysX()->createMaterial( 0.5, 0.5, 0.5 );//sdk.createMaterial( 0.5, 0.5, 0.5 );
	PxCapsuleControllerDesc desc;
	desc.height					= height;		//角色高度
	desc.radius					= 1.0f;
	desc.density				= 10.0f;
	desc.scaleCoeff				= 0.899998f;
	desc.material				= mMaterial;
	desc.position				= PxExtendedVec3( 0.0f, 0.0f, 0.0f );
	//desc.slopeLimit				= cosf(PxMath::degToRad(45.0f));
	desc.stepOffset				= 0.05f;
	desc.invisibleWallHeight	= 0.0f;
	desc.maxJumpHeight			= 10.0f;
	desc.callback				= 0;
	desc.behaviorCallback		= 0;
	desc.upDirection = PxVec3( 0.0f, 1.0f, 0.0f );
	desc.slopeLimit = 0.0;//cosf( static_cast<float>(0.4*PxPi));
	//desc.nonWalkableMode = PxCCTNonWalkableMode::ePREVENT_CLIMBING;
	desc.invisibleWallHeight	= 3000.0f;

	this->pCharacterCtl = pManager->createController( *MyGame3DDevice::GetSingleton()->getPhysX(), pScene, desc );

	this->pCharacterCtl->setStepOffset( height*2.0f );//TODO:
	PxExtendedVec3 pos = pCharacterCtl->getFootPosition();
	pos.y = -pCharacterCtl->getFootPosition().y;
	pCharacterCtl->setPosition( pos );		//把脚底的起始位置设为0，因为模型的坐标是按脚底位置为基准的
}


MyCharacterController::~MyCharacterController(void)
{
	this->pCharacterCtl->release();
}

void MyCharacterController::move( float x, float y, float z, PxF32 elapsedTime ) const
{
	if( pCharacterCtl->getFootPosition().y  - 1.0 > 0.0 )//如果脚在空中
	{
		PxVec3 moveVec( 0.0f, -pCharacterCtl->getFootPosition().y, 0.0f );
		static PxControllerFilters filters( 0 );//只是为了防止这个对象重复构造而已
		pCharacterCtl->move( moveVec, 0.01f, elapsedTime, filters );

		//phxScene->fetchResults();
		//phxScene->simulate( elapsedTime );

		const PxExtendedVec3 pos = pCharacterCtl->getFootPosition();//脚的位置

		this->pCharEntity->getNode()->setPosition( pos.x, pos.y, pos.z );
	}
	PxVec3 moveVec( x, y, z );
	static PxControllerFilters filters( 0 );//只是为了防止这个对象重复构造而已
	pCharacterCtl->move( moveVec, 0.01f, elapsedTime, filters );
	
	//phxScene->simulate( elapsedTime );

	//phxScene->fetchResults();
	
	const PxExtendedVec3 pos = pCharacterCtl->getFootPosition();//脚的位置

	this->pCharEntity->getNode()->setPosition( pos.x, pos.y, pos.z );
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

D3DVECTOR MyCharacterController::getPosition()
{
	PxExtendedVec3 fpos = pCharacterCtl->getFootPosition();
	D3DVECTOR pos = { fpos.x, fpos.y, fpos.z };
	return pos;
}