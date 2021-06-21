//=============================================================================
//
//  [CInput.h]
// Author : 長岡和真
//
//=============================================================================
#include "CInput.h"
#include<stdio.h>
#include<stdlib.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COMMAND_MAX (8)
#define TIME_MAX		(3800)
#define DEADZONE		2500			
#define RANGE_MAX		1000			
#define RANGE_MIN		-1000	
//*****************************************************************************
// 列挙体定義
//*****************************************************************************
enum
{
	COMMAND_ATTACK = 1<<0,
	COMMAND_JUMP = 1<<1,
	COMMAND_UP = 1<<2,
	COMMAND_DOWN = 1<<3,
	COMMAND_RIGHT = 1<<4,
	COMMAND_LEFT = 1<<5,
	COMMAND_KICK = 1<<6,
	COMMAND_DAMAGE = 1<<7,
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct Key2Com	
{
	unsigned char Key;
	unsigned char command;
};
Key2Com K2CList[COMMAND_MAX] ={
		{DIK_SPACE,COMMAND_JUMP},
		{DIK_X,COMMAND_ATTACK},
		{DIK_W,COMMAND_UP},
		{DIK_S,COMMAND_DOWN},
		{DIK_A,COMMAND_LEFT},
		{DIK_D,COMMAND_RIGHT},
		{DIK_2,COMMAND_KICK},
		{DIK_3,COMMAND_DAMAGE},
	};
//*****************************************************************************
// グローバル変数
//*****************************************************************************
unsigned char Keycommand[TIME_MAX];
unsigned char KeycommandLOAD[TIME_MAX];

BYTE					g_aKeyState[KEYNUM_MAX];				// キーボードの押下状態を受け取るワーク
BYTE					g_TringKeyState[KEYNUM_MAX];			// トリガーキー情報
BYTE					g_ReleseKeyState[KEYNUM_MAX];			// リリースキー情報
BYTE					g_RpeatKeyState[KEYNUM_MAX];			// リピートキー情報
static int				RpeatKey_count[KEYNUM_MAX];				// リピートキー用カウント

LPDIRECTINPUT8			g_pDInput = NULL;						// DirectInputオブジェクトのポインタ


		


static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = {NULL,NULL};// パッドデバイス

static DWORD	padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD	padTrigger[GAMEPADMAX];
static int		padCount = 0;			// 検出したパッドの数




LPDIRECTINPUT8 CInput::m_pDInput = NULL;
LPDIRECTINPUT8 CInputPad::m_pDInputPad = NULL;


CInput::CInput(void)
{
}
CInput::~CInput(void)
{
}

HRESULT CInput::Init(HINSTANCE hInstance,HWND hWnd)
{
	HRESULT hr;

	m_count = 0;
	if(m_pDInput == NULL)
	{
		hr = DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void **)&m_pDInput,NULL);

		if(FAILED(hr))
		{
			//メッセージボックスで失敗表示
			return hr;
		}
	}

	hr = m_pDInput->CreateDevice(GUID_SysKeyboard,&m_pDIDevKeyboard,NULL);

	if(FAILED(hr))
	{
		MessageBox(NULL,"DirectInput8オブジェクトの作成に失敗","Direct Input Error",MB_OK);
		return hr;
	}

	hr=m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);

	if(FAILED(hr))
	{
			//
		MessageBox(NULL,"DirectInput8オブジェクトの作成に失敗","Direct Input Error",MB_OK);
	}

	hr=m_pDIDevKeyboard->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if(FAILED(hr))
	{
		MessageBox(NULL,"DirectInput8オブジェクトの作成に失敗","Direct Input Error",MB_OK);
		return hr;
		
	}
	hr = m_pDIDevKeyboard->Acquire();

	return S_OK;
}
HRESULT CInputKeyboard::Init(HINSTANCE hInstance,HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	return S_OK;
}

void CInput::Uninit(void)
{
}

