//=============================================================================
//
// メイン処理 [main.h]
// Author : 長岡　和真
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)    // 警告対策用
#include "dinput.h"
#include "xaudio2.h"

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(768)				// ウインドウの高さ

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

//*****************************************************************************
//  列挙体
//*****************************************************************************
typedef enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画
	MODE_TUTO,
	MODE_MAX,				// 
}MODE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
void PauseInitGame(void);
void PauseUninitGame(void);
void SetPauseflag(bool flag);
void SetWindowflag(bool flag);


#endif