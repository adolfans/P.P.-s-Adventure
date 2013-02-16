#pragma once
//#include "MyGameSceneNode.h"
#include "MyGameScene.h"
#include "MyGameSceneNode.h"
#include "MyGameBoneNode.h"
#include "MyGameSceneEntity.h"
#include "SkinnedMesh.h"
#include "d3dx9.h"
namespace MyGameScene{
class MyGameSceneManager
{
private:
	static D3DXMATRIX vMat;
	static D3DXMATRIX pMat;
	static D3DXMATRIX lightViewProjMat;
	static IDirect3DTexture9* shadowMap;
	vector< MyGameSceneEntity * > entityList;
public:
	MyGameSceneManager(void);
	~MyGameSceneManager(void);

	/*static*/ MyGameSceneNode* CreateSceneNode( const char* name )
	{ return new MyGameSceneNode( name ); }//Yooooooooooo~

	MyGameBoneNode* CreateBoneNode( const char* name, bool managedByDx )
	{ return new MyGameBoneNode( name, managedByDx ); }

	MyGameSceneEntity* CreateSceneEntity( MyGameMesh* _pMesh, const char* entName )
	{	
		MyGameSceneNode* node = new MyGameSceneNode( entName );
		MyGameSceneEntity* newEnt = new MyGameSceneEntity(_pMesh, entName, node, this );
		entityList.push_back( newEnt );
		return newEnt;
	}
	/*static*/ void setViewProjMat( D3DXMATRIX& _vMat, D3DXMATRIX& _pMat );
	/*static*/ void setLightViewProjMat( D3DXMATRIX& lightMat )
	{ lightViewProjMat = lightMat; }
	/*static*/ void setShadowMap( IDirect3DTexture9* _tex );
	/*static*/ IDirect3DTexture9* getShadowMap();

	/*static*/ D3DXMATRIX& getViewMat(){ return vMat; }
	/*static*/ D3DXMATRIX& getProjMat(){ return pMat;}
	/*static*/ D3DXMATRIX  getViewProjCombinedMat();

	/*static*/ D3DXMATRIX&  getLightViewProjMat(){ return lightViewProjMat;}

	template< typename N >
	void destroy( N& name ){ delete name; };
	void destroyAllEntities()
	{
		for( vector<MyGameSceneEntity*>::iterator _itr = entityList.begin();
			_itr != entityList.end();
			++ _itr )
		{
			delete *_itr;
		}
	}
};
}