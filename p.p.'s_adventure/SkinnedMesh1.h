#pragma once
#include <d3dx9.h>
class SkinnedMesh1
{
private:
	class AllocMeshHierarchy : public ID3DXAllocateHierarchy
	{
	public:
		HRESULT STDMETHODCALLTYPE CreateFrame( THIS_ PCSTR Name, D3DXFRAME** ppNewFrame )
		{
			*ppNewFrame = new D3DXFRAME;
			memset( *ppNewFrame, 0, sizeof( D3DXFRAME ) );
			if(Name)
			{
				unsigned int len = strlen(Name);
				(*ppNewFrame)->Name = new char[len+1];
				strcpy( (*ppNewFrame)->Name, Name );
			}
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
			*ppNewMeshContainer = new D3DXMESHCONTAINER;
			memset( *ppNewMeshContainer, 0, sizeof(D3DXMESHCONTAINER) );
			
			D3DXMESHCONTAINER* ctn = *ppNewMeshContainer;
			ctn->NumMaterials = NumMaterials;
			if(pMeshData)
			{
				(*ppNewMeshContainer)->MeshData = *pMeshData;
				pMeshData->pMesh->AddRef();
			}
			if(Name)
			{
				unsigned int len = strlen( Name );
				char* name = new char[len+1];
				strcpy( name, Name );
				(*ppNewMeshContainer)->Name = name;
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
			}
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE DestroyFrame(
								THIS_ D3DXFRAME* pFrameToFree)
		{
			if( pFrameToFree->Name )
				delete[] pFrameToFree->Name;
			delete pFrameToFree;
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
	D3DXFRAME* pRootBone;
	ID3DXAnimationController* pAnimCtrl;
	D3DXMATRIX* pBoneMatrixPts;
	void SetupBoneMatrixPointers( D3DXFRAME* bone );
public:
	SkinnedMesh1(void);
	~SkinnedMesh1(void);
	void render();
	void prepare();
};

