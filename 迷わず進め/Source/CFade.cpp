//=============================================================================
//
//  [CFade.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CFade.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static D3DXCOLOR g_color;
float alpha=0.0f;
CFade *CFade::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,MODE_FADE mode,int time,D3DXCOLOR color)
{
	CFade *pFade;
	pFade = new CFade(0);
	g_color=color;
	pFade->Init(m_pD3DDevice,fileName);
	pFade->SetPos(m_pos);

	return pFade;


}
HRESULT CFade::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	
	CScene2D::Init(m_pD3DDevice,fileName);
	m_color = g_color;
	fadeAlpha = 0.0f;
	CScene2D::SetColor(g_color);
	m_mode = FADE_NONE;
	return S_OK;
}
void CFade::Uninit(void)
{
	CScene2D::Uninit();
}
void CFade::Update(void)
{

	SetVertexPolygon();

	switch(m_mode)
		{
		case FADE_NONE:
			fadeAlpha=0;
			break;
		case FADE_OUT:
			if(fadeAlpha<=255)
			{
				fadeAlpha += alpha;
				if(fadeAlpha>255)
				{
					fadeAlpha=255;
				}
			}
			if(255 <= fadeAlpha)
			{

				m_mode = FADE_OUTEND;
			}
			break;
		case FADE_IN:
			if(fadeAlpha>0)
			{

				fadeAlpha -= alpha;
				if(fadeAlpha<0)
				{
					fadeAlpha=0;
				}
			}
			else
			{
				m_mode = FADE_NONE;
			}
			break;
		case FADE_INEND:
			break;
		case FADE_OUTEND:
			break;
		case FADE_MAX:
			break;
		}

	m_color = D3DCOLOR_RGBA((int)m_color.r,(int)m_color.g,(int)m_color.b,(int)fadeAlpha);
	CScene2D::SetColor(m_color);
	CScene2D::SetPos(D3DXVECTOR3(0,0,1));
	CScene2D::Update();
	}
void CFade::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	CScene2D::Draw(m_pD3DDevice);
}

void CFade::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CFade::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CFade::GetPosDest(void)
{

	return m_pos;
}
void CFade::SetRot(D3DXVECTOR3 set_rot)
{
		m_pos = set_rot;
}
D3DXVECTOR3 CFade::GetRot(void)
{
	return m_rot;
}


void CFade::SetVertexPolygon(void)
{
}
void CFade::StartFade(MODE_FADE mode,int m_time,D3DXCOLOR color)
{
	m_mode = mode;
	fadeTime = m_time;
	fadeAlpha = color.a*255;
	m_color = color;
	alpha = fadeTime / 256;
}
MODE_FADE CFade::GetModeFade(void)
{
	return m_mode;
}