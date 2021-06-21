//=============================================================================
//
//  [CLife.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CLife.h"
#include "CInput.h"
#include "CMeshWall.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
int g_life=0;
CNumber *g_pLifeNum[LIFE_MAX];

CLife *CLife::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,int m_num)
{
	CLife *pLife;
	g_life = m_num;
	pLife = new CLife();
	int work = 10000;
	int workbox[LIFE_MAX];

	for(int nCntPlace=0;nCntPlace<LIFE_MAX;nCntPlace++)
	{
		workbox[nCntPlace] = ( g_life / work) % 10;

		work /= 10;

		g_pLifeNum[nCntPlace] = CNumber::Create(m_pD3DDevice,fileName,D3DXVECTOR3(m_pos.x+nCntPlace*50.0f,m_pos.y,1.0f),workbox[nCntPlace]);

	}
	pLife->Init(m_pD3DDevice,fileName);
	pLife->SetPos(m_pos);
	

	return pLife;
}
HRESULT CLife::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{


	LoadTexture(m_pD3DDevice);

	CNumber::Init(m_pD3DDevice,fileName);

	return S_OK;
}
void CLife::Uninit(void)
{
	//---テクスチャの解放---//
	CNumber::Uninit();

	for(int nCntPlace=0;nCntPlace<LIFE_MAX;nCntPlace++)
	{
		g_pLifeNum[nCntPlace]->Uninit();
		g_pLifeNum[nCntPlace] = NULL;
	}

}
void CLife::Update(void)
{

	int work = 10000;
	int workbox[LIFE_MAX];


	for(int nCntPlace=0;nCntPlace<LIFE_MAX;nCntPlace++)
	{
		workbox[nCntPlace] = ( g_life / work) % 10;

		work /= 10;

		if(g_pLifeNum[nCntPlace]!=NULL)
		{
			g_pLifeNum[nCntPlace]->SetNum(workbox[nCntPlace]);
		}
	}



	// 座標更新
	CNumber::SetPos(m_pos);
	CNumber::SetRot(m_rot);


}
void CLife::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	CNumber::Draw(m_pD3DDevice);

}
void CLife::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CLife::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CLife::GetPosDest(void)
{

	return m_pos;
}
void CLife::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CLife::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CLife::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	//--------テクスチャの読み込み----------//
	D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\white.jpg",&m_pD3DTextureModel);
}
void CLife::SetLife(int i)
{
	g_life = i;
}
int CLife::GetLife(void)
{
	return g_life;
}