void CInputKeyboard::Update(void)
{
	static int		m_RpeatKey_count[KEYNUM_MAX];				// リピートキー用カウント

	BYTE LastKeyState[KEYNUM_MAX];

	memcpy(LastKeyState,g_aKeyState,KEYNUM_MAX);

	if(SUCCEEDED(m_pDIDevKeyboard->GetDeviceState(sizeof(g_aKeyState),&g_aKeyState[0])))
	{
		for(int nkey=0;nkey<KEYNUM_MAX;nkey++)
		{
			//トリガーキー情報作成
			g_TringKeyState[nkey] = (LastKeyState[nkey] ^ g_aKeyState[nkey])&g_aKeyState[nkey];

			//リリースキーー情報作成
			g_ReleseKeyState[nkey] = (LastKeyState[nkey] ^ g_aKeyState[nkey])&LastKeyState[nkey];

			//リピートキー処理
			if(g_aKeyState[nkey])											//今押されているキー
			{
				if(RpeatKey_count[nkey] == 0)								// カウント0なら
				{
					g_RpeatKeyState[nkey] = g_aKeyState[nkey];				// ON
				}
				else
				{
					g_RpeatKeyState[nkey] = 0;
				}

				RpeatKey_count[nkey]++;

				if(RpeatKey_count[nkey] > RPEAT_TIME)						// 一定フレームこえたなら
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
		m_pDIDevKeyboard->Acquire();
	}
}								// キー更新
void CInputKeyboard::Uninit(void)
{
	if(m_pDInput != NULL)
	{
		m_pDInput->Release();
		m_pDInput = NULL;
	}
	if(m_pDIDevKeyboard != NULL)
	{
		m_pDIDevKeyboard->Unacquire();
		m_pDIDevKeyboard->Release();
		m_pDIDevKeyboard = NULL;
	}

	/*FILE *fp;

	fp=fopen("デモ.bin","wb");

	if(fp==NULL)
	{
		printf("オープンエラーです\n");
		exit(0);
	}
	char poi[3800];
	for(int i=0;i<TIME_MAX;i++)
	{
		poi[i] = Keycommand[i];
	}
	
	 fwrite(&poi[0], 1, TIME_MAX, fp);

	fclose(fp);*/

}								// キー終了
bool CInputKeyboard::GetkeyboardPress(int key)
{
	if(g_aKeyState[key] & 0x80)
	{
		return true;
	}
	return false;
}						// キーチェック
bool CInputKeyboard::tringkeyboardPress(int key)
{
	if(g_TringKeyState[key] & 0x80)
	{
		g_TringKeyState[key] = 0;
		return true;
	}
	return false;
}						// トリガーキーチェック
bool CInputKeyboard::ReleasekeyboardPress(int key)
{
	if(g_TringKeyState[key] & 0x80)
	{
		return true;
	}
	return false;
}					// リリースキーチェック
bool CInputKeyboard::RepeatkeyboardPress(int key)
{
	if(g_RpeatKeyState[key] & 0x80)
	{
		return true;
	}
	return false;
}						// リピートキーチェック


bool CInputKeyboard::PresskeyCheak(void)
{
	for(int i=0; i<KEYNUM_MAX; i++)
	{
		if(g_aKeyState[i]==0x80)
		{
			return true;
		}
	}
	return false;
}
void CInputKeyboard::SaveKey(void)
{
	unsigned char input = 0;
	int work = 0;

	if(m_count!=TIME_MAX)
	{

		for(int i=0;i<COMMAND_MAX;i++)
		{
			if(GetkeyboardPress(K2CList[i].Key))
			{
				input = K2CList[i].command;
			}
		}
		Keycommand[m_count] = input;
		m_count++;
	}
}
void CInputKeyboard::LoadKey(void)
{
	FILE *fp;

	fp=fopen("data\\DEMO\\デモ.bin","rb");

	if(fp==NULL)
	{
		printf("オープンエラーです\n");
		exit(0);
	}
	char  a[3800];
	fgets(&a[0],3800,fp);
	for(int i=0;i<TIME_MAX;i++)
	{
		KeycommandLOAD[i] = a[i];
	}
	

	fclose(fp);
}
void CInputKeyboard::ActionKey(void)
{
	//入力無視
	for(int i=0;i<KEYNUM_MAX;i++)
	{
		if(i!=DIK_RETURN)
		g_aKeyState[i] = 0;
	}
	if(m_count!=TIME_MAX)
	{

		for(int i=0;i<COMMAND_MAX;i++)
		{
			if(K2CList[i].command == KeycommandLOAD[m_count])
			{
				//そのキーを押す
				g_aKeyState[K2CList[i].Key] = 128;
				g_TringKeyState[K2CList[i].Key] = 128;
				break;
			}
		}

		m_count++;
	}
	else
	{
		m_count = 0;
	}

}
//=============================================================================
// ゲームパッド
//=============================================================================
BOOL CALLBACK CInputPad::SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID )
{
	HRESULT result;
	result = m_pDInputPad->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}
CInputPad::CInputPad(void)
{
}
CInputPad::~CInputPad(void)
{
}

//=============================================================================
// パッド初期化
//=============================================================================
HRESULT CInputPad::InitPad(HINSTANCE hInstance,HWND hWnd)
{
	HRESULT hr;

	if(m_pDInputPad == NULL)
	{
		hr = DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void **)&m_pDInputPad,NULL);

		if(FAILED(hr))
		{
			//メッセージボックスで失敗表示
			return hr;
		}
	}

	HRESULT		result;
	int i;

	padCount = 0;
	// ジョイパッドを探す
	m_pDInputPad->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for ( i=0 ; i<padCount ; i++ ) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
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
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}
		
	return true;

}
//=============================================================================
// パッド終了処理
//=============================================================================
void CInputPad::UninitPad(void)
{
	if(m_pDInputPad != NULL)
	{
		m_pDInputPad->Release();
		m_pDInputPad = NULL;
	}
	for (int i=0 ; i<GAMEPADMAX ; i++) {
		if ( pGamePad[i] )
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
 	}

}

