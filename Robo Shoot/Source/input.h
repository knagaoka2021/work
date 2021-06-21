//=============================================================================
//
// キー入力処理 [input.h]
// Author : 長岡　和真
//
//=============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void UpdateKeyBoard(void);								// キー更新
void Uninitkeyboard(void);								// キー終了
bool GetkeyboardPress(int key);						// キーチェック
bool tringkeyboardPress(int key);						// トリガーキーチェック
bool ReleasekeyboardPress(int key);					// リリースキーチェック
bool RepeatkeyboardPress(int key);						// リピートキーチェック

HRESULT InitPad(void);
BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID );
void UpdatePad(void);
void UninitPad(void);
bool PresskeyCheak(void);

#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)
#define BUTTON_UR		0x10000000l	// 方向キー右上
#define BUTTON_DR		0x20000000l	// 方向キー右下
#define BUTTON_UL		0x30000000l	// 方向キー左上
#define BUTTON_DL		0x40000000l	// 方向キー左下
#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_X		0x00000040l	// Xボタン(.rgbButtons[2]&0x80)
#define BUTTON_Y		0x00000080l	// Yボタン(.rgbButtons[3]&0x80)
#define BUTTON_L		0x00000100l	// Lボタン(.rgbButtons[4]&0x80)
#define BUTTON_R		0x00000200l	// Rボタン(.rgbButtons[5]&0x80)
#define BUTTON_SELECT		0x00000400l	// SELECTボタン(.rgbButtons[6]&0x80)
#define BUTTON_START		0x00000800l	// STARTボタン(.rgbButtons[7]&0x80)
#define GAMEPADMAX		2			// 接続するジョイパッドの最大数
bool PresspadCheak(int padNo,DWORD button);
bool tringpadPress(int padNo,DWORD button);



#endif