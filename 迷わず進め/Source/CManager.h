//=============================================================================
//
//  [CManager.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CMANAGER_H
#define _CMANAGER_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "CTitle.h"
#include "CGame.h"
#include "CResult.h"
#include <process.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
//  列挙体
//*****************************************************************************
typedef enum
{
	PHASE_TITLE = 0,		// タイトル画面
	PHASE_GAME,				// ゲーム画面
	PHASE_RESULT,			// リザルト画
	PHASE_TUTO,
	PHASE_GAMERESTART,
	PHASE_PAUSE,
	PHASE_MAX,				// 
}PHASE;

class CRenderer;



class CManager
{
	public:
	CManager(void);
	~CManager(void);

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd , BOOL bWindow);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static void SetPhase(PHASE m_phase);
	PHASE GetPhase(void);
	static void ChangePhase(void);

	static CRenderer* GetRenderer(void)
	{
		return m_pRenderer;
	};


	static LPDIRECT3DDEVICE9 GetDevice(void);

	static bool m_bLowdingflag;
	static void SetLowdingflag(bool lowdingflag);
	static bool GetLowdingflag(void);
	static  unsigned int __stdcall ReceiveThread( void* );
	private:
		static CRenderer* m_pRenderer;
		static CTitle *m_pTitle;
		static CGame *m_pGame;
		static CResult *m_pResult;
		
		

};


#endif