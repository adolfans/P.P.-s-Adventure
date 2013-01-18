#include "StdAfx.h"
#include "MyGameLevelMainMenu.h"
#include "MyGame3DDevice.h"
#include <CEGUI.h>
#include <RendererModules/Direct3D9/CEGUIDirect3D9Renderer.h>
MyGameLevelMainMenu::MyGameLevelMainMenu(void)
	:MyGameLevel()
{
	
	IDirect3DDevice9* pD3D9InstanceDevice = 
	MyGame3DDevice::GetSingleton()->GetDevice();

	//编译effect
	HR( D3DXCreateEffectFromFile( pD3D9InstanceDevice, L"effect.fx", 0, 0, 0, 0, &pBloomEffect, 0 ) );
	
	HR( D3DXCreateEffectFromFile( pD3D9InstanceDevice, L"transform.fx", 0, 0, 0, 0, &pTransformEffect, 0 ) );

	//创建顶点声明
	D3DVERTEXELEMENT9 decl[]=
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
		D3DDECLUSAGE_POSITION, 0},
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
		D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	HR(pD3D9InstanceDevice->CreateVertexDeclaration( decl, &pMyGameVertexDecl ));

	//启用顶点声明
	HR(pD3D9InstanceDevice->SetVertexDeclaration( pMyGameVertexDecl ));

	//创建vertex buffer
	HR( pD3D9InstanceDevice->CreateVertexBuffer( 5*6*sizeof(float),
												D3DUSAGE_WRITEONLY,
												0,
												D3DPOOL_MANAGED,
												&pVertexBuffer,
												0 ) );

	float vertices[5*6] = 
	{
//		x     y      z		u		v 
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f,0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,  0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f
	};
	
	//锁定并填充vertex buffer
	float* _dest;
	pVertexBuffer->Lock( 0, 5*6*sizeof(float), (void**)&_dest, 0 );
	memcpy( _dest, vertices, 5*6*sizeof(float) );
	pVertexBuffer->Unlock();	//解除锁定

	//HR( pD3D9InstanceDevice->CreateTexture( 800, 600, 1, D3DX_DEFAULT, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pBgTexture, 0 ) );

	//创建背景贴图    
	HR( D3DXCreateTextureFromFile( pD3D9InstanceDevice, L"ys_s 2012-10-08 15-22-18-67.bmp", &pBgTexture ) ); 

	HR( pD3D9InstanceDevice->CreateTexture( MyGame3DDevice::GetWidth(),
											MyGame3DDevice::GetHeight(),
											1,
											D3DUSAGE_RENDERTARGET,
											//D3DFMT_A8R8G8B8,
											D3DFMT_R16F,
											D3DPOOL_DEFAULT,
											&pPass1RenderTarget,
											0 ) );


	HR( pD3D9InstanceDevice->GetRenderTarget( 0, &pScreenRenderSurface ) );

	//HR( pD3D9InstanceDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pScreenRenderSurface ));
	pPass1RenderTargetSurface = 0;
	HR( pPass1RenderTarget->GetSurfaceLevel( 0, &pPass1RenderTargetSurface ) );

	
	using namespace CEGUI;
	//CEGUI::Logger::getSingleton().setLogFilename("cegui.log");
	CEGUI::Direct3D9Renderer& myRenderer =
    CEGUI::Direct3D9Renderer::bootstrapSystem( pD3D9InstanceDevice );

	DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(
									CEGUI::System::getSingleton().getResourceProvider());
	//rp->setResourceGroupDirectory("imagesets", "D:\Visual Studio 2012\Projects\testGaussianBlur\datafiles\imagesets");
	
	rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
	//rp->setResourceGroupDirectory("animation
	// This is only really needed if you are using Xerces and need to
	// specify the schemas location
	rp->setResourceGroupDirectory("schemas", "../datafiles/xml_schemas/");
	
	
	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	System::getSingleton().setDefaultTooltip( "TaharezLook/Tooltip" );
	
	// setup default group for validation schemas
	CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
	if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
	    parser->setProperty("SchemaDefaultResourceGroup", "schemas");


	// (this auto-loads the TaharezLook looknfeel and imageset files)
	CEGUI::SchemeManager::getSingleton().create( "TaharezLook.scheme" );
	
	// (this auto-loads the TaharezLook looknfeel and imageset files)
	CEGUI::SchemeManager::getSingleton().create( "test.scheme" );

	if(! CEGUI::FontManager::getSingleton().isDefined( "DejaVuSans-10" ) )
	{
		CEGUI::FontManager::getSingleton().create( "DejaVuSans-10.font" );
	}
	
	System::getSingleton().setDefaultFont( "DejaVuSans-10" );

    // The next thing we do is to set a default mouse cursor image.  This is
    // not strictly essential, although it is nice to always have a visible
    // cursor if a window or widget does not explicitly set one of its own.
    //
    // The TaharezLook Imageset contains an Image named "MouseArrow" which is
    // the ideal thing to have as a defult mouse cursor image.
    
	System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	Window* myRoot = WindowManager::getSingleton().loadWindowLayout( "testttt.layout" );
	
	//subscribeEvent("TextDemo/HorzLeft", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
  
	System::getSingleton().setGUISheet( myRoot );

	CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML( "../datafiles/animations/example.xml", "animation" );

	CEGUI::Window* startBtn =WindowManager::getSingleton().getWindow("Root/main/startBtn");
	CEGUI::Animation *anim = CEGUI::AnimationManager::getSingleton().getAnimation( "MyMainMenuItem" );
	CEGUI::Animation *anim2 = CEGUI::AnimationManager::getSingleton().getAnimation( "MyMainMenuItem2" );

	CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set its target window
	instance->setTargetWindow(startBtn);	//设置到btn
	instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	instance->setTargetWindow(startBtn);
	
	CEGUI::Window* loadBtn =WindowManager::getSingleton().getWindow("Root/main/loadBtn");
	CEGUI::AnimationInstance* instance2 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set its target window
	instance2->setTargetWindow(loadBtn);	//设置到btn
	instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	instance->setTargetWindow(loadBtn);


	CEGUI::Window* configBtn =WindowManager::getSingleton().getWindow("Root/main/configBtn");
	CEGUI::AnimationInstance* instance3 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set its target window
	instance3->setTargetWindow(configBtn);	//设置到btn
	instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	instance->setTargetWindow(configBtn);

	
	CEGUI::Window* exitBtn =WindowManager::getSingleton().getWindow("Root/main/exitBtn");
	CEGUI::AnimationInstance* instance4 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set its target window
	instance4->setTargetWindow(exitBtn);	//设置到btn
	instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	instance->setTargetWindow(exitBtn);




	CEGUI::Window* btttn;
	btttn	=WindowManager::getSingleton().getWindow("Root/new/new");

	CEGUI::AnimationInstance* inst = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set its target window
	inst->setTargetWindow(btttn);	//设置到btn
	inst = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	inst->setTargetWindow(btttn);
	myconn= btttn->subscribeEvent( ButtonBase::EventMouseClick, Event::Subscriber( &MyGameLevelMainMenu::Button1Clicked, this ) );


	btttn =WindowManager::getSingleton().getWindow("Root/new/load");
	inst = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set its target window
	inst->setTargetWindow(btttn);	//设置到btn
	inst = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	inst->setTargetWindow(btttn);


	btttn =WindowManager::getSingleton().getWindow("Root/new/sys");
	inst = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set its target window
	inst->setTargetWindow(btttn);	//设置到btn
	inst = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	inst->setTargetWindow(btttn);

	
	btttn =WindowManager::getSingleton().getWindow("Root/new/info");
	inst = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set its target window
	inst->setTargetWindow(btttn);	//设置到btn
	inst = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	inst->setTargetWindow(btttn);

}


