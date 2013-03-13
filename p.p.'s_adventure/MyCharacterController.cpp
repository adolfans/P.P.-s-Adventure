#include "StdAfx.h"
#include <math.h>
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
			pCharMesh(pMesh),pCharEntity( pEnt ),phxScene(pScene),animating( false )
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

void MyCharacterController::move( float x, float y, float z, PxF32 elapsedTime )
{
	if( pCharacterCtl->getFootPosition().y  - 1.0 > -120.0 )//如果脚在空中
	{
		PxVec3 moveVec( 0.0f, -pCharacterCtl->getFootPosition().y-120, 0.0f );
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

	//根据移动方向计算角度：
	D3DXVECTOR3 moveDir( x, 0.0f, z );
	D3DXVECTOR3 currentDir( 0.0f, 0.0f, -1.0f );
	float angle;
	float dot = D3DXVec3Dot( &moveDir, &currentDir );
	dot = dot/(D3DXVec3Length(&moveDir)*D3DXVec3Length(&currentDir) );
	if( x > 0 )//逆时针转动的,取负值 
	{
		angle = -acosf( dot );
	}else
	{
		angle = acosf( dot );
	}
	//this->changeDirection( angle );
	if( angle - currentDirection > 0.001 || angle - currentDirection < -0.001 )
	{
		this->changeDirection( angle );
		currentDirection = angle;
	}
	if( !this->animating )//如果正在攻击动作的动画，则不切换动画,如果没移动多少，或者说本函数的参数小于某个范围，则播放“stop”动画（这个好像不用我播放）
		//this->pCharMesh->setAnimation( string( "run" ), true );//尽量减少setAnimation的调用，把判断动画是否应该终止放到SkinnedMesh中，并由SkinnedMesh来管理是否停止
		if( fabsf(x) < 0.1 && fabsf(y) < 0.1 && fabsf(z) < 0.1 )
			pCharMesh->setAnimation( string("stop"), true );
		else
			pCharMesh->setAnimation( string("run"), true );
	//如果move的距离小于一定的值，并且没有其他动画正在播放，则循环播放stop，注意，循环播放stop的函数只要调用一次就够了
}

void MyCharacterController::bindAnimToMove( const char* aniName )
{
	moveAni = aniName;
}

void MyCharacterController::playAnim( const string& aniName )
{
	if( !animating )
	{
		pCharMesh->setAnimation( aniName, false );
		animating = true;
	}
}

D3DVECTOR MyCharacterController::getPosition()
{
	PxExtendedVec3 fpos = pCharacterCtl->getFootPosition();
	D3DVECTOR pos = { fpos.x, fpos.y, fpos.z };
	return pos;
}

void MyCharacterController::update()
{
	if( animating&&pCharMesh->ifAnimEnded() )
	{
			animating = false;
	}
	if( animQueue.size()&&!animating )//如果在奔跑或者是在静止
	{
		this->playAnim( animQueue.front() );
		animQueue.pop();
		animating = true;
	}
}

void MyCharacterController::addAnimToQueue( const string& animName )
{
	animQueue.push( animName );
}