//=============================================================================
//
//  [CSceneBillboard.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CExplotion.h"
#include "CCamera.h"
#include "common.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
CCamera *pECamera = NULL;
VERTEX_3D* pExpVtx;

#define POS	(80.0f)
VERTEX_3D* g_pEVtx;

CExplotion::CExplotion(int Priority):CScene(3)
{
	m_pos = D3DXVECTOR3(1.0f,0.0f,100.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);


	m_fLength = 0;																//対角線の長さ
	m_fAngle = 0;																//対角線の角度


}
//=============================================================================
// クリエイト
//=============================================================================
CExplotion *CExplotion::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	CExplotion *pExplotion;
	pExplotion = new CExplotion();
	pExplotion->Init(m_pD3DDevice,fileName);
	pExplotion->SetPos(m_pos);


	pECamera = CCamera::GetTypeClass();

	return pExplotion;


}

HRESULT CExplotion::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	

		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*4,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	
	VERTEX_3D Vertices[4] =
	{
		{ D3DXVECTOR3( -POS , POS , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,
 		{ D3DXVECTOR3( POS , POS , 0.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.1f,0.0f) },
		{ D3DXVECTOR3( -POS ,  -POS , 0.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,
		{ D3DXVECTOR3( POS , -POS , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(0.1f,1.0f)} ,
	};
	
	
	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&g_pEVtx , 0 );

	memcpy( g_pEVtx , Vertices , sizeof( Vertices ) );

	m_pD3DVtxBuff->Unlock();

	D3DXCreateTextureFromFile(m_pD3DDevice,fileName,&m_pD3DTex);
	//m_pD3DTex = GetTexture();


	m_ExpllosionTexuv = 0.0f;
	m_ExpllosionTime = 1.0f;


	return S_OK;
}
void CExplotion::Uninit(void)
{
	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff =NULL;
	}
	//if(m_pD3DTex != NULL)
	//{
	//	m_pD3DTex->Release();
	//	m_pD3DTex =NULL;
	//}
	pECamera = NULL;

	CScene::Relese();
}
void CExplotion::Update(void)
{
	SetVertexPolygon();
	//pCamera->UninitCamera();
}
void CExplotion::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;


	
	m_mtxView = pECamera->GetCameraMtxView();

	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixInverse(&m_mtxWorld,NULL,&m_mtxView);

	m_mtxWorld._41 =0.0f;
	m_mtxWorld._42 =0.0f;
	m_mtxWorld._43 =0.0f;


	D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxTranslate);

	m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);


	m_pD3DDevice->SetStreamSource( 0 , m_pD3DVtxBuff , 0 , sizeof( VERTEX_3D ) );

	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

	m_pD3DDevice->SetTexture(0,m_pD3DTex);										// テクスチャの設定

	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);								// 2はポリゴン数(又はポリゴン数を３で割る)


}

void CExplotion::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CExplotion::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CExplotion::GetPosDest(void)
{

	return m_pos;
}
void CExplotion::SetRot(D3DXVECTOR3 set_rot)
{
		m_pos = set_rot;
}
D3DXVECTOR3 CExplotion::GetRot(void)
{
	return m_rot;
}


void CExplotion::SetVertexPolygon(void)
{

	if(m_ExpllosionTime%3==0&&m_ExpllosionTexuv != 10)
	{
		m_ExpllosionTexuv++;
	}

	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&g_pEVtx , 0 );

	g_pEVtx[0].tex=D3DXVECTOR2(0.1f*m_ExpllosionTexuv,0);

	g_pEVtx[1].tex=D3DXVECTOR2(0.1f*m_ExpllosionTexuv+0.1f,0);

	g_pEVtx[2].tex=D3DXVECTOR2(0.1f*m_ExpllosionTexuv,1.0f);

	g_pEVtx[3].tex=D3DXVECTOR2(0.1f*m_ExpllosionTexuv+0.1f,1.0f);

	m_pD3DVtxBuff->Unlock();



	if(m_ExpllosionTexuv == 10)
	{
		this->Uninit();
	}

	m_ExpllosionTime++;

}
