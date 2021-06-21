//=============================================================================
//
//  [CManager.cpp]
// Author : 長岡和真
//
//=============================================================================

#include "CManager.h"
#include "CRenderer.h"
#include "CDebugproc.h"
#include "CCamera.h"
#include "CLight.h"
#include "CInput.h"
#include "CScene.h"
#include "CScene2D.h"
#include "CScene3D.h"
#include "CSceneX.h"
#include "CSceneBillboard.h"
#include "CMeshField.h"
#include "CMeshWall.h"
#include "CCylinder.h"
#include "CSkydome.h"
#include "CObjModel.h"
#include "CObjBillboard.h"
#include "CScore.h"
#include "CTimer.h"
#include "CFade.h"
#include "CPause.h"
#include "Csound.h"
#include"CMotionModel.h"
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
CRenderer* CManager::m_pRenderer = NULL;
CCamera *g_pCamera =  NULL;
CLight *g_pLight = NULL;
CInputKeyboard *g_pInput = NULL;
CInputPad *g_pInputPad = NULL;
CDebugproc *g_pDebug = NULL;
CPlayer *g_pPlayer = NULL;
CPlayerMotion *g_pMotion = NULL;
CFade *g_fade = NULL;
CPause *g_pause = NULL;
PHASE g_mode = PHASE_TITLE;
PHASE g_modeLast = PHASE_TITLE;
PAUSE g_phase = PAUSE_NONE;
int g_cameramode =0;
GAMEJUDG g_gamejudg = GAME_NONE;
CTitle* CManager::m_pTitle = NULL;
CGame* CManager::m_pGame = NULL;
CResult* CManager::m_pResult = NULL;
CThread2D *gThread = NULL;
CThread2D *gThread2 = NULL;

