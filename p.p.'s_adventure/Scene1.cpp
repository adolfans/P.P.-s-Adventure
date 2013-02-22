#include "StdAfx.h"
#include "Scene1.h"
#include "MyGame3DDevice.h"
#include "MyGameFunctions.h"
#include <time.h>
#include "MyGameSceneManager.h"
#include "MyGameSceneEntity.h"
#include "MyGameMusic.h"
#include <d3dx9math.h>
#include <exception>
using std::runtime_error;
#include <iostream>
#include <math.h>
#include "MyGameInstance.h"
using std::cerr;
using std::endl;
#ifdef free
#undef free
#endif
#ifdef realloc
#undef realloc
#endif
#include <PxPhysicsAPI.h>
using namespace physx;

//static PxPhysics* gPhysicsSDK = NULL;
//static PxDefaultErrorCallback gDefaultErrorCallback;
//static PxDefaultAllocator gDefaultAllocatorCallback;
////static PxSimulationFilterShader gDefaultFilterShader=PxDefaultSimulationFilterShader;
//
//PxFoundation* mFoundation;
//
//static PxProfileZoneManager* mProfileZoneManager;

// = NULL;
#include <d3dx9math.h>
#include <math.h>
PxReal myTimestep = 1.0f/60.0f;
PxRigidActor *box;
PxDefaultCpuDispatcher* mCpuDispatcher;

