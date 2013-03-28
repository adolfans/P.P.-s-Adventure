#ifndef MYGAMEFBXSCENELOADER_H
#define MYGAMEFBXSCENELOADER_H

#include "MyGameSceneManager.h"

#include "MyGameSceneEntity.h"
#include <vector>
using std::vector;
class MyGameFbxSceneLoader
{
public:
	MyGameFbxSceneLoader(void);
	virtual ~MyGameFbxSceneLoader(void);

	static void getNodeFromFbxNode( MyGameScene::MyGameSceneManager* sceneMgr,
							FbxNode* node,
							MyGameSceneNode* parentSceneNode,
							vector<MyGameSceneEntity*>& entityList );
	static void loadSceneFromFbx( MyGameScene::MyGameSceneManager* sceneMgr,
							const string& fileName,
							MyGameSceneNode* node,
							vector<MyGameSceneEntity*>& entityList  );
	
};

#endif