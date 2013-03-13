#pragma once
#include <d3dx9.h>
#include <map>
using std::map;

#include "MyGameTexture.h"
#include "MyGameScene.h"
using namespace MyGameScene;

class MyGame3DEffect
{
public:
private:
	ID3DXEffect*	pD9Effect;
	map<string,MyGameSceneEntity* > entList;

	D3DXHANDLE hWVPMatrix;
	D3DXHANDLE hFinalMatArray;
	D3DXHANDLE hTexture_o;
	D3DXHANDLE hVertBlend;
	D3DXHANDLE hMainTech;
	D3DXHANDLE hLVPMatrix;
	D3DXHANDLE hShadowMap;


	static IDirect3DDevice9* pD9Device;
	
	D3DXHANDLE hCameraPosition;
	D3DXHANDLE hCameraVector;
	D3DXHANDLE hDiffuse;
	D3DXHANDLE hAmbient;
	D3DXHANDLE hSpecular;
	D3DXHANDLE hParallelLightPos;
	D3DXHANDLE hViewMatrix;
	D3DXHANDLE hWorldMatrix;
	D3DXHANDLE hTexture[6];		//代表六重纹理
	D3DXHANDLE hViewProjMatrix;

	map< D3DXHANDLE, IDirect3DTexture9* >	textureList;

	map< D3DXHANDLE, D3DXMATRIX >			matrixList;

public:
	static const char* WVPMATRIX;// = "WorldViewProj";
	static const char* FINMATARRAY;
	static const char* TEXTURE;
	static const char* VERTBLEND;
	static const char* TECH;
	static const char* LVPMATRIX;
	static const char* SHADOWMAP;
	MyGame3DEffect(void);
	MyGame3DEffect( int _resourceID );
	MyGame3DEffect( const char* fileName );
	~MyGame3DEffect(void);

	static void setEffectDevice( IDirect3DDevice9* _pDevice );
	
/*	void SetMatrixByHandle( D3DXMATRIX& _matrix, const char* _matrixHandle );
	void SetTechniqueByHandle( const char* _techHandle );
	void SetBOOLByHandle( BOOL _ifEnable, const char* _boolVarHandle );
	void SetMatrixArrayByHandle( D3DXMATRIX*& _pMat, unsigned int count, const char* _matArrayHandle );
	void SetTextureByHandle( IDirect3DTexture9* _pTex, const char* _texHandle );
	*/
	
	void setMatrixByName( D3DXMATRIX& _matrix, const char* _matrixName );
	void setTechniqueByName( const char* _techName );
	void setBOOLByName( BOOL _ifEnable, const char* _boolVarName );
	void setMatrixArrayByName( D3DXMATRIX*& _pMat, unsigned int count, const char* _matArrayName );
	void setTexture( MyGameTexture* _pTexture, const char* _texName );
	void setTextureByName( IDirect3DTexture9* _pTex, const char* _texName );
	void setTexture( unsigned int index, IDirect3DTexture9* );
	//D3DXHANDLE GetHandleByName(const char* name)
	//{
	//	if( name == WVPMATRIX )
	//		return hWVPMatrix;
	//	if( name == FINMATARRAY )
	//		return hFinalMatArray;
	//	if( name == TEXTURE )
	//		return hTexture;
	//	if( name == VERTBLEND )
	//		return hVertBlend;
	//	if( name == TECH )
	//		return hMainTech;
	//}

	//void BeginEffect();
	//void EndEffect();

	//

	void setDiffuse( D3DVECTOR& vec ){ pD9Effect->SetValue( hDiffuse, &vec, sizeof(D3DVECTOR) );}
	void setAmbient( D3DVECTOR& vec ){ pD9Effect->SetValue( hAmbient, &vec, sizeof(D3DVECTOR) ); }
	void setSpecular( D3DVECTOR& vec ){ pD9Effect->SetValue( hSpecular, &vec, sizeof(D3DVECTOR)); }

	void setWorldMatrix( D3DXMATRIX& worldMatrix ){ pD9Effect->SetMatrix( hWorldMatrix, &worldMatrix ); }  

	void Begin( unsigned int &num )
	{  this->pD9Effect->Begin( &num, 0 );}
	void End(){ this->pD9Effect->End(); }
	void BeginPass( unsigned int num ){ pD9Effect->BeginPass( num ); }
	void EndPass(){ pD9Effect->EndPass(); }
	void CommitChanges() { pD9Effect->CommitChanges(); }


	void AddEntity( MyGameSceneEntity* _ent );

	void attachTextureToName( const char* name, IDirect3DTexture9* texture )
	{
		D3DXHANDLE handle = this->pD9Effect->GetParameterByName( 0, name );
		pD9Effect->SetTexture( handle, texture );
		this->textureList[handle] = texture;
	}

	void attachMatrixToName( const char* name, const D3DXMATRIX& matrix )
	{
		D3DXHANDLE handle = pD9Effect->GetParameterByName( 0, name );
		pD9Effect->SetMatrix( handle, &matrix );
	}

	void RenderAllEntities( MyGameSceneManager* sceneMgr );
	D3DXHANDLE getHandleByName( string name )
	{	
		return this->pD9Effect->GetParameterByName( 0, name.c_str() );
	}
	void setValueByHandle( D3DXHANDLE handle, void* data, unsigned int size )
	{
		this->pD9Effect->SetValue( handle, data, size );
	}

	void setEntities( MyGame3DEffect* anotherEffect );
};
