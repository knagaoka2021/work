//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : �����@�a�^
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

#define START_ENEMY_NUM (50)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
D3DXVECTOR3 GetPosEnemy(void);
D3DXVECTOR3 GetRotdateEnemy(void);
float GetRotEnemy(void);
float GetPosEnemyX(void);
float GetPosEnemyZ(void);
void EnemyPosSet(float x,float y,float z);
#endif
