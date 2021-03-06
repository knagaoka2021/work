//=============================================================================
//
//  [CTitle.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CTITLE_H
#define _CTITLE_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "CCamera.h"
#include "CPlayerMotion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************

class CTitle
{
	public:
	CTitle(void);
	~CTitle(void);


	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,CCamera *p);
	void Uninit(void);
	void Update(void);
	static CPlayerMotion* addrPresent(void);
};

#endif
// EOF