MyGameLevelMainMenu::~MyGameLevelMainMenu(void)
{	

	HR( D3DXSaveTextureToFile( L"囧.png",D3DXIFF_PNG,pPass1RenderTarget, NULL ) );
	this->pBgTexture->Release();
	this->pPass1RenderTarget->Release();
	this->pPass1RenderTargetSurface->Release();
	this->pScreenRenderSurface->Release();
	this->pBloomEffect->Release();
	this->pVertexBuffer->Release();
	this->pMyGameVertexDecl->Release();
	this->pTransformEffect->Release();

	myconn->disconnect();
   
	
}


void MyGameLevelMainMenu::Update()
{
	D3DXMATRIX mat;

	D3DXMatrixIdentity(&mat);
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	pDevice->SetVertexDeclaration(this->pMyGameVertexDecl);

	{	
		D3DXHANDLE hTech = pTransformEffect->GetTechniqueByName( "TransformTech" );
		HR(this->pTransformEffect->SetTechnique( hTech ) );
		HR(pDevice->SetStreamSource( 0, this->pVertexBuffer, 0, sizeof(float)*5 ) );
		D3DXHANDLE hTex = pTransformEffect->GetParameterByName( 0, "gTex" );
		D3DXHANDLE hSampler = pTransformEffect->GetParameterByName( 0, "Texture0");
		HR(this->pTransformEffect->SetTexture( hTex, this->pBgTexture ));
		//D3DXPARAMETER_DESC desc;
		//this->pTransformEffect->GetParameterDesc( hSampler, &desc );
		unsigned int num = 0;
		HR(this->pTransformEffect->Begin( &num, 0 ) );
		//for( unsigned int i = 0; i < num; ++i )
		{
			HR(this->pTransformEffect->BeginPass(/*i*/0));
				pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
				HR(pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ));
			HR(this->pTransformEffect->EndPass());
		}
		HR(this->pTransformEffect->End());
	}
	
}



