//=============================================================================
//
//  [CSceneBillboard.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CSceneBillboard.h"
#include "CCamera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
float CSceneBillboard::m_W = 0.0f;
CCamera *pCamera = NULL;
VERTEX_3D* g_pVtx;

//=============================================================================
// クリエイト
//=============================================================================
CSceneBillboard *CSceneBillboard::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	CSceneBillboard *pSceneBillboard;
	pSceneBillboard = new CSceneBillboard();
	pSceneBillboard->Init(m_pD3DDevice,fileName);
	pSceneBillboard->SetPos(m_pos);


	pCamera = CCamera::GetTypeClass();
	return pSceneBillboard;


}

HRESULT CSceneBillboard::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*4,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	
	VERTEX_3D Vertices[4] =
	{
		{ D3DXVECTOR3( -m_fSize , m_fSize , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,
 		{ D3DXVECTOR3( m_fSize , m_fSize , 0.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3( -m_fSize ,  -m_fSize , 0.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,
		{ D3DXVECTOR3( m_fSize , -m_fSize , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(1.0f,1.0f)} ,
	};
	
	
	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&g_pVtx , 0 );

	memcpy( g_pVtx , Vertices , sizeof( Vertices ) );

	m_pD3DVtxBuff->Unlock();

	D3DXCreateTextureFromFile(m_pD3DDevice,fileName,&m_pD3DTex);


	return S_OK;
}
void CSceneBillboard::Uninit(void)
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
	pCamera = NULL;

	CScene::Relese();
}
void CSceneBillboard::Update(void)
{
	SetVertexPolygon();
	//pCamera->UninitCamera();
}
void CSceneBillboard::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;
	
	m_mtxView = pCamera->GetCameraMtxView();

	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixInverse(&m_mtxWorld,NULL,&m_mtxView);


	// Y軸のみ
	m_mtxWorld._12 = 0.0f;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = 0.0f;
	m_mtxWorld._23 = 0.0f;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._32 = 0.0f;
	m_mtxWorld._34 = 0.0f;

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

void CSceneBillboard::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CSceneBillboard::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CSceneBillboard::GetPosDest(void)
{

	return m_pos;
}

void CSceneBillboard::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CSceneBillboard::GetRot(void)
{
	return m_rot;
}
void CSceneBillboard::SetSize(float width)
{
	m_fSize = width;
}


void CSceneBillboard::SetVertexPolygon(void)
{

}
