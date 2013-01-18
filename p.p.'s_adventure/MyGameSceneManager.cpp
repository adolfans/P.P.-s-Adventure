#include "StdAfx.h"
#include "MyGameSceneManager.h"


namespace MyGameScene{
	D3DXMATRIX MyGameSceneManager::vMat;
	D3DXMATRIX MyGameSceneManager::pMat;
MyGameSceneManager::MyGameSceneManager(void)
{
}


MyGameSceneManager::~MyGameSceneManager(void)
{
}

void MyGameSceneManager::SetViewProjMat( D3DXMATRIX& _vMat, D3DXMATRIX& _pMat )
{
	vMat = _vMat; pMat = _pMat;
}

D3DXMATRIX MyGameSceneManager::getViewProjCombinedMat()
{
	return vMat* pMat;

}
}