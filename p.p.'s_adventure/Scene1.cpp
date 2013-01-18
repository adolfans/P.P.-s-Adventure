#include "StdAfx.h"
#include "Scene1.h"
#include "MyGame3DDevice.h"
#include "MyGameFunctions.h"
#include <time.h>
#include "MyGameSceneManager.h"
#include "MyGameSceneEntity.h"
Scene1::Scene1(void)
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();


	D3DXVECTOR3 eye( 0.0f, 30.0f, -40.0f );	//最后一个数一开始是-20= =||
	D3DXVECTOR3 target( 0.0f, 0.0f ,0.0f );
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );

	D3DXMatrixLookAtLH( &viewMat, &eye, &target,&up );

	MyGameSceneBillboard::SetViewMatrix( &viewMat );

	float h = (float)MyGame3DDevice::GetHeight();
	float w = (float)MyGame3DDevice::GetWidth();
	float pi = 3.1415926536f;	//囧

	D3DXMatrixPerspectiveFovLH( &perspectiveMat, pi*0.2F, w/h, 5.0f, 100.0f );//最后一个一开始是2000= =||

	HR( D3DXCreateTextureFromFile( pDevice, L"colorful-1556.jpg", &_tex) );

	D3DXMatrixIdentity( &idMat );

	pEffect = new MyGame3DEffect( "shadowMapping.fx" );

	testBoard = MyGameSceneBillboard::CreateBillboard();
	MyGameSceneBillboard::AttachBillboardToScene( testBoard );
	//testBoard->SetPosition( 0, 3, 0 );
	testBoard->SetSize( 3.0f, 3.0f );
	testBoard->CreateTextureFromFile( 0, "951905dfc03b942d4854031e.jpg" );

	testBoard->EnableTextureFromId( 0 );

	loli = new SkinnedMesh;
	loli->prepare();

	loliParentNode = new MyGameSceneNode("loliParent");

	sceneRoot = new MyGameSceneNode("sceneRoot");

	sceneRoot->AddChild( loliParentNode );

	
	loliParentNode->scale( 0.005, 0.005, 0.005 );

	sword = new MyGameMesh(); //TODO:该物体显示了看不到(估计是跑到视野外面去了),可将渲染方式改为shader实现
	sword->loadMeshFromXFile( "equalizer.X" );
	swordEnt = new MyGameSceneEntity( sword, "testSword" );
	pEffect->AddEntity( swordEnt );

	MyGameSceneNode::getNodeByName("Bip001_R_Hand")->attachEntity( swordEnt );

	//loliParentNode->attachEntity( swordEnt );
	
	loliParentNode->AddChild( loli->getSkeletonRoot() );

	//
	//加载plane
	//
	plane = new MyGameMesh();//创建这个mesh
	plane->createPlaneXZ( 40.0f, 20.0f );//生成模型
	planeEnt = new MyGameSceneEntity( plane, "testPlane" );//创建entity，把模型传入
	sceneRoot->attachEntity( planeEnt );//将entity attach到sceneRoot节点下面（其实是作为其子节点存在）
	pEffect->AddEntity( planeEnt );//将entity放入pEffect内，由其管理entity的渲染模式



	HR( pDevice->CreateTexture( MyGame3DDevice::GetWidth(),
											MyGame3DDevice::GetHeight(),
											1,
											D3DUSAGE_RENDERTARGET,
											//D3DFMT_A8R8G8B8,
											D3DFMT_R16F,
											D3DPOOL_DEFAULT,
											&pPass1RenderTarget,
											0 ) );

		
	HR( pDevice->GetRenderTarget( 0, &pScreenRenderSurface ) );//获取到当前的显示表面

	pPass1RenderTargetSurface = 0;
	HR( pPass1RenderTarget->GetSurfaceLevel( 0, &pPass1RenderTargetSurface ) );

}

Scene1::~Scene1(void)
{
//	IRelease(rectBuffer);
//	IRelease(vDecl);
	HR( D3DXSaveSurfaceToFileW( L"test.tga", D3DXIFF_TGA  , pPass1RenderTargetSurface, 0, 0 ) );
		
		this->pPass1RenderTarget->Release();
	this->pPass1RenderTargetSurface->Release();
	
	IRelease(_tex);
	delete pEffect;
	delete testBoard;
	delete sword;
	delete planeEnt;
	delete swordEnt;
	//loliParent->BreakOutChildLink( loli->getSkeletonRoot() );
	//delete loliParentNode;
	delete loli;
	delete plane;
	MyGameSceneNode::DestroyAllNodes();
	MyGameSceneBillboard::DestroyAllBillboards();

}

void Scene1::Render()
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	//D3DXMATRIX com = MyGameSceneManager::getViewProjCombinedMat();

	MyGameSceneManager::SetViewProjMat( viewMat, perspectiveMat );
	

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

	pDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	
	//深度图渲染
	{
		HR( pDevice->SetRenderTarget( 0, pPass1RenderTargetSurface ) );
			
		pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	
		pDevice->BeginScene();
	
		{
			pEffect->SetTextureByName( _tex, TEXTURE );
			pEffect->RenderAllEntities();
		
			D3DXMatrixIdentity( &idMat );
			loliParentNode->move( 0.002, 0, 0.002 );
			sceneRoot->ComputeCombinedMatrix( idMat);//TODO:这里更新的矩阵是到下一帧才应用到画面上
		
			this->loli->render();

			//this->loli->render( pEffect );

			pDevice->SetTexture( 0, _tex );
			//MyGameSceneBillboard::DrawAllBillboards();
			pDevice->SetRenderState( D3DRS_ZENABLE, true );
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
		}
		pDevice->EndScene();
	
		HR( pDevice->SetRenderTarget(0, pScreenRenderSurface) );
	}
	


	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	
	{	

		//pEffect->SetTechniqueByName( TECH );

		pEffect->SetTextureByName( _tex, TEXTURE );
		//pEffect->SetMatrixByName( viewMat* perspectiveMat, WVPMATRIX );

		//unsigned int num = 0;


		//pEffect->Begin( num );

		//for( unsigned int i = 0; i < num; ++i )
		//{
		//	pEffect->BeginPass( i );
		//		plane->render(pEffect);
		//	pEffect->EndPass();
		//}
		//pEffect->End();
		pEffect->RenderAllEntities();
	}

	//D3DXMATRIX rotateMat;

	//D3DXMatrixRotationX( &rotateMat,  3.1415927/2 );

	//D3DXMATRIX fMat = rotateMat * jiong;

	
	D3DXMatrixIdentity( &idMat );
	loliParentNode->move( 0.002, 0, 0.002 );
	sceneRoot->ComputeCombinedMatrix( idMat);//TODO:这里更新的矩阵是到下一帧才应用到画面上
	
	this->loli->render();

	//this->sword->Draw();

	pDevice->SetTexture( 0, _tex );
	MyGameSceneBillboard::DrawAllBillboards();

	

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

}