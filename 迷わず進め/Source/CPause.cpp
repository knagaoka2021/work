//=============================================================================
//
//  [CPause.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CPause.h"
#include "CScene.h"
#include "CScene2D.h"
#include "CInput.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHIFTSIZE	(160)
#define OFSETX	(50)
#define OFSETY	(240)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CScene2D *p1=NULL;
CScene2D *p2=NULL;
CScene2D *p3=NULL;
CPause::CPause(void)
{
};
CPause::~CPause(void)
{
};
static int Pos =0;
PAUSE g_pauseMode = PAUSE_NONE;
//=============================================================================
// ライトの初期化処理
//=============================================================================
HRESULT CPause::Init(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	p1 = CScene2D::Create(m_pD3DDevice,"data\\TEXTURE\\tes.png",D3DXVECTOR3(-SCREEN_WIDTH/6,20,1.0f),D3DXVECTOR3(0,0,0),SCREEN_WIDTH,SCREEN_HEIGHT,D3DCOLOR_RGBA(255,0,0,0));
	p2 = CScene2D::Create(m_pD3DDevice,"data\\TEXTURE\\curs.png",D3DXVECTOR3(0.0f,160.0f,1.0f),D3DXVECTOR3(0,0,0),64.0f,64.0f,D3DCOLOR_RGBA(255,0,0,0));
	p3 = CScene2D::Create(m_pD3DDevice,"data\\TEXTURE\\manu.png",D3DXVECTOR3(SCREEN_WIDTH-(SCREEN_WIDTH/2),0.0f,1.0f),D3DXVECTOR3(0,0,0),SCREEN_WIDTH/2,SCREEN_HEIGHT/2,D3DCOLOR_RGBA(255,0,0,0));
	g_pauseMode = PAUSE_NONE;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	p1->Uninit();
	p2->Uninit();
	p3->Uninit();
	Pos = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	p1->Update();
	p2->Update();
	p3->Update();

	if((CInputPad::tringpadPress(0,BUTTON_DOWN)||CInputKeyboard::tringkeyboardPress(DIK_S))||CInputKeyboard::tringkeyboardPress(DIK_DOWN))
	{
		Pos +=SHIFTSIZE;

		if(Pos>SHIFTSIZE*2)
		{
			Pos = 0;
		}

	}
	if((CInputPad::tringpadPress(0,BUTTON_UP)||CInputKeyboard::tringkeyboardPress(DIK_W))||CInputKeyboard::tringkeyboardPress(DIK_UP))
	{
		Pos -=SHIFTSIZE;

		if(Pos<0.0f)
		{
			Pos = SHIFTSIZE*2;
		}

	}
	p2->SetPos(D3DXVECTOR3(OFSETX,OFSETY+Pos,1.0f));

	if((CInputPad::tringpadPress(0,BUTTON_Y) || CInputKeyboard::tringkeyboardPress(DIK_Z) || CInputKeyboard::tringkeyboardPress(DIK_RETURN)))
	{
		if(Pos==0)
		{
			g_pauseMode = PAUSE_GAME;
		}
		if(Pos==SHIFTSIZE)
		{
			g_pauseMode = PAUSE_RESTART;
		}
		if(Pos==SHIFTSIZE*2)
		{
			g_pauseMode = PAUSE_TITLE;
		}
	}
}
void CPause::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	p1->Draw(m_pD3DDevice);
	p2->Draw(m_pD3DDevice);
	p3->Draw(m_pD3DDevice);
}
PAUSE CPause::GetPauseMode(void)
{
	return g_pauseMode;
}