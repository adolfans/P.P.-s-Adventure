//////////////////////////
//一个Mesh对应多个Entity
//一个Entity只有一个mesh
//说白了Entity只是mesh的实例
//mesh没有Entity就毫无意义
//只有Entity才能把mesh绘制出来
//话说我在这里写什么啊混蛋。
//一个Entity对应一个node
//一个Effect里面保存一个Entity List
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