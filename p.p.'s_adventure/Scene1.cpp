#include "StdAfx.h"
#include "Scene1.h"
#include "MyGame3DDevice.h"
#include "MyGameFunctions.h"
#include <time.h>
#include "MyGameSceneManager.h"
#include "MyGameSceneEntity.h"
#include <d3dx9math.h>
Scene1::Scene1(void)
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	//
	//����viewMat
	//
	D3DXVECTOR3 eye( 0.0f, 30.0f, -40.0f );	//���һ����һ��ʼ��-20= =||
	//D3DXVECTOR3 eye( -40.0f, 10.0f, 10.0f );
	D3DXVECTOR3 target( 0.0f, 0.0f ,0.0f );
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &viewMat, &eye, &target,&up );

	//
	//����persMat
	//
	float h = (float)MyGame3DDevice::GetHeight();
	float w = (float)MyGame3DDevice::GetWidth();
	float pi = 3.1415926536f;	//�壬������= =
	D3DXMatrixPerspectiveFovLH( &perspectiveMat, pi*0.2F, w/h, 5.0f, 100.0f );//���һ��һ��ʼ��2000= =||
	//D3DXMatrixPerspectiveFovLH( &perspectiveMat, pi*0.2F, w/h, 20.0f, 80.0f );//���һ��һ��ʼ��2000= =||
	//D3DXMatrixOrthoLH( &perspectiveMat, w/20.0f, h/20.0f, 5.0f, 100.0f );
	
	//
	//lightViewMat
	//
	D3DXMATRIX lightViewMat, lightPersMat;
	D3DXVECTOR3 light( -10.0f, 30.0f, 10.0f );
	D3DXMatrixLookAtLH( &lightViewMat, &light, &target,&up );

	//
	//lightPersMat (��ȥ����������ˣ�perspective��͸�ӡ��ﲻ���ˡ�)
	//
	//D3DXMatrixPerspectiveFovLH( &perspectiveMat, pi*0.2F, w/h, 5.0f, 100.0f );//���һ��һ��ʼ��2000= =||
	//D3DXMatrixPerspectiveFovLH( &perspectiveMat, pi*0.2F, w/h, 20.0f, 80.0f );//���һ��һ��ʼ��2000= =||
	D3DXMatrixOrthoLH( &lightPersMat, w/20.0f, h/20.0f, 5.0f, 100.0f );
	
	//
	//lightViewPersMat
	//
	lightViewPersMat = lightViewMat* lightPersMat;



	MyGameSceneBillboard::SetViewMatrix( &viewMat );

	HR( D3DXCreateTextureFromFile( pDevice, L"colorful-1556.jpg", &_tex) );

	D3DXMatrixIdentity( &idMat );

	pGenShadowMapEffect = new MyGame3DEffect( "shadowMapping.fx" );

	pLoliEffect = new MyGame3DEffect( "newMesh.fx" );

	pPlatformEffect = new MyGame3DEffect( "scene.fx" );

	testBoard = MyGameSceneBillboard::CreateBillboard();
	MyGameSceneBillboard::AttachBillboardToScene( testBoard );
	//testBoard->SetPosition( 0, 3, 0 );
	testBoard->SetSize( 3.0f, 3.0f );
	testBoard->CreateTextureFromFile( 0, "951905dfc03b942d4854031e.jpg" );

	testBoard->EnableTextureFromId( 0 );

	
	sceneRoot = new MyGameSceneNode("sceneRoot");
	
	
	loli = new SkinnedMesh;//����ģ��
	loli->prepare();
	loliParentNode = new MyGameSceneNode("loliParent");//�����ڵ�
	sceneRoot->AddChild( loliParentNode );
	loliParentNode->scale( 0.005, 0.005, 0.005 );
	loliEnt = new MyGameSceneEntity( loli, "testLoli" );
	loliParentNode->attachEntity( loliEnt );
	pGenShadowMapEffect->AddEntity( loliEnt );
	pLoliEffect->AddEntity( loliEnt );
	

	sword = new MyGameMesh(); //TODO:��������ʾ�˿�����(�������ܵ���Ұ����ȥ��),�ɽ���Ⱦ��ʽ��Ϊshaderʵ��(��ʵ��)
	sword->loadMeshFromXFile( "equalizer.X" );
	swordEnt = new MyGameSceneEntity( sword, "testSword" );
	pGenShadowMapEffect->AddEntity( swordEnt );
	MyGameSceneNode::getNodeByName("Bip001_R_Hand")->attachEntity( swordEnt );


	//
	//����plane
	//
	plane = new MyGameMesh();//�������mesh
	plane->createPlaneXZ( 40.0f, 20.0f );//����ģ��
	plane->createTexture( "colorful-1556.jpg" );
	planeEnt = new MyGameSceneEntity( plane, "testPlane" );//����entity����ģ�ʹ���
	sceneRoot->attachEntity( planeEnt );//��entity attach��sceneRoot�ڵ����棨��ʵ����Ϊ���ӽڵ���ڣ�
	pGenShadowMapEffect->AddEntity( planeEnt );//��entity����pGenShadowMapEffect�ڣ��������entity����Ⱦģʽ

	pPlatformEffect->AddEntity( planeEnt );

	HR( pDevice->CreateTexture( MyGame3DDevice::GetWidth(),
											MyGame3DDevice::GetHeight(),
											1,
											D3DUSAGE_RENDERTARGET,
											//D3DFMT_A8R8G8B8,
											D3DFMT_R16F,
											D3DPOOL_DEFAULT,
											&pPass1RenderTarget,
											0 ) );

	MyGameSceneManager::SetShadowMap( pPass1RenderTarget );

		
	HR( pDevice->GetRenderTarget( 0, &pScreenRenderSurface ) );//��ȡ����ǰ����ʾ����

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
	pScreenRenderSurface->Release();
	IRelease(_tex);
	delete pGenShadowMapEffect;
	delete testBoard;
	delete sword;
	delete planeEnt;
	delete swordEnt;
	//loliParent->BreakOutChildLink( loli->getSkeletonRoot() );
	//delete loliParentNode;
	delete loli;
	delete loliEnt;
	delete plane;
	delete pLoliEffect;
	delete pPlatformEffect;
	MyGameSceneNode::DestroyAllNodes();
	MyGameSceneBillboard::DestroyAllBillboards();

}

