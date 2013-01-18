#pragma once
#include <d3dx9.h>
#include <vector>
#include <map>
#include <string>
#include <WinDef.h>
using std::map;
using std::vector;
using std::string;

#include "MyGameSceneNode.h"
using namespace MyGameScene;

#include "MyGameBoneNode.h"
#include "MyGameMesh.h"
#include "MyGame3DEffect.h"

struct D3DXMESHCONTAINER_EX
	:D3DXMESHCONTAINER
{
	DWORD NumInfl;
	DWORD NumAttributeGroups;
	DWORD NumPaletteEntries;
};


class SkinnedMesh
	: public MyGameMesh
{
private:
	class AllocMeshHierarchy : public ID3DXAllocateHierarchy
	{
	public:
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
			MyGameBoneNode* myNode = new MyGameBoneNode( Name, true );
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
				delete pFrame;
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


	D3DXFRAME* findNodeWithMesh(D3DXFRAME* frame );
	void buildSkinnedMesh( ID3DXMesh* mesh );
	void buildCombinedTransforms();
	LPD3DXFRAME pHierarchyRoot;
	ID3DXAnimationController* pAnimCtrller;
	ID3DXSkinInfo* pSkinInfo;
	DWORD maxVertInfluences;
	ID3DXMesh* pSkinnedMesh;
	vector<D3DXMATRIX> combinedTransforms;
	//D3DXMATRIX combinedTransforms[21];
	vector<IDirect3DTexture9*> texList;
	/*
	ID3DXEffect* meshEffect;
	*/
	IDirect3DVertexDeclaration9* vDecl;
	unsigned int numBones;	
	DWORD  numBoneComboEntries;

	float lastTime;

/*
	D3DXHANDLE hTech;
	D3DXHANDLE hTexture;
	D3DXHANDLE hWVPMat;
	D3DXHANDLE hFinalMat;
	D3DXHANDLE gMtrl;
	D3DXHANDLE gLight;
*/	
	DWORD NumAttributeGroups;
	D3DXATTRIBUTERANGE* attributeTable;
	D3DXMATRIX* finalTransforms;

	vector<string> animList;

	//vector<D3DXMATRIX> offsetMatrixList;
	
	MyGame3DEffect* effect;
	
	static const unsigned int MAX_NUM_BONES_SUPPORTED = 26;

	MyGameSceneNode* rootSceneNode;

public:
	SkinnedMesh(void);
	~SkinnedMesh(void);

	void render();
	void render( MyGame3DEffect* _pEffect );
	void prepare();
	

	void  GetAnimation( vector<string> &animations );

	void frameMove( /*float deltaTime*//*, const D3DXMATRIX& worldViewProj */);

	MyGameSceneNode* getSkeletonRoot(){ return rootSceneNode; }
};



struct FrameEx : public D3DXFRAME
{
	//D3DXMATRIX combinedTransform;

	/*static void CombineTransforms( FrameEx* frame, D3DXMATRIX& P )
	{
		D3DXMATRIX& L = frame->TransformationMatrix;
		D3DXMATRIX& C = frame->combinedTransform;
		C = L * P;

		FrameEx* sibling = (FrameEx*)frame->pFrameSibling;
		FrameEx* firstChild = (FrameEx*)frame->pFrameFirstChild;

		if(sibling)
			CombineTransforms( sibling, P );

		if( firstChild )
			CombineTransforms( firstChild, C );
	}*/

	//static void CombineTransforms( D3DXFRAME* frame, D3DXMATRIX& P, map<string, D3DXMATRIX>& jiong )
	//{
	//	D3DXMATRIX& L = frame->TransformationMatrix;
	//	D3DXMATRIX C;
	//	C = L * P;

	//	D3DXFRAME* sibling = (D3DXFRAME*)frame->pFrameSibling;
	//	D3DXFRAME* firstChild = (D3DXFRAME*)frame->pFrameFirstChild;

	//	if( frame->Name && strlen(frame->Name)!=0 )
	//		jiong[frame->Name] = C;
	//	//else
	//	//	//jiong[""]=C;
	//	//	jiong[frame->Name] = C;
	//	if(sibling)
	//		CombineTransforms( sibling, P, jiong );

	//	if( firstChild )
	//		CombineTransforms( firstChild, C, jiong );
	//}
	static void CombineTransforms( D3DXFRAME* frame, D3DXMATRIX& P, map<string, D3DXMATRIX>& jiong )
	{
		MyGameBoneNode* root = static_cast<MyGameBoneNode*>(frame);
		root->ComputeCombinedMatrix( P, jiong );
	}
};