int Time = 0;
bool CManager::m_bLowdingflag=FALSE;
bool startLoadflag = false;
//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager(void)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager(void)
{
}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstanse,HWND hWnd , BOOL bWindow)
{
	int x=0,y=0,z=0;


	m_pRenderer = new CRenderer;

	if(FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	g_pInput = new CInputKeyboard;

	if(FAILED(g_pInput->Init(hInstanse, hWnd)))
	{
		return -1;
	}

	g_pInputPad = new CInputPad;
	if(FAILED(g_pInputPad->InitPad(hInstanse, hWnd)))
	{
		return -1;
	}


	g_pDebug = new CDebugproc();
	g_pDebug->InitDebugProc(m_pRenderer->GetDevice());

	g_pCamera = new CCamera;
	g_pCamera->InitCamera();
	g_pCamera->SetClass(g_pCamera);

	g_pLight = new CLight;
	g_pLight->InitLight(m_pRenderer->GetDevice());

	g_fade = CFade::Create(m_pRenderer->GetDevice(),NULL,D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),FADE_NONE,600,D3DCOLOR_RGBA(255,0,0,255));
	
	InitSound(hWnd);
			
	PlaySound(SOUND_LABEL_BGM000,XAUDIO2_LOOP_INFINITE);

	SetVolume(0.001f);

	CreateTex(m_pRenderer->GetDevice(),"data\\TEXTURE\\exp.png.png");
	g_pInput->LoadKey();

	g_mode = PHASE_TITLE;

	switch(g_mode)
	{
	case PHASE_TITLE:
		m_pTitle = new CTitle;
		m_pTitle->Init(m_pRenderer->GetDevice(),g_pCamera);
		g_pMotion = m_pTitle->addrPresent();
		g_pCamera->InitCamera();
	//CTITLEInit
	break;

	}

	gThread = CThread2D::Create(GetDevice(),"data\\TEXTURE\\nowloding.png",D3DXVECTOR3((SCREEN_WIDTH/2)+SCREEN_WIDTH/5,SCREEN_HEIGHT-128.0f,1.0f),D3DXVECTOR3(0,0,0),250.0f,128.0f,D3DCOLOR_RGBA(255,0,0,0),TYPE2D_NOMAR);
	gThread2 = CThread2D::Create(GetDevice(),"data\\TEXTURE\\loding.png",D3DXVECTOR3(SCREEN_WIDTH-128.0f,SCREEN_HEIGHT-128.0f,1.0f),D3DXVECTOR3(0,0,0),128.0f,128.0f,D3DCOLOR_RGBA(255,0,0,0),TYPE2D_ANIMATION);
	

	// スレッド開始
	_beginthreadex( NULL, 0, ReceiveThread, NULL, 0, NULL );
	m_bLowdingflag = FALSE;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	switch(g_mode)
	{
	//CPAUSE
	case PHASE_TITLE:
		m_pTitle->Uninit();
		delete m_pTitle;
		if(m_pTitle != NULL)
		{
			m_pTitle = NULL;
		}
		
		
	break;
		case PHASE_GAME:
		m_pGame->Uninit();
		delete m_pGame;
		g_pPlayer = NULL;
		if(m_pGame != NULL)
		{
			m_pGame = NULL;
		}
		break;
	case PHASE_RESULT:
		m_pResult->Uninit();
		delete m_pResult;
		if(m_pResult != NULL)
		{
			m_pResult = NULL;
		}

	break;
	}

	m_pRenderer->Uninit();

	g_pInput->Uninit();

	g_pInputPad->UninitPad();

	g_pCamera->UninitCamera();

	g_pLight->UpdateLight();

	g_pDebug->UninitDebugProc();

	UninitSound();

	if(gThread !=NULL)
	{
		gThread->Uninit();
		gThread = NULL;
	}
	if(gThread2!=NULL)
	{
		gThread2->Uninit();
		gThread2 = NULL;
	}

	TexUninit();
	delete m_pRenderer;
	delete g_pCamera;
	delete g_pLight;
	delete g_pInput;
	delete g_pInputPad;
	delete g_pDebug;

	CScene::ReleseAll();


}
//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	CPlayerMotion *motion = NULL;

	if (CInputKeyboard::tringkeyboardPress(DIK_F2))
	{
		if (!GetDebug())
		{
			SetDebug(TRUE);
		}
		else
		{
			SetDebug(FALSE);
		}
	}

	switch(g_mode)
	{
		case PHASE_TITLE:
		motion = m_pTitle->addrPresent();
		break;

		case PHASE_GAME:
		motion = m_pGame->addrPresent();
		break;

		case PHASE_RESULT:
		break;
	}

	// 入力更新
	g_pInput->Update();
	
	if(g_mode==PHASE_TITLE)
	{
		g_pInput->ActionKey();
	}
	g_pInputPad->UpdatePad();

	g_pDebug->UpdateDebugProc();

	if(!m_bLowdingflag)
	{

	g_modeLast = GetPhase();


	switch(g_mode)
	{
		case PHASE_TITLE:
			m_pTitle->Update();

		//CTITLE
		if((CInputKeyboard::GetkeyboardPress(DIK_RETURN)||CInputPad::tringpadPress(0,BUTTON_Y))&&g_fade->GetModeFade()==FADE_NONE)
		{
			//FADEIN
			PlaySound(SOUND_LABEL_SE_SYSTEM,0);
			g_fade->StartFade(FADE_OUT,600,D3DCOLOR_RGBA(255,0,0,0));

			//MODE_FADE
			g_phase = PAUSE_NONE;
		}
		if(g_fade->GetModeFade() == FADE_OUTEND)
		{
			SetPhase(PHASE_GAME);
			g_fade->StartFade(FADE_IN,256,D3DCOLOR_RGBA(255,0,0,255));
			CManager::SetLowdingflag(TRUE);

		}
		break;
		case PHASE_GAME:
		//リザルトに行く処理
		if((timerflag()==TRUE||GetJudg()==GAME_OVER||GetJudg()==GAME_CLEAR)&&g_fade->GetModeFade()==FADE_NONE)
		{
			g_gamejudg = GetJudg();
			g_cameramode = 2;
			if(g_gamejudg== GAME_NONE)
			{
				g_gamejudg = GAME_OVER;
				g_cameramode = 0;
			}
			
			//FADEIN
			g_fade->StartFade(FADE_OUT,600,D3DCOLOR_RGBA(255,0,0,0));

			//MODE_FADE
			g_phase = PAUSE_NONE;
		}
		if(g_fade->GetModeFade() == FADE_OUTEND&&g_phase == PAUSE_NONE)
		{
			SetPhase(PHASE_RESULT);
			g_fade->StartFade(FADE_IN,256,D3DCOLOR_RGBA(255,0,0,255));
			CManager::SetLowdingflag(TRUE);

		}

		//リスタート
		if(g_fade->GetModeFade() == FADE_OUTEND&&g_phase == PAUSE_RESTART)
		{

			SetPhase(PHASE_GAMERESTART);
			g_phase = PAUSE_NONE;

			g_pPlayer = NULL;
			if(m_pGame != NULL)
			{
				m_pGame->Uninit();
				delete m_pGame;
				m_pGame = NULL;
			}
			g_fade->StartFade(FADE_IN,256,D3DCOLOR_RGBA(255,0,0,255));
			g_modeLast=PHASE_GAME;
			CManager::SetLowdingflag(TRUE);



			break;
		}
		//タイトル
		if(g_fade->GetModeFade() == FADE_OUTEND&&g_phase == PAUSE_TITLE)
		{
			
 			SetPhase(PHASE_TITLE);
			g_phase = PAUSE_NONE;
			g_pPlayer = NULL;
			if(m_pGame != NULL)
			{
				m_pGame->Uninit();
				delete m_pGame;
				m_pGame = NULL;
			}
			g_fade->StartFade(FADE_IN,256,D3DCOLOR_RGBA(255,0,0,255));
			g_modeLast=PHASE_RESULT;
			StopSound(SOUND_LABEL_BGM003);
			CManager::SetLowdingflag(TRUE);

			break;
		}
		if((CInputKeyboard::tringkeyboardPress(DIK_P)||CInputPad::tringpadPress(0,BUTTON_START))&&g_phase == PAUSE_NONE&&g_fade->GetModeFade()==FADE_NONE)
		{
			SetPhase(PHASE_PAUSE);
			g_modeLast = PHASE_PAUSE;
			g_pause = new CPause;
			g_pause->Init(m_pRenderer->GetDevice());
		}
		m_pGame->Update();
		break;

		case PHASE_RESULT:
		m_pResult->Update();
		if(CInputKeyboard::tringkeyboardPress(DIK_Z)&&g_fade->GetModeFade()==FADE_NONE)
		{
			//FADEIN
			g_fade->StartFade(FADE_OUT,600,D3DCOLOR_RGBA(255,0,0,0));
			//MODE_FADE
		}
		if(m_pResult->GetTime()<=0&&g_fade->GetModeFade() == FADE_NONE)
		{
			//FADEIN
			g_fade->StartFade(FADE_OUT,600,D3DCOLOR_RGBA(255,0,0,0));
			//MODE_FADE
		}

		if(g_fade->GetModeFade() == FADE_OUTEND)
		{

			SetPhase(PHASE_TITLE);
			g_fade->StartFade(FADE_IN,256,D3DCOLOR_RGBA(255,0,0,255));
			CManager::SetLowdingflag(TRUE);

		}

		break;
		case PHASE_PAUSE:

		g_pause->Update();

		PAUSE PauseMode = g_pause->GetPauseMode();

		if((CInputPad::tringpadPress(0,BUTTON_PAUSE)||CInputKeyboard::tringkeyboardPress((DIK_X)))||PauseMode == PAUSE_GAME)
		{
			SetPhase(PHASE_GAME);
			g_pause->Uninit();
			delete g_pause;
			g_pause = NULL;
		}
		if(PauseMode == PAUSE_RESTART)
		{
			g_phase = PAUSE_RESTART;
			SetPhase(PHASE_GAME);
			g_pause->Uninit();
			delete g_pause;
			g_pause = NULL;
			g_fade->StartFade(FADE_OUT,600,D3DCOLOR_RGBA(255,0,0,0));
		}
		if(PauseMode == PAUSE_TITLE)
		{
			g_phase = PAUSE_TITLE;
			SetPhase(PHASE_GAME);
			g_pause->Uninit();
			delete g_pause;
			g_pause = NULL;
			g_fade->StartFade(FADE_OUT,600,D3DCOLOR_RGBA(255,0,0,0));
		}

		break;
	}

		if(g_mode!=PHASE_PAUSE&&!m_bLowdingflag)
		{
			// カメラ更新
			g_pCamera->UpdateCamera(g_cameramode,motion);
			m_pRenderer->Update();
		}
	}

			if(g_mode != PHASE_PAUSE&&g_modeLast != PHASE_PAUSE)
			{
				if(g_mode != g_modeLast)
				{
					CManager::ChangePhase();
				}
			}


}
//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if(!m_bLowdingflag)
	{
		if(g_pCamera !=NULL)
		{
			g_pCamera->SetCamera(g_cameramode,GetDevice());
		}

		m_pRenderer->Draw();
	}


}
LPDIRECT3DDEVICE9 CManager::GetDevice(void)
{
	return m_pRenderer->GetDevice();
}
void CManager::SetPhase(PHASE phase)
{
	g_mode = phase;
}
PHASE CManager::GetPhase(void)
{
	return g_mode;
}
void CManager::ChangePhase(void)
{

		switch(g_mode)
		{
			case PHASE_TITLE:

				if(m_pResult != NULL)
				{
					m_pResult->Uninit();
					delete m_pResult;
					m_pResult = NULL;
				}
				g_cameramode = 0;
				m_pTitle = new CTitle;
				gThread->SetVisibleflag(TRUE);
				gThread2->SetVisibleflag(TRUE);
				StopSound(SOUND_LABEL_BGM004);
				PlaySound(SOUND_LABEL_BGM000,XAUDIO2_LOOP_INFINITE);
				m_pTitle->Init(m_pRenderer->GetDevice(),g_pCamera);
				gThread->SetVisibleflag(FALSE);
				gThread2->SetVisibleflag(FALSE);
				g_pMotion = m_pTitle->addrPresent();
				g_pCamera->InitCamera();
				CManager::SetLowdingflag(FALSE);
				g_modeLast = PHASE_TITLE;
				g_pCamera->InitCamera();

			break;

			case PHASE_GAME:

			if(g_phase != PAUSE_RESTART)
			{
				g_cameramode = 0;
				if(m_pTitle != NULL)
				{
					m_pTitle->Uninit();
					delete m_pTitle;
					m_pTitle = NULL;
				}

				m_pGame = new CGame;
				gThread->SetVisibleflag(TRUE);
				gThread2->SetVisibleflag(TRUE);
				m_pGame->Init(m_pRenderer->GetDevice(),g_pCamera);
				gThread->SetVisibleflag(FALSE);
				gThread2->SetVisibleflag(FALSE);
				StopSound(SOUND_LABEL_BGM000);
				PlaySound(SOUND_LABEL_BGM003,XAUDIO2_LOOP_INFINITE);
				g_pMotion = m_pGame->addrPresent();
				CManager::SetLowdingflag(FALSE);
				g_modeLast = PHASE_GAME;
				g_pCamera->InitCamera();
				startLoadflag = FALSE;
			}
			else
			{
				g_phase = PAUSE_NONE;
			}

			//CRESULT
			break;

			case PHASE_RESULT:

			g_cameramode = 1;

			g_pPlayer = NULL;
			if(m_pGame != NULL)
			{
				m_pGame->Uninit();
				delete m_pGame;
				m_pGame = NULL;
			}
			m_pResult = new CResult;
			gThread->SetVisibleflag(TRUE);
			gThread2->SetVisibleflag(TRUE);
			m_pResult->Init(GetDevice(),g_gamejudg);
			gThread->SetVisibleflag(FALSE);
			gThread2->SetVisibleflag(FALSE);
			StopSound(SOUND_LABEL_BGM003);
			PlaySound(SOUND_LABEL_BGM004,XAUDIO2_LOOP_INFINITE);
			CManager::SetLowdingflag(FALSE);
			g_modeLast = PHASE_RESULT;
			//CTITLE
			g_pCamera->InitCamera();
			break;

			case PHASE_GAMERESTART:

			CManager::SetPhase(PHASE_GAME);
			m_pGame = new CGame;
			gThread->SetVisibleflag(TRUE);
			gThread2->SetVisibleflag(TRUE);
			m_pGame->Init(m_pRenderer->GetDevice(),g_pCamera);
			gThread->SetVisibleflag(FALSE);
			gThread2->SetVisibleflag(FALSE);
			g_modeLast = PHASE_GAME;
			CManager::SetLowdingflag(FALSE);
			g_pCamera->InitCamera();
			break;

			case PHASE_PAUSE:

			break;

		}

		
}
//=============================================================================
// スレッド
//=============================================================================
unsigned int __stdcall CManager::ReceiveThread( void* Param )
{
	
	while( true )
	{
			if(m_bLowdingflag)
			{
				startLoadflag = TRUE;
				gThread2->Update();

				LPDIRECT3DDEVICE9 l_pD3DDevice = m_pRenderer->GetDevice();

				if(l_pD3DDevice)
				{
					l_pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET),D3DCOLOR_ARGB(255,0,0,0),1.0f,0);

					if(SUCCEEDED(l_pD3DDevice->BeginScene()))
					{
						gThread->Draw(l_pD3DDevice);
						gThread2->Draw(l_pD3DDevice);
						l_pD3DDevice->EndScene();
					}
				l_pD3DDevice->Present(NULL,NULL,NULL,NULL);
				}
			}
	
	}

	return 0;

}
void CManager::SetLowdingflag(bool lowdingflag)
{
	m_bLowdingflag =lowdingflag;
}
bool CManager::GetLowdingflag(void)
{
	return  m_bLowdingflag;
}