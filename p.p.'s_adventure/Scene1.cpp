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
PxReal myTimestep = 1.0f/60.0f;
PxRigidActor *box;
PxDefaultCpuDispatcher* mCpuDispatcher;
#include <sstream>
#include <iostream>
using std::stringstream;
Scene1::Scene1(void):
	bgm(string("YSO_001.ogg"))
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	mirrorTexture = new MyRenderTargetTexture( MyRenderTargetTexture::ARGB );


	
	//WindowManager::getSingleton().destroyAllWindows();
	//CEGUI::WindowManager::getSingleton().cleanDeadPool(); 
	
	
	Window* myRoot = WindowManager::getSingleton().loadWindowLayout( "level1.layout");
	
	CEGUI::System::getSingleton().getGUISheet()->addChildWindow(myRoot);

	waterSpeedController = static_cast<CEGUI::Scrollbar*>(WindowManager::getSingleton().getWindow( "Root/WaterSpeed" ));
	waterSpeedController->setScrollPosition( 0.5 );
	bgmVolume = static_cast<CEGUI::Scrollbar*>(WindowManager::getSingleton().getWindow( "Root/Volume" ));
	bgmVolume->setScrollPosition( 0.125 );//防止音量过大
	shadowCheckBox = static_cast<CEGUI::Checkbox*>(WindowManager::getSingleton().getWindow( "Root/shadow" ));
	shadowCheckBox->setSelected( true );
	shadowCheckBox->subscribeEvent( ButtonBase::EventMouseClick, Event::Subscriber( &Scene1::OnShadowChanged, this ));
	reflectionCheckBox = static_cast<CEGUI::Checkbox*>(WindowManager::getSingleton().getWindow( "Root/reflection" ));
	reflectionCheckBox->subscribeEvent( ButtonBase::EventMouseClick, Event::Subscriber( &Scene1::OnWaterReflectionChanged, this ));
	fpsBoard = WindowManager::getSingleton().getWindow( "Root/FPS" );

	lightAngle = static_cast<CEGUI::Scrollbar*>(WindowManager::getSingleton().getWindow( "Root/LightAngle"));
	lightAngle->subscribeEvent( CEGUI::Scrollbar::EventMouseEnters, Event::Subscriber( &Scene1::OnLightAngleChanged, this ) );
	lightAngle->setScrollPosition( 0.5 );
	
	
	sceneMgr = new MyGameSceneManager;
	
	sceneRoot = sceneMgr->getRootNode();//->CreateSceneNode("sceneRoot");
	cam = sceneMgr->CreateCamera( 0.0f, 30.0f, -40.0f, 0.0f, 0.0f, 0.0f );

	sceneMgr->setCamera( cam );

	//lightCam,表示光的位置= =||
	lightCam = sceneMgr->CreateCamera( -10.0f, 30.0f, -10.0f, 0.0f, 0.0f, 0.0f  );

	sceneMgr->setParallelMainLight( lightCam );

	D3DXVECTOR3 vec = lightCam->getLookingVector();
	vec.x = 0;

	lightCam->setLookingVector( vec );
	//MyGameSceneBillboard::SetViewMatrix( &viewMat );
	//MyGameSceneBillboard::SetViewMatrix( &cam->getViewMatrix() );

	D3DXMatrixIdentity( &idMat );

	pGenShadowMapEffect = new MyGame3DEffect( "shadowMapping.fx" );

	//pLoliEffect = new MyGame3DEffect( "newMesh.fx" );

	pPlatformEffect = new MyGame3DEffect( "scene.fx" );

	waterEffect = new MyGame3DEffect( "water.fx" );
	

	waterSpeedHandle = waterEffect->getHandleByName( ("waterspeed") );

	//shadow map 创建

	shadowMap = new MyRenderTargetTexture( MyRenderTargetTexture::Gray );

	sceneMgr->setShadowMap( shadowMap->getTexture() );

	testBoard = MyGameSceneBillboard::CreateBillboard( sceneMgr );
	MyGameSceneBillboard::AttachBillboardToScene( testBoard );
	testBoard->SetSize( 3000.0f, 3000.0f );
	sceneRoot->AddChild( testBoard->getNode() );
	testBoard->CreateTextureFromFile( 0, "951905dfc03b942d4854031e.jpg" );

	testBoard->EnableTextureFromId( 0 );

	//BillboardSet

	testBoard1 = new MyGameSceneBillboardSet( 2 , "feena1.png");
	MyGameSceneNode* testBoard1Node = sceneMgr->CreateSceneNode( "testBoard1Node" );
	testBoard1Node->scale( 3000.0f, 3000.0f, 1.0f );
	MyGameSceneNode* testBoard1Node2 = sceneMgr->CreateSceneNode( "testBoard1Node2" );
	testBoard1Node2->scale( 3000.0f, 3000.0f, 1.0f );
	testBoard1Node2->setPosition( 3000.0f, 0.0f, 3000.0f );
	testBoard1->addEntity( testBoard1Node );
	testBoard1->addEntity( testBoard1Node2 );

	sceneRoot->AddChild( testBoard1Node );
	sceneRoot->AddChild( testBoard1Node2 );

	MyGameMesh* waterMesh = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	waterMesh->loadMeshFromXFile( "water.X" );
	waterMesh->createTexture( "wave0.dds" );
	waterMesh->createExtraTexture( "wave1.dds" );
	MyGameSceneEntity* waterEnt = sceneMgr->CreateSceneEntity( waterMesh, "myWater" );
	sceneRoot->attachEntity( waterEnt );
	this->waterEffect->AddEntity( waterEnt );




	//
	//计算倒影矩阵
	D3DXVECTOR3 minPos, maxPos;
	waterMesh->getBoundingBox( minPos, maxPos );
	D3DXMATRIX mirrorMatrix;
	D3DXPLANE  waterPlane( 0, 1, 0,  -sceneMgr->sceneLengthToNormalLength(minPos.y));
	D3DXMatrixReflect( &mirrorMatrix, &waterPlane );
	sceneMgr->setMirrorReflectionMatrix( mirrorMatrix );



	//sceneRoot->setTranslationMatrix( mirrorMatrix );
	//sceneRoot->setRotateMatrix( mirrorMatrix );

	//必须打开那啥编译选项
	loli = dynamic_cast<SkinnedMesh*>(MyGameMeshManager::createMyGameMesh( MyGameMeshManager::SKINNED ));
	loli->loadFromX( sceneMgr );
	loliEnt = sceneMgr->CreateSceneEntity( loli, "testLoli" );
	sceneRoot->attachEntity( loliEnt );
	pGenShadowMapEffect->AddEntity( loliEnt );
	//pLoliEffect->AddEntity( loliEnt );
	pPlatformEffect->AddEntity( loliEnt );
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
	//加载plane
	//
	plane = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	plane->createPlaneXZ( 4000.0f, 2000.0f );//生成模型
	plane->createTexture( "colorful-1556.jpg" );
	planeEnt = sceneMgr->CreateSceneEntity( plane, "testPlane" );
	sceneRoot->attachEntity( planeEnt );//将entity attach到sceneRoot节点下面（其实是作为其子节点存在）
	pGenShadowMapEffect->AddEntity( planeEnt );//将entity放入pGenShadowMapEffect内，由其管理entity的渲染模式
	pPlatformEffect->AddEntity( planeEnt );
	
	//sceneRoot->scale( 0.005, 0.005, 0.005 );
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



	MyGameSceneNode* teapotNode = sceneMgr->CreateSceneNode( "teapotNode" );
	sceneRoot->AddChild(teapotNode);
	MyGameMesh* teapotMesh = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
	teapotMesh->loadMeshFromXFile( "testTeapot.X" );
	//teapotMesh->loadMeshFromFbxFile( "myteapot.FBX" );
	MyGameSceneEntity* teapotEnt= sceneMgr->CreateSceneEntity( teapotMesh, "testTeapot" );
	//teapotNode->attachEntity( teapotEnt );
	//teapotNode->scale( 10.0f, 10.0f, 10.0f );
	::loadSceneFromFbx( sceneMgr, "myteapot.FBX", teapotNode, pPlatformEffect );
	pPlatformEffect->AddEntity( teapotEnt );
	pGenShadowMapEffect->AddEntity( teapotEnt );
	
	
	mirrorEffect = new MyGame3DEffect( "mirror.fx" );

	mirrorEffect->setEntities( pPlatformEffect );

	mirrorEffect->getHandleByName( "reflectionMatrix" );

	mirrorEffect->attachMatrixToName( "reflectionMatrix", mirrorMatrix );

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
	role1 = new MyPlayerRole( gScene, manager, 1500.0f, this->loli, loliEnt );
	IDirect3DTexture9* texture;
	D3DXCreateTextureFromFileA( pDevice, "grid.jpg", &texture );
	waterEffect->attachTextureToName( "reflectionTexture", this->mirrorTexture->getTexture()/*texture*/ );
	//waterEffect->attachTextureToName( "reflectionTexture", texture );

}

