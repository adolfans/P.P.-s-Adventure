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
#include "SkinnedMesh.h"
namespace MyGameScene{
	class MyGameSceneEntity
	{
	private:
		MyGameSceneNode* pNode;
		MyGameMesh* pMesh;
	public:
		MyGameSceneEntity( MyGameMesh* _pMesh, const char* entName );
		MyGameSceneEntity( SkinnedMesh* _pSkinnedMesh, const char* entName );
		~MyGameSceneEntity(void);

		void render( MyGame3DEffect* _pEffect );
		MyGameSceneNode* getNode();
	};
}