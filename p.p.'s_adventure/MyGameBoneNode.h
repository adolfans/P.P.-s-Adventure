//////////////////////////
//注意：暂不提供增加/删除骨骼的接口
//在骨骼节点上增加的节点，要么全为其他类型的节点，要么自带准备好的或者以后自行准备的D3DXFRAME的内容
//在此类中，继承自父类的的data member全废，combinedMatrix除外<?>
//////////////////////////
//这里非常需要考虑如果下一级节点是非骨骼节点，也就是scenenode的问题
//////////////////////////

#pragma once
#include <map>
using std::map;

#include "mygamescenenode.h"

class MyGameBoneNode :
	public MyGameScene::MyGameSceneNode,
	public D3DXFRAME
{
	friend class MyGameScene::MyGameSceneManager; 
private:
	D3DXMATRIX& localMatrix;
	D3DXFRAME*& pSiblingBoneNode;//总觉得其实不必要是引用，因为一般情况下，这里的骨骼层次关系是不会被改变的
	D3DXFRAME*& pFirstChildBoneNode;
	D3DXMATRIX  offsetMat;
	bool mangdByDXAllocHrc;
protected:
	MyGameBoneNode( const char* nodeName, bool _mangdByDXAllocHrc );
	virtual ~MyGameBoneNode(void);
public:

	virtual void setOffsetMatrix( D3DXMATRIX& offMat )
	{ offsetMat = offMat; }

	virtual void ComputeCombinedMatrix(D3DXMATRIX& parentMat);

	bool getIfMngdByDXAllocHrc(){ return this->mangdByDXAllocHrc; }
};

