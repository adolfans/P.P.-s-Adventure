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
        bool        visible;
		MyGameSceneEntity( MyGameMesh* _pMesh, const char* entName, MyGameSceneNode* node, MyGameSceneManager* sMgr );
		//MyGameSceneEntity( SkinnedMesh* _pSkinnedMesh, const char* entName, MyGameSceneNode* node, MyGameSceneManager* sMgr );
		virtual ~MyGameSceneEntity(void);


	public:
		void render( MyGame3DEffect* _pEffect );
		MyGameSceneNode* getNode();
        MyGameMesh*      getMesh();
		void prepare();
		bool intersectTest( const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayDir, float& dist );
        void setVisible(bool _visible );
		void getBoundingBox( D3DXVECTOR3& _minPoint, D3DXVECTOR3& _maxPoint )
		{
			this->pMesh->getBoundingBox( _minPoint, _maxPoint );
		}
	};
}
