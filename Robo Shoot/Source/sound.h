//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 長岡　和真
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_LASER,		// レーザー発射音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_HIT000,		// 衝突音0
	SOUND_LABEL_SE_HIT001,		// 衝突音1
	SOUND_LABEL_SE_DEFEND,		// 防御音
	SOUND_LABEL_SE_LOCKON,		// ロックオン音
	SOUND_LABEL_SE_SYSTEM,		// システム音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label,UINT32 LoopCHK);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
