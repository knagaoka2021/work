//=============================================================================
//
//  [CScore.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CScore.h"
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
int g_score=0;
int g_scoreTime = 0;
CNumber *g_pCNum[KETA];

CScore *CScore::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,int m_num)
{
	CScore *pScore;
	g_score = m_num;
	pScore = new CScore();
	int work = 10000;
	int workbox[KETA];

	for(int nCntPlace=0;nCntPlace<KETA;nCntPlace++)
	{
		workbox[nCntPlace] = ( g_score / work) % 10;

		work /= 10;

		g_pCNum[nCntPlace] = CNumber::Create(m_pD3DDevice,fileName,D3DXVECTOR3(m_pos.x+nCntPlace*50.0f,m_pos.y,1.0f),workbox[nCntPlace]);

	}
	pScore->Init(m_pD3DDevice,fileName);
	pScore->SetPos(m_pos);
	

	return pScore;
}
HRESULT CScore::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{


	LoadTexture(m_pD3DDevice);

	CNumber::Init(m_pD3DDevice,fileName);

	return S_OK;
}
void CScore::Uninit(void)
{
	//---テクスチャの解放---//
	CNumber::Uninit();
	for(int nCntPlace=0;nCntPlace<KETA;nCntPlace++)
	{
		g_pCNum[nCntPlace]->Uninit();
		g_pCNum[nCntPlace] = NULL;
	}

}
void CScore::Update(void)
{

	int work = 10000;
	int workbox[KETA];


	for(int nCntPlace=0;nCntPlace<KETA;nCntPlace++)
	{
		workbox[nCntPlace] = ( g_score / work) % 10;

		work /= 10;

		if(g_pCNum[nCntPlace]!=NULL)
		{
			g_pCNum[nCntPlace]->SetNum(workbox[nCntPlace]);
		}
	}


	//if(CInputKeyboard::GetkeyboardPress(DIK_W))
	//{
	//	g_score++;
	//}
	//if(CInputKeyboard::GetkeyboardPress(DIK_S))
	//{
	//	g_score++;
	//}
	//if(CInputKeyboard::GetkeyboardPress(DIK_D))
	//{
	//	g_score++;
	//}
	//if(CInputKeyboard::GetkeyboardPress(DIK_A))
	//{ 
	//	g_score++;
	//	
	//}

	// 座標更新
	CNumber::SetPos(m_pos);
	CNumber::SetRot(m_rot);


}
void CScore::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{


	CNumber::Draw(m_pD3DDevice);

}
void CScore::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CScore::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CScore::GetPosDest(void)
{

	return m_pos;
}
void CScore::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CScore::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CScore::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	//--------テクスチャの読み込み----------//

	D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\white.jpg",&m_pD3DTextureModel);
}