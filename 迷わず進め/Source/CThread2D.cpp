//=============================================================================
//
//  [CThread2D.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CThread2D.h"
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
TYPE2D CThread2D::m_type2d = TYPE2D_NOMAR;
float CThread2D::m_wsize = 0.0f;
float CThread2D::m_hsize = 0.0f;

CThread2D *CThread2D::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,float wsize,float hsize,D3DXCOLOR color,TYPE2D type2d)
{
	CThread2D *pThread2D;
	pThread2D = new CThread2D(0);
	pThread2D->SetColor(color);
	m_wsize = wsize;
	m_hsize = hsize;
	m_type2d = type2d;
	pThread2D->Init(m_pD3DDevice,fileName);
	pThread2D->SetPos(m_pos);

	return pThread2D;


}
HRESULT CThread2D::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	m_pD3DTex = NULL;



	CScene2D::Init(m_pD3DDevice,fileName);
	CScene2D::SetWidth(m_wsize);
	CScene2D::SetHeight(m_hsize);
	CScene2D::SetType2D(m_type2d);
	m_color = D3DCOLOR_RGBA(255,255,255,255);

	m_fLength = 0;																//対角線の長さ
	m_fAngle = 0;																//対角線の角度
	m_visble = FALSE;

	return S_OK;
}
void CThread2D::Uninit(void)
{
	m_type2d = TYPE2D_NOMAR;
	CScene2D::Uninit();
	CScene::Relese();
}
void CThread2D::Update(void)
{
	CScene2D::SetPos(m_pos);
	CScene2D::Update();
}
void CThread2D::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	if(m_visble)
	{
		CScene2D::Draw(m_pD3DDevice);
	}

}

void CThread2D::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CThread2D::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CThread2D::GetPosDest(void)
{

	return m_pos;
}
void CThread2D::SetRot(D3DXVECTOR3 set_rot)
{
		m_pos = set_rot;
}
D3DXVECTOR3 CThread2D::GetRot(void)
{
	return m_rot;
}
void CThread2D::SetColor(D3DXCOLOR color)
{
	m_color = color;
}

void CThread2D::SetVertexPolygon(void)
{
}
void CThread2D::SetVisibleflag(bool flag)
{
	m_visble = flag;
}
bool CThread2D::GetVisibleflag(void)
{
	return m_visble;
}