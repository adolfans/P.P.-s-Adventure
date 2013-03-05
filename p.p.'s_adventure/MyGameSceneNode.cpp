#include "StdAfx.h"
#include "MyGameSceneNode.h"
#include "MyGameBoneNode.h"
#include "MyGameSceneEntity.h"
#include <exception>
using std::runtime_error;
//应用矩阵的顺序是：先缩放，再旋转，在平移
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
			throw runtime_error("已存在同名的节点:"+this->getName() +"\n无法添加！");
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
			throw runtime_error( "不存在该节点:"+ string(name) );
		else
			return nodeMap[name];
	}else
	{
		throw runtime_error( "节点名不正确！" );
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
		//if( node )//如果这货是个MyGameBoneNode*类型
		//{
		//	//判断是否……
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