void Scene1::Render()
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	//D3DXMATRIX com = MyGameSceneManager::getViewProjCombinedMat();
	
	//
	static float x_A = 20.0f, y_A = 20.0f;
	static float _n= 5.0f, _f = 100.0f;
	if(GetAsyncKeyState('A') & 0x8000)
		x_A -= 3;
	if(GetAsyncKeyState('D') & 0x8000)
		x_A += 3;
	if(GetAsyncKeyState('W') & 0x8000)
		y_A += 3;
	if(GetAsyncKeyState('S') & 0x8000)
		y_A -= 3;
	if(GetAsyncKeyState(VK_UP) & 0x8000)
		_n += 3;
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
		_n -=3;
	
	//::D3DXMatrixOrthoOffCenterLH( &perspectiveMat, -x_A, x_A, -y_A, y_A, _n, _f );

	MyGameSceneManager::SetViewProjMat( viewMat, perspectiveMat );
	
	MyGameSceneManager::setLightViewProjMat( lightViewPersMat );

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

	pDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	
	//���ͼ��Ⱦ
	{
		HR( pDevice->SetRenderTarget( 0, pPass1RenderTargetSurface ) );
			
		pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0);
	
		//pDevice->BeginScene();
	
		{
			pGenShadowMapEffect->SetTextureByName( _tex, MyGame3DEffect::TEXTURE );
			//��Ⱦ��Effect���������Entity
			pGenShadowMapEffect->RenderAllEntities();
		
			D3DXMatrixIdentity( &idMat );
			loliParentNode->move( 0.002, 0, 0.002 );
			sceneRoot->ComputeCombinedMatrix( idMat);//TODO:������µľ����ǵ���һ֡��Ӧ�õ�������
		
			//this->loli->render();

			//this->loli->render( pGenShadowMapEffect );

			pDevice->SetTexture( 0, _tex );
			//MyGameSceneBillboard::DrawAllBillboards();
			pDevice->SetRenderState( D3DRS_ZENABLE, true );
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
		}
		pDevice->EndScene();
	
		HR( pDevice->SetRenderTarget(0, pScreenRenderSurface) );

		pDevice->BeginScene();
	}
	


	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0);
	
	{	

		//pGenShadowMapEffect->SetTechniqueByName( MyGame3DEffect::TECH );

		pGenShadowMapEffect->SetTextureByName( _tex, MyGame3DEffect::TEXTURE );
		//pGenShadowMapEffect->SetMatrixByName( viewMat* perspectiveMat, MyGame3DEffect::WVPMATRIX );

		//unsigned int num = 0;


		//pGenShadowMapEffect->Begin( num );

		//for( unsigned int i = 0; i < num; ++i )
		//{
		//	pGenShadowMapEffect->BeginPass( i );
		//		plane->render(pGenShadowMapEffect);
		//	pGenShadowMapEffect->EndPass();
		//}
		//pGenShadowMapEffect->End();
		pLoliEffect->RenderAllEntities();
		pPlatformEffect->RenderAllEntities();
	}

	//D3DXMATRIX rotateMat;

	//D3DXMatrixRotationX( &rotateMat,  3.1415927/2 );

	//D3DXMATRIX fMat = rotateMat * jiong;

	
	D3DXMatrixIdentity( &idMat );
	loliParentNode->move( 0.002, 0, 0.002 );
	sceneRoot->ComputeCombinedMatrix( idMat);//TODO:������µľ����ǵ���һ֡��Ӧ�õ�������
	
	//this->loli->render();

	//this->sword->Draw();

	pDevice->SetTexture( 0, _tex );
	MyGameSceneBillboard::DrawAllBillboards();

	

	pDevice->SetRenderState( D3DRS_ZENABLE, true );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );

}