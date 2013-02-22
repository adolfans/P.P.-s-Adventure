#include "StdAfx.h"
#include "MyGameCamera.h"


MyGameCamera::MyGameCamera(float eyeX, float eyeY, float eyeZ, 
				float targetX, float targetY, float targetZ)
{
	D3DXVECTOR3 eye( eyeX, eyeY, eyeZ );	//最后一个数一开始是-20= =||
	//D3DXVECTOR3 eye( -40.0f, 10.0f, 10.0f );
	// 0.0f 10.f, 40.0f
	D3DXVECTOR3 target( targetX, targetY ,targetZ );
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );
	::D3DXMatrixLookAtLH( &viewMatrix, &eye, &target, &up );
}


MyGameCamera::~MyGameCamera(void)
{
}

void MyGameCamera::updatePosToScene()
{
	//sceneMgr->SetViewMat( );
}

void MyGameCamera::rotateY( float degree )
{
	D3DXMATRIX rotateMat;
	::D3DXMatrixRotationY( &rotateMat, degree );
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