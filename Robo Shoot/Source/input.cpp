//=============================================================================
//
// キー入力処理 [input.cpp]
// Author : 長岡　和真
//
//=============================================================================
#include "input.h"
#define					KEYNUM_MAX			(256)				// キーバッファサイズ
#define					RPEAT_TIME			(30)				// リピートする間隔
LPDIRECTINPUT8			g_pDInput = NULL;						// DirectInputオブジェクトのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;				// デバイスオブジェクトのポインタ(キーボード用)
BYTE					g_aKeyState[KEYNUM_MAX];				// キーボードの押下状態を受け取るワーク
BYTE					g_TringKeyState[KEYNUM_MAX];			// トリガーキー情報
BYTE					g_ReleseKeyState[KEYNUM_MAX];			// リリースキー情報
BYTE					g_RpeatKeyState[KEYNUM_MAX];			// リピートキー情報
static int				RpeatKey_count[KEYNUM_MAX];				// リピートキー用カウント


#define DEADZONE		2500			
#define RANGE_MAX		1000			
#define RANGE_MIN		-1000			

static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL,NULL };// パッドデバイス

static DWORD	padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD	padTrigger[GAMEPADMAX];
static int		padCount = 0;			// 検出したパッドの数


BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// パッド検査コールバック



//=============================================================================
// キーボード初期化処理
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
    HRESULT hr;

    if (g_pDInput == NULL)
    {
        hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL);

        if (FAILED(hr))
        {
            //メッセージボックスで失敗表示
            return hr;
        }
    }

    hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);

    if (FAILED(hr))
    {
        MessageBox(NULL, "DirectInput8オブジェクトの作成に失敗", "Direct Input Error", MB_OK);
        return hr;
    }

    hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);

    if (FAILED(hr))
    {
        //
        MessageBox(NULL, "DirectInput8オブジェクトの作成に失敗", "Direct Input Error", MB_OK);
    }

    hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    if (FAILED(hr))
    {
        MessageBox(NULL, "DirectInput8オブジェクトの作成に失敗", "Direct Input Error", MB_OK);
        return hr;

    }
    hr = g_pDIDevKeyboard->Acquire();

    return S_OK;
}


//=============================================================================
// キー更新
//=============================================================================
void UpdateKeyBoard(void)
{
    BYTE LastKeyState[256];

    memcpy(LastKeyState, g_aKeyState, KEYNUM_MAX);

    if (SUCCEEDED(g_pDIDevKeyboard->GetDeviceState(sizeof(g_aKeyState), &g_aKeyState[0])))
    {
        for (int nkey = 0; nkey < KEYNUM_MAX; nkey++)
        {
            //トリガーキー情報作成
            g_TringKeyState[nkey] = (LastKeyState[nkey] ^ g_aKeyState[nkey]) & g_aKeyState[nkey];

            //リリースキーー情報作成
            g_ReleseKeyState[nkey] = (LastKeyState[nkey] ^ g_aKeyState[nkey]) & LastKeyState[nkey];

            //リピートキー処理
            if (g_aKeyState[nkey])											//今押されているキー
            {
                if (RpeatKey_count[nkey] == 0)								// カウント0なら
                {
                    g_RpeatKeyState[nkey] = g_aKeyState[nkey];				// ON
                }
                else
                {
                    g_RpeatKeyState[nkey] = 0;
                }

                RpeatKey_count[nkey]++;

                if (RpeatKey_count[nkey] > RPEAT_TIME)						// 一定フレームこえたなら
                {
                    g_RpeatKeyState[nkey] = g_aKeyState[nkey];				// ON
                }
            }
            else
            {
                g_RpeatKeyState[nkey] = 0;

                RpeatKey_count[nkey] = 0;
            }
        }
    }
    else
    {
        g_pDIDevKeyboard->Acquire();
    }

}

//=============================================================================
// 押されているキーチェック
//=============================================================================
bool GetkeyboardPress(int key)
{
    if (g_aKeyState[key] == 0x80)
    {
        return true;
    }
    return false;
}

//=============================================================================
// トリガーキーチェック
//=============================================================================
bool tringkeyboardPress(int key)
{
    if (g_TringKeyState[key] == 0x80)
    {
        g_TringKeyState[key] = 0;
        return true;
    }
    return false;
}

//=============================================================================
// リリースキーチェック
//=============================================================================
bool ReleasekeyboardPress(int key)
{
    return g_ReleseKeyState[key] & 0x80;
}

//=============================================================================
// リピートキーチェック
//=============================================================================
bool RepeatkeyboardPress(int key)
{
    return g_RpeatKeyState[key] & 0x80;
}

