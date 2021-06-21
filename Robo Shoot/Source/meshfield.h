//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.h]
// Author : �����@�a�^
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
HRESULT SetVtxData(void);
void LoadTexture(void);

#endif
