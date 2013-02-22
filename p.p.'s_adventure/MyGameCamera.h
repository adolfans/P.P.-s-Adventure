#pragma once
#include "MyGameScene.h"
#include "MyGameSceneManager.h"
class MyGameCamera
{
	friend MyGameCamera* MyGameSceneManager::CreateCamera( float eyeX, float eyeY, float eyeZ, 
				float targetX, float targetY, float targetZ);
	friend void MyGameSceneManager::destroyAllCameras();
private:
	MyGameScene::MyGameSceneManager* sceneMgr;
	D3DXMATRIX viewMatrix;
	MyGameCamera( float eyeX, float eyeY, float eyeZ, 
				float targetX, float targetY, float targetZ);
	~MyGameCamera(void);
public:
	void updatePosToScene();
	const D3DXMATRIX& getViewMatrix();
	void rotateY( float degree );
	void move( float _x, float _y, float _z );
};

