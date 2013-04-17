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

void MyGameBoneNode::ComputeCombinedMatrix( const D3DXMATRIX& parentMat )
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
