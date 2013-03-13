#include "StdAfx.h"
#include "MyGame3DDevice.h"
#include "Resource.h"

#include "MyGameTexture.h"
#include "MyGame3DEffect.h"
#include <exception>
using std::runtime_error;
MyGame3DDevice*	MyGame3DDevice::pMyGameDevice = 0;
MyGameWindow*	MyGame3DDevice::pGameWindow		= 0;

MyGame3DDevice::MyGame3DDevice(void)
	:
#ifdef DX9
	pMyGameUIVertexDecl(0),
	pD3D9InstanceDevice(0)/*,
	uiMoveMatrix0( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ),
	uiMoveMatrix( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ),
	worldMatName( "gUIWorld" ),
	viewMatName( "gUIView" ),
	projMatName( "gUIProj" ),
	moveMatName( "gUIMove" ),
	techniqueName( "TransformTech" ),
	textureHandleName( "gTex" ),
	pUIIndexBuffer( 0 ),
	d3dpp(),
	mFX(0),
	mhTex(0),
	mhUIWorldMatHandle(0),
	mhUIViewMatHandle(0),
	mhUIProjMatHandle(0),
	mhUIMoveMatHandle(0),
	mhAlphaEnabled(0),
	pScreenTarget(0),
	pTextureTarget(0)*/
#endif
{
/*	uiRectIndices[0] = 0;
	uiRectIndices[1] = 1;
	uiRectIndices[2] = 2;
	uiRectIndices[3] = 2;
	uiRectIndices[4] = 1;
	uiRectIndices[5] = 3;*//*{ 0, 1, 2, 2, 1, 3 };*/
}

	//typedef void (*new_handler) ();
	//new_handler set_new_handler( new_handler p ) throw();


/*
void MyGame3DDevice::SetDeviceVersion(D3DXVERSION _deviceVersion)
{
	this->deviceVersion = _deviceVersion;
}*/


bool MyGame3DDevice::InitVertexDecl( D3DVERTEXELEMENT9* _decl )
{
	if( pMyGameUIVertexDecl )
	{
		MyGameMessage( "VertexDeclaration was already initialized" );
		return true;
	}
	if( FAILED( pD3D9InstanceDevice->CreateVertexDeclaration
							( _decl, &pMyGameUIVertexDecl ) ) )
	{
		MyGameMessage("CreateVetexDeclaration error!!");
		return false;
	}
	if(FAILED(pD3D9InstanceDevice->SetVertexDeclaration( pMyGameUIVertexDecl )))
	{	MyGameMessage("set vertex declaration error");
		
	}
	return true;
}
bool MyGame3DDevice::InitDevice()
{
	phxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, phxDefaultAllocatorCallback, phxDefaultErrorCallback);
	if(!phxFoundation)
	    throw runtime_error("PxCreateFoundation failed!");
	bool recordMemoryAllocations = true;
	phxProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(phxFoundation);
	phxSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *phxFoundation,
            PxTolerancesScale(), recordMemoryAllocations, phxProfileZoneManager );

	if(phxSDK == NULL) {
		throw runtime_error("Error creating PhysX device.");
	}
	 
	phxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *phxFoundation, PxCookingParams());

	if( !phxCooking )
		throw runtime_error( "PxCreateCooking failed" );

    if(!PxInitExtensions(*phxSDK))
	{
		throw runtime_error("PxInitExtensions failed!");
	}

	phxControllerManager = PxCreateControllerManager( *phxFoundation );

		////////////////////////////////
		//if choosed D3DX9
		////////////////////////////////
#ifdef DX9
		MyGameMessage("using d3d9");
		return InitD3DDevice9();
#endif
		////////////////////////////////
		//if choosed D3DX11
		////////////////////////////////
#ifdef DX11
		MyGameMessage("using d3d11");
		return InitD3DDevice11();
#endif
}

