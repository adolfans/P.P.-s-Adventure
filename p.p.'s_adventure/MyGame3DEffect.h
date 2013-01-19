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


	static IDirect3DDevice9* pD9Device;
	

public:
	static const char* WVPMATRIX;// = "WorldViewProj";
	static const char* FINMATARRAY;
	static const char* TEXTURE;
	static const char* VERTBLEND;
	static const char* TECH;
	static const char* LVPMATIX;
	static const char* SHADOWMAP;
	MyGame3DEffect(void);
	MyGame3DEffect( int _resourceID );
	MyGame3DEffect( const char* fileName );
	~MyGame3DEffect(void);

	static void SetEffectDevice( IDirect3DDevice9* _pDevice );

	void SetMatrixByName( D3DXMATRIX& _matrix, const char* _matrixName );
	void SetTechniqueByName( const char* _techName );
	void SetBOOLByName( BOOL _ifEnable, const char* _boolVarName );
	void SetMatrixArrayByName( D3DXMATRIX*& _pMat, unsigned int count, const char* _matArrayName );
	void SetTexture( MyGameTexture* _pTexture, const char* _texName );
	void SetTextureByName( IDirect3DTexture9* _pTex, const char* _texName );

	//void BeginEffect();
	//void EndEffect();
	void Begin( unsigned int &num )
	{  this->pD9Effect->Begin( &num, 0 );}
	void End(){ this->pD9Effect->End(); }
	void BeginPass( unsigned int num ){ pD9Effect->BeginPass( num ); }
	void EndPass(){ pD9Effect->EndPass(); }
	void CommitChanges() { pD9Effect->CommitChanges(); }


	void AddEntity( MyGameSceneEntity* _ent );

	void RenderAllEntities();
};
