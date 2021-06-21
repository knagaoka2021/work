//=============================================================================
//
// モデル処理 [model.h]
// Author : 長岡　和真
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
D3DXVECTOR3 GetPos(void);
D3DXVECTOR3 GetRot(void);
float GetRotbullet(void);
float GetPosX(void);
float GetPosZ(void);
void SetHitPosition(D3DXVECTOR3 pos);
void RollModel(void);
bool CollsionCheck(float x1,float y1,float z1,float r1,float x2,float y2,float z2,float r2);
#endif