#ifdef DX9
IDirect3DDevice9* MyGame3DDevice::GetDevice()
{
	return pD3D9InstanceDevice;
}
#endif
MyGame3DDevice* MyGame3DDevice::GetSingleton()
{
	if(pMyGameDevice)
		return pMyGameDevice;
	pMyGameDevice = new MyGame3DDevice;
	pMyGameDevice->InitDevice();
	
	//
	//init UI vertex declaration
	//
/*	D3DVERTEXELEMENT9 decl[]=
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
		D3DDECLUSAGE_POSITION, 0},
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
		D3DDECLUSAGE_NORMAL, 0},
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
		D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	pMyGameDevice->InitVertexDecl( decl );
	//init effect shader
	pMyGameDevice->InitUIEffectShader();
	pMyGameDevice->InitUIIndexBuffer();
	MyGameTexture::SetTextureDevice(pMyGameDevice->GetDevice());
	MyGame3DEffect::SetEffectDevice( pMyGameDevice->GetDevice() );
	MyGameTextBase::SetDevice( pMyGameDevice->GetDevice() );
	MyGameIndexBuffer::SetIndexBufferDevice( pMyGameDevice->GetDevice() );
	*/
	return MyGame3DDevice::pMyGameDevice;
}
void MyGame3DDevice::SetGameWindow(MyGameWindow* _pGameWindow)
{
	pGameWindow = _pGameWindow;
}

MyGame3DDevice::~MyGame3DDevice(void)
{
	
#ifdef DX9
	//Release all the texture in list

	//for( map< int, IDirect3DTexture9* >::iterator _iter = myTextureList.begin();
	//for( map< int, MyGameTexture* >::iterator _iter = myTextureList.begin();
	//	_iter != myTextureList.end();
	//	++ _iter )
	//{	/*( *_iter ).second->Release();*/
	//	delete ( *_iter ).second;
	//}
		//IRelease(pUIIndexBuffer);
		//IRelease(pUIIndexBuffer);
	phxControllerManager->release();
	PxCloseExtensions();
	phxCooking->release();
	phxSDK->release();
	phxProfileZoneManager->release();
	phxFoundation->release();
	IRelease( screenSurface );
		IRelease( pMyGameUIVertexDecl );
		//pD3D9InstanceDevice->Release();
		//pD3D9InstanceDevice->Release();
		//pD3D9InstanceDevice->Release();
		IRelease(pD3D9InstanceDevice);
		//this->d3d9->Release();
		//d3d9->Release();
		//d3d9->Release();
		//this->d3d9->Release();
		//this->d3d9->Release();
		IRelease(this->d3d9);
#endif
#ifdef DX11
		IRelease(this->d3d11Objects.pD3D11DeviceContext);
		IRelease(this->d3d11Objects.pD3D11InstanceDevice);
		IRelease(this->d3d11Objects.pRenderTargetView);
#endif
}


#ifdef DX9
bool MyGame3DDevice::InitD3DDevice9()
{
	if( pD3D9InstanceDevice )
		return true;
	//Get the pointer to the IDirect3D9 interface
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&caps);

	//check if we can use hardware vertex processing
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		//	vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	/*
	if( this->wClient != _Width || height != _Height )
		::MessageBox( 0, L"the size of back buffer dosen't adjust the size of window", L"Error", 0 );
*/ 

	d3dpp.BackBufferWidth			= pGameWindow->GetWidthClient();
	d3dpp.BackBufferHeight			= pGameWindow->GetHeightClient();
	d3dpp.BackBufferFormat			= D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount			= 1;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow				= pGameWindow->GethWnd();
	d3dpp.Windowed					= true;				//Windowed
	d3dpp.EnableAutoDepthStencil	= true;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;		//depth format
	d3dpp.Flags						= 0;
	d3dpp.FullScreen_RefreshRateInHz= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr;

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		pGameWindow->GethWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&(pD3D9InstanceDevice)
		);
/*
==========================Initialize the global variable -- pd3dDevice
*/

	//d3d9 = 0;

	HR( pD3D9InstanceDevice->GetRenderTarget( 0, &screenSurface ) );//获取当前的显示表面


	if(FAILED(hr))
		return false;
	else
