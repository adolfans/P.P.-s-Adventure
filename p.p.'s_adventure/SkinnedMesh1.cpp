#include "StdAfx.h"
#include "SkinnedMesh1.h"
#include "MyGame3DDevice.h"		

SkinnedMesh1::SkinnedMesh1(void)
{
	AllocMeshHierarchy boneHierarchy;

	IDirect3DDevice9* pDevice = MyGame3DDevice::GetSingleton()->GetDevice();

	D3DXLoadMeshHierarchyFromXW( L"testx4.X", D3DXMESH_MANAGED,
								pDevice, &boneHierarchy, NULL,
								&pRootBone,
								&pAnimCtrl );

}


SkinnedMesh1::~SkinnedMesh1(void)
{
}


void SkinnedMesh1::SetupBoneMatrixPointers( D3DXFRAME* bone )
{
	if( bone->pMeshContainer != 0 )
	{
		D3DXMESHCONTAINER* boneMesh = bone->pMeshContainer;

		if( boneMesh->pSkinInfo )
		{
			int NumBones = boneMesh->pSkinInfo->GetNumBones();
			pBoneMatrixPts = new D3DXMATRIX[NumBones];

			for( int i =0; i < NumBones; i++ )
			{

			}
		}
	}
}