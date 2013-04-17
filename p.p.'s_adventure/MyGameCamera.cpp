#include "StdAfx.h"
#include "MyGameCamera.h"


MyGameCamera::MyGameCamera(float eyeX, float eyeY, float eyeZ, 
				float targetX, float targetY, float targetZ)
{
	position.x = eyeX;
	position.y = eyeY;
	position.z = eyeZ;
	D3DXVECTOR3 eye( eyeX, eyeY, eyeZ );	//最后一个数一开始是-20= =||
	//D3DXVECTOR3 eye( -40.0f, 10.0f, 10.0f );
	// 0.0f 10.f, 40.0f
	D3DXVECTOR3 target( targetX, targetY ,targetZ );
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );
	::D3DXMatrixLookAtLH( &viewMatrix, &eye, &target, &up );
	D3DXVECTOR3 _vec = target-eye;
	lookingVector.x = _vec.x;
	lookingVector.y = _vec.y;
	lookingVector.z = _vec.z;
}


MyGameCamera::~MyGameCamera(void)
{
}

void MyGameCamera::updatePosToScene()
{
	//sceneMgr->SetViewMat( );
}

void MyGameCamera::rotateY( float angle )
{
	D3DXMATRIX rotateMat;
	::D3DXMatrixRotationY( &rotateMat, angle );
	viewMatrix*=rotateMat;
}

void MyGameCamera::move( float _x, float _y, float _z )
{
	D3DXMATRIX transMat;
	::D3DXMatrixTranslation( &transMat, _x, _y, _z );
	viewMatrix*=transMat;
}

const D3DXMATRIX& MyGameCamera::getViewMatrix()
{
	return this->viewMatrix;
}

void MyGameCamera::setPosition( float _x, float _y, float _z )
{
	//TODO: 需要根据looking vector计算相机目标点的位置，然后再生成view matrix
	//未完成注意！

	this->position.x = _x;
	position.y = _y;
	position.z = _z;
}

void MyGameCamera::setTargetPosition( D3DXVECTOR3 target )
{
	//根据比例变换，或将更改
	target = this->sceneMgr->sceneLengthToNormalLength( target );
	this->position = target - lookingVector;
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );
	::D3DXMatrixIdentity( &viewMatrix );
	::D3DXMatrixLookAtLH( &viewMatrix, &this->position, &target, &up );
}

D3DXVECTOR3 MyGameCamera::getPosition()
{
	return this->position;
}

void MyGameCamera::rotate( float angle )	//around target
{
	//D3DXMATRIX rotateMat;
	//::D3DXMatrixRotationY( &rotateMat, angle );
	//viewMatrix*=rotateMat;
	D3DXVECTOR3 target = this->position + lookingVector;

	D3DXMATRIX rotateMat;
	D3DXMatrixRotationY( &rotateMat, angle );

	//target*= rotateMat;
	D3DXVECTOR4 newLookingVector;
	newLookingVector.w = 0;
	D3DXVec3Transform( &newLookingVector, &lookingVector, &rotateMat );
	
	lookingVector.x = newLookingVector.x;
	lookingVector.y = newLookingVector.y;
	lookingVector.z = newLookingVector.z;

	this->setTargetPosition( target );
}