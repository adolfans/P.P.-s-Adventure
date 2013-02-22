#pragma once
//#include "MyGameSceneNode.h"
#include "MyGameScene.h"
#include "MyGameSceneNode.h"
#include "MyGameBoneNode.h"
#include "MyGameSceneEntity.h"
#include "SkinnedMesh.h"
#include "d3dx9.h"
#include "MyGame3DDevice.h"

class MyGameCamera;
namespace MyGameScene{
class MyGameSceneManager
{
public:
	enum UnitOfLength
	{
		Millimeter = 1000,
		Meter = 1
	};
private:
	//D3DXMATRIX vMat;
	D3DXMATRIX pMat;
	D3DXMATRIX lightViewProjMat;
	IDirect3DTexture9* shadowMap;
	vector< MyGameSceneEntity * > entityList;
	vector< MyGameCamera* >		cameraList; 
	UnitOfLength unit;
	PxDefaultCpuDispatcher*		mCpuDispatcher;
	PxScene*					phxScene;
	MyGameCamera*				currentCamera;
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

	MyGameCamera* CreateCamera( float eyeX, float eyeY, float eyeZ, 
				float targetX, float targetY, float targetZ);

	void destroyAllCameras();

	/*static*/// void setViewProjMat( const D3DXMATRIX& _vMat,const D3DXMATRIX& _pMat );
	//void setViewMat( const D3DXMATRIX& _vMat ){ vMat = _vMat; }
	/*static*/ void setLightViewProjMat( const D3DXMATRIX& lightMat )
	{ lightViewProjMat = lightMat; }
	/*static*/ void setShadowMap( IDirect3DTexture9* _tex );

	void setCamera( MyGameCamera* cam );

	/*static*/ IDirect3DTexture9* getShadowMap();
	/*static*/ const D3DXMATRIX& getViewMat();//{ return vMat; }
	/*static*/ D3DXMATRIX& getProjMat(){ return pMat;}
	/*static*/ D3DXMATRIX  getViewProjCombinedMat();

	/*static*/ D3DXMATRIX&  getLightViewProjMat(){ return lightViewProjMat;}

	PxScene*		getPhysXScene() { return phxScene; }

	UnitOfLength getUnitOfLength()
	{	return this->unit; }

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