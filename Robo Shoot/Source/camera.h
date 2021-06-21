//=============================================================================
//
// �J�������� [camera.h]
// Author : ���� �a�^
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(int nIndex);

void SetCamera(int nIndex);
D3DXVECTOR3 GetRotCamera(void);
D3DXMATRIX GetMtxView(void);
float GetView(void);


#endif
