#include "StdAfx.h"
#include "MyGameSceneEntity.h"
#include "MyGameSceneNode.h"
#include "MyGameSceneManager.h"
#include "MyGameMesh.h"
namespace MyGameScene{
	MyGameSceneEntity::MyGameSceneEntity(MyGameMesh* _pMesh, const char* entName, MyGameSceneNode* node, MyGameSceneManager* sMgr )
		:pNode( node ), sceneMgr( sMgr )
	{
		//pNode = new MyGameSceneNode( entName );
		pMesh = _pMesh;
		SkinnedMesh* pSkinned = dynamic_cast< SkinnedMesh* >( _pMesh );
		if( pSkinned )
			pNode->AddChild( pSkinned->getSkeletonRoot() );
	}
	
/*	MyGameSceneEntity::MyGameSceneEntity( SkinnedMesh* _pSkinnedMesh, const char* entName, MyGameSceneNode* node, MyGameSceneManager* sMgr )
		:pNode( node ), sceneMgr( sMgr )
	{
		//pNode = new MyGameSceneNode( entName );
		pMesh = _pSkinnedMesh;
		pNode->AddChild(_pSkinnedMesh->getSkeletonRoot());
	}
	*/
	MyGameSceneEntity::~MyGameSceneEntity(void)
	{
	}
	void MyGameSceneEntity::render( MyGame3DEffect* _pEffect )
	{
		_pEffect->setMatrixByName( pNode->getCombinedMatrix()* sceneMgr->getViewProjCombinedMat(),
									MyGame3DEffect::WVPMATRIX );

		_pEffect->setMatrixByName( pNode->getCombinedMatrix()* sceneMgr->getLightViewProjMat(),
									MyGame3DEffect::LVPMATRIX );

		_pEffect->setWorldMatrix( pNode->getCombinedMatrix() );
		//_pEffect->CommitChanges();
		
		pMesh->render( _pEffect );
	}

	MyGameSceneNode* MyGameSceneEntity::getNode()
	{
		return pNode;
	}


}