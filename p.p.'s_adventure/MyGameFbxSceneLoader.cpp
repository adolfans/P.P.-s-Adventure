#include "StdAfx.h"
#include "MyGameFbxSceneLoader.h"
#include <sstream>
using std::stringstream;

MyGameFbxSceneLoader::MyGameFbxSceneLoader(void)
{
}


MyGameFbxSceneLoader::~MyGameFbxSceneLoader(void)
{
}

void MyGameFbxSceneLoader::getNodeFromFbxNode( MyGameScene::MyGameSceneManager* sceneMgr, FbxNode* node, MyGameSceneNode* parentSceneNode, vector<MyGameSceneEntity*>& entityList )
{
	MyGameSceneNode* snode = sceneMgr->CreateSceneNode( node->GetName() );
	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();
	snode->setPosition( translation.mData[0], translation.mData[1], translation.mData[2] );
	snode->rotateX( rotation.mData[0] );
	snode->rotateY( rotation.mData[1] );
	snode->rotateZ( rotation.mData[2] );
	snode->scale( scaling.mData[0], scaling.mData[1], scaling.mData[2] );

	parentSceneNode->AddChild( snode );
	
	for( int i = 0; i != node->GetNodeAttributeCount(); ++ i )
	{

		if( node->GetNodeAttributeByIndex( i )->GetAttributeType() == FbxNodeAttribute::eMesh )
		{
				//如果这个节点的attribute是mesh的话就创建mesh和entity，并且把它绑定到节点
			//TODO: 有可能创建重复的mesh
			MyGameMesh* mesh = MyGameMeshManager::createMyGameMesh( MyGameMeshManager::MESH );
			mesh->loadMeshFromFbxNodeAttribute( node->GetNodeAttributeByIndex( i ) );
			stringstream ss;
			ss<< node->GetName() << i;			
			MyGameSceneEntity* ent = sceneMgr->CreateSceneEntity( mesh, ss.str().c_str() );
			snode->attachEntity( ent );
			entityList.push_back( ent );
			break;
		}
	}
	for( int i = 0; i != node->GetChildCount(); ++ i )
	{
		getNodeFromFbxNode( sceneMgr, node->GetChild( i ), snode, entityList);
	}
}


void MyGameFbxSceneLoader::loadSceneFromFbx( MyGameScene::MyGameSceneManager* sceneMgr, const string& fileName, MyGameSceneNode* node, vector<MyGameSceneEntity*>& entityList )
{
	FbxManager* fbxMgr= FbxManager::Create();
	FbxIOSettings* fbxIO = FbxIOSettings::Create( fbxMgr, IOSROOT );
	fbxMgr->SetIOSettings( fbxIO );

	FbxScene* fbxScene = FbxScene::Create( fbxMgr, "test" );
	FbxImporter* importer = FbxImporter::Create( fbxMgr, "" );
	importer->Initialize( fileName.c_str(), -1, fbxMgr->GetIOSettings() );
	importer->Import( fbxScene );
	fbxIO->Destroy();

	FbxNode* rootNode = fbxScene->GetRootNode();

	FbxGeometryConverter converter( fbxMgr );
	converter.TriangulateInPlace( rootNode );


	getNodeFromFbxNode( sceneMgr, rootNode, node, entityList );

	rootNode->Destroy();


	fbxScene->Destroy();
	fbxMgr->Destroy();
}