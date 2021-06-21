//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
// Author : 長岡 和真
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(int x,int y);

void PrintDebugProc(const char *fmt,...);

#endif