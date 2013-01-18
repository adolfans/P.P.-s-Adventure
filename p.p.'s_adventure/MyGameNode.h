#pragma once
#include <string>
using std::string;
#include <d3dx9.h>
//�Ƿ���Ҫʵ��һ����������
class MyGameNode
{
private:
	string nodeName;
	MyGameNode* pSiblingNode;
	MyGameNode* pFirstChildNode;
public:
	MyGameNode( const char* name );
	virtual ~MyGameNode(void);

	virtual MyGameNode* getSiblingNode(){ return pSiblingNode;}
	virtual MyGameNode* getFirstChild(){ return pFirstChildNode; }

	virtual void AddChild( MyGameNode* child );
	virtual void BreakOutChildLink( MyGameNode* child );
	virtual void ComputeCombinedMatrix(D3DXMATRIX& parentMat) = 0;

	virtual string getName(){ return nodeName; }
};

