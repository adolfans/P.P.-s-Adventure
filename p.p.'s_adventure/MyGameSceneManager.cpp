#include "StdAfx.h"
#include "MyGameSceneManager.h"
#include "MyGameCamera.h"
#include <d3dx9math.h>
#include <exception>
using std::runtime_error;
namespace MyGameScene{
	/*
	D3DXMATRIX MyGameSceneManager::vMat;
	D3DXMATRIX MyGameSceneManager::pMat;
	D3DXMATRIX MyGameSceneManager::lightViewProjMat;
	IDirect3DTexture9* MyGameSceneManager::shadowMap = 0;
	*/
MyGameSceneManager::MyGameSceneManager(void)
	:unit(Millimeter)
{
	//Create the scene
	PxPhysics* pxPhysics = MyGame3DDevice::GetSingleton()->getPhysX();
	PxSceneDesc sceneDesc(pxPhysics->getTolerancesScale());
	
	sceneDesc.gravity=PxVec3(0.0f, -9.8f, 0.0f)*unit;

	if(!sceneDesc.cpuDispatcher) {
		mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if(!mCpuDispatcher)
		   throw runtime_error( "PxDefaultCpuDispatcherCreate failed!" );
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}
	if(!sceneDesc.filterShader)
		sceneDesc.filterShader  = PxDefaultSimulationFilterShader;

	 
	
	phxScene = pxPhysics->createScene(sceneDesc);
	if (!phxScene)
		runtime_error( "createScene failed!" );

	phxScene->setVisualizationParameter(PxVisualizationParameter::eSCALE,				 1.0);
	phxScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES,	1.0f);
	
	float h = (float)MyGame3DDevice::GetHeight();
	float w = (float)MyGame3DDevice::GetWidth();
	D3DXMatrixPerspectiveFovLH( &pMat, D3DX_PI*0.2f, w/h, 5.0f, 100.0f );//最后一个一开始是2000= =||
	
	root = new MyGameSceneNode( "root" );

	root->scale( 0.003, 0.003f, 0.003f );
}


MyGameSceneManager::~MyGameSceneManager(void)
{
	mCpuDispatcher->release();
	phxScene->release();
}
/*
void MyGameSceneManager::setViewProjMat( const D3DXMATRIX& _vMat, const D3DXMATRIX& _pMat )
{
	vMat = _vMat; pMat = _pMat;
}
*/
D3DXMATRIX MyGameSceneManager::getViewProjCombinedMat()
{
	//return vMat* pMat;
	return this->currentCamera->getViewMatrix() * pMat;
}

void MyGameSceneManager::setShadowMap( IDirect3DTexture9* _tex )
{
	shadowMap = _tex;
}

IDirect3DTexture9* MyGameSceneManager::getShadowMap()
{
	return shadowMap;
}

const D3DXVECTOR3& MyGameSceneManager::getCameraVector() const
{
	return this->currentCamera->getLookingVector();
}

MyGameCamera* MyGameSceneManager::CreateCamera( float eyeX, float eyeY, float eyeZ, 
				float targetX, float targetY, float targetZ)
{
	MyGameCamera* newCam = new MyGameCamera( eyeX, eyeY, eyeZ, 
						targetX, targetY, targetZ);
	newCam->sceneMgr = this;
	cameraList.push_back( newCam );
	return newCam;
}

void MyGameSceneManager::destroyAllCameras()
{
	for( vector< MyGameCamera* >::iterator _itr = cameraList.begin();
		_itr != cameraList.end();
		++ _itr )
	{
		delete *_itr;
	}
}

void MyGameSceneManager::setCamera( MyGameCamera* cam)
{
	this->currentCamera = cam;
}

const D3DXMATRIX& MyGameSceneManager::getViewMat()
{
	return this->currentCamera->getViewMatrix();
}

D3DVECTOR MyGameSceneManager::getCameraPosition()
{
	return currentCamera->getPosition();
}

void MyGameSceneManager::setParallelMainLight( MyGameCamera* cam )
{
	
	float h = (float)MyGame3DDevice::GetHeight();
	float w = (float)MyGame3DDevice::GetWidth();
	D3DXMatrixOrthoLH( &this->lightProjMat, w/20.0f, h/15.0f, 15.0f, 50.0f );

	this->lightCamera = cam;

	this->lightViewProjMat = cam->getViewMatrix( ) * lightProjMat;
}

D3DXMATRIX& MyGameSceneManager::getLightViewProjMat()
{
	lightViewProjMat = lightCamera->getViewMatrix( ) * lightProjMat;
	return lightViewProjMat;
}

D3DVECTOR MyGameSceneManager::getMainLightPosition()
{
	return this->lightCamera->getPosition();
}

D3DXVECTOR3 MyGameSceneManager::getMainLightVector()
{
	return -lightCamera->getLookingVector();
}
void MyGameSceneManager::updateAllEntities()
{
	for( auto _itr = entityList.begin();
		_itr != entityList.end();
		++ _itr )
	{
		(*_itr)->prepare();
	}
}
}