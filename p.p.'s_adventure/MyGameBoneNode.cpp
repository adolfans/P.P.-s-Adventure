#include "StdAfx.h"
#include "MyGameBoneNode.h"

#include <exception>
using std::bad_cast;

MyGameBoneNode::MyGameBoneNode(const char* nodeName, bool _mangdByDXAllocHrc)
	:MyGameSceneNode( nodeName ),
	localMatrix( TransformationMatrix ),
	pFirstChildBoneNode( (pFrameFirstChild) ),
	pSiblingBoneNode( pFrameSibling),
	mangdByDXAllocHrc( _mangdByDXAllocHrc )
{
	pFrameFirstChild = 0;
	pFrameSibling = 0;
	//memset( &TransformationMatrix, 0, sizeof( D3DXMATRIX ));
	D3DXMatrixIdentity( &TransformationMatrix );
	D3DXMatrixIdentity( &offsetMat );
	if(nodeName)
	{
		unsigned int len = strlen(nodeName);
		Name = new char[len+1];
		//strcpy( Name, nodeName );
		memcpy( Name, nodeName, len+1 );
	}else
	{
		this->Name = 0;
	}
	this->pMeshContainer = 0;
}


MyGameBoneNode::~MyGameBoneNode(void)
{
	//if( mangdByDXAllocHrc )
	//统一由MyGameSceneNode销毁
	{
		if( Name != 0 && strlen(Name) >= 0 )
			delete[] this->Name;
	}
}

void MyGameBoneNode::ComputeCombinedMatrix( D3DXMATRIX& parentMat )
{
	
	//MyGameSceneNode::ComputeCombinedMatrix( parentMat );

	this->SetCombinedMatrix( localMatrix * parentMat );

	if( this->pFirstChildBoneNode )
	{
		MyGameBoneNode* my = static_cast<MyGameBoneNode*>(pFirstChildBoneNode);
		my->ComputeCombinedMatrix( this->getCombinedMatrix() );
	}
	if( this->pSiblingBoneNode )
	{
		MyGameBoneNode* my = static_cast<MyGameBoneNode*>(pSiblingBoneNode);
		my->ComputeCombinedMatrix( parentMat );
	}
	//如果子节点是个scene node节点或者有作为scene node的子节点
	//萌大奶！！
	if( this->getFirstChild() )
	{
		this->getFirstChild()->ComputeCombinedMatrix( this->getCombinedMatrix() );
	}
	if( this->getSiblingNode() )
		this->getSiblingNode()->ComputeCombinedMatrix( parentMat );

	this->SetCombinedMatrix( this->offsetMat * localMatrix * parentMat );


}

void MyGameBoneNode::ComputeCombinedMatrix( D3DXMATRIX& parentMat, map<string, D3DXMATRIX>& matMap )//已废，功能上也已废
{
	
	MyGameSceneNode::ComputeCombinedMatrix( parentMat );

	this->SetCombinedMatrix( localMatrix * parentMat );

	
	//if( this->Name && strlen(this->Name)!=0 )
		//matMap[this->Name] = this->getCombinedMatrix();
	if( this->Name && strlen(this->Name)!=0 )
		matMap[this->Name] = MyGameSceneNode::getNodeByName( this->Name )->getCombinedMatrix();

	try{
	if( this->pFirstChildBoneNode )
	{
		MyGameBoneNode* my = static_cast<MyGameBoneNode*>(pFirstChildBoneNode);
		my->ComputeCombinedMatrix( this->getCombinedMatrix(), matMap );
	}
	if( this->pSiblingBoneNode )
	{
		MyGameBoneNode* my = static_cast<MyGameBoneNode*>(pSiblingBoneNode);
		my->ComputeCombinedMatrix( parentMat, matMap );
	}
	}catch( bad_cast &e){
		throw bad_cast( "bad cast: 从D3DXFRAME*到MyGameBoneNode*的转换失败" );
	}
	//如果子节点是个scene node节点或者有作为scene node的子节点
/*	if( this->getFirstChild() )
	{
		this->getFirstChild()->ComputeCombinedMatrix( this->getCombinedMatrix() );
	}
	if( this->getSiblingNode() )
		this->getSiblingNode()->ComputeCombinedMatrix( parentMat );
*/
	this->SetCombinedMatrix( this->offsetMat * localMatrix * parentMat );

}