#include "StdAfx.h"
#include "SkinnedMesh.h"
#include "MyGame3DDevice.h"
#include "MyGameSceneManager.h"
#include <time.h>
#include <stack>
using std::stack;

#include <exception>
using std::runtime_error;

SkinnedMesh::SkinnedMesh(void)
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	
	AllocMeshHierarchy allocMeshHierarchy;

	HR( D3DXLoadMeshHierarchyFromX( L"testLoliske.X", 
								D3DXMESH_MANAGED,
								pDevice,
								&allocMeshHierarchy,
								0,
								&pHierarchyRoot,
								&pAnimCtrller ) );


	HR( D3DXCreateEffectFromFile( pDevice, L"newMesh.fx", 0, 0, D3DXSHADER_DEBUG|D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_IEEE_STRICTNESS|D3DXSHADER_NO_PRESHADER|D3DXSHADER_SKIPVALIDATION , 0, &meshEffect, 0 ) );
	hTech = meshEffect->GetTechniqueByName( "main" );
	hTexture = meshEffect->GetParameterByName( 0, "Tex" );					
	hWVPMat = meshEffect->GetParameterByName( 0, "WorldViewProj" );
	hFinalMat = meshEffect->GetParameterByName( 0, "FinalTransforms" );

	//gMtrl = meshEffect->GetParameterByName( 0, "gMtrl" );
	//gLight = meshEffect->GetParameterByName( 0, "gLight" );

	
	if( pHierarchyRoot )
	{
		//遍历树。目前已经毫无意义了	
		//stack< D3DXFRAME* > hiStack;
		//while( true )
		//{
		//	for( ; node; node = node->pFrameFirstChild )
		//	{
		//		//::MessageBoxA( 0, node->Name, 0, 0 );
		//		hiStack.push( node );
		//	}
		//	if( !hiStack.size() )
		//		break;
		//	node = hiStack.top();
		//	hiStack.pop();
		//	node = node->pFrameSibling;
		//}
	}

	this->rootSceneNode = static_cast<MyGameBoneNode*>(pHierarchyRoot);
}

SkinnedMesh::~SkinnedMesh(void)
{
	if( pHierarchyRoot )
	{
		AllocMeshHierarchy allocMeshHierarchy;
		HR( D3DXFrameDestroy( pHierarchyRoot, &allocMeshHierarchy) );
		pHierarchyRoot = 0;
	}
	
	IRelease(pSkinnedMesh);
	IRelease(pAnimCtrller);
	IRelease(vDecl);
	for( vector<IDirect3DTexture9*>::iterator _itr = texList.begin(); 
		_itr != texList.end(); ++ _itr )
	{
		IRelease( *_itr );
	}
	IRelease(meshEffect);
	IRelease(pSkinInfo);

	delete[] finalTransforms;
	delete[] attributeTable;
}

D3DXFRAME* SkinnedMesh::findNodeWithMesh( D3DXFRAME	* frame )
{
	if( frame->pMeshContainer )
		if( frame ->pMeshContainer->MeshData.pMesh != 0 )
			return frame;

	D3DXFRAME* f = 0;
	if( frame->pFrameSibling )
		if( f = findNodeWithMesh(frame->pFrameSibling) )
			return f;  

	if( frame->pFrameFirstChild )
		if( f = findNodeWithMesh( frame->pFrameFirstChild ) )
			return f;

	return 0;
}
 
void SkinnedMesh::prepare()
{
	
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	D3DXFRAME* f = findNodeWithMesh( this->pHierarchyRoot );
	if( f==0 ) HR(E_FAIL);
	D3DXMESHCONTAINER* meshContainer = f->pMeshContainer;
	//D3DXMESHCONTAINER* meshContainer = pHierarchyRoot->pMeshContainer;
	pSkinInfo = meshContainer->pSkinInfo;
	pSkinInfo->AddRef();

	numBones = pSkinInfo->GetNumBones();
	
	//pSkinInfo获取offset Matrix
	for( unsigned int i = 0; i < numBones; ++ i )
	{
		const char* boneName = pSkinInfo->GetBoneName( i );
		if( boneName == 0 )
			::MessageBoxA( 0, "cuo", 0, 0 );
		else
		{
			MyGameBoneNode* bone = dynamic_cast<MyGameBoneNode*>(MyGameSceneNode::getNodeByName(boneName) );
			if( !bone )
				throw runtime_error( "不能转化为MyGameBoneNode*类型~" );

			bone->SetOffsetMatrix( *pSkinInfo->GetBoneOffsetMatrix(i) );
		}
	}


	D3DVERTEXELEMENT9 verElmt[MAX_FVF_DECL_SIZE];
	pSkinInfo->GetDeclaration( verElmt );
	pDevice->CreateVertexDeclaration( verElmt, &vDecl );

	for( unsigned int i = 0; i < meshContainer->NumMaterials; ++ i )
	{
		IDirect3DTexture9* pTex = 0;
		D3DXCreateTextureFromFileA( pDevice, meshContainer->pMaterials[i].pTextureFilename, &pTex );
		texList.push_back( pTex );
	}

	ID3DXMesh* pMesh = meshContainer->MeshData.pMesh;

	this->buildSkinnedMesh( pMesh );
	
	

	finalTransforms = new D3DXMATRIX[numBones];
	this->buildCombinedTransforms();

	this->GetAnimation( this->animList );
	
	lastTime = clock();

}

