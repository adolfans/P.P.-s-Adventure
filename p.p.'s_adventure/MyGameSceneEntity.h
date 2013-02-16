//////////////////////////
//һ��Mesh��Ӧ���Entity
//һ��Entityֻ��һ��mesh
//˵����Entityֻ��mesh��ʵ��
//meshû��Entity�ͺ�������
//ֻ��Entity���ܰ�mesh���Ƴ���
//��˵��������дʲô���쵰��
//һ��Entity��Ӧһ��node
//һ��Effect���汣��һ��Entity List
//////////////////////////

#pragma once
#include "MyGameScene.h"
#include "MyGame3DEffect.h"
#include "MyGameMesh.h"
#include "SkinnedMesh.h"
namespace MyGameScene{
	class MyGameSceneEntity
	{
		friend class MyGameSceneManager;
	private:
		MyGameSceneManager* sceneMgr;
		MyGameSceneNode* pNode;
		MyGameMesh* pMesh;
		MyGameSceneEntity( MyGameMesh* _pMesh, const char* entName, MyGameSceneNode* node, MyGameSceneManager* sMgr );
		//MyGameSceneEntity( SkinnedMesh* _pSkinnedMesh, const char* entName, MyGameSceneNode* node, MyGameSceneManager* sMgr );
		virtual ~MyGameSceneEntity(void);

	public:
		void render( MyGame3DEffect* _pEffect );
		MyGameSceneNode* getNode();
	};
}