//=============================================================================
// パッド更新
//=============================================================================
void CInputPad::UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for ( i=0 ; i<padCount ; i++ ) 
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// 初期化

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if ( FAILED(result) )
		{
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る

		if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
		{
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if ( dijs.lY < 0 )
		{
			padState[i] |= BUTTON_UP;
		}
		//* y-axis (backward)
		if ( dijs.lY > 0 )
		{
			padState[i] |= BUTTON_DOWN;
		}
		//* x-axis (left)
		if ( dijs.lX < 0 )
		{
			padState[i] |= BUTTON_LEFT;
		}
		//* x-axis (right)
		if ( dijs.lX > 0 )
		{
			padState[i] |= BUTTON_RIGHT;
		}
		if ( dijs.lY > 0 && dijs.lX < 0  )
		{
				padState[i] |= BUTTON_DL;
		}
		if ( dijs.lY < 0 && dijs.lX < 0  )
		{
				padState[i] |= BUTTON_UL;
		}
		if ( dijs.lY > 0 && dijs.lX > 0  )
		{
				padState[i] |= BUTTON_DR;
		}
		if ( dijs.lY < 0 && dijs.lX > 0  )
		{
				padState[i] |= BUTTON_UR;
		}

		//* Ａボタン
		if ( dijs.rgbButtons[0] & 0x80 )
		{
				padState[i] |= BUTTON_A;
		}
		//* Ｂボタン
		if ( dijs.rgbButtons[1] & 0x80 )
		{
				padState[i] |= BUTTON_B;
		}
		//* Xボタン
		if ( dijs.rgbButtons[2] & 0x80 )
		{
				padState[i] |= BUTTON_X;
		}
		//* Yボタン
		if ( dijs.rgbButtons[3] & 0x80 )
		{
				padState[i] |= BUTTON_Y;
		}
		//* Lボタン
		if ( dijs.rgbButtons[4] & 0x80 )
		{
				padState[i] |= BUTTON_L;
		}
		//* Rボタン
		if ( dijs.rgbButtons[5] & 0x80 )
		{
				padState[i] |= BUTTON_R;
		}
		//* SELECTボタン
		if ( dijs.rgbButtons[6] & 0x80 )
		{
				padState[i] |= BUTTON_L2;
		}
		//* STARTボタン
		if ( dijs.rgbButtons[7] & 0x80 )
		{
			padState[i] |= BUTTON_R2;
		}

		if ( dijs.rgbButtons[10] & 0x80 )
		{
				padState[i] |= BUTTON_PAUSE;
		}
		if ( dijs.rgbButtons[11] & 0x80 )
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
bool CInputPad::PresspadCheak(int padNo,DWORD button)
{
	return (button & padState[padNo]);
}
//=============================================================================
// トリガーパッドチェック
//=============================================================================
bool CInputPad::tringpadPress(int padNo,DWORD button)
{
	return (button & padTrigger[padNo]);
}