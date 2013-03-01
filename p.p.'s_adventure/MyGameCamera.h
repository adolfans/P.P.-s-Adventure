#pragma once
#include "MyGameScene.h"
#include "MyGameSceneManager.h"
class MyGameCamera
{
	friend MyGameCamera* MyGameSceneManager::CreateCamera( float eyeX, float eyeY, float eyeZ, 
				float targetX, float targetY, float targetZ);
	friend void MyGameSceneManager::destroyAllCameras();
private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookingVector;
	MyGameScene::MyGameSceneManager* sceneMgr;
	D3DXMATRIX viewMatrix;
	MyGameCamera( float eyeX, float eyeY, float eyeZ, 
				float targetX, float targetY, float targetZ);
	~MyGameCamera(void);
public:
	void updatePosToScene();
	const D3DXMATRIX& getViewMatrix();
	const D3DXVECTOR3& getLookingVector() const{ return lookingVector;}
	void rotateY( float angle );
	void move( float _x, float _y, float _z );

	void setPosition( float _x, float _y, float _z );
	void setTargetPosition( D3DXVECTOR3 pos );

	void rotate( float angle );//Œ£œ’£°

	D3DXVECTOR3 getPosition();
};

