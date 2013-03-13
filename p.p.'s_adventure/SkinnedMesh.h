#pragma once
#include <d3dx9.h>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <WinDef.h>
using std::map;
using std::vector;
using std::string;
using std::queue;

#include "MyGameMesh.h"
#include "MyGameBoneNode.h"
#include "MyGame3DEffect.h"

using namespace MyGameScene;

struct D3DXMESHCONTAINER_EX
	:D3DXMESHCONTAINER
{
	DWORD NumInfl;
	DWORD NumAttributeGroups;
	DWORD NumPaletteEntries;
};


class SkinnedMesh
	: public MyGameMesh
{
	friend class MyGameMeshManager;
private:
	MyGameSceneManager* sceneMgr;

	D3DXFRAME* findNodeWithMesh(D3DXFRAME* frame );

	void buildSkinnedMesh( ID3DXMesh* mesh );

	LPD3DXFRAME pHierarchyRoot;

	ID3DXAnimationController* pAnimCtrller;
	ID3DXSkinInfo* pSkinInfo;
	DWORD maxVertInfluences;

	vector<D3DXMATRIX> combinedTransforms;
	unsigned int numBones;	
	DWORD  numBoneComboEntries;

	float lastTime;

	DWORD NumAttributeGroups;
	D3DXATTRIBUTERANGE* attributeTable;
	D3DXMATRIX* finalTransforms;

	vector<string> animList;
	
	static const unsigned int MAX_NUM_BONES_SUPPORTED = 26;

	MyGameSceneNode* rootSceneNode;

	double currentAnimDurationTime;

	bool animLoop;

	string currentAnim;

protected:
		
	SkinnedMesh(void);
	~SkinnedMesh(void);

public:

	void render();
	void render( MyGame3DEffect* _pEffect );
	void loadFromX(MyGameSceneManager* sMgr);
	

	void  getAnimation( vector<string> &animations );

	void frameMove( /*float deltaTime*//*, const D3DXMATRIX& worldViewProj */);

	MyGameSceneNode* getSkeletonRoot(){ return rootSceneNode; }

	void setAnimation( const string &name, bool loop );

	bool ifAnimEnded();

	//bool ifStopped();

	virtual void prepare();
};