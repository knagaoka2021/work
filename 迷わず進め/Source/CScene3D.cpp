//=============================================================================
//
//  [CScene3D.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CScene3D.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POS	(900.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
VERTEX_3D* pVtx;

//=============================================================================
// クリエイト
//=============================================================================
CScene3D *CScene3D::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	CScene3D *pScene3D;
	pScene3D = new CScene3D();
	pScene3D->Init(m_pD3DDevice,fileName);
	pScene3D->SetPos(m_pos);

	return pScene3D;


}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CScene3D::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	

		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*4,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	
	VERTEX_3D Vertices[4] =
	{

 		{ D3DXVECTOR3( POS , 0.0f , POS) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.01) },
		{ D3DXVECTOR3( -POS , 0.0f , POS) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.01) } ,
		{ D3DXVECTOR3( POS , 0.0f , -POS) , D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(0.0f,0.01)} ,
		{ D3DXVECTOR3( -POS ,  0.0f , -POS) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.01) } ,
	};
	
	
	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&pVtx , 0 );

	memcpy( pVtx , Vertices , sizeof( Vertices ) );

	m_pD3DVtxBuff->Unlock();

	D3DXCreateTextureFromFile(m_pD3DDevice,fileName,&m_pD3DTex);

	m_pos = D3DXVECTOR3(0.0f,1.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_fLength = 0;																//対角線の長さ
	m_fAngle = 0;																//対角線の角度


	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CScene3D::Uninit(void)
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

	CScene::Relese();
}
//=============================================================================
// 更新処理
//=============================================================================
void CScene3D::Update(void)
{
	//if(CInputKeyboard::GetkeyboardPress(DIK_A))
	//{
	//	this->Uninit();
	//}
	SetVertexPolygon();
}
//=============================================================================
// 描画処理
//=============================================================================
void CScene3D::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;


	// World Matrix
	D3DXMatrixIdentity(&m_mtxWorld);															// 初期化

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y,m_rot.x,m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRot);

	D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxTranslate);

	m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);


	m_pD3DDevice->SetStreamSource( 0 , m_pD3DVtxBuff , 0 , sizeof( VERTEX_3D ) );

	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

	m_pD3DDevice->SetTexture(0,m_pD3DTex);										// テクスチャの設定

	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);								// 2はポリゴン数(又はポリゴン数を３で割る)


}
//=============================================================================
// 処理
//=============================================================================
void CScene3D::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CScene3D::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CScene3D::GetPosDest(void)
{

	return m_pos;
}
//=============================================================================
// 処理
//=============================================================================
void CScene3D::SetRot(D3DXVECTOR3 set_rot)
{
	m_rot = set_rot;
}
D3DXVECTOR3 CScene3D::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// 
//=============================================================================
void CScene3D::SetVertexPolygon(void)
{

}