//=============================================================================
// キー終了
//=============================================================================
void Uninitkeyboard(void)
{
    if (g_pDInput != NULL)
    {
        g_pDInput->Release();
        g_pDInput = NULL;
    }
    if (g_pDIDevKeyboard != NULL)
    {
        g_pDIDevKeyboard->Unacquire();
        g_pDIDevKeyboard->Release();
        g_pDIDevKeyboard = NULL;
    }
}
//=============================================================================
// キーが押されているかのチェック
//=============================================================================
bool PresskeyCheak(void)
{
    for (int i = 0; i < 256; i++)
    {
        if (g_aKeyState[i] == 0x80)
        {
            return true;
        }
    }
    return false;
}
//=============================================================================
// ゲームパッド
//=============================================================================
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
    HRESULT result;

    result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
    return DIENUM_CONTINUE;	// 次のデバイスを列挙

}
//=============================================================================
// パッド初期化
//=============================================================================
HRESULT InitPad(void)
{
    HRESULT		result;
    int			i;

    padCount = 0;
    // ジョイパッドを探す
    g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
    // セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

    for (i = 0; i < padCount; i++) {
        // ジョイスティック用のデータ・フォーマットを設定
        result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
        if (FAILED(result))
            return false; // データフォーマットの設定に失敗

        // モードを設定（フォアグラウンド＆非排他モード）
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // モードの設定に失敗

        // 軸の値の範囲を設定
        // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
        // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
        // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
        DIPROPRANGE				diprg;
        ZeroMemory(&diprg, sizeof(diprg));
        diprg.diph.dwSize = sizeof(diprg);
        diprg.diph.dwHeaderSize = sizeof(diprg.diph);
        diprg.diph.dwHow = DIPH_BYOFFSET;
        diprg.lMin = RANGE_MIN;
        diprg.lMax = RANGE_MAX;
        // X軸の範囲を設定
        diprg.diph.dwObj = DIJOFS_X;
        pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
        // Y軸の範囲を設定
        diprg.diph.dwObj = DIJOFS_Y;
        pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

        // 各軸ごとに、無効のゾーン値を設定する。
        // 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
        // 指定する値は、10000に対する相対値(2000なら20パーセント)。
        DIPROPDWORD				dipdw;
        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
        dipdw.diph.dwHow = DIPH_BYOFFSET;
        dipdw.dwData = DEADZONE;
        //X軸の無効ゾーンを設定
        dipdw.diph.dwObj = DIJOFS_X;
        pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Y軸の無効ゾーンを設定
        dipdw.diph.dwObj = DIJOFS_Y;
        pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

        //ジョイスティック入力制御開始
        pGamePad[i]->Acquire();
    }

    return true;

}
//=============================================================================
// パッド終了処理
//=============================================================================
void UninitPad(void)
{
    for (int i = 0; i < GAMEPADMAX; i++) {
        if (pGamePad[i])
        {
            pGamePad[i]->Unacquire();
            pGamePad[i]->Release();
        }
    }

}

//=============================================================================
// パッド更新
//=============================================================================
void UpdatePad(void)
{
    HRESULT			result;
    DIJOYSTATE2		dijs;
    int				i;

    for (i = 0; i < padCount; i++)
    {
        DWORD lastPadState;
        lastPadState = padState[i];
        padState[i] = 0x00000000l;	// 初期化

        result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
        if (FAILED(result)) {
            result = pGamePad[i]->Acquire();
            while (result == DIERR_INPUTLOST)
                result = pGamePad[i]->Acquire();
        }

        result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
        if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
            result = pGamePad[i]->Acquire();
            while (result == DIERR_INPUTLOST)
                result = pGamePad[i]->Acquire();
        }

        // ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
        //* y-axis (forward)
        if (dijs.lY < 0)
        {
            padState[i] |= BUTTON_UP;
        }
        //* y-axis (backward)
        if (dijs.lY > 0)
        {
            padState[i] |= BUTTON_DOWN;
        }
        //* x-axis (left)
        if (dijs.lX < 0)
        {
            padState[i] |= BUTTON_LEFT;
        }
        //* x-axis (right)
        if (dijs.lX > 0)
        {
            padState[i] |= BUTTON_RIGHT;
        }
        if (dijs.lY > 0 && dijs.lX < 0)
        {
            padState[i] |= BUTTON_DL;
        }
        if (dijs.lY < 0 && dijs.lX < 0)
        {
            padState[i] |= BUTTON_UL;
        }
        if (dijs.lY > 0 && dijs.lX > 0)
        {
            padState[i] |= BUTTON_DR;
        }
        if (dijs.lY < 0 && dijs.lX > 0)
        {
            padState[i] |= BUTTON_UR;
        }

        //* Ａボタン
        if (dijs.rgbButtons[0] & 0x80)
        {
            padState[i] |= BUTTON_A;
        }
        //* Ｂボタン
        if (dijs.rgbButtons[1] & 0x80)
        {
            padState[i] |= BUTTON_B;
        }
        //* Xボタン
        if (dijs.rgbButtons[2] & 0x80)
        {
            padState[i] |= BUTTON_X;
        }
        //* Yボタン
        if (dijs.rgbButtons[3] & 0x80)
        {
            padState[i] |= BUTTON_Y;
        }
        //* Lボタン
        if (dijs.rgbButtons[4] & 0x80)
        {
            padState[i] |= BUTTON_L;
        }
        //* Rボタン
        if (dijs.rgbButtons[5] & 0x80)
        {
            padState[i] |= BUTTON_R;
        }
        //* SELECTボタン
        if (dijs.rgbButtons[6] & 0x80)
        {
            padState[i] |= BUTTON_SELECT;
        }
        //* STARTボタン
        if (dijs.rgbButtons[7] & 0x80)
        {
            padState[i] |= BUTTON_START;
        }

        // Trigger設定
        padTrigger[i] = ((lastPadState ^ padState[i])     // 前回と違っていて
            & padState[i]);				  // しかも今ONのやつ

    }

}
//=============================================================================
// パッドが押されているかのチェック
//=============================================================================
bool PresspadCheak(int padNo, DWORD button)
{
    return (button & padState[padNo]);
}
//=============================================================================
// トリガーパッドチェック
//=============================================================================
bool tringpadPress(int padNo, DWORD button)
{
    return (button & padTrigger[padNo]);
}