//		this->deviceInited = true;
		return true;

}
#endif
#ifdef DX11
bool MyGame3DDevice::InitD3DDevice11()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	DXGI_SWAP_CHAIN_DESC sd;
	::memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = this->wClient;
	sd.BufferDesc.Height = this->hClient;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = this->hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	//driver type 
	D3D_DRIVER_TYPE	g_driverType = D3D_DRIVER_TYPE_NULL;

	//swap chain
	IDXGISwapChain*         g_pSwapChain = NULL;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL,
										g_driverType,
										NULL,
										createDeviceFlags,
										featureLevels,
										numFeatureLevels,
										D3D11_SDK_VERSION,
										&sd,
										&g_pSwapChain,
										&(d3d11Objects.pD3D11InstanceDevice),
										&(d3d11Objects.d3d11FeatureLevel),
										&(d3d11Objects.pD3D11DeviceContext));

		if( SUCCEEDED( hr ) )
			break;

	}
	if( FAILED( hr ) )
		return false;

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
		return false;

	hr = d3d11Objects.pD3D11InstanceDevice->CreateRenderTargetView(
											pBackBuffer,
											NULL,
											&(d3d11Objects.pRenderTargetView) );

	pBackBuffer->Release();

	if( FAILED( hr ) )
        return false;

	d3d11Objects.pD3D11DeviceContext->OMSetRenderTargets( 1,
													&(d3d11Objects.pRenderTargetView),
													NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)wClient;
    vp.Height = (FLOAT)hClient;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
	d3d11Objects.pD3D11DeviceContext->RSSetViewports( 1, &vp );

	g_pSwapChain->Release();

	return true;
/*	
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
	UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
	hr = d3d11Objects.pD3D11InstanceDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &g_pVertexLayout );
*/

}
#endif




void MyGame3DDevice::StartRender()
{
	HR( pD3D9InstanceDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0) );
	HR( pD3D9InstanceDevice->BeginScene() );
}

void MyGame3DDevice::EndRender()
{
	HR(pD3D9InstanceDevice->EndScene());
	HR(pD3D9InstanceDevice->Present( 0, 0, 0, 0 ) );
}

MyGameTexture* MyGame3DDevice::CreateMyTextureFromFile( const char* _fileName )
{
	static unsigned int textureKey = 0;
/*
	IDirect3DTexture9* pSpriteTexture;

	HR(D3DXCreateTextureFromFileExA( pMyGameDevice->pD3D9InstanceDevice,
								_fileName,
								D3DX_DEFAULT,
								D3DX_DEFAULT,
								D3DX_DEFAULT,
								0,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								D3DX_DEFAULT,
								D3DX_DEFAULT,
								0,
								NULL,
								NULL,
								&pSpriteTexture));
*/
	MyGameTexture* pSpriteTexture = new MyGameTexture(_fileName);
	/*pMyGameDevice->myTextureList[textureKey] = pSpriteTexture;*/

	//return textureKey++;
	return pSpriteTexture;
}

MyGameTexture* MyGame3DDevice::CreateMyTextureForText( int _width, int _height )
{
	MyGameTexture* pSpriteTexture = new MyGameTexture( _width, _height);
	/*pMyGameDevice->myTextureList[textureKey] = pSpriteTexture;*/

	//return textureKey++;
	return pSpriteTexture;
}



//int MyGame3DDevice::CreateVertexBuffer( VertexType _type, void* _vertices, unsigned int verticeSize )
//{
//	static unsigned int key = 0;
//	
//	IDirect3DVertexBuffer9*	pVertexBuffer;
//
//	HR( pMyGameDevice->pD3D9InstanceDevice->CreateVertexBuffer(
//												verticeSize,
//												D3DUSAGE_WRITEONLY,
//												0,
//												D3DPOOL_MANAGED,
//												&pVertexBuffer,
//												0 ) );
//
//	myVertex* destVertices = 0;
//	pVertexBuffer->Lock( 0, 0, (void**)&destVertices, 0 );
//	memcpy( destVertices, _vertices, verticeSize );
//	pVertexBuffer->Unlock();
//
//	MyGameVertexBuffer* _myVertexBuffer = new MyGameVertexBuffer;
//
//	_myVertexBuffer->SetBufferSize( verticeSize );
//
//	_myVertexBuffer->EnableVertexBuffer( pVertexBuffer );
//
//	pMyGameDevice->myVertexBufferList[key] = _myVertexBuffer;
//
//	return key++;
//}

