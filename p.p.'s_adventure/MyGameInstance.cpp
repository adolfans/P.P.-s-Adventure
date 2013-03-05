#include "StdAfx.h"
#include "MyGameInstance.h"
#include "MyGameFunctions.h"
//#include "MyGameDecoration.h"
#include <time.h>
#include "Resource.h"
#include <RendererModules/Direct3D9/CEGUIDirect3D9Renderer.h>
#include <CEGUI.h>

//#include "MyGameClock.h"
#include <exception>
using std::runtime_error;
using std::bad_cast;

#include "MyGameMusic.h"

void GenerateGrid( int _width, int _height, float _stride, /*in & out*/Vertex** _pVertices, /*in & out*/WORD** _pIndices );

void CalculateNormal( D3DXVECTOR3* _vector0, D3DXVECTOR3* _vector1, D3DXVECTOR3* _vector2, D3DXVECTOR3* _outVector ); 

MyGameInstance*	MyGameInstance::pMyGameInstance = NULL;

string	configFileName = "config.lua";


bool MyGameInstance::MainLoop()
{
	static MyGameInstance* gameInstance = MyGameInstance::GetMyGameInstance();
	MSG msg;

	while(true)
	{
		DispatchMyMessage();

		//inject time pulse
		
		static clock_t lastTime = clock();

		clock_t currentTime = clock();

		float timePassed = (float)(currentTime - lastTime)/(float)CLOCKS_PER_SEC;

		CEGUI::System::getSingleton().injectTimePulse( timePassed );

		lastTime = currentTime;

		if( GAMEQUIT == GetGameState() )
			return false;
		if(::PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			using namespace CEGUI;
			if(msg.message == WM_MOUSEMOVE )
			{
				POINT cursorPosition;
				GetCursorPos(&cursorPosition);
				//if( ::GetAsyncKeyState( VK_LBUTTON ) & 0x8000f )

				ScreenToClient(this->pMyWindow->GethWnd(),&cursorPosition);

				CEGUI::System::getSingleton().injectMousePosition(cursorPosition.x, cursorPosition.y );
			}else if(msg.message == WM_LBUTTONDOWN)
			{
				MouseButton button =  LeftButton;
				System::getSingleton().injectMouseButtonDown(button);

			}else if(msg.message == WM_LBUTTONUP)
			{
				MouseButton button =  LeftButton;
				System::getSingleton().injectMouseButtonUp(button);

			}

			if( msg.message == WM_QUIT || msg.message == WM_DESTROY )
				return false;
			if( true == testIfStateChanged() )
			{
				if( NEWGAME == GetGameState() )
				{
					resetStateChanged();
					SendMyMessage( runningGame );
					DispatchMyMessage();
					resetStateChanged();
					//this->NewGame();
				}
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		UpdateCurrentKeyState();
		//this->UpdateGame();
		//gameInstance->UpdateScene_tmp2();
	}
	return true;
}

void MyGameInstance::RunLevel(MyGameLevel *level)
{
	static MyGameInstance* gameInstance = MyGameInstance::GetMyGameInstance();
	MSG msg;

	while(true)
	{
		try{
			DispatchMyMessage();

			//inject time pulse
		
			static clock_t lastTime = clock();
		
			clock_t currentTime = clock();

			float timePassed = (float)(currentTime - lastTime)/(float)CLOCKS_PER_SEC;

			CEGUI::System::getSingleton().injectTimePulse( timePassed );

			lastTime = currentTime;
			
			if( GAMEQUIT == GetGameState() )
				return;
			if(::PeekMessage(&msg,0,0,0,PM_REMOVE))
			{
				using namespace CEGUI;
				if(msg.message == WM_MOUSEMOVE )
				{
					POINT cursorPosition;
					GetCursorPos(&cursorPosition);
					//if( ::GetAsyncKeyState( VK_LBUTTON ) & 0x8000f )

					ScreenToClient(this->pMyWindow->GethWnd(),&cursorPosition);

					CEGUI::System::getSingleton().injectMousePosition(cursorPosition.x, cursorPosition.y );
				}else if(msg.message == WM_LBUTTONDOWN)
				{
					MouseButton button =  LeftButton;
					System::getSingleton().injectMouseButtonDown(button);

				}else if(msg.message == WM_LBUTTONUP)
				{
					MouseButton button =  LeftButton;
					System::getSingleton().injectMouseButtonUp(button);
				}

				if( msg.message == WM_QUIT || msg.message == WM_DESTROY )
				{
					//delete level;
					return;
				}
				if( true == testIfStateChanged() )
				{
					if( NEWGAME == GetGameState() )
					{
						resetStateChanged();
						SendMyMessage( runningGame );
						DispatchMyMessage();
						resetStateChanged();
						//this->NewGame();
					}
				}

				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			UpdateCurrentKeyState();
			MyGameLevel* l = level->GetCurrentLevel();
			if( level !=  l)
				delete level;
			level = l;
			this->pLevel = level;
			gameInstance->UpdateLevel(level,msg);
		}catch( const runtime_error &e ){
			::MessageBoxA( 0, e.what(), 0, 0 );
		}catch( const bad_cast &e ){
			::MessageBoxA( 0, e.what(), 0, 0 );
		}
	}
	return;
}

void MyGameInstance::UpdateLevel( MyGameLevel* level, MSG msg )
{
	
	IDirect3DDevice9* pD3D9InstanceDevice = this->pMyDevice->GetDevice();

	level->Update( msg );

	pD3D9InstanceDevice->BeginScene();

	level->Render();

	//D3DXMATRIX mat;

	//D3DXMatrixIdentity(&mat);

	//pD3D9InstanceDevice->SetTransform( D3DTS_WORLD, &mat );

	//{	
	//	D3DXHANDLE hTech = pTransformEffect->GetTechniqueByName( "TransformTech" );
	//	HR(this->pTransformEffect->SetTechnique( hTech ) );
	//	HR(pD3D9InstanceDevice->SetStreamSource( 0, this->pVertexBuffer, 0, sizeof(float)*5 ) );
	//	D3DXHANDLE hTex = pTransformEffect->GetParameterByName( 0, "gTex" );
	//	D3DXHANDLE hSampler = pTransformEffect->GetParameterByName( 0, "Texture0");
	//	HR(this->pTransformEffect->SetTexture( hTex, this->pBgTexture ));
	//	//D3DXPARAMETER_DESC desc;
	//	//this->pTransformEffect->GetParameterDesc( hSampler, &desc );
	//	unsigned int num = 0;
	//	HR(this->pTransformEffect->Begin( &num, 0 ) );
	//	//for( unsigned int i = 0; i < num; ++i )
	//	{
	//		HR(this->pTransformEffect->BeginPass(/*i*/0));
	//			pD3D9InstanceDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	//			HR(pD3D9InstanceDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ));
	//		HR(this->pTransformEffect->EndPass());
	//	}
	//	HR(this->pTransformEffect->End());
	//}
	
	//DRAW UI
	CEGUI::System::getSingleton().renderGUI();

	//停止绘制
	pD3D9InstanceDevice->EndScene();
	pD3D9InstanceDevice->Present( 0, 0, 0, 0 );
}


MyGameInstance::MyGameInstance(void)
	:config(configFileName),
	//newLevel(0),
	pMyDevice(0),
	pMyWindow(0)//,
	//pMainUI(0)
{
	pMyWindow = new MyGameWindow;
}

MyGameInstance::~MyGameInstance(void)
{
	//pUIText->DestroyText();
	//delete pUIText;
	//MyGameLevel::DestroyDialog();
	
	//MyGameUISprite::DestroyAllSprites();
	MyGameMusic::ExitMusicSystem();
	CEGUI::Direct3D9Renderer::destroySystem(		);
	//HR( D3DXSaveTextureToFile( L"囧.png",D3DXIFF_PNG,pPass1RenderTarget, NULL ) );
	//this->pBgTexture->Release();
	//this->pPass1RenderTarget->Release();
	//this->pPass1RenderTargetSurface->Release();
	//this->pScreenRenderSurface->Release();
	//this->pBloomEffect->Release();
	//this->pVertexBuffer->Release();
	//this->pMyGameVertexDecl->Release();
	delete pLevel;
	delete pMyDevice;
	delete pMyWindow;
	if(MyGameInstance::pMyGameInstance)
	{
		pMyGameInstance = NULL;
	}
}

MyGameInstance* MyGameInstance::GetMyGameInstance(HINSTANCE _hInstance, int _nCmdShow)
{
	//::MessageBoxA(0,"囧，GetMyGameInstance开始调用",0,0);
	if(pMyGameInstance==NULL)
	{
		pMyGameInstance = new MyGameInstance;

		pMyGameInstance->pMyWindow->SetInstance( _hInstance );

		pMyGameInstance->pMyWindow->SetCmdShow( _nCmdShow );

	}

	return pMyGameInstance;


}


MyGameInstance* MyGameInstance::GetMyGameInstance()
{
	if(pMyGameInstance==NULL)//if Uninitialized
	{
		return 0;
	}

	return pMyGameInstance;
}


//for the window with all configuration set
void MyGameInstance::InitGame()
{

	config.SetLoadedSettingsToInst(pMyWindow);

	pMyWindow->CreateGameWindow();

	MyGame3DDevice::SetGameWindow( pMyWindow );

	pMyDevice = MyGame3DDevice::GetSingleton();

	MyGameMusic::InitMusicSystem();
}

//HRESULT MyGameInstance::UpdateScene_tmp2()
//{
///*	
//	this->pMyWindow->SetCursorPos();
//	MyGameUISprite::SetCursorPos( pMyWindow->GetCursorX(), pMyWindow->GetCursorY() );
//	MyGameUISprite::SetClicked( pMyWindow->IfClicked() );
//
//	pMyDevice->StartRender();
//	
//	
//	this->RenderSceneObjects();
//
//	MyGameDecoration::updateAllDecration();
//	MyGameClock::UpdateAllClock();
//	MyGame3DDevice::EnableUIAlphaBlending();
//	MyGameUISprite::RenderAllSprites();
//	MyGameUIText::RenderAllText();
//	
//	pMyDevice->EndRender();
//*/
//	IDirect3DDevice9* pD3D9InstanceDevice = this->pMyDevice->GetDevice();
//	//清除屏幕和zbuffer
//	//开始绘制
//	//pD3D9InstanceDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
//	/*
//	pD3D9InstanceDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
//
//	pD3D9InstanceDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//	pD3D9InstanceDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//	pD3D9InstanceDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT  );
//	pD3D9InstanceDevice->SetRenderState( D3DRS_LIGHTING, false );
//	*/
//	//effect
//	//HR(pD3D9InstanceDevice->SetVertexDeclaration(t));
//	
//	pD3D9InstanceDevice->SetVertexDeclaration( pMyGameVertexDecl );
//
//	pD3D9InstanceDevice->BeginScene();
//
//	D3DXMATRIX mat;
//
//	D3DXMatrixIdentity(&mat);
//
//	pD3D9InstanceDevice->SetTransform( D3DTS_WORLD, &mat );
//
//	{
//		//D3DXHANDLE hTech = pBloomEffect->GetTechniqueByName( "TransformTech" );
//		//HR(this->pBloomEffect->SetTechnique( hTech ) );
//		//HR(pD3D9InstanceDevice->SetStreamSource( 0, this->pVertexBuffer, 0, sizeof(float)*5 ) );
//		//D3DXHANDLE hTex = pBloomEffect->GetParameterByName( 0, "gTex" );
//		//HR(this->pBloomEffect->SetTexture( hTex, this->pBgTexture ));
//		//D3DXHANDLE hTex_t = pBloomEffect->GetParameterByName( 0, "gTex_t" );
//		//HR(this->pBloomEffect->SetTexture( hTex_t, this->pPass1RenderTarget ));
//		//unsigned int num = 0;
//		//HR(this->pBloomEffect->Begin( &num, 0 ) );
//		////for( unsigned int i = 0; i < num; ++i )
//		//{
//		//	HR(this->pBloomEffect->BeginPass(/*i*/0));
//		//		HR(pD3D9InstanceDevice->SetRenderTarget( 0, this->pPass1RenderTargetSurface ));
//		//		pD3D9InstanceDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
//		//		HR(pD3D9InstanceDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ));
//		//	HR(this->pBloomEffect->EndPass());
//		//	HR( pBloomEffect->BeginPass(1) );
//		//		HR(	pD3D9InstanceDevice->SetRenderTarget( 0, this->pScreenRenderSurface ));
//		//		pD3D9InstanceDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
//		//		//HR(this->pBloomEffect->CommitChanges() );
//		//		HR(pD3D9InstanceDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ));
//		//	HR( pBloomEffect->EndPass() );
//		//}
//		//HR(this->pBloomEffect->End());
//
//
//		
//		D3DXHANDLE hTech = pTransformEffect->GetTechniqueByName( "TransformTech" );
//		HR(this->pTransformEffect->SetTechnique( hTech ) );
//		HR(pD3D9InstanceDevice->SetStreamSource( 0, this->pVertexBuffer, 0, sizeof(float)*5 ) );
//		D3DXHANDLE hTex = pTransformEffect->GetParameterByName( 0, "gTex" );
//		D3DXHANDLE hSampler = pTransformEffect->GetParameterByName( 0, "Texture0");
//		HR(this->pTransformEffect->SetTexture( hTex, this->pBgTexture ));
//		//D3DXPARAMETER_DESC desc;
//		//this->pTransformEffect->GetParameterDesc( hSampler, &desc );
//		unsigned int num = 0;
//		HR(this->pTransformEffect->Begin( &num, 0 ) );
//		//for( unsigned int i = 0; i < num; ++i )
//		{
//			HR(this->pTransformEffect->BeginPass(/*i*/0));
//				pD3D9InstanceDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
//				HR(pD3D9InstanceDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ));
//			HR(this->pTransformEffect->EndPass());
//		}
//		HR(this->pTransformEffect->End());
//
//
//	//			pD3D9InstanceDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
//	//	//pD3D9InstanceDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
//	//	pD3D9InstanceDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
//	//	HR(pD3D9InstanceDevice->SetStreamSource( 0, this->pVertexBuffer, 0, sizeof(float)*5 ) );
//	//	HR(pD3D9InstanceDevice->SetTexture( 0, this->pBgTexture ));
//	//	
//	//pD3D9InstanceDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//	//pD3D9InstanceDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//	//pD3D9InstanceDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT  );
//	//pD3D9InstanceDevice->SetRenderState( D3DRS_LIGHTING, false );
//	//			HR(pD3D9InstanceDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ));
//	}
//
//	//DRAW UI
//	CEGUI::System::getSingleton().renderGUI();
//
//	//停止绘制
//	pD3D9InstanceDevice->EndScene();
//	pD3D9InstanceDevice->Present( 0, 0, 0, 0 );
//	return S_OK;
//}

//void MyGameInstance::InitGameSceneObjects()
//{
////	pD3D9InstanceDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
////	pD3D9InstanceDevice->CreateVertexBuffer(
//	
//	//IDirect3DDevice9* pD3D9InstanceDevice = this->pMyDevice->GetDevice();
//	
//	//pMyGameNotiy->MyGameNotifyBox("");
//
//	//return S_OK;
//}




struct header{
	unsigned int	bfSize;		//4 bytes
	unsigned int	bfReserved12;	//4 bytes
	unsigned int	bfOffBits;	//4 bytes
	unsigned int	biSize;		//4 bytes
	unsigned int	Width;		//4 bytes
	int	Height;		//4 bytes
	char biPlanes[4];
	unsigned int	biCompression;	//4 bytes
	unsigned int	biSizeImage;	//4 bytes
	unsigned int	biXPelsPerMeter;
	unsigned int	biYPelsPerMeter;
	unsigned int	biClrUsed;
	unsigned int	biClrImportant;
};

void GenerateGrid( int _width, int _height, float stride, /*in & out*/Vertex** _pVertices, /*in & out*/WORD** _pIndices )
{
	
	// map buffer
//	Vertex	mapVertices[11*11];
//	WORD	mapIndices[10*10*2*3];

	//FILE *pFile = fopen( "c.bmp", "rb" );
	FILE *pFile;
	fopen_s( &pFile, "c.bmp", "rb" );
	char bm[3];
	bm[2] = '\0';
	fread( bm, 1, 2, pFile );
	MyGameMessage( bm );

	struct header bmpHeader;
	fread( &bmpHeader, sizeof( struct header ), 1, pFile );

	unsigned char a[256*4];
	fread( a, 4, 256, pFile );
	unsigned char jiong[20*20];
	fread( jiong, 1, 20*20, pFile );

	*_pVertices = new Vertex[ ( _width + 1 ) * ( _height + 1 ) ];                                   

	*_pIndices	= new WORD[ _width * _height * 2 * 3];


	memset( *_pVertices, 0, sizeof(Vertex) *( _width + 1 ) * ( _height + 1 ) );

	srand ( time(NULL) );

	for( int i = 0; i < ( _width + 1 ) * ( _height + 1 ); ++i )
	{
		(*_pVertices)[i].pos.x = 0.0f + i%(_width+1)*stride;
		(*_pVertices)[i].pos.y = 480.0f - i/(_width+1)*stride;
		(*_pVertices)[i].pos.z = /*1.0f*/ /* rand()%20*/ -((float)jiong[i])/10.0f;
		(*_pVertices)[i].texCoord.x = (i%(_width+1))/(float)_width;
		(*_pVertices)[i].texCoord.y = (i/(_width+1))/(float)_height;
	}

	WORD* _tmpIndices = *_pIndices;
	for( int i = 0; i < _height; ++i )
	{
		for( int j = 0; j < _width; ++j )
		{
			D3DXVECTOR3 normalVector;
			*(_tmpIndices++) = ( i + 1 ) * (_width+1) + j;
			*(_tmpIndices++) =  i *(_width+1) +j;
			*(_tmpIndices++) =	i * (_width+1) + j + 1 ;

			CalculateNormal( &((*_pVertices)[( i + 1 ) * (_width+1) + j].pos),
							&((*_pVertices)[i *(_width+1) +j].pos),
							&((*_pVertices)[i * (_width+1) + j + 1 ].pos),
							&normalVector );

			(*_pVertices)[( i + 1 ) * (_width+1) + j].normal	+= normalVector;
			(*_pVertices)[i *(_width+1) +j].normal				+= normalVector;
			(*_pVertices)[i * (_width+1) + j + 1 ].normal		+= normalVector;
								


			*(_tmpIndices++) = ( i + 1 ) * (_width+1) + j;
			*(_tmpIndices++) = i * (_width+1) + j + 1;
			*(_tmpIndices++) = ( i + 1 ) * (_width+1) + j +1;

			CalculateNormal( &((*_pVertices)[( i + 1 ) * (_width+1) + j].pos),
							&((*_pVertices)[i * (_width+1) + j + 1].pos),
							&((*_pVertices)[( i + 1 ) * (_width+1) + j + 1 ].pos),
							&normalVector );

			(*_pVertices)[( i + 1 ) * (_width+1) + j].normal			+= normalVector;
			(*_pVertices)[i * (_width+1) + j + 1].normal				+= normalVector;
			(*_pVertices)[( i + 1 ) * (_width+1) + j + 1 ].normal		+= normalVector;
		}
	}

	
	for( int i = 0; i < ( _width + 1 ) * ( _height + 1 ); ++i )
	{
		D3DXVec3Normalize( &(*_pVertices)[i].normal, &(*_pVertices)[i].normal );
	}
}

void CalculateNormal( D3DXVECTOR3* _vector0, D3DXVECTOR3* _vector1, D3DXVECTOR3* _vector2, D3DXVECTOR3* _outVector )
{
	D3DXVECTOR3 A = *_vector1 - *_vector0;
	D3DXVECTOR3 B = *_vector2 - *_vector0;

	D3DXVec3Cross( _outVector, &A, &B );
}