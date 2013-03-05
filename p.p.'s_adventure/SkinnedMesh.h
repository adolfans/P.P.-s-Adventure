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
//#include "MyGameSceneNode.h"
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

/*
	D3DXHANDLE hTech;
	D3DXHANDLE hTexture;
	D3DXHANDLE hWVPMat;
	D3DXHANDLE hFinalMat;
	D3DXHANDLE gMtrl;
	D3DXHANDLE gLight;
*/	
	DWORD NumAttributeGroups;
	D3DXATTRIBUTERANGE* attributeTable;
	D3DXMATRIX* finalTransforms;

	vector<string> animList;

	//vector<D3DXMATRIX> offsetMatrixList;
	
	//MyGame3DEffect* effect;
	
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



struct FrameEx : public D3DXFRAME
{
	//D3DXMATRIX combinedTransform;

	/*static void CombineTransforms( FrameEx* frame, D3DXMATRIX& P )
	{
		D3DXMATRIX& L = frame->TransformationMatrix;
		D3DXMATRIX& C = frame->combinedTransform;
		C = L * P;

		FrameEx* sibling = (FrameEx*)frame->pFrameSibling;
		FrameEx* firstChild = (FrameEx*)frame->pFrameFirstChild;

		if(sibling)
			CombineTransforms( sibling, P );

		if( firstChild )
			CombineTransforms( firstChild, C );
	}*/

	//static void CombineTransforms( D3DXFRAME* frame, D3DXMATRIX& P, map<string, D3DXMATRIX>& jiong )
	//{
	//	D3DXMATRIX& L = frame->TransformationMatrix;
	//	D3DXMATRIX C;
	//	C = L * P;

	//	D3DXFRAME* sibling = (D3DXFRAME*)frame->pFrameSibling;
	//	D3DXFRAME* firstChild = (D3DXFRAME*)frame->pFrameFirstChild;

	//	if( frame->Name && strlen(frame->Name)!=0 )
	//		jiong[frame->Name] = C;
	//	//else
	//	//	//jiong[""]=C;
	//	//	jiong[frame->Name] = C;
	//	if(sibling)
	//		CombineTransforms( sibling, P, jiong );

	//	if( firstChild )
	//		CombineTransforms( firstChild, C, jiong );
	//}
	static void CombineTransforms( D3DXFRAME* frame, D3DXMATRIX& P, map<string, D3DXMATRIX>& jiong )
	{
		MyGameBoneNode* root = static_cast<MyGameBoneNode*>(frame);
		root->ComputeCombinedMatrix( P, jiong );
	}
};