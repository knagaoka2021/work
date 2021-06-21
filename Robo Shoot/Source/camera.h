//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 長岡 和真
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(int nIndex);

void SetCamera(int nIndex);
D3DXVECTOR3 GetRotCamera(void);
D3DXMATRIX GetMtxView(void);
float GetView(void);


#endif
