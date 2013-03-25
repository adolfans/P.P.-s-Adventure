#pragma once
#include <string>
#include <vector>
using std::vector;
using std::string;

#include "MyGameSceneBillboardSet.h"
#include "MyGameScene.h"
class MyBarrageTransmitter
{
private:
	string textureFileName;
	float	width, height;
	float	x, y, z;
	unsigned int num;
	float	lifeTime;
	MyGameSceneBillboardSet*	billboardSet;
	MyGameSceneManager*			sceneManager;
	vector< MyGameSceneNode* >	nodeList;
	MyGameSceneNode*			transmitterRoot;
public:
	MyBarrageTransmitter(void);
	~MyBarrageTransmitter(void);

	void setImage( const string& fileName );
	void setSize( float width, float height );
	void setPosition( float x, float y, float z );
	void setMaxNum( unsigned int num );
	void setLifeCircle( float time );		//生命周期结束后会变成新的barrage

	void generate( MyGameSceneManager* sceneMgr );						//生成实体BillboardSet,不然没法绘制……

	void draw( float elapsedTime  );
};