PxController* characterCtl;
Scene1::Scene1(void):
	bgm(string("YSO_001.ogg"))
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	
	sceneMgr = new MyGameSceneManager;
	

	cam = sceneMgr->CreateCamera( 0.0f, 30.0f, -40.0f, 0.0f, 0.0f, 0.0f );

	sceneMgr->setCamera( cam );

	//
	//lightViewMat
	//
	D3DXVECTOR3 target( 0.0f, 0.0f ,0.0f );
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );
	D3DXMATRIX lightViewMat, lightPersMat;
	D3DXVECTOR3 light( -10.0f, 30.0f, -10.0f );
	//D3DXVECTOR3 light( -1000.0f, 3000.0f, -1000.0f );
	D3DXMatrixLookAtLH( &lightViewMat, &light, &target,&up );

	//
	//lightPersMat (我去这名字起错了，perspective是透视。嘛不管了。)
	//
	//D3DXMatrixPerspectiveFovLH( &perspectiveMat, pi*0.2F, w/h, 5.0f, 100.0f );//最后一个一开始是2000= =||
	//D3DXMatrixPerspectiveFovLH( &perspectiveMat, pi*0.2F, w/h, 20.0f, 80.0f );//最后一个一开始是2000= =||
	float h = (float)MyGame3DDevice::GetHeight();
	float w = (float)MyGame3DDevice::GetWidth();
	D3DXMatrixOrthoLH( &lightPersMat, w/20.0f, h/20.0f, 5.0f, 100.0f );

	//
	//lightViewPersMat
	//
	lightViewPersMat = lightViewMat* lightPersMat;

	//MyGameSceneBillboard::SetViewMatrix( &viewMat );
	MyGameSceneBillboard::SetViewMatrix( &cam->getViewMatrix() );

	D3DXMatrixIdentity( &idMat );

	pGenShadowMapEffect = new MyGame3DEffect( "shadowMapping.fx" );

	pLoliEffect = new MyGame3DEffect( "newMesh.fx" );

	pPlatformEffect = new MyGame3DEffect( "scene.fx" );

	testBoard = MyGameSceneBillboard::CreateBillboard();
	MyGameSceneBillboard::AttachBillboardToScene( testBoard );
	testBoard->SetSize( 3.0f, 3.0f );
	testBoard->CreateTextureFromFile( 0, "951905dfc03b942d4854031e.jpg" );

	testBoard->EnableTextureFromId( 0 );

	sceneRoot = sceneMgr->CreateSceneNode("sceneRoot");
	
	//必须打开那啥编译选项
	loli = dynamic_cast<SkinnedMesh*>(MyGameMeshManager::createMyGameMesh( MyGameMeshManager::SKINNED ));
	loli->loadFromX( sceneMgr );
	loliEnt = sceneMgr->CreateSceneEntity( loli, "testLoli" );
	sceneRoot->attachEntity( loliEnt );
	pGenShadowMapEffect->AddEntity( loliEnt );
	pLoliEffect->AddEntity( loliEnt );
	loliEnt->getNode()->move( 0.0f, 1000.0f, 0.0f );
	

	sword = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	sword->loadMeshFromXFile( "equalizer.X" );
	swordEnt = sceneMgr->CreateSceneEntity( sword, "testSword" );
	pGenShadowMapEffect->AddEntity( swordEnt );
	MyGameSceneNode::getNodeByName("Bip001_R_Hand")->attachEntity( swordEnt );
	
	pPlatformEffect->AddEntity( swordEnt );

	MyGameMesh* ball = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	ball->loadMeshFromXFile( "ball.X" );
	MyGameSceneEntity* ballEnt = sceneMgr->CreateSceneEntity( ball, "ball" );
	//pGenShadowMapEffect->AddEntity( ballEnt );
	//pPlatformEffect->AddEntity( ballEnt );
	MyGameSceneNode::getNodeByName("Bip001_Head")->attachEntity( ballEnt );

	//
	//加载plane
	//
	plane = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	plane->createPlaneXZ( 4000.0f, 2000.0f );//生成模型
	plane->createTexture( "colorful-1556.jpg" );
	planeEnt = sceneMgr->CreateSceneEntity( plane, "testPlane" );
	sceneRoot->attachEntity( planeEnt );//将entity attach到sceneRoot节点下面（其实是作为其子节点存在）
	pGenShadowMapEffect->AddEntity( planeEnt );//将entity放入pGenShadowMapEffect内，由其管理entity的渲染模式
	pPlatformEffect->AddEntity( planeEnt );
	
	sceneRoot->scale( 0.01, 0.01, 0.01 );
	//
	//Box模型
	boxParentNode = sceneMgr->CreateSceneNode( "boxParent" );
	sceneRoot->AddChild(boxParentNode);
	boxMesh = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	boxMesh->loadMeshFromXFile( "testBox.X" );
	boxEnt = sceneMgr->CreateSceneEntity( boxMesh, "testBox" );
	boxParentNode->attachEntity( boxEnt );
	pPlatformEffect->AddEntity( boxEnt );
	pGenShadowMapEffect->AddEntity( boxEnt );



	HR( pDevice->CreateTexture( MyGame3DDevice::GetWidth(),
											MyGame3DDevice::GetHeight(),
											1,
											D3DUSAGE_RENDERTARGET,
											//D3DFMT_A8R8G8B8,
											D3DFMT_R16F,
											D3DPOOL_DEFAULT,
											&pPass1RenderTarget,
											0 ) );

	//MyGameSceneManager::setShadowMap( pPass1RenderTarget );
	sceneMgr->setShadowMap( pPass1RenderTarget );
		
	HR( pDevice->GetRenderTarget( 0, &pScreenRenderSurface ) );//获取当前的显示表面

	pPass1RenderTargetSurface = 0;
	HR( pPass1RenderTarget->GetSurfaceLevel( 0, &pPass1RenderTargetSurface ) );

	


	//physx
	
	PxPhysics* gPhysicsSDK = MyGame3DDevice::GetSingleton()->getPhysX();

	PxScene* gScene = this->sceneMgr->getPhysXScene();

	PxMaterial* mMaterial = gPhysicsSDK->createMaterial(0.5,0.5,0.5);


	//Create actors 
    //1) Create ground plane
    PxReal d = 0.0f;	 
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f),PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));

	PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(pose);
	if (!plane)
			cerr<<"create plane failed!"<<endl;

	PxShape* shape = plane->createShape(PxPlaneGeometry(), *mMaterial);
	if (!shape)
		cerr<<"create shape failed!"<<endl;
	gScene->addActor(*plane);

	//2) Create cube	 
	PxReal density = 1.0f;
	PxTransform transform(PxVec3(15.0f, 20, 0.0f), PxQuat::createIdentity());
	PxVec3 dimensions(5.5,10.0,5.5);
	PxBoxGeometry geometry(dimensions);
    
	PxRigidDynamic *actor = PxCreateDynamic(*gPhysicsSDK, transform, geometry, *mMaterial, density);
    //actor->setAngularDamping(0.75);
    //actor->setLinearVelocity(PxVec3(0,0,0)); 
	if (!actor)
		cerr<<"create actor failed!"<<endl;
	gScene->addActor(*actor);

	box = actor;

	//manager = PxCreateControllerManager( *mFoundation );

	PxCapsuleControllerDesc desc;
	desc.height					= 5.0f;
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
	desc.upDirection			= PxVec3( 0.0f, 1.0f, 0.0f );
	desc.slopeLimit				= cosf( 0.25 *PxPi);

	PxControllerManager* manager = MyGame3DDevice::GetSingleton()->getPhysXControllerManager();

	characterCtl = manager->createController( *gPhysicsSDK, gScene, desc );
	characterCtl->setStepOffset( 20.0 );
	//gScene->addActor( *characterCtl->getActor() );

	//PxObstacleContext* obCon = manager->createObstacleContext();
	//PxObstacle obst1;
	//obst1.mPos = PxExtendedVec3( 10, 10, 10 ); 
	//obCon->addObstacle( obst1 );
	stepLength = 20.0f;

	//MyGameMusic newPlayer( string("YSO_001.ogg") );
}

