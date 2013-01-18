//////////////////////////
//ע�⣺�ݲ��ṩ����/ɾ�������Ľӿ�
//�ڹ����ڵ������ӵĽڵ㣬ҪôȫΪ�������͵Ľڵ㣬Ҫô�Դ�׼���õĻ����Ժ�����׼����D3DXFRAME������
//�ڴ����У��̳��Ը���ĵ�data memberȫ�ϣ�combinedMatrix����<?>
//////////////////////////
//����ǳ���Ҫ���������һ���ڵ��Ƿǹ����ڵ㣬Ҳ����scenenode������
//////////////////////////

#pragma once
#include "mygamescenenode.h"
#include <map>
using std::map;
class MyGameBoneNode :
	public MyGameScene::MyGameSceneNode,
	public D3DXFRAME
{
private:
	D3DXMATRIX& localMatrix;
	D3DXFRAME*& pSiblingBoneNode;//�ܾ�����ʵ����Ҫ�����ã���Ϊһ������£�����Ĺ�����ι�ϵ�ǲ��ᱻ�ı��
	D3DXFRAME*& pFirstChildBoneNode;
	D3DXMATRIX  offsetMat;
	bool mangdByDXAllocHrc;
public:
	MyGameBoneNode( const char* nodeName, bool _mangdByDXAllocHrc );
	virtual ~MyGameBoneNode(void);

	virtual void SetOffsetMatrix( D3DXMATRIX& offMat )
	{ offsetMat = offMat; }

	virtual void ComputeCombinedMatrix(D3DXMATRIX& parentMat);

	virtual void ComputeCombinedMatrix(D3DXMATRIX& parentMat, map<string, D3DXMATRIX>& matMap );
	
	bool getIfMngdByDXAllocHrc(){ return this->mangdByDXAllocHrc; }
};
