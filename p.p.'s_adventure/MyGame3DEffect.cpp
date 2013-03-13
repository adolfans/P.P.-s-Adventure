#include "StdAfx.h"
#include "MyGame3DEffect.h"

#include "MyGame3DDevice.h"
#include "MyGameSceneEntity.h"
#include "MyGameSceneNode.h"
#include "MyGameSceneManager.h"

#include <exception>
using std::runtime_error;

IDirect3DDevice9*	MyGame3DEffect::pD9Device = 0;
const char* MyGame3DEffect::WVPMATRIX = "WorldViewProj";
const char* MyGame3DEffect::FINMATARRAY = "FinalTransforms";
const char* MyGame3DEffect::TEXTURE = "Tex";
const char* MyGame3DEffect::VERTBLEND = "VertBlend";
const char* MyGame3DEffect::TECH = "main";
const char* MyGame3DEffect::LVPMATRIX = "LightViewProj";
const char* MyGame3DEffect::SHADOWMAP = "ShadowMap";

MyGame3DEffect::MyGame3DEffect(void)
{
}

MyGame3DEffect::MyGame3DEffect( int _resourceID )
{
	HRSRC hResource = ::FindResource( NULL, MAKEINTRESOURCE(_resourceID), RT_RCDATA );
	HGLOBAL hResourceData = ::LoadResource( NULL, hResource );
	LPVOID pData = ::LockResource( hResourceData );

	ID3DXBuffer* errorBuffer = 0;
	HR( D3DXCreateEffect( pD9Device,
							pData,
							::SizeofResource( NULL, hResource ),
							0,
							0,
							0,
							0,
							&pD9Effect,
							&errorBuffer ) );
	if( errorBuffer )
	{
		throw runtime_error( (char*)errorBuffer->GetBufferPointer() );
		errorBuffer->Release();
		return;
	}

	return;
}

MyGame3DEffect::MyGame3DEffect( const char* fileName )
{
	ID3DXBuffer* errorBuffer = 0;
	this->pD9Device = MyGame3DDevice::GetSingleton()->GetDevice();
	HR( D3DXCreateEffectFromFileA( pD9Device, 
									fileName, 
									0, 
									0, 
									D3DXSHADER_DEBUG|
									D3DXSHADER_SKIPOPTIMIZATION|/*
									D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT|
									D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT|*/
									D3DXSHADER_IEEE_STRICTNESS|
									D3DXSHADER_NO_PRESHADER|
									D3DXSHADER_SKIPVALIDATION, 
									0, 
									&pD9Effect,
									&errorBuffer ) );

	if( errorBuffer )
	{
		throw runtime_error( (char*)errorBuffer->GetBufferPointer() );
		errorBuffer->Release();//废了= =||
		return;
	}

	hWVPMatrix = pD9Effect->GetParameterByName( 0, WVPMATRIX );
	hFinalMatArray = pD9Effect->GetParameterByName( 0, FINMATARRAY );
	hTexture_o = pD9Effect->GetParameterByName( 0, TEXTURE );
	hVertBlend = pD9Effect->GetParameterByName( 0, VERTBLEND );
	hMainTech = pD9Effect->GetTechniqueByName(TECH );
	hLVPMatrix = pD9Effect->GetParameterByName( 0, LVPMATRIX );
	hShadowMap = pD9Effect->GetParameterByName( 0, SHADOWMAP );

	hCameraPosition = pD9Effect->GetParameterBySemantic( 0, "CameraPosition" );
	hDiffuse		= pD9Effect->GetParameterBySemantic( 0, "Diffuse" );
	hAmbient		= pD9Effect->GetParameterBySemantic( 0, "Ambient" );
	hSpecular		= pD9Effect->GetParameterBySemantic( 0, "Specular" );
	hParallelLightPos = pD9Effect->GetParameterBySemantic( 0, "ParallelLightVector" );
	hViewMatrix		= pD9Effect->GetParameterBySemantic( 0, "ViewMatrix" );
	hWorldMatrix	= pD9Effect->GetParameterBySemantic( 0, "WorldMatrix" );
	hViewProjMatrix = pD9Effect->GetParameterBySemantic( 0, "ViewProjMatrix" );

	
	for( int i = 0; i != 6; ++ i )
	{
		char buff[4];//其实一个字节就够了- -
		_itoa_s(i, buff, 4);
		string textureName( string("Texture") + buff );
		hTexture[i] = pD9Effect->GetParameterBySemantic( 0, textureName.c_str() );
	}
	hCameraVector = pD9Effect->GetParameterBySemantic( 0, "ViewMatrix" );

}

void MyGame3DEffect::setMatrixByName( D3DXMATRIX& _matrix, const char* _matrixName )
{
	if( _matrixName == WVPMATRIX )
	{	pD9Effect->SetMatrix( hWVPMatrix, &_matrix ); return; }
	if( _matrixName == LVPMATRIX )
	{	pD9Effect->SetMatrix( hLVPMatrix, &_matrix ); return; }
	D3DXHANDLE mhMatrix = pD9Effect->GetParameterByName( 0, _matrixName );
	if( !mhMatrix )
	{
		string err = "当前Effect中找不到";
		err	+=_matrixName;
		err +="这个Matrix";
		throw runtime_error(err);
	}
	pD9Effect->SetMatrix( mhMatrix, &_matrix );
}