MyGameVertexBuffer* MyGame3DDevice::CreateVertexBuffer( VertexType _type, void* _vertices, unsigned int verticeSize )
{
	static unsigned int key = 0;
	
	IDirect3DVertexBuffer9*	pVertexBuffer;

	HR( pMyGameDevice->pD3D9InstanceDevice->CreateVertexBuffer(
												verticeSize,
												D3DUSAGE_WRITEONLY,
												0,
												D3DPOOL_MANAGED,
												&pVertexBuffer,
												0 ) );

	myVertex* destVertices = 0;
	pVertexBuffer->Lock( 0, 0, (void**)&destVertices, 0 );
	memcpy( destVertices, _vertices, verticeSize );
	pVertexBuffer->Unlock();

	MyGameVertexBuffer* _myVertexBuffer = new MyGameVertexBuffer;

	_myVertexBuffer->SetBufferSize( verticeSize );

	_myVertexBuffer->EnableVertexBuffer( pVertexBuffer );

	pMyGameDevice->myVertexBufferList[key] = _myVertexBuffer;

	return _myVertexBuffer;
}

MyGameIndexBuffer* MyGame3DDevice::CreateIndexBuffer( int _indicesNum, void* _pSrcIndexBuffer, unsigned int _size )
{
	MyGameIndexBuffer* _pIndexBuffer = new MyGameIndexBuffer( _indicesNum );
	_pIndexBuffer->SetIndexBuffer( _pSrcIndexBuffer, _size );
	return _pIndexBuffer;
}
void MyGame3DDevice::EnableVertexBuffer( int _vertexBufferKey )
{
	if( !pMyGameDevice->myVertexBufferList.count( _vertexBufferKey ) )
		return;

	HR( pMyGameDevice->pD3D9InstanceDevice->SetStreamSource( 0,
											( pMyGameDevice->myVertexBufferList[_vertexBufferKey]->GetVertexBuffer()),
											0,
											sizeof( myVertex )));
}

void MyGame3DDevice::EnableVertexBuffer( MyGameVertexBuffer* _vb )
{
		HR( pMyGameDevice->pD3D9InstanceDevice->SetStreamSource( 0,
											( _vb->GetVertexBuffer()),
											0,
											sizeof( myVertex )));
}

void MyGame3DDevice::DrawIndexedTrianglePrimitive( int verticesCount, int triangleCount )
{
	HR( pMyGameDevice->pD3D9InstanceDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, verticesCount, 0, triangleCount) );
}

void MyGame3DDevice::DestroyVertexBuffer( int _bufferKey )
{
	if( !(pMyGameDevice->myVertexBufferList.count( _bufferKey )) )
		return;

	//pMyGameDevice->myVertexBufferList[ _bufferKey ].vertexBuffer->Release();

	delete (pMyGameDevice->myVertexBufferList[ _bufferKey ]);//因为是动态分配的所以要清除……么

	pMyGameDevice->myVertexBufferList.erase( _bufferKey );
}

void MyGame3DDevice::DestroyVertexBuffer( MyGameVertexBuffer* _vb )
{
	delete _vb;
}

void MyGame3DDevice::DrawTrianglePrimitive( D3DPRIMITIVETYPE _type, UINT StartVertex, UINT PrimitiveCount  )
{
	HR( pMyGameDevice->pD3D9InstanceDevice->DrawPrimitive( _type, StartVertex, PrimitiveCount) );
}

MyGame3DEffect*	MyGame3DDevice::CreateEffectFromResourceID( int _rsID )
{
	MyGame3DEffect* _pBloomEffect = new MyGame3DEffect( _rsID );
	return _pBloomEffect;
}

PxPhysics*		MyGame3DDevice::getPhysX()
{
	return	phxSDK;
}

PxFoundation*	MyGame3DDevice::getPhysXFoundation()
{
	return this->phxFoundation;
}

PxControllerManager* MyGame3DDevice::getPhysXControllerManager()
{
	return this->phxControllerManager;
}

PxCooking*		MyGame3DDevice::getPhysXCooking()
{
	return phxCooking;
}

IDirect3DSurface9* MyGame3DDevice::getScreenSurface()
{
	return screenSurface;
}

void MyGame3DDevice::restoreScreenRenderTarget()
{
	HR( pD3D9InstanceDevice->SetRenderTarget(0, this->screenSurface) );	

}