void SkinnedMesh::GetAnimation( vector<string> &animations )
{
	ID3DXAnimationSet *anim = 0;
	
	for( int i = 0; i < this->pAnimCtrller->GetMaxNumAnimationSets(); ++ i )
	{
		anim = 0;
		pAnimCtrller->GetAnimationSet( i, &anim );
		if( anim )
		{
			animations.push_back( anim->GetName() );
			pAnimCtrller->SetTrackAnimationSet(0, anim);
			anim->Release();
		}
	}

	//this->pAnimCtrller->GetAnimationSet(0, &anim);
			
}



void SkinnedMesh::buildSkinnedMesh( ID3DXMesh* mesh )
{
	ID3DXBuffer* boneComboTable = 0;
	pSkinnedMesh = 0;
	maxVertInfluences= 0;

	mesh->AddRef();
	HR( pSkinInfo->ConvertToIndexedBlendedMesh( 
				mesh,
				D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
				SkinnedMesh::MAX_NUM_BONES_SUPPORTED,
				0,
				0,
				0,
				0,
				&maxVertInfluences,
				&numBoneComboEntries,
				&boneComboTable,
				&pSkinnedMesh ) );

	
	/*
	HR( pSkinInfo->ConvertToBlendedMesh(
					mesh,
					D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
	*/
	IRelease(boneComboTable );

	IRelease( mesh );
	
	//Get Attribute Table
	pSkinnedMesh->GetAttributeTable(NULL, &NumAttributeGroups);
	attributeTable = new D3DXATTRIBUTERANGE[NumAttributeGroups];
	pSkinnedMesh->GetAttributeTable(attributeTable, NULL);


}

void SkinnedMesh::buildCombinedTransforms()
{
	D3DXMATRIX idMat;
	D3DXMatrixIdentity( &idMat );
	map<string, D3DXMATRIX> jionga;
	//FrameEx::CombineTransforms( pHierarchyRoot, idMat, jionga);
	//TODO :
	//this->rootSceneNode->ComputeCombinedMatrix(idMat);
	//this->rootSceneNode = static_cast<MyGameBoneNode*>(pHierarchyRoot);
	//rootSceneNode->ComputeCombinedMatrix(idMat);
	//把jionga中的东西保存到combinedTransforms中去？
	combinedTransforms.clear();
	for( unsigned int i = 0; i < numBones; ++ i )
	{
		const char* boneName = pSkinInfo->GetBoneName( i );
		if( MyGameSceneNode::getNodeByName(boneName) == 0 )
			::MessageBoxA( 0, "cuo", 0, 0 );
		else
			//this->combinedTransforms.push_back( jionga[boneName] );
			this->combinedTransforms.push_back( MyGameSceneNode::getNodeByName(boneName)->getCombinedMatrix() );
	}
}

void SkinnedMesh::render()
{
	this->frameMove();
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	
	pDevice->SetVertexDeclaration(this->vDecl);

	meshEffect->SetTechnique(hTech);

	unsigned int numPasses = 0;

	meshEffect->Begin( &numPasses, 0 );

	//pSkinnedMesh->GetAttributeTable( 0, &NumAttributeGroups );

	for( unsigned int i = 0; i < numPasses; ++i )
	{
		meshEffect->BeginPass(i);
		
		unsigned int j = 0;
		for( vector<IDirect3DTexture9*>::iterator _itr = texList.begin();
			_itr != texList.end();
			 ++ _itr )
		{
			meshEffect->SetTexture( this->hTexture, *_itr );
			for( int k = 0; k < NumAttributeGroups; ++ k )
			{
				int mtrlIndex = attributeTable[k].AttribId;
				
				pSkinnedMesh->DrawSubset( mtrlIndex );
			}
			++j;
		}

		meshEffect->EndPass();
	}

	meshEffect->End();
}

void SkinnedMesh::frameMove( /*float deltaTime*/ /*,
							const D3DXMATRIX& worldViewProj*/ )
{
	float timeDelta = (clock() - lastTime)/1000.0f;
	lastTime = clock();

	this->pAnimCtrller->AdvanceTime( timeDelta, 0 );

	D3DXMATRIX worldViewProj = MyGameSceneManager::getViewProjCombinedMat();

	//buildCombinedTransforms();

	for( unsigned int i = 0; i < numBones; ++ i )
	{
		const char* boneName = pSkinInfo->GetBoneName( i );
		D3DXMATRIX& combinedTemp = combinedTransforms[i];
		finalTransforms[i] =MyGameSceneNode::getNodeByName(boneName)->getCombinedMatrix();
	}

	meshEffect->SetMatrix( hWVPMat, &worldViewProj );

	meshEffect->SetMatrixArray( hFinalMat, &finalTransforms[0], numBones );
}