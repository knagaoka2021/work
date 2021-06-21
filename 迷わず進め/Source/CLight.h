//=============================================================================
//
//  [CLight.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CLIGHT_H
#define _CLIGHT_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "CScene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCENE_MAX	(4)

//*****************************************************************************
// 構造体定義
//*****************************************************************************




class CLight
{
	public:
	CLight(void);
	~CLight(void);


	HRESULT InitLight(LPDIRECT3DDEVICE9 m_pD3DDevice);
	void UninitLight(void);
	void UpdateLight(void);

};

#endif
// EOF