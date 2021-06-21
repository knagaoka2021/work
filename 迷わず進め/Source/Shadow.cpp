//=============================================================================
//
//  [Shadow.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "Shadow.h"
#include "CInput.h"
#include "CMeshWall.h"
#include "CMotionModel.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOWPOSH (6.25f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
VERTEX_2D vertex[4];

Shadow *Shadow::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	Shadow *pShadow;
	pShadow = new Shadow();
	pShadow->Init(m_pD3DDevice,fileName);
	m_pos.y = 2.0f;
	pShadow->SetPos(m_pos);
	pShadow->SetRot(m_rot);
	return pShadow;
}
HRESULT Shadow::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	LoadTexture(m_pD3DDevice);
	m_pShadowDevice = m_pD3DDevice;
	CSceneX::Init(m_pD3DDevice,fileName);

	vertex[0].vtx.x=0.0f;
	vertex[0].vtx.y=0.0f;

	vertex[1].vtx.x=SCREEN_WIDTH;
	vertex[1].vtx.y=0.0f;

	vertex[2].vtx.x=0.0f;
	vertex[2].vtx.y=SCREEN_HEIGHT;

	vertex[3].vtx.x=SCREEN_WIDTH;
	vertex[3].vtx.y=SCREEN_HEIGHT;

	vertex[0].diffuse = D3DCOLOR_RGBA(0,0,0,255);
	vertex[1].diffuse = D3DCOLOR_RGBA(0,0,0,255);
	vertex[2].diffuse = D3DCOLOR_RGBA(0,0,0,255);
	vertex[3].diffuse = D3DCOLOR_RGBA(0,0,0,255);
	vertex[0].rhw=1.0f;
	vertex[1].rhw=1.0f;
	vertex[2].rhw=1.0f;
	vertex[3].rhw=1.0f;
	vertex[0].tex=D3DXVECTOR2(0,0);

	vertex[1].tex=D3DXVECTOR2(1,0);

	vertex[2].tex=D3DXVECTOR2(0,1);

	vertex[3].tex=D3DXVECTOR2(1,1);

	return S_OK;
}
void Shadow::Uninit(void)
{
	//---テクスチャの解放---//


	CSceneX::Uninit();

}
void Shadow::Update(void)
{
	m_pos.y+=-SHADOWPOSH;

	// 座標更新
	CSceneX::SetPos(m_pos);
	CSceneX::SetRot(m_rot);

}
void Shadow::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

// ステンシル有効
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	// Ｚバッファへの書き込み無効
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// カラーバッファへの書き込み無効(sampleではF2で切り替え)
	m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	// 何をもって合格？ ALWAYSいつも合格
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	// ステンシルテスト合格・Zテスト合格-0
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
	// ステンシルテスト合格・Zテスト不合格-+=1
	m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
	// ステンシルテスト不合格・Zテスト不合格-
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);

	// カリングを逆にする
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	CSceneX::Draw(m_pD3DDevice);

	// ステンシル参照値1
	m_pD3DDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	// ステンシル値が1以上で合格
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);
	// ステンシルテスト合格・Zテスト合格-0
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	// ステンシルテスト合格・Zテスト不合格-+=1
	m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);
	// ステンシルテスト不合格・Zテスト不合格-
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	CSceneX::Draw(m_pD3DDevice);

	// ステンシル参照値2
	m_pD3DDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	// ステンシル値が1以上で合格
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	// ステンシルテスト合格・Zテスト合格-0
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	// ステンシルテスト合格・Zテスト不合格-+=1
	m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	// ステンシルテスト不合格・Zテスト不合格-
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	//// カリングをもとに戻す
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_ALPHA));

	//m_pD3DDevice->SetRenderState( D3DRS_LIGHTING , FALSE );


	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	m_pD3DDevice->SetTexture(0,NULL);//テクスチャの設定

	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2, &vertex[0],sizeof(VERTEX_2D));//2はポリゴン数

	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE,FALSE);


}
void Shadow::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 Shadow::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 Shadow::GetPosDest(void)
{

	return m_pos;
}
void Shadow::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 Shadow::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void Shadow::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	//--------テクスチャの読み込み----------//
	D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\white.jpg",&m_pD3DTextureModel);
}