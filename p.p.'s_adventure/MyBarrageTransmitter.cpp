#include "StdAfx.h"
#include "MyBarrageTransmitter.h"
#include "MyGameSceneManager.h"
#include <sstream>
using std::stringstream;
MyBarrageTransmitter::MyBarrageTransmitter(void)
	:textureFileName( "" ),
	width(0.0f),height(0.0f),
	x( 0.0f ), y( 0.0f ), z( 0.0f ),
	num( 0 ),
	lifeTime( 0.0f ),
	billboardSet( 0 )
{
}


MyBarrageTransmitter::~MyBarrageTransmitter(void)
{
	delete billboardSet;
}

void MyBarrageTransmitter::setImage( const string& fileName )
{
	textureFileName = fileName;
}

void MyBarrageTransmitter::setSize( float width, float height )
{
	this->width = width;
	this->height = height;
}

void MyBarrageTransmitter::setPosition( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void MyBarrageTransmitter::setMaxNum( unsigned int num )
{
	this->num = num;
}

void MyBarrageTransmitter::setLifeCircle( float time )
{
	this->lifeTime = time;
}

void MyBarrageTransmitter::generate( MyGameSceneManager* sceneMgr )
{
	sceneManager = sceneMgr;
	billboardSet = new MyGameSceneBillboardSet( num, textureFileName );
	MyGameSceneNode* root =	sceneMgr->getRootNode();
	
	MyGameSceneNode* rotateNode = sceneMgr->CreateSceneNode( "testTransmitterRotateNode" );
	root->AddChild( rotateNode );
	rotateNode->setPosition( x, y, z );
	transmitterRoot = rotateNode;
	for( unsigned int i = 0; i != num; ++ i )
	{
		stringstream ss;
		ss<<"barrageNode"<<i;
		MyGameSceneNode* barrageNode = sceneMgr->CreateSceneNode( ss.str().c_str() );
		barrageNode->setPosition( 2000.0f* cosf( (float)i * 2 * D3DX_PI / (float)num ),
									2000.0f ,
									2000.0f* sinf( (float)i * 2 * D3DX_PI / (float)num )
									);
		barrageNode->scale( width, height, 1.0f );
		rotateNode->AddChild( barrageNode );
		billboardSet->addEntity( barrageNode );
		nodeList.push_back( barrageNode );
	}
}

void MyBarrageTransmitter::draw( float elapsedTime )
{
	int i = 0;
	for( auto itr = nodeList.begin();
		itr != nodeList.end();
		++ itr )
	{
		(*itr)->move( cosf( (float)i * 2 * D3DX_PI / (float)num ) * elapsedTime *200 ,
								0.0f,//2000.0f ,
								sinf( (float)i * 2 * D3DX_PI / (float)num ) * elapsedTime *200
								);
		++i;
	}
	transmitterRoot->rotateY( D3DX_PI * 0.05 * elapsedTime );
	billboardSet->draw( sceneManager );
}