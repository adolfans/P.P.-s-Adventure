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
					if( !pMaterials[i].pTextureFilename )
						ctn->pMaterials[i].pTextureFilename = 0;
					else
					{
						unsigned int len = strlen( pMaterials[i].pTextureFilename );
						ctn->pMaterials[i].pTextureFilename = new char[len+1];
						strcpy( ctn->pMaterials[i].pTextureFilename, pMaterials[i].pTextureFilename ); 
					}
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
	:animLoop( true )
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
	
	IRelease(pAnimCtrller);

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

	HR( D3DXLoadMeshHierarchyFromXW(L"aaaaaaa.X",// L"testLoliske.X", 
								D3DXMESH_MANAGED,
								pDevice,
								&allocMeshHierarchy,
								0,
								&pHierarchyRoot,
								&pAnimCtrller ) );

	

	this->rootSceneNode = static_cast<MyGameBoneNode*>(pHierarchyRoot);


	D3DXFRAME* f = findNodeWithMesh( this->pHierarchyRoot );
	if( f==0 ) HR(E_FAIL);
	D3DXMESHCONTAINER* meshContainer = f->pMeshContainer;
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
	pSkinInfo->GetDeclaration( verElmt );;

	ID3DXMesh* pMesh = meshContainer->MeshData.pMesh;

	this->buildSkinnedMesh( pMesh );
	
	finalTransforms = new D3DXMATRIX[numBones];

	this->getAnimation( this->animList );

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

}

void SkinnedMesh::setAnimation( const string &name, bool loop)
{
	if( loop && currentAnim == name )	// 如果正在循环并且正在播放的动画就是当前想要播放的动画，则直接返回
		return;
	ID3DXAnimationSet *anim = 0;
	pAnimCtrller->GetAnimationSetByName( name.c_str(), &anim );
		pAnimCtrller->SetTrackAnimationSet(0, anim);
		currentAnimDurationTime = anim->GetPeriod();
		pAnimCtrller->SetTrackPosition( 0, 0.0f );
		//pAnimCtrller->KeyTrackPosition( 0, 0.0f, 0.0f );
	
	anim->Release();
	currentAnim = name;
	pAnimCtrller->ResetTime();
	this->animLoop = loop;//设置是否循环
}

void SkinnedMesh::buildSkinnedMesh( ID3DXMesh* mesh )
{
	ID3DXBuffer* boneComboTable = 0;
	//pSkinnedMesh = 0;
	maxVertInfluences= 0;

	mesh->AddRef();

	ID3DXMesh* pSkinnedMesh;

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

	D3DXFRAME* f = findNodeWithMesh( this->pHierarchyRoot );
	if( f==0 ) HR(E_FAIL);
	D3DXMESHCONTAINER* meshContainer = f->pMeshContainer;
	
	this->setID3DXMesh( pSkinnedMesh );

	for( unsigned int i = 0; i < meshContainer->NumMaterials; ++ i )
	{
		
		this->addMtrls(meshContainer->pMaterials[i].MatD3D, meshContainer->pMaterials[i].pTextureFilename );
	}

	IRelease(boneComboTable );

	IRelease( mesh );
	
	//Get Attribute Table
	pSkinnedMesh->GetAttributeTable(NULL, &NumAttributeGroups);
	attributeTable = new D3DXATTRIBUTERANGE[NumAttributeGroups];
	pSkinnedMesh->GetAttributeTable(attributeTable, NULL);

	

}
void SkinnedMesh::render( MyGame3DEffect* _pEffect )
{
	this->frameMove();

	D3DXMATRIX worldViewProj = sceneMgr->getViewProjCombinedMat();

	//_pEffect->setMatrixByName( worldViewProj, MyGame3DEffect::WVPMATRIX );

	_pEffect->setMatrixByName( sceneMgr->getLightViewProjMat(), MyGame3DEffect::LVPMATRIX );

	_pEffect->setMatrixArrayByName( finalTransforms, numBones, MyGame3DEffect::FINMATARRAY );

	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	_pEffect->setBOOLByName( TRUE, MyGame3DEffect::VERTBLEND );

	MyGameMesh::render( _pEffect );

	_pEffect->setBOOLByName( FALSE, MyGame3DEffect::VERTBLEND );
}

void SkinnedMesh::prepare()
{
	this->frameMove();
}

void SkinnedMesh::frameMove( /*float deltaTime*/ /*,
							const D3DXMATRIX& worldViewProj*/ )	
{
	float timeDelta = (clock() - lastTime)/1000.0f;
	lastTime = clock();

	this->pAnimCtrller->AdvanceTime( timeDelta, 0 );

	for( unsigned int i = 0; i < numBones; ++ i )
	{
		const char* boneName = pSkinInfo->GetBoneName( i );
		finalTransforms[i] =MyGameSceneNode::getNodeByName(boneName)->getCombinedMatrix();
	}

}

bool SkinnedMesh::ifAnimEnded()
{
	//float time = pAnimCtrller->GetTime();
	//如果是在循环播放，则永远返回false
	if( animLoop )
		return false;
	if( currentAnimDurationTime - pAnimCtrller->GetTime() < 0.05 )
		return true;
	else
		return false;
}