void MyGame3DEffect::setTechniqueByName( const char* _techName )
{
	if( _techName == TECH )
	{	pD9Effect->SetTechnique( hMainTech); return ; }
	D3DXHANDLE mhTech = pD9Effect->GetTechniqueByName( _techName );
	if( !mhTech )
	{
		string err = "当前Effect中找不到";
		err	+=_techName;
		err +="这个Technique";
		throw runtime_error(err);
	}
	pD9Effect->SetTechnique( mhTech );
}

void MyGame3DEffect::setBOOLByName( BOOL _ifEnable, const char* _boolVarName )
{
	if( _boolVarName== VERTBLEND)
	{  pD9Effect->SetBool( hVertBlend, _ifEnable ); return; }
	D3DXHANDLE mhBOOL = pD9Effect->GetParameterByName( 0, _boolVarName );
	if( !mhBOOL )
	{
		throw runtime_error(string("当前Effect中找不到")+ _boolVarName +"这个BOOL");
	}	
	pD9Effect->SetBool( mhBOOL, _ifEnable );
}

MyGame3DEffect::~MyGame3DEffect(void)
{
	pD9Effect->Release();
}

void MyGame3DEffect::setEffectDevice( IDirect3DDevice9* _pDevice )
{
	pD9Device = _pDevice;
}

void MyGame3DEffect::setTexture( MyGameTexture* _pTexture, const char* _texName )
{
	D3DXHANDLE mhTexture = pD9Effect->GetParameterByName( 0, _texName );
	if( _pTexture == 0 )
	{
		HR( pD9Effect->SetTexture( mhTexture, 0 ) );
	}else
		HR( pD9Effect->SetTexture( mhTexture, _pTexture->GetD9Texture() ) );
	pD9Effect->CommitChanges();
}

void MyGame3DEffect::setTexture( unsigned int index, IDirect3DTexture9* _tex )
{
	this->pD9Effect->SetTexture( this->hTexture[index], _tex );
}

void MyGame3DEffect::setTextureByName( IDirect3DTexture9* _pTex, const char* _texName )
{
	if( _texName == TEXTURE )
	{	pD9Effect->SetTexture( hTexture_o, _pTex ); return; }
	D3DXHANDLE mhTexture = pD9Effect->GetParameterByName( 0, _texName );
	if( !mhTexture )
		throw runtime_error( string("当前Effect中没有找到")+_texName+"这个变量");
	HR( pD9Effect->SetTexture( mhTexture, _pTex ) );
}

void MyGame3DEffect::setMatrixArrayByName( D3DXMATRIX*& _pMat, unsigned int count, const char* _matArrayName )
{
	if( _matArrayName == FINMATARRAY )
	{	pD9Effect->SetMatrixArray( hFinalMatArray, _pMat, count ); return; }
	D3DXHANDLE mhMatArray = pD9Effect->GetParameterByName( 0, _matArrayName );
	if( !mhMatArray )
	{
		throw runtime_error(string("当前Effect中找不到")+_matArrayName+"这个BOOL");
	}
	HR( pD9Effect->SetMatrixArray( mhMatArray, _pMat, count ) );
}

void MyGame3DEffect::AddEntity( MyGameSceneEntity* _ent )
{
	//即使重复插入也没有关系，因为Effect并不管理对象的删除，不必关心内存泄漏
	entList[_ent->getNode()->getName()] = _ent;
	//this->entList[ 
}

void MyGame3DEffect::RenderAllEntities( MyGameSceneManager* sceneMgr )
{
	unsigned int num;
	this->setTechniqueByName( MyGame3DEffect::TECH );
	this->setTextureByName( sceneMgr->getShadowMap(), MyGame3DEffect::SHADOWMAP );
	this->setTextureByName( sceneMgr->getShadowMap(), MyGame3DEffect::SHADOWMAP );
	this->pD9Effect->SetValue( this->hCameraPosition, &sceneMgr->getCameraPosition(), sizeof( D3DVECTOR ) );
	this->pD9Effect->SetValue( this->hCameraVector, &sceneMgr->getCameraVector(), sizeof( D3DXVECTOR3 ) );
	//this->pD9Effect->SetValue( this->hViewProjMatrix, &sceneMgr->getViewProjCombinedMat(), sizeof( D3DXMATRIX ) );
	this->pD9Effect->SetValue( this->hParallelLightPos, &sceneMgr->getMainLightVector(), sizeof( D3DVECTOR ) );
	this->pD9Effect->SetMatrix( this->hViewMatrix, &sceneMgr->getViewMat() );
	this->pD9Effect->SetMatrix( this->hViewProjMatrix, &sceneMgr->getViewProjCombinedMat() );
	this->Begin(num);
	for( unsigned int i = 0; i < num; ++ i )
	{
		this->BeginPass( i );
		for( map<string,MyGameSceneEntity* >::iterator _itr = entList.begin();
			_itr != entList.end();
			++ _itr )
		{
			_itr->second->render( this );
		}
		this->EndPass();
	}
	this->End();
}

void MyGame3DEffect::setEntities( MyGame3DEffect* anotherEffect )
{
	for( auto _itr = anotherEffect->entList.begin();
		_itr != anotherEffect->entList.end();
		 ++ _itr )
	{
		this->entList[_itr->first] = _itr->second;
	}
}