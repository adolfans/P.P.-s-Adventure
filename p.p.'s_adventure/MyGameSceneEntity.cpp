#include "StdAfx.h"
#include "MyGameSceneEntity.h"
#include "MyGameSceneNode.h"
#include "MyGameSceneManager.h"
#include "MyGameMesh.h"
namespace MyGameScene{
	MyGameSceneEntity::MyGameSceneEntity(MyGameMesh* _pMesh, const char* entName)
	{
		pNode = new MyGameSceneNode( entName );
		pMesh = _pMesh;
	}
	
	MyGameSceneEntity::MyGameSceneEntity( SkinnedMesh* _pSkinnedMesh, const char* entName )
	{
		pNode = new MyGameSceneNode( entName );
		pMesh = _pSkinnedMesh;
		pNode->AddChild(_pSkinnedMesh->getSkeletonRoot());
	}

	MyGameSceneEntity::~MyGameSceneEntity(void)
	{
	}
	void MyGameSceneEntity::render( MyGame3DEffect* _pEffect )
	{
		_pEffect->SetMatrixByName( pNode->getCombinedMatrix()* MyGameSceneManager::getViewProjCombinedMat(),
									MyGame3DEffect::WVPMATRIX );

		_pEffect->SetMatrixByName( pNode->getCombinedMatrix()* MyGameSceneManager::getLightViewProjMat(),
									MyGame3DEffect::LVPMATRIX );

		//_pEffect->CommitChanges();
		
		pMesh->render( _pEffect );
	}

	MyGameSceneNode* MyGameSceneEntity::getNode()
	{
		return pNode;
	}


}