void Scene1::Update( MSG msg )
{
	static unsigned int lastTime = clock();
	unsigned int currentTime = clock();
	static int frames;
	if( currentTime - lastTime < 1000.0f )//不满一秒
	{
		frames++;
	}else
	{
		stringstream ss;
		ss << "FPS:"<<(float)frames/(currentTime - lastTime)*1000.0f;
		string fpsText = ss.str();
		lastTime = currentTime;
		fpsBoard->setText( fpsText );
		frames = 0;
	}

	PxScene* gScene = this->sceneMgr->getPhysXScene();

	//PxVec3 moveVec( 0.0f, 0.0f, 0.0f );

	if(msg.message == WM_KEYDOWN)//如果有键按下
	{
		if( msg.wParam == 'A' )
			cam->rotate( D3DX_PI * 0.0125 );
		else if( msg.wParam == 'D' )
			cam->rotate( D3DX_PI * -0.0125 );

	}

	role1->update( msg );

	PxControllerFilters filters( 0 );


	this->cam->setTargetPosition( role1->getPosition());
	lightCam->setTargetPosition( role1->getPosition());

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
	//HR( D3DXSaveSurfaceToFileW( L"test.tga", D3DXIFF_TGA  , pPass1RenderTargetSurface, 0, 0 ) );
	::D3DXSaveTextureToFile( L"test.png", D3DXIFF_PNG, sceneMgr->getShadowMap(), 0 );
	//delete this->con;
	delete role1;
	delete pGenShadowMapEffect;
	delete testBoard;
	delete pPlatformEffect;
	delete waterEffect;
	delete mirrorEffect;
	delete shadowMap;
	//delete pLoliEffect;
	MyGameMeshManager::destroyAllMeshes();
	MyGameSceneNode::destroyAllNodes();
	MyGameSceneBillboard::DestroyAllBillboards();

	sceneMgr->destroyAllCameras();
	sceneMgr->destroyAllEntities();
	
	delete this->sceneMgr;
	delete mirrorTexture;
}

