#pragma once
#include <d3dx9.h>
#include <map>
using std::map;

#include "MyGameTexture.h"
#include "MyGameScene.h"
using namespace MyGameScene;
/*
#define WVPMATRIX "WorldViewProj"
#define FINMATARRAY "FinalTransforms"
#define TEXTURE "Tex"
#define VERTBLEND "VertBlend"
#define TECH		"main"
*/
class MyGame3DEffect
{
public:
private:
	ID3DXEffect*	pD9Effect;
	map<string,MyGameSceneEntity* > entList;

	D3DXHANDLE hWVPMatrix;
	D3DXHANDLE hFinalMatArray;
	D3DXHANDLE hTexture;
	D3DXHANDLE hVertBlend;
	D3DXHANDLE hMainTech;
	D3DXHANDLE hLVPMatrix;
	D3DXHANDLE hShadowMap;


	static IDirect3DDevice9* pD9Device;
	


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
	void Begin( unsigned int &num )
	{  this->pD9Effect->Begin( &num, 0 );}
	void End(){ this->pD9Effect->End(); }
	void BeginPass( unsigned int num ){ pD9Effect->BeginPass( num ); }
	void EndPass(){ pD9Effect->EndPass(); }
	void CommitChanges() { pD9Effect->CommitChanges(); }


	void AddEntity( MyGameSceneEntity* _ent );

	void RenderAllEntities( MyGameSceneManager* sceneMgr );
};
