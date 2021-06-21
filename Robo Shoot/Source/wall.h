//=============================================================================
//
// メッシュウォールの処理 [wall.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshWall(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,int stetas);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
