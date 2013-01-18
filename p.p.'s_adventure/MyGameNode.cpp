#include "StdAfx.h"
#include "MyGameNode.h"
#include <exception>
using std::runtime_error;

MyGameNode::MyGameNode( const char* name ):
		pSiblingNode(0), pFirstChildNode(0)
{
	if( !name ) nodeName = "";else nodeName = name;
}


MyGameNode::~MyGameNode(void)
{
	/*
	if( this->pFirstChildNode )
		delete pFirstChildNode;
	if( this->pSiblingNode )
		delete pSiblingNode;
		*/
}

void MyGameNode::AddChild( MyGameNode* child )
{
	if(! pFirstChildNode )
		pFirstChildNode = child;
	else
	{
		MyGameNode* currentChild = pFirstChildNode;
		while( currentChild->pSiblingNode )//如果后面有兄弟
		{
			currentChild = currentChild->pSiblingNode;
		}//当后面没兄弟的时候
		currentChild->pSiblingNode = child;
	}
}

void MyGameNode::BreakOutChildLink( MyGameNode* child )
{
	if( this->pFirstChildNode )
	{
		if( child == pFirstChildNode )
			pFirstChildNode = 0;
		else
		{
			MyGameNode* pChild = pFirstChildNode;
			while( pChild->pSiblingNode )
			{
				if( pChild->pSiblingNode == child )
					pChild->pSiblingNode = 0;
				else
					pChild = pChild->pSiblingNode;
			}
			if( pChild->pSiblingNode == 0 )				
				throw runtime_error( "节点"+this->nodeName+"没有这个子节点！！" );
		}
	}
	else
	{
		throw runtime_error( "节点"+this->nodeName+"没有这个子节点！！" );
	}
}