void Scene1::UpdatePhysx()
{
	PxScene* gScene = this->sceneMgr->getPhysXScene();

	PxVec3 moveVec( 0.0f, 0.0f, 0.0f );
	if(GetAsyncKeyState('A') & 0x8000)
		//box->mov -= 3;
		moveVec.x = -stepLength;
	if(GetAsyncKeyState('D') & 0x8000)
		moveVec.x = stepLength;//x_A += 3;
	if(GetAsyncKeyState('W') & 0x8000)
		moveVec.z = stepLength;//y_A += 3;
	if(GetAsyncKeyState('S') & 0x8000)
		moveVec.z = -stepLength;//y_A -= 3;
	
	//PxControllerFilters filters(0, mFilterData, mFilterCallback);
	PxControllerFilters filters( 0 );
	if(GetAsyncKeyState('X') & 0x8000)
	{
		//characterCtl->getActor()->setGlobalPose( PxTransform( (PxVec3(0, 10, 0 ) )), true );
		//characterCtl->setPosition( PxExtendedVec3( 0, 10, 0 ) );
		moveVec.y = stepLength;

	}
	
	characterCtl->move( moveVec, 0.001f, myTimestep, filters );

	gScene->simulate(myTimestep);       

	//...perform useful work here using previous frame's state data        
	while(!gScene->fetchResults() )     
	{
		// do something useful        
	}
	PxU32 nShapes = box->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];
	 
	       
	box->getShapes( shapes, nShapes );
	while( nShapes-- )
	{
		//DrawShape(shapes[nShapes]);
		//PxTransform pT = PxShapeExt::getGlobalPose(*shapes[nShapes]);
		PxTransform pT = box->getGlobalPose();
		boxParentNode->setPosition( pT.p.x, pT.p.y, pT.p.z );
		this->loliEnt->getNode()->setPosition( characterCtl->getPosition().x, characterCtl->getPosition().y, characterCtl->getPosition().z );
		PxBoxGeometry bg;
		shapes[nShapes]->getBoxGeometry(bg);
		//PxMat33 m = PxMat33(pT.q ); //PxMat33Legacy(pT.q );
	}

	delete[] shapes;
}

Scene1::~Scene1(void)
{
//	delete boxMesh;
	//delete boxEnt;
	characterCtl->release();
	HR( D3DXSaveSurfaceToFileW( L"test.tga", D3DXIFF_TGA  , pPass1RenderTargetSurface, 0, 0 ) );
		
	IRelease(pPass1RenderTarget);
	IRelease(pPass1RenderTargetSurface);
	IRelease(pScreenRenderSurface);//GetRenderTarget会增加它的引用计数
	delete pGenShadowMapEffect;
	delete testBoard;
	delete pLoliEffect;
	delete pPlatformEffect;
	MyGameMeshManager::destroyAllMeshes();
	MyGameSceneNode::destroyAllNodes();
	MyGameSceneBillboard::DestroyAllBillboards();

	sceneMgr->destroyAllCameras();
	sceneMgr->destroyAllEntities();
	
	delete this->sceneMgr;
}

void Scene1::Render()
{
	UpdatePhysx();
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	
	//
	
	sceneRoot->ComputeCombinedMatrix( idMat);//TODO:这里更新的矩阵是到下一帧才应用到画面上(?)并非如此

	//sceneMgr->setViewProjMat( viewMat, perspectiveMat );
	


	sceneMgr->setLightViewProjMat( lightViewPersMat );

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

	pDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	
	//深度图渲染
	{
		HR( pDevice->SetRenderTarget( 0, pPass1RenderTargetSurface ) );
			
		pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0);
	
		//pDevice->BeginScene();
	
		{
			//渲染该Effect里面的所有Entity
			pGenShadowMapEffect->RenderAllEntities( this->sceneMgr );
		
			D3DXMatrixIdentity( &idMat );
			//loliParentNode->move( 0.002, 0, 0.002 );
			//sceneRoot->ComputeCombinedMatrix( idMat);//TODO:这里更新的矩阵是到下一帧才应用到画面上
			pDevice->SetRenderState( D3DRS_ZENABLE, true );
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
		}
		pDevice->EndScene();
	

	}

	HR( pDevice->SetRenderTarget(0, pScreenRenderSurface) );	
	pDevice->BeginScene();
	
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0);
	
	{

		pLoliEffect->RenderAllEntities( this->sceneMgr );
		pPlatformEffect->RenderAllEntities(this->sceneMgr);
	}

	D3DXMatrixIdentity( &idMat );
	//loliParentNode->move( 0.002, 0, 0.002 );
	
	MyGameSceneBillboard::DrawAllBillboards();

	

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

}