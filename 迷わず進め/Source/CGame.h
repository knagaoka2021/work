//=============================================================================
//
//  [CGame.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CGAME_H
#define _CGAME_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "CCamera.h"
#include "CPlayerMotion.h"
#include "CLife.h"
#include "CObjModel.h"
#include "CGimmick.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	GAME_NONE,
	GAME_CLEAR,
	GAME_OVER,
	GAME_MAX,
}GAMEJUDG;


class CGame
{
	public:
	CGame(void);
	~CGame(void);


	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,CCamera *p);
	void Uninit(void);
	void Update(void);
	static CPlayerMotion* addrPresent(void);
	static CLife* CLifeAddrPresent(void);
	static void CEnemyAddrDlete(int num);
	static void SetPresent(void);
	static bool Enemy(void);
	static CObjModel *CObjModelAddrPresent(int num);
	static CGimmick *CTestAddrPresentAddrPresent(int num);
	static void CObjModelAddrDlete(int num);
	static int GetCObjModelNum(void);
	static int ObjCount;
};
GAMEJUDG GetJudg(void);
void SetJudg(GAMEJUDG judg);

#endif
// EOF