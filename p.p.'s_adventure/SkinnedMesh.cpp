#include "StdAfx.h"
#include "SkinnedMesh.h"
#include "MyGame3DDevice.h"
#include "MyGameSceneManager.h"
#include <time.h>
#include <stack>
using std::stack;
#include <exception>
using std::runtime_error;
//using std::e;
#include "MyGame3DEffect.h"
#include "MyGameSceneManager.h"
class AllocMeshHierarchy : public ID3DXAllocateHierarchy
	{
	private:
		MyGameScene::MyGameSceneManager* sceneManager;
	public:
		AllocMeshHierarchy( MyGameSceneManager* sMgr ): sceneManager(sMgr){}
		HRESULT STDMETHODCALLTYPE CreateFrame( THIS_ PCSTR Name, D3DXFRAME** ppNewFrame )
		{
			//*ppNewFrame = new D3DXFRAME;
			//memset( *ppNewFrame, 0, sizeof( D3DXFRAME ) );
			//if(Name)
			//{
			//	unsigned int len = strlen(Name);
			//	(*ppNewFrame)->Name = new char[len+1];
			//	strcpy( (*ppNewFrame)->Name, Name );
			//}
			//const char* myName = 0;
			//if( strlen( Name ) <1 )
			//	myName = 0;
			//else
			//	myName = Name;
			//MyGameBoneNode* myNode = new MyGameBoneNode( Name, true );
			MyGameBoneNode* myNode = this->sceneManager->CreateBoneNode( Name, true );
			//if(Name)
			//{
			//	unsigned int len = strlen(Name);
			//	myNode->Name = new char[len+1];
			//	strcpy( myNode->Name, Name );
			//}else
			//{
			//	myNode->Name = 0;
			//}
			*ppNewFrame = myNode;

			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE CreateMeshContainer( PCSTR Name,
									const D3DXMESHDATA* pMeshData,
									const D3DXMATERIAL* pMaterials,
									const D3DXEFFECTINSTANCE* pEffectInstances,
									DWORD NumMaterials,
									const DWORD *pAjacency,
									ID3DXSkinInfo* pSkinInfo,
									D3DXMESHCONTAINER** ppNewMeshContainer )
		{
			//*ppNewMeshContainer = new D3DXMESHCONTAINER_EX;
			//memset( *ppNewMeshContainer, 0, sizeof(D3DXMESHCONTAINER_EX) );
			
			D3DXMESHCONTAINER_EX* ctn = new D3DXMESHCONTAINER_EX;
			memset( ctn, 0, sizeof(D3DXMESHCONTAINER_EX) );
			ctn->NumMaterials = NumMaterials;
			if(pMeshData)
			{
				ctn->MeshData = *pMeshData;
				pMeshData->pMesh->AddRef();
			}
			if(Name)
			{
				unsigned int len = strlen( Name );
				char* name = new char[len+1];
				strcpy( name, Name );
				ctn->Name = name;
			}
			if(pMaterials)
			{
				ctn->pMaterials = new D3DXMATERIAL[NumMaterials];
				for( unsigned int i = 0; i != NumMaterials; ++ i )
				{
					ctn->pMaterials[i] = pMaterials[i];
					unsigned int len = strlen( pMaterials[i].pTextureFilename );
					ctn->pMaterials[i].pTextureFilename = new char[len+1];
					strcpy( ctn->pMaterials[i].pTextureFilename, pMaterials[i].pTextureFilename ); 
				}
			}
			if( pEffectInstances )
			{
				ctn->pEffects = new D3DXEFFECTINSTANCE;
				*(ctn->pEffects) = *pEffectInstances;
			}
			if( pAjacency )
			{
				ctn->pAdjacency = new DWORD[3];
				for( unsigned int i = 0; i != 3; ++ i )
					ctn->pAdjacency[i] = pAjacency[i];
			}
			if( pSkinInfo )
			{
				ctn->pSkinInfo = pSkinInfo;
				ctn->pSkinInfo->AddRef();
				//ID3DXBuffer* boneComboTable = 0;
				//UINT MaxMatrices = 26;
				////ctn->NumPaletteEntries = min( MaxMatrices, pSkinInfo->GetNumBones() );
				//
				//HR( pSkinInfo->ConvertToIndexedBlendedMesh(
				//	pMeshData->pMesh,
				//	D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
				//	ctn->NumPaletteEntries,
				//	//MAX_NUM_BONES_SUPPORTED,
				//	//pMeshContainer->pAdjacency,
				//	NULL,
				//	NULL, NULL, NULL,
				//	&ctn->NumInfl,
				//	&ctn->NumAttributeGroups,
				//	&boneComboTable,
				//	&ctn->MeshData.pMesh ));
				//IRelease(boneComboTable);
				/*
				//Get Attribute Table
				DWORD NumAttributeGroups;
				ctn->MeshData.pMesh->GetAttributeTable(NULL, &NumAttributeGroups);
				rand();
				*/
			}

			*ppNewMeshContainer = ctn;
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE DestroyFrame(
								THIS_ D3DXFRAME* pFrameToFree)
		{
			//此处不作释放操作，改为由MyGameSceneNode作释放操作
			//MyGameBoneNode* pFrame= static_cast<MyGameBoneNode*>(pFrameToFree);
			//delete pFrame;
			//由于MyGameSceneNode所存储的node不包括name为0，或者为空字符串的，所以
			//此处仅释放那些节点
			if( !pFrameToFree->Name ||  strlen(pFrameToFree->Name) < 1 )
			{	MyGameBoneNode* pFrame= static_cast<MyGameBoneNode*>(pFrameToFree);
				//delete pFrame;
				this->sceneManager->destroy( pFrame );
			}
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE DestroyMeshContainer(
								THIS_ D3DXMESHCONTAINER* pMeshContainerBase )
		{
			if(pMeshContainerBase->Name)
				delete[] pMeshContainerBase->Name;
			if(pMeshContainerBase->pMaterials)
			{
				for( int i = 0; i != pMeshContainerBase->NumMaterials; ++ i )
				{
					delete[] pMeshContainerBase->pMaterials[i].pTextureFilename;
				}
				delete[] pMeshContainerBase->pMaterials;
			}
			if( pMeshContainerBase->pEffects )
				delete pMeshContainerBase->pEffects;
			if( pMeshContainerBase->pAdjacency )
				delete[] pMeshContainerBase->pAdjacency;
			if( pMeshContainerBase->pSkinInfo )
				IRelease(pMeshContainerBase->pSkinInfo);
			if( pMeshContainerBase->MeshData.pMesh )
				IRelease( pMeshContainerBase->MeshData.pMesh );
			delete pMeshContainerBase;
			return S_OK;
		}
	};
SkinnedMesh::SkinnedMesh(void)
{
}

SkinnedMesh::~SkinnedMesh(void)
{
	if( pHierarchyRoot )
	{
		AllocMeshHierarchy allocMeshHierarchy( this->sceneMgr );
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
	/*
	IRelease(meshEffect);
	*/

	delete effect;

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
 
void SkinnedMesh::loadFromX(MyGameSceneManager* sMgr)
{
		IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	
	AllocMeshHierarchy allocMeshHierarchy( sMgr );

	this->sceneMgr = sMgr;

	HR( D3DXLoadMeshHierarchyFromXW( L"testLoliske.X", 
								D3DXMESH_MANAGED,
								pDevice,
								&allocMeshHierarchy,
								0,
								&pHierarchyRoot,
								&pAnimCtrller ) );

	effect = new MyGame3DEffect( "newMesh.fx" );
	
	effect->setBOOLByName( TRUE, MyGame3DEffect::VERTBLEND );

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

	this->rootSceneNode = static_cast<MyGameBoneNode*>(pHierarchyRoot);

	//IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

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

			bone->setOffsetMatrix( *pSkinInfo->GetBoneOffsetMatrix(i) );
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
//	this->buildCombinedTransforms();

	this->getAnimation( this->animList );

	setAnimation( string("attack1") );
	lastTime = clock();

}

void SkinnedMesh::getAnimation( vector<string> &animations )
{
	ID3DXAnimationSet *anim = 0;
	
	for( unsigned int i = 0; i < this->pAnimCtrller->GetMaxNumAnimationSets(); ++ i )
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

void SkinnedMesh::setAnimation( string &name )
{
	ID3DXAnimationSet *anim = 0;
	pAnimCtrller->GetAnimationSetByName( name.c_str(), &anim );
		pAnimCtrller->SetTrackAnimationSet(0, anim);
		currentAnimDurationTime = anim->GetPeriod();
	anim->Release();

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
/*
void SkinnedMesh::buildCombinedTransforms()//已废弃，以后不需要此方法了
{
	D3DXMATRIX idMat;
	D3DXMatrixIdentity( &idMat );
	map<string, D3DXMATRIX> jionga;
	//FrameEx::CombineTransforms( pHierarchyRoot, idMat, jionga);
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
*/
void SkinnedMesh::render()
{
	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	
	//meshEffect->SetTechnique(hTech);

	effect->setTechniqueByName( MyGame3DEffect::TECH );

	unsigned int numPasses = 0;
	
	//meshEffect->Begin( &numPasses, 0 );

	effect->Begin( numPasses );

	//pSkinnedMesh->GetAttributeTable( 0, &NumAttributeGroups );

	for( unsigned int i = 0; i < numPasses; ++i )
	{
		//meshEffect->BeginPass(i);
		
		effect->BeginPass( i );
		{
			this->render( effect );
		}
		effect->EndPass();
	}
	//meshEffect->End();
	effect->End();
}

void SkinnedMesh::render( MyGame3DEffect* _pEffect )
{
	this->frameMove();

	D3DXMATRIX worldViewProj = sceneMgr->getViewProjCombinedMat();

	_pEffect->setMatrixByName( worldViewProj, MyGame3DEffect::WVPMATRIX );

	_pEffect->setMatrixByName( sceneMgr->getLightViewProjMat(), MyGame3DEffect::LVPMATRIX );

	_pEffect->setMatrixArrayByName( finalTransforms, numBones, MyGame3DEffect::FINMATARRAY );

	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();
	
	pDevice->SetVertexDeclaration(this->vDecl);

	_pEffect->setBOOLByName( TRUE, MyGame3DEffect::VERTBLEND );

	unsigned int j = 0;
	for( vector<IDirect3DTexture9*>::iterator _itr = texList.begin();
			_itr != texList.end();
			 ++ _itr )
	{
			_pEffect->setTextureByName( *_itr, MyGame3DEffect::TEXTURE );
			for( int k = 0; k < NumAttributeGroups; ++ k )
			{
				_pEffect->CommitChanges();
				int mtrlIndex = attributeTable[k].AttribId;
				
				pSkinnedMesh->DrawSubset( mtrlIndex );
			}
			++j;
	}
	_pEffect->setBOOLByName( FALSE, MyGame3DEffect::VERTBLEND );
}

void SkinnedMesh::frameMove( /*float deltaTime*/ /*,
							const D3DXMATRIX& worldViewProj*/ )	
{
	float timeDelta = (clock() - lastTime)/1000.0f;
	lastTime = clock();

	//if( currentAnimDurationTime > pAnimCtrller->GetTime() )
		this->pAnimCtrller->AdvanceTime( timeDelta, 0 );
	//if( ifAnimEnded() )
	//{
	//	this->setAnimation( string("stop")); 
	//}

	for( unsigned int i = 0; i < numBones; ++ i )
	{
		const char* boneName = pSkinInfo->GetBoneName( i );
		finalTransforms[i] =MyGameSceneNode::getNodeByName(boneName)->getCombinedMatrix();
	}

}

bool SkinnedMesh::ifAnimEnded()
{
	 if( currentAnimDurationTime <= pAnimCtrller->GetTime() )
		 return true;
	 else
		 return false;
}