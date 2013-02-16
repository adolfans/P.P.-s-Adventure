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
		if( currentChild == child )
			throw runtime_error( "�������иýڵ㣬�����ظ����룡��" );
		while( currentChild->pSiblingNode )//����������ֵ�
		{
			currentChild = currentChild->pSiblingNode;
			if( currentChild == child )
				throw runtime_error( "�������иýڵ㣬�����ظ����룡��" );
		}//������û�ֵܵ�ʱ��
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
				throw runtime_error( "�ڵ�"+this->nodeName+"û������ӽڵ㣡��" );
		}
	}
	else
	{
		throw runtime_error( "�ڵ�"+this->nodeName+"û������ӽڵ㣡��" );
	}
}