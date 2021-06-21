//=============================================================================
//
// メッシュ地面の処理 [meshfield.h]
// Author : 長岡　和真
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
HRESULT SetVtxData(void);
void LoadTexture(void);

#endif
