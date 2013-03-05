#include "StdAfx.h"
#include "MyGameSceneNode.h"
#include "MyGameBoneNode.h"
#include "MyGameSceneEntity.h"
#include <exception>
using std::runtime_error;
//Ӧ�þ����˳���ǣ������ţ�����ת����ƽ��
namespace MyGameScene{
	map<string, MyGameSceneNode*> MyGameSceneNode::nodeMap;

MyGameSceneNode::MyGameSceneNode(const char* name):
MyGameNode( name )
{
	D3DXMatrixIdentity( &localMatrix );
	D3DXMatrixIdentity( &combinedMatrix );
	D3DXMatrixIdentity( &rotateMat );
	D3DXMatrixIdentity( &scaleMat );
	D3DXMatrixIdentity( &posMat );
	if( name && strlen(name) >= 1 )
	{
		if( nodeMap.count( name ) )
			throw runtime_error("�Ѵ���ͬ���Ľڵ�:"+this->getName() +"\n�޷���ӣ�");
		else
			nodeMap[name] = this;
	}
}


MyGameSceneNode::~MyGameSceneNode(void)
{
}

void MyGameSceneNode::ComputeCombinedMatrix(D3DXMATRIX& parentMat)
{
	this->combinedMatrix = this->localMatrix * parentMat;

	if( this->getFirstChild() )
		this->getFirstChild()->ComputeCombinedMatrix( combinedMatrix );

	if( this->getSiblingNode() )
		this->getSiblingNode()->ComputeCombinedMatrix( parentMat );
}

MyGameSceneNode* MyGameSceneNode::getNodeByName( const char* name )
{
	if( name && strlen( name ) > 1 )
	{
		if( !nodeMap.count( name ) )
			throw runtime_error( "�����ڸýڵ�:"+ string(name) );
		else
			return nodeMap[name];
	}else
	{
		throw runtime_error( "�ڵ�������ȷ��" );
	}
}


void MyGameSceneNode::move( float x, float y, float z )
{
	D3DXMATRIX nosMat;
	D3DXMatrixTranslation( &nosMat, x, y, z );
	posMat*=nosMat;
	localMatrix = scaleMat*rotateMat*posMat;
}

void MyGameSceneNode::scale( float x, float y, float z )
{
	D3DXMATRIX nsMat;
	D3DXMatrixScaling( &nsMat, x, y, z );
	scaleMat*=nsMat;
	localMatrix = scaleMat*rotateMat*posMat;
}

void MyGameSceneNode::rotateX( float angle )
{
	D3DXMATRIX nscMat;
	D3DXMatrixRotationX( &nscMat, angle );
	rotateMat*=nscMat;
	localMatrix = scaleMat*rotateMat*posMat;
}
void MyGameSceneNode::rotateY( float angle )
{
	D3DXMATRIX nscMat;
	D3DXMatrixRotationY( &nscMat, angle );
	rotateMat*=nscMat;
	localMatrix = scaleMat*rotateMat*posMat;
}
void MyGameSceneNode::rotateZ( float angle )
{
	D3DXMATRIX nscMat;
	D3DXMatrixRotationZ( &nscMat, angle );
	rotateMat*=nscMat;
	localMatrix = scaleMat*rotateMat*posMat;
}

void MyGameSceneNode::setRotateMatrix( const D3DXMATRIX& rotatemat )
{
	rotateMat = rotatemat;
	localMatrix = scaleMat*rotateMat*posMat;
}

void MyGameSceneNode::setRotationAngleY( float angle )
{
	D3DXMATRIX nscMat;
	D3DXMatrixRotationY( &nscMat, angle );
	rotateMat = nscMat;
	localMatrix = scaleMat*rotateMat*posMat;
}

void MyGameSceneNode::destroyAllNodes( )
{
	for( map<string, MyGameSceneNode*>::iterator _itr = nodeMap.begin();
		_itr != nodeMap.end();
		++ _itr )
	{
		MyGameSceneNode* pToDel = _itr->second;
		//MyGameBoneNode* node = dynamic_cast<MyGameBoneNode*>(pToDel);
		//if( node )//�������Ǹ�MyGameBoneNode*����
		//{
		//	//�ж��Ƿ񡭡�
		//	if( node->getIfMngdByDXAllocHrc() ){}
		//	else
		//		delete node;
		//}else
			delete pToDel;
	}
}
	void MyGameSceneNode::attachEntity( MyGameSceneEntity* _ent )
	{
		this->AddChild(_ent->getNode());
	}

	void MyGameSceneNode::setPosition( float x, float y, float z )
	{
		D3DXMatrixTranslation( &posMat, x, y, z );
		
		localMatrix = scaleMat*rotateMat*posMat;
	}
}
