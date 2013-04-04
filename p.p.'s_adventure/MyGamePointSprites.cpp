#include "StdAfx.h"
#include "MyGamePointSprites.h"
#include "MyGame3DDevice.h"

MyGamePointSprites::MyGamePointSprites(int num)
{
	device = MyGame3DDevice::GetSingleton()->GetDevice();

	HR(device->CreateVertexBuffer( 
		sizeof( POINTVERTEX ) * num,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY/* | D3DUSAGE_POINTS*/,
		D3DFVF_XYZ | D3DFVF_DIFFUSE,
		D3DPOOL_DEFAULT,
		&vb,
		0 ) );
	
	POINTVERTEX* vertices;
	vb->Lock( 0, sizeof( POINTVERTEX ), (void**)&vertices, 0 );

	vertices->x = 1.0f;
	vertices->y = 1.0f;
	vertices->z = 0.0f;
	vertices->color = 0xffffffff;
	
	vb->Unlock();

}


MyGamePointSprites::~MyGamePointSprites(void)
{
	IRelease(vb);
}

void MyGamePointSprites::render( MyGameSceneManager* sceneMgr )
{
    device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ); // 开启Blend
    //device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
    //device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
    device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE) ;
    device->SetRenderState(D3DRS_POINTSCALEENABLE, true) ;
	float pointSize = 0.5f;//000;
    device->SetRenderState( D3DRS_POINTSIZE, *((DWORD*)&pointSize )); // 点大小
	
	device->SetRenderState( D3DRS_ZENABLE, false );
	device->SetRenderState( D3DRS_ZWRITEENABLE, false );
	D3DXMATRIX mat;

	D3DXMatrixIdentity( &mat );
	/*D3DXMATRIX wmp = mat//worldmat里只是scale的矩阵吧
			*sceneMgr->getViewMat()
			*sceneMgr->getProjMat();*/
	device->SetTransform( D3DTS_WORLD, &mat);
	device->SetTransform( D3DTS_PROJECTION, &mat );
	device->SetTransform( D3DTS_VIEW, &mat );

	device->SetStreamSource( 0, vb, 0, sizeof( POINTVERTEX ) );
	device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	device->DrawPrimitive( D3DPT_POINTLIST, 0, 1 );

}