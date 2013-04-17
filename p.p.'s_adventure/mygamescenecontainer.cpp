#include "StdAfx.h"
#include "mygamescenecontainer.h"

#include "MyGameSceneEntity.h"
#include "mygamescenenode.h"
#include "MyGameSceneManager.h"

#include <stack>
#include <sstream>
#include <exception>

using std::runtime_error;
using std::stringstream;
using std::stack;

MyGame3DEffect*     MyGameSceneContainer::defaultEffect = NULL;
const string        MyGameSceneContainer::defaultEffectName = ("default" );

MyGameSceneContainer::MyGameSceneContainer( MyGameSceneManager* mgr, const string& sceneName )
    :clonedCount(0)
{
    this->effectMap[defaultEffectName] = defaultEffect;

    this->rootNode = mgr->CreateSceneNode( sceneName.c_str() );
}

MyGameSceneContainer::~MyGameSceneContainer()
{
}


void MyGameSceneContainer::loadDefaultEffect(const string &fileName)
{
    defaultEffect = new MyGame3DEffect( fileName.c_str() );
}

void MyGameSceneContainer::setDefaultEffect(MyGame3DEffect *effect)
{
    defaultEffect = effect;
}

void MyGameSceneContainer::cloneNodesWithEntities(MyGameSceneManager *mgr,
                            vector<MyGameSceneEntity *> &entityVec,
                            MyGameNode *originNode,
                            MyGameNode *newParentNode,
                            unsigned int num )
{
    stringstream ss;
    ss << originNode->getName() << "_instance_" << num;
    MyGameSceneNode* newNode = static_cast<MyGameSceneNode*>(mgr->CreateSceneNode(ss.str().c_str()));
    MyGameSceneNode* originalSceneNode = static_cast<MyGameSceneNode*>(originNode);
    float x,y,z;
    originalSceneNode->getPosition( x, y, z );
    newNode->setPosition( x, y, z );
    originalSceneNode->getRotation( x, y, z );
    newNode->setRotationAngleY( y );
    originalSceneNode->getScale( x, y, z );
    newNode->setScale( x, y, z );
    newParentNode->AddChild(newNode);

    for( map<  MyGameSceneEntity*, MyGame3DEffect* >::iterator itr = this->entityMap.begin();
         itr != this->entityMap.end();
         ++ itr )
    {
        if( originNode == itr->first->getNode() )
        {
            MyGameSceneEntity* newEnt = mgr->CreateSceneEntity( itr->first->getMesh(), static_cast<MyGameSceneNode*>(newNode) );
            //defaultEffect->AddEntity( newEnt );
            entityVec.push_back( newEnt );
            itr->second->AddEntity( newEnt );
        }
    }

    if( originNode->getFirstChild() )
        cloneNodesWithEntities(mgr, entityVec, originNode->getFirstChild(), newNode, num );
    if( originNode->getSiblingNode() )
        cloneNodesWithEntities(mgr, entityVec, originNode->getSiblingNode(), newParentNode, num );
}

MyGameNode *MyGameSceneContainer::clone(MyGameSceneManager *mgr, vector<MyGameSceneEntity *> &entityVec)
{
    stringstream ss;
    ss << this->rootNode->getName() << "_instance_" << clonedCount;
    MyGameSceneNode* newNode = static_cast<MyGameSceneNode*>(mgr->CreateSceneNode(ss.str().c_str()));
    MyGameSceneNode* originalSceneNode = static_cast<MyGameSceneNode*>(rootNode);
    float x = 0.0f, y = 0.0f, z = 0.0f;
    originalSceneNode->getPosition( x, y, z );
    newNode->setPosition( x, y, z );
    originalSceneNode->getRotation( x, y, z );
    newNode->setRotationAngleY( y );
    originalSceneNode->getScale( x, y, z );
    newNode->setScale( x, y, z );

    if( rootNode->getFirstChild() )
        cloneNodesWithEntities(mgr, entityVec, rootNode->getFirstChild(), newNode, clonedCount );

    clonedCount++;

    return newNode;
}

MyGameSceneNode* MyGameSceneContainer::getRootNode()
{
    return this->rootNode;
}

void MyGameSceneContainer::addEffect( const string& name, MyGame3DEffect *effect)
{
    this->effectMap[name] = effect;
}

void MyGameSceneContainer::addEntity( MyGameSceneEntity *entity, const string effectName )
{
    if( !this->effectMap.count( effectName ) )
        throw runtime_error( "Cannot find effect:" + effectName );
    this->entityMap[entity] = effectMap[effectName];
    effectMap[effectName]->AddEntity( entity );
    entity->setVisible( false );
}

void MyGameSceneContainer::setVisible( bool visible )
{
    for( map< MyGameSceneEntity*, MyGame3DEffect* >::iterator itr = this->entityMap.begin();
         itr != entityMap.end();
         ++ itr )
    {
        itr->first->setVisible( visible );
    }
}
