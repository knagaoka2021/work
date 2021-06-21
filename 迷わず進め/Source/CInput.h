//=============================================================================
//
//  [CInput.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CINPUT_H
#define _CINPUT_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#define DIRECTINPUT_VERSION (0x0800)    // 警告対策用
#include <dinput.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define					KEYNUM_MAX			(256)				// キーバッファサイズ
#define					RPEAT_TIME			(30)				// リピートする間隔

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
#define BUTTON_L2		0x00000400l	// L2ボタン(.rgbButtons[6]&0x80)
#define BUTTON_R2		0x00000800l	// R2ボタン(.rgbButtons[7]&0x80)
#define BUTTON_PAUSE	0x00008000l	// PAUSEボタン(.rgbButtons[10]&0x80)
#define BUTTON_START	0x00020000l	// STARTボタン(.rgbButtons[11]&0x80)

#define GAMEPADMAX		2			// 接続するジョイパッドの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************

class CInput
{
	public:
	CInput(void);
	virtual ~CInput(void);


	virtual HRESULT Init(HINSTANCE hInstance,HWND hWnd);

	virtual void Uninit(void);
	virtual void Update(void) = 0;

	static LPDIRECTINPUT8			m_pDInput;						// DirectInputオブジェクトのポインタ
		   LPDIRECTINPUTDEVICE8	m_pDIDevKeyboard;				// デバイスオブジェクトのポインタ(キーボード用)
	int m_count;

};

class CInputKeyboard:public CInput
{
	public :
	HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	void Update(void);								// キー更新
	void Uninit(void);								// キー終了
	static bool GetkeyboardPress(int key);						// キーチェック
	static bool tringkeyboardPress(int key);						// トリガーキーチェック
	static bool ReleasekeyboardPress(int key);					// リリースキーチェック
	static bool RepeatkeyboardPress(int key);						// リピートキーチェック
	void SaveKey(void);
	void LoadKey(void);
	void ActionKey(void);


	//HRESULT InitPad(void);
	//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID );
	//void UpdatePad(void);
	//void UninitPad(void);
	bool PresskeyCheak(void);
};
class CInputPad
{
	public :
	CInputPad(void);
	virtual ~CInputPad(void);
	HRESULT InitPad(HINSTANCE hInstance,HWND hWnd);
	static BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID );
	void UpdatePad(void);
	void UninitPad(void);
	static LPDIRECTINPUT8			m_pDInputPad;						// DirectInputオブジェクトのポインタ

	static bool PresspadCheak(int padNo,DWORD button);
	static bool tringpadPress(int padNo,DWORD button);
};

#endif
// EOF