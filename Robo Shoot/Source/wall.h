//=============================================================================
//
// ���b�V���E�H�[���̏��� [wall.h]
// Author : �����a�^
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshWall(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,int stetas);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
