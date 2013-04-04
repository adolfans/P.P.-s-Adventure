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

	void MyGameSceneEntity::prepare()
	{
		pMesh->prepare();
	}

	bool MyGameSceneEntity::intersectTest( const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayDir, float& dist )
	{
		D3DXMATRIX inversedMat;

		D3DXMatrixInverse( &inversedMat, NULL, &pNode->getCombinedMatrix() );

		D3DXVECTOR3 pos;// = rayPos;
		D3DXVECTOR3 dir;// = rayDir;

		pos.x = rayPos.x * inversedMat._11 + rayPos.y * inversedMat._21 + rayPos.z * inversedMat._31 + 1.0 * inversedMat._41;
		pos.y = rayPos.x * inversedMat._12 + rayPos.y * inversedMat._22 + rayPos.z * inversedMat._32 + 1.0 * inversedMat._42;
		pos.z = rayPos.x * inversedMat._13 + rayPos.y * inversedMat._23 + rayPos.z * inversedMat._33 + 1.0 * inversedMat._43;
		float w = rayPos.x * inversedMat._14 + rayPos.y * inversedMat._24 + rayPos.z * inversedMat._34 + 1.0 * inversedMat._44;

		pos.x /= w;
		pos.y /= w;
		pos.z /= w;

		dir.x = rayDir.x * inversedMat._11 + rayDir.y * inversedMat._21 + rayDir.z * inversedMat._31 + 0.0 * inversedMat._41;
		dir.y = rayDir.x * inversedMat._12 + rayDir.y * inversedMat._22 + rayDir.z * inversedMat._32 + 0.0 * inversedMat._42;
		dir.z = rayDir.x * inversedMat._13 + rayDir.y * inversedMat._23 + rayDir.z * inversedMat._33 + 0.0 * inversedMat._43;
		//float w = rayPos.x * inversedMat._14 + rayPos.y * inversedMat._24 + rayPos.z * inversedMat._34 + 1.0 * inversedMat._44;


		if( pMesh->intersectTest( pos, dir, dist ) ) 
		{
			float dirLen = sqrt( dir.x * dir.x + dir.y * dir.y + dir.z * dir.z );

			dir = dir / dirLen * dist;	//变换到dist的长度

			D3DXVECTOR3 intersect = pos + dir;//相交点的坐标

			D3DXMATRIX currentMat( pNode->getCombinedMatrix() );

			D3DXVECTOR3 transformedIntersect;
		
			transformedIntersect.x = intersect.x * currentMat._11 + intersect.y * currentMat._21 + intersect.z * currentMat._31 + 1.0 * currentMat._41;
			transformedIntersect.y = intersect.x * currentMat._12 + intersect.y * currentMat._22 + intersect.z * currentMat._32 + 1.0 * currentMat._42;
			transformedIntersect.z = intersect.x * currentMat._13 + intersect.y * currentMat._23 + intersect.z * currentMat._33 + 1.0 * currentMat._43;
			float transformedIntersectw = intersect.x * currentMat._14 + intersect.y * currentMat._24 + intersect.z * currentMat._34 + 1.0 * currentMat._44;

			transformedIntersect /=transformedIntersectw;

			D3DXVECTOR3 rayVector = transformedIntersect - rayPos;

			float rayLen = sqrt( rayVector.x * rayVector.x + rayVector.y * rayVector.y + rayVector.z * rayVector.z );

			dist = rayLen;
			
			return true;
		}else
			return false;
	}
}