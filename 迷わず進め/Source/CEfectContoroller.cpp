//=============================================================================
//
//  [CEfectContoroller.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CEfectContoroller.h"
#include "CCamera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFECT_NUM (2)
#define POS	(23.0f)
#define EFECTSIZE (2)
struct EFECTSET
{
	CEfect *data;

};
EFECTSET g_efectSet[EFECT_NUM];


//=============================================================================
// クリエイト
//=============================================================================
CEfectContoroller *CEfectContoroller::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	CEfectContoroller *pEfect;
	pEfect = new CEfectContoroller();
	pEfect->Init(m_pD3DDevice,fileName);
	m_pos.y += 10.0f;
	pEfect->SetPos(m_pos);
	//pEfectCamera = CCamera::GetTypeClass();

	return pEfect;


}

HRESULT CEfectContoroller::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	for(int i=0;i<EFECT_NUM;i++)
	{
		g_efectSet[i].data = CEfect::Create(m_pD3DDevice,fileName,m_pos,m_rot);
	}
	m_EfectTexuv = 0.0f;
	m_EfectTime = 1.0f;
	m_alpha = 255;


	return S_OK;
}
void CEfectContoroller::Uninit(void)
{
	for(int i=0;i<EFECT_NUM;i++)
	{
		if(g_efectSet[i].data!=NULL)
		{
		g_efectSet[i].data->Uninit();
		g_efectSet[i].data = NULL;
		}
	}
	CScene::Relese();
}
void CEfectContoroller::Update(void)
{
	SetVertexPolygon();

	for(int i=0;i<EFECT_NUM;i++)
	{
		if(g_efectSet[i].data!=NULL)
		{
			g_efectSet[i].data->Update();
			g_efectSet[i].data->SetPos(D3DXVECTOR3(m_EfectTime,1.0f,0.0f));
		}
	}
}
void CEfectContoroller::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
}

void CEfectContoroller::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CEfectContoroller::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CEfectContoroller::GetPosDest(void)
{

	return m_pos;
}
void CEfectContoroller::SetRot(D3DXVECTOR3 set_rot)
{
	m_rot = set_rot;
}
D3DXVECTOR3 CEfectContoroller::GetRot(void)
{
	return m_rot;
}


void CEfectContoroller::SetVertexPolygon(void)
{

	if(m_EfectTime%6==0&&m_EfectTexuv != EFECTSIZE)
	{
		m_EfectTexuv++;
	}


	m_EfectTime++;

}
