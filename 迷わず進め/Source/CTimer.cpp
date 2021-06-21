//=============================================================================
//
//  [CTimer.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CTimer.h"
#include "CInput.h"
#include "CMeshWall.h"
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
int g_timer=0;
int g_timerTime = 0;
CNumber *g_pCTimer[TIMEDIGITS];

CTimer *CTimer::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,int m_num)
{
	CTimer *pTimer;
	g_timer = m_num;
	pTimer = new CTimer();
	int work = 100;
	int workbox[TIMEDIGITS];

	for (int nCntPlace = 0; nCntPlace<TIMEDIGITS; nCntPlace++)
	{
		workbox[nCntPlace] = ( g_timer / work) % 10;

		work /= 10;

		g_pCTimer[nCntPlace] = CNumber::Create(m_pD3DDevice,fileName,D3DXVECTOR3(m_pos.x+nCntPlace*50.0f,m_pos.y,1.0f),workbox[nCntPlace]);

	}
	pTimer->Init(m_pD3DDevice,fileName);
	pTimer->SetPos(m_pos);
	

	return pTimer;
}
HRESULT CTimer::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{


	LoadTexture(m_pD3DDevice);
	CNumber::Init(m_pD3DDevice,fileName);

	return S_OK;
}
void CTimer::Uninit(void)
{
	//---テクスチャの解放---//


	CNumber::Uninit();
	for (int nCntPlace = 0; nCntPlace<TIMEDIGITS; nCntPlace++)
	{
		g_pCTimer[nCntPlace]->Uninit();
		g_pCTimer[nCntPlace] = NULL;
	}

}
void CTimer::Update(void)
{

	int work = 100;
	int workbox[TIMEDIGITS];


	for (int nCntPlace = 0; nCntPlace<TIMEDIGITS; nCntPlace++)
	{
		workbox[nCntPlace] = ( g_timer / work) % 10;

		work /= 10;

		if(g_pCTimer[nCntPlace]!=NULL)
		{
			g_pCTimer[nCntPlace]->SetNum(workbox[nCntPlace]);
		}
	}


	if(g_timerTime%60 == 0&&g_timer!=0)
	{
		g_timer--;
	}

	// 座標更新
	CNumber::SetPos(m_pos);

	g_timerTime--;

}
void CTimer::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{


	CNumber::Draw(m_pD3DDevice);

}
void CTimer::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CTimer::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CTimer::GetPosDest(void)
{

	return m_pos;
}
void CTimer::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CTimer::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CTimer::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	//--------テクスチャの読み込み----------//

	D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\white.jpg",&m_pD3DTextureModel);
}
bool timerflag(void)
{
	if(g_timer==0)
	{
		return TRUE;
	}
	return FALSE;
}