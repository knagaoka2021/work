//=============================================================================
//
// �f�o�b�O�\������ [debugproc.h]
// Author : ���� �a�^
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(int x,int y);

void PrintDebugProc(const char *fmt,...);

#endif