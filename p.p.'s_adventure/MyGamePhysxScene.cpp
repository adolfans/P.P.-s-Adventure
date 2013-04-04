#include "StdAfx.h"
#include "MyGamePhysxScene.h"

MyGamePhysxScene::MyGamePhysxScene( int unit )
{
	//Create the scene
	PxPhysics* pxPhysics = MyGame3DDevice::GetSingleton()->getPhysX();
	PxSceneDesc sceneDesc(pxPhysics->getTolerancesScale());
	
	sceneDesc.gravity=PxVec3(0.0f, -9.8f, 0.0f)*(physx::PxReal)unit;

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
	
}


MyGamePhysxScene::~MyGamePhysxScene(void)
{
	mCpuDispatcher->release();
	phxScene->release();
}
