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
		//MyGameMessage( (char*)errorBuffer->GetBufferPointer() );
		throw runtime_error( (char*)errorBuffer->GetBufferPointer() );
		errorBuffer->Release();
		return;
	}

	//initialize matrix in shader

	//D3DXMATRIX worldMatrix;

	//D3DXMatrixTranslation( &worldMatrix, -512.0f, -384.0f, 0 );

	//D3DXVECTOR3 position( 0.0f, 0.0f, 20.0f );
	//D3DXVECTOR3 targetPoint( 0.0f, 0.0f ,0.0f );
	//D3DXVECTOR3 upVector( 0.0f, -1.0f, 0.0f );

	//D3DXMATRIX	viewMatrix;

	//D3DXMatrixLookAtLH( &viewMatrix, &position, &targetPoint, &upVector );

	//D3DXMATRIX  projMatrix;

	//D3DXMatrixOrthoLH( &projMatrix, 1024.0f, 768.0f, -1009.0f, 5000.0f );

	//D3DXMatrixTranslation( &uiMoveMatrix, 0.0f, 0.0f, 0 );

	//mhUIWorldMatHandle	= pD9Effect->GetParameterByName( 0, worldMatName );
	//mhUIViewMatHandle	= pD9Effect->GetParameterByName( 0, viewMatName );
	//mhUIProjMatHandle	= pD9Effect->GetParameterByName( 0, projMatName );
	//mhUIMoveMatHandle	= pD9Effect->GetParameterByName( 0, moveMatName );
	//mhAlphaEnabled		= pD9Effect->GetParameterByName( 0, "alphaEnable" );
	//D3DXHANDLE mhTech	= pD9Effect->GetTechniqueByName( techniqueName );

	//mhTex = mFX->GetParameterByName( 0, "gTex" );

	//mFX->SetMatrix( mhUIWorldMatHandle, &worldMatrix );
	//mFX->SetMatrix( mhUIViewMatHandle, &viewMatrix );
	//mFX->SetMatrix( mhUIProjMatHandle, &projMatrix );
	//mFX->SetMatrix( mhUIMoveMatHandle, &uiMoveMatrix );

	//mFX->SetTechnique( mhTech );

	//HR(mFX->SetBool( mhAlphaEnabled, FALSE ));

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
		//MyGameMessage( (char*)errorBuffer->GetBufferPointer() );
		//errorBuffer->Release();
		throw runtime_error( (char*)errorBuffer->GetBufferPointer() );
		errorBuffer->Release();//����= =||
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
	//hTexture1		= pD9Effect->GetParameterBySemantic( "Texture", 1 );
	
	for( int i = 0; i != 6; ++ i )
	{
		char buff[4];//��ʵһ���ֽھ͹���- -
		_itoa_s(i, buff, 4);
		string textureName( string("Texture") + buff );
		hTexture[i] = pD9Effect->GetParameterBySemantic( 0, textureName.c_str() );
	}
	hCameraVector = pD9Effect->GetParameterBySemantic( 0, "ViewMatrix" );

	//if( !mhMatrix )
	//{
	//	string err = "��ǰEffect���Ҳ���";
	//	err	+=_matrixName;
	//	err +="���Matrix";
	//	throw runtime_error(err);
	//}

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
		string err = "��ǰEffect���Ҳ���";
		err	+=_matrixName;
		err +="���Matrix";
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
		string err = "��ǰEffect���Ҳ���";
		err	+=_techName;
		err +="���Technique";
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
		throw runtime_error(string("��ǰEffect���Ҳ���")+ _boolVarName +"���BOOL");
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

//void MyGame3DEffect::BeginEffect()
//{
//	UINT num = 0;
//
//	HR( pD9Effect->Begin( &num, 0 ) );
//
//	HR( pD9Effect->BeginPass( 0 ) );
//}
//
//void MyGame3DEffect::EndEffect()
//{
//	HR( pD9Effect->EndPass() );
//
//	HR( pD9Effect->End() );
//}

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
		throw runtime_error( string("��ǰEffect��û���ҵ�")+_texName+"�������");
	HR( pD9Effect->SetTexture( mhTexture, _pTex ) );
}

void MyGame3DEffect::setMatrixArrayByName( D3DXMATRIX*& _pMat, unsigned int count, const char* _matArrayName )
{
	if( _matArrayName == FINMATARRAY )
	{	pD9Effect->SetMatrixArray( hFinalMatArray, _pMat, count ); return; }
	D3DXHANDLE mhMatArray = pD9Effect->GetParameterByName( 0, _matArrayName );
	if( !mhMatArray )
	{
		throw runtime_error(string("��ǰEffect���Ҳ���")+_matArrayName+"���BOOL");
	}
	HR( pD9Effect->SetMatrixArray( mhMatArray, _pMat, count ) );
}

void MyGame3DEffect::AddEntity( MyGameSceneEntity* _ent )
{
	//��ʹ�ظ�����Ҳû�й�ϵ����ΪEffect������������ɾ�������ع����ڴ�й©
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
	this->pD9Effect->SetValue( this->hParallelLightPos, &sceneMgr->getMainLightVector(), sizeof( D3DVECTOR ) );
	this->pD9Effect->SetMatrix( this->hViewMatrix, &sceneMgr->getViewMat() );
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

