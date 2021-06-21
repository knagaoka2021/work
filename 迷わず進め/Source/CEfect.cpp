//=============================================================================
//
//  [CEfect.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CEfect.h"
#include "CCamera.h"
#include "common.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
CCamera *pEfectCamera = NULL;

#define POS	(50.0f)
#define EFECTSIZE (250)
VERTEX_3D* g_pEfectVtx;


//=============================================================================
// クリエイト
//=============================================================================
CEfect *CEfect::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	CEfect *pEfect;
	pEfect = new CEfect();
	pEfect->Init(m_pD3DDevice,fileName);
	m_pos.y += 10.0f;
	pEfect->SetPos(m_pos);

	return pEfect;
}

HRESULT CEfect::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	
	m_pEfectCamera = CCamera::GetTypeClass();
		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*4,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	
	VERTEX_3D Vertices[4] =
	{
		{ D3DXVECTOR3( -POS , POS , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,
 		{ D3DXVECTOR3( POS , POS , 0.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3( -POS ,  -POS , 0.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,
		{ D3DXVECTOR3( POS , -POS , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(1.0f,1.0f)} ,
	};
	
	
	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&g_pEfectVtx , 0 );

	memcpy( g_pEfectVtx , Vertices , sizeof( Vertices ) );

	m_pD3DVtxBuff->Unlock();

	D3DXCreateTextureFromFile(m_pD3DDevice,fileName,&m_pD3DTex);
	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_EfectTexuv = 0.0f;
	m_EfectTime = 1.0f;
	m_alpha = 255;


	return S_OK;
}
void CEfect::Uninit(void)
{
	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff =NULL;
	}
	if(m_pD3DTex != NULL)
	{
		m_pD3DTex->Release();
		m_pD3DTex =NULL;
	}
	pEfectCamera = NULL;

	CScene::Relese();
}
void CEfect::Update(void)
{
	SetVertexPolygon();

}
void CEfect::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	if(m_pD3DVtxBuff!=NULL)
	{
	m_pD3DDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);
	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,0);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	//m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);

	//m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);

	//m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	m_mtxView = pEfectCamera->GetCameraMtxView();

	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixInverse(&m_mtxWorld,NULL,&m_mtxView);

	m_mtxWorld._41 =0.0f;
	m_mtxWorld._42 =0.0f;
	m_mtxWorld._43 =0.0f;


	// World Matrix
	//D3DXMatrixIdentity(&m_mtxWorld);															// 初期化

	//D3DXMatrixScaling(&mtxScl,m_scl.x,m_scl.y,m_scl.z);

	//D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

	D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxTranslate);

	m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);


	m_pD3DDevice->SetStreamSource( 0 , m_pD3DVtxBuff , 0 , sizeof( VERTEX_3D ) );

	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

	m_pD3DDevice->SetTexture(0,m_pD3DTex);										// テクスチャの設定

	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);								// 2はポリゴン数(又はポリゴン数を３で割る)

	m_pD3DDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);

	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);

	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	}



}

void CEfect::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CEfect::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CEfect::GetPosDest(void)
{

	return m_pos;
}
void CEfect::SetRot(D3DXVECTOR3 set_rot)
{
	m_rot = set_rot;
}
D3DXVECTOR3 CEfect::GetRot(void)
{
	return m_rot;
}


void CEfect::SetVertexPolygon(void)
{

	if(m_EfectTime%1==0&&m_EfectTexuv != EFECTSIZE)
	{
		m_EfectTexuv++;
	}
	if(m_pD3DVtxBuff!=NULL)
	{
	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&g_pEfectVtx , 0 );

	if(m_EfectTexuv<EFECTSIZE)
	{

		//m_scl = D3DXVECTOR3(EFECTSIZE-m_EfectTexuv,EFECTSIZE-m_EfectTexuv,1.0f);
	
		m_alpha=255-m_EfectTexuv;

		g_pEfectVtx[0].diffuse = D3DCOLOR_RGBA(255,0,0,m_alpha);
		g_pEfectVtx[1].diffuse = D3DCOLOR_RGBA(255,0,0,m_alpha);
		g_pEfectVtx[2].diffuse = D3DCOLOR_RGBA(255,0,0,m_alpha);
		g_pEfectVtx[3].diffuse = D3DCOLOR_RGBA(255,0,0,m_alpha);
	}
	//g_pEfectVtx[0].tex=D3DXVECTOR2(0.1f*m_EfectTexuv,0);

	//g_pEfectVtx[1].tex=D3DXVECTOR2(0.1f*m_EfectTexuv+0.1f,0);

	//g_pEfectVtx[2].tex=D3DXVECTOR2(0.1f*m_EfectTexuv,1.0f);

	//g_pEfectVtx[3].tex=D3DXVECTOR2(0.1f*m_EfectTexuv+0.1f,1.0f);

	m_pD3DVtxBuff->Unlock();
	}



	if(m_EfectTexuv == EFECTSIZE)
	{
		//this->Uninit();
		m_EfectTexuv=0.0f;
	}

	m_EfectTime++;

}