#include "MyGameLevel1.h"
bool MyGameLevelMainMenu::Button1Clicked(const CEGUI::EventArgs&)
{
	MyGameLevel *childLevel = new MyGameLevel1();
	//childLevel->SetPtr( &childLevel );
	this->ModifyCurrentLevel( childLevel );
	return true;
}

bool MyGameLevelMainMenu::Button2Clicked(const CEGUI::EventArgs&)
{
        //This function gets called when the button is clicked
        //std::cout << "Button clicked" << std::endl;
	//MessageBox( 0, L"点击", 0 , 0 );
	D3DXHANDLE handle = pBloomEffect->GetParameterByName( 0, "mode" );
	pBloomEffect->SetInt( handle, 2 );
	return true;

}
bool MyGameLevelMainMenu::Button3Clicked(const CEGUI::EventArgs&)
{
        //This function gets called when the button is clicked
        //std::cout << "Button clicked" << std::endl;
	//MessageBox( 0, L"点击", 0 , 0 );
	D3DXHANDLE handle = pBloomEffect->GetParameterByName( 0, "mode" );
	pBloomEffect->SetInt( handle, 3 );
	return true;
}
bool MyGameLevelMainMenu::Button4Clicked(const CEGUI::EventArgs&)
{
        //This function gets called when the button is clicked
        //std::cout << "Button clicked" << std::endl;
	//MessageBox( 0, L"点击", 0 , 0 );
	D3DXHANDLE handle = pBloomEffect->GetParameterByName( 0, "mode" );
	pBloomEffect->SetInt( handle, 4 );
	return true;
}
bool MyGameLevelMainMenu::Button5Clicked(const CEGUI::EventArgs&)
{
        //This function gets called when the button is clicked
        //std::cout << "Button clicked" << std::endl;
	//MessageBox( 0, L"点击", 0 , 0 );
	D3DXHANDLE handle = pBloomEffect->GetParameterByName( 0, "mode" );
	pBloomEffect->SetInt( handle, 5 );
	return true;
}