void Scene1::Render()
{
	//UpdatePhysx();
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	
	
	float waterSpeed = waterSpeedController->getScrollPosition();

	static D3DXVECTOR2 vec(0.0f, 0.0f);
	vec.x-=(0.0006*waterSpeed);
	vec.y+=0.0012*waterSpeed;

	bgm.setVolume( bgmVolume->getScrollPosition() );

	//waterSpeedController->getsc
	this->waterEffect->setValueByHandle( waterSpeedHandle, &vec, sizeof( D3DXVECTOR2 ) );
	//

	D3DXMatrixIdentity( &idMat );
	sceneRoot->ComputeCombinedMatrix( idMat);//TODO:这里更新的矩阵是到下一帧才应用到画面上(?)并非如此

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

	pDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	
	//深度图渲染

	if( shadowCheckBox->isSelected() )	//如果启用了shadow
	{
		this->shadowMap->setRenderTarget( 0 );
		{
			//渲染该Effect里面的所有Entity
			pGenShadowMapEffect->RenderAllEntities( this->sceneMgr );/*
			pDevice->SetRenderState( D3DRS_ZENABLE, true );
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );*/
		}
		MyGame3DDevice::GetSingleton()->restoreScreenRenderTarget();
	}

	//镜像图渲染
	if( reflectionCheckBox->isSelected() )
	{
		mirrorTexture->setRenderTarget( 0 );
	
		mirrorEffect->RenderAllEntities( this->sceneMgr );

		MyGame3DDevice::GetSingleton()->restoreScreenRenderTarget();
	}
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0);
	
	{
		//pLoliEffect->RenderAllEntities( this->sceneMgr );
		pPlatformEffect->RenderAllEntities(this->sceneMgr);
		waterEffect->RenderAllEntities( this->sceneMgr );
	}
	//loliParentNode->move( 0.002, 0, 0.002 );
	
	//MyGameSceneBillboard::DrawAllBillboards();

	pDevice->SetTexture( 0, mirrorTexture->getTexture() );

	testBoard1->draw( sceneMgr );

	//sprites.render(sceneMgr);

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

}

bool Scene1::OnShadowChanged( const CEGUI::EventArgs& e )
{
	//CEGUI::UVector2 vec = waterSpeedController->getPosition();
	//e
	this->shadowMap->clear();
	return true;
}

bool Scene1::OnLightAngleChanged( const CEGUI::EventArgs& e )
{
	float pos = this->lightAngle->getScrollPosition();
	pos-=0.5;
	D3DXVECTOR3 vec = lightCam->getLookingVector();
	vec.x = -vec.z * 5 * pos;
	lightCam->setLookingVector( vec );
	return true;
}

bool Scene1::OnWaterReflectionChanged( const CEGUI::EventArgs& e )
{
	this->mirrorTexture->clear();
	return true;
}