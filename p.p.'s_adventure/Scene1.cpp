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

//PxController* characterCtl;
Scene1::Scene1(void):
	bgm(string("YSO_001.ogg"))
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	
	sceneMgr = new MyGameSceneManager;
	
	sceneRoot = sceneMgr->getRootNode();//->CreateSceneNode("sceneRoot");
	cam = sceneMgr->CreateCamera( 0.0f, 30.0f, -40.0f, 0.0f, 0.0f, 0.0f );

	sceneMgr->setCamera( cam );

	//lightCam,��ʾ���λ��= =||
	lightCam = sceneMgr->CreateCamera( -10.0f, 30.0f, -10.0f, 0.0f, 0.0f, 0.0f  );

	sceneMgr->setParallelMainLight( lightCam );

	//MyGameSceneBillboard::SetViewMatrix( &viewMat );
	//MyGameSceneBillboard::SetViewMatrix( &cam->getViewMatrix() );

	D3DXMatrixIdentity( &idMat );

	pGenShadowMapEffect = new MyGame3DEffect( "shadowMapping.fx" );

	pLoliEffect = new MyGame3DEffect( "newMesh.fx" );

	pPlatformEffect = new MyGame3DEffect( "scene.fx" );

	waterEffect = new MyGame3DEffect( "water.fx" );
	

	waterSpeedHandle = waterEffect->getHandleByName( ("waterspeed") );
	//shadow map ����
	shadowMap = new MyShadowMap(MyGame3DDevice::GetWidth(), MyGame3DDevice::GetHeight() );

	sceneMgr->setShadowMap( shadowMap->getShadowMap() );

	testBoard = MyGameSceneBillboard::CreateBillboard( sceneMgr );
	MyGameSceneBillboard::AttachBillboardToScene( testBoard );
	testBoard->SetSize( 3000.0f, 3000.0f );
	sceneRoot->AddChild( testBoard->getNode() );
	testBoard->CreateTextureFromFile( 0, "951905dfc03b942d4854031e.jpg" );

	testBoard->EnableTextureFromId( 0 );

	
	MyGameMesh* waterMesh = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	waterMesh->loadMeshFromXFile( "water.X" );
	waterMesh->createTexture( "wave0.dds" );
	waterMesh->createExtraTexture( "wave1.dds" );
	MyGameSceneEntity* waterEnt = sceneMgr->CreateSceneEntity( waterMesh, "myWater" );
	sceneRoot->attachEntity( waterEnt );
	this->waterEffect->AddEntity( waterEnt );


	//�������ɶ����ѡ��
	loli = dynamic_cast<SkinnedMesh*>(MyGameMeshManager::createMyGameMesh( MyGameMeshManager::SKINNED ));
	loli->loadFromX( sceneMgr );
	loliEnt = sceneMgr->CreateSceneEntity( loli, "testLoli" );
	sceneRoot->attachEntity( loliEnt );
	pGenShadowMapEffect->AddEntity( loliEnt );
	pLoliEffect->AddEntity( loliEnt );
	//loliEnt->getNode()->move( 0.0f, 0.01f, 0.0f );
	//loliEnt->getNode()->setPosition( 0.0f, 0.0f ,0.0f );

	sword = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	sword->loadMeshFromXFile( "equalizer.X" );
	swordEnt = sceneMgr->CreateSceneEntity( sword, "testSword" );
	pGenShadowMapEffect->AddEntity( swordEnt );
	MyGameSceneNode::getNodeByName("Bip001_R_Hand")->attachEntity( swordEnt );
	
	pPlatformEffect->AddEntity( swordEnt );

	MyGameMesh* ground = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	ground->loadMeshFromXFile( "ground.X" );
	MyGameSceneEntity* ballEnt = sceneMgr->CreateSceneEntity( ground, "ball" );
	pGenShadowMapEffect->AddEntity( ballEnt );
	pPlatformEffect->AddEntity( ballEnt );
	sceneRoot->attachEntity( ballEnt );
	//MyGameSceneNode::getNodeByName("Bip001_Head")->attachEntity( ballEnt );

	//
	//����plane
	//
	plane = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	plane->createPlaneXZ( 4000.0f, 2000.0f );//����ģ��
	plane->createTexture( "colorful-1556.jpg" );
	planeEnt = sceneMgr->CreateSceneEntity( plane, "testPlane" );
	sceneRoot->attachEntity( planeEnt );//��entity attach��sceneRoot�ڵ����棨��ʵ����Ϊ���ӽڵ���ڣ�
	pGenShadowMapEffect->AddEntity( planeEnt );//��entity����pGenShadowMapEffect�ڣ��������entity����Ⱦģʽ
	pPlatformEffect->AddEntity( planeEnt );
	
	//sceneRoot->scale( 0.005, 0.005, 0.005 );
	//
	//Boxģ��
	boxParentNode = sceneMgr->CreateSceneNode( "boxParent" );
	sceneRoot->AddChild(boxParentNode);
	boxMesh = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	boxMesh->loadMeshFromXFile( "testBox.X" );
	boxEnt = sceneMgr->CreateSceneEntity( boxMesh, "testBox" );
	boxParentNode->attachEntity( boxEnt );
	pPlatformEffect->AddEntity( boxEnt );
	pGenShadowMapEffect->AddEntity( boxEnt );



	MyGameSceneNode* teapotNode = sceneMgr->CreateSceneNode( "teapotNode" );
	sceneRoot->AddChild(teapotNode);
	MyGameMesh* teapotMesh = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	teapotMesh->loadMeshFromXFile( "testTeapot.X" );
	MyGameSceneEntity* teapotEnt= sceneMgr->CreateSceneEntity( teapotMesh, "testTeapot" );
	teapotNode->attachEntity( teapotEnt );
	pPlatformEffect->AddEntity( teapotEnt );
	pGenShadowMapEffect->AddEntity( teapotEnt );
	//physx
	
	PxPhysics* gPhysicsSDK = MyGame3DDevice::GetSingleton()->getPhysX();

	PxScene* gScene = this->sceneMgr->getPhysXScene();

	PxMaterial* mMaterial = gPhysicsSDK->createMaterial(0.5,0.5,0.5);


	//Create actors 
    //1) Create ground plane
 //   PxReal d = 0.0f;	 
	//PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f),PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));

	//PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(pose);
	//if (!plane)
	//		cerr<<"create plane failed!"<<endl;

	//PxShape* shape = plane->createShape(PxPlaneGeometry(), *mMaterial);//Shapes with a PxPlaneGeometry may only be created for static actors.
	//if (!shape)
	//	cerr<<"create shape failed!"<<endl;
	//gScene->addActor(*plane);
	////Shapes with triangle mesh geometries may only be created for static and kinematic actors:

	PxReal d = 0.0f;	 
	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f),PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));

	PxRigidStatic* pxground = gPhysicsSDK->createRigidStatic(PxTransform::createIdentity());
	if (!plane)
			throw runtime_error("create plane failed!");
	PxTriangleMeshGeometry triGeom;
	triGeom.triangleMesh = ground->generatePxTriangleMesh();
	PxShape* shape = pxground->createShape(triGeom, *mMaterial);//Shapes with a PxPlaneGeometry may only be created for static actors.
	if (!shape)
		cerr<<"create shape failed!"<<endl;
	gScene->addActor(*pxground);
	//Shapes with triangle mesh geometries may only be created for static and kinematic actors:

	//2) Create cube	 
	PxReal density = 1.0f;
	PxTransform transform(PxVec3(1500.0f, 2000, 0.0f), PxQuat::createIdentity());
	PxVec3 dimensions(500.5,2000.0,500.5);
	PxBoxGeometry geometry(dimensions);
    
	PxRigidDynamic *actor = PxCreateDynamic(*gPhysicsSDK, transform, geometry, *mMaterial, density);
    //actor->setAngularDamping(0.75);
    //actor->setLinearVelocity(PxVec3(0,0,0)); 
	if (!actor)
		cerr<<"create actor failed!"<<endl;
	gScene->addActor(*actor);

	box = actor;
	
	PxControllerManager* manager = MyGame3DDevice::GetSingleton()->getPhysXControllerManager();
	stepLength = 20.0f;

	//MyGameMusic newPlayer( string("YSO_001.ogg") );

	con = new MyCharacterController( gScene, manager, 1500.0f, this->loli, loliEnt );
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


	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )
		cam->rotate( D3DX_PI * 0.0125 );
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )
		cam->rotate( D3DX_PI * -0.0125 );
	this->con->move( moveVec.x, moveVec.y, moveVec.z, myTimestep );
	
	this->cam->setTargetPosition( D3DXVECTOR3(con->getPosition()));
	lightCam->setTargetPosition( D3DXVECTOR3(con->getPosition()));

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
		PxBoxGeometry bg;
		shapes[nShapes]->getBoxGeometry(bg);
		//PxMat33 m = PxMat33(pT.q ); //PxMat33Legacy(pT.q );
	}

	delete[] shapes;
}

Scene1::~Scene1(void)
{
	//characterCtl->release();
	//HR( D3DXSaveSurfaceToFileW( L"test.tga", D3DXIFF_TGA  , pPass1RenderTargetSurface, 0, 0 ) );
	delete this->con;
	delete pGenShadowMapEffect;
	delete testBoard;
	delete pLoliEffect;
	delete pPlatformEffect;
	delete waterEffect;
	delete shadowMap;
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
	
	static D3DXVECTOR2 vec(0.0f, 0.0f);
	vec.x-=0.001;
	vec.y+=0.002;
	this->waterEffect->setValueByHandle( waterSpeedHandle, &vec, sizeof( D3DXVECTOR2 ) );
	//

	D3DXMatrixIdentity( &idMat );
	sceneRoot->ComputeCombinedMatrix( idMat);//TODO:������µľ����ǵ���һ֡��Ӧ�õ�������(?)�������

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

	pDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	
	//���ͼ��Ⱦ

	this->shadowMap->start();
	
	{
			//��Ⱦ��Effect���������Entity
			pGenShadowMapEffect->RenderAllEntities( this->sceneMgr );
			pDevice->SetRenderState( D3DRS_ZENABLE, true );
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
	}

	this->shadowMap->end();

	
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0);
	
	{

		pLoliEffect->RenderAllEntities( this->sceneMgr );
		pPlatformEffect->RenderAllEntities(this->sceneMgr);
		waterEffect->RenderAllEntities( this->sceneMgr );
	}
	//loliParentNode->move( 0.002, 0, 0.002 );
	
	MyGameSceneBillboard::DrawAllBillboards();

	

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

}