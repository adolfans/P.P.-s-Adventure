#pragma once
#include "MyGame3DDevice.h"
class MyGamePhysxScene
{
private:
	PxDefaultCpuDispatcher*		mCpuDispatcher;
	PxScene*					phxScene;
public:
	PxScene*		getPhysXScene() { return phxScene; }
	MyGamePhysxScene(int unit);
	~MyGamePhysxScene(void);
};

