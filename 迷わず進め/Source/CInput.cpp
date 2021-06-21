//=============================================================================
//
//  [CInput.h]
// Author : �����a�^
//
//=============================================================================
#include "CInput.h"
#include<stdio.h>
#include<stdlib.h>
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define COMMAND_MAX (8)
#define TIME_MAX		(3800)
#define DEADZONE		2500			
#define RANGE_MAX		1000			
#define RANGE_MIN		-1000	
//*****************************************************************************
// �񋓑̒�`
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
// �\���̒�`
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
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char Keycommand[TIME_MAX];
unsigned char KeycommandLOAD[TIME_MAX];

BYTE					g_aKeyState[KEYNUM_MAX];				// �L�[�{�[�h�̉�����Ԃ��󂯎�郏�[�N
BYTE					g_TringKeyState[KEYNUM_MAX];			// �g���K�[�L�[���
BYTE					g_ReleseKeyState[KEYNUM_MAX];			// �����[�X�L�[���
BYTE					g_RpeatKeyState[KEYNUM_MAX];			// ���s�[�g�L�[���
static int				RpeatKey_count[KEYNUM_MAX];				// ���s�[�g�L�[�p�J�E���g

LPDIRECTINPUT8			g_pDInput = NULL;						// DirectInput�I�u�W�F�N�g�̃|�C���^


		


static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = {NULL,NULL};// �p�b�h�f�o�C�X

static DWORD	padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD	padTrigger[GAMEPADMAX];
static int		padCount = 0;			// ���o�����p�b�h�̐�




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
			//���b�Z�[�W�{�b�N�X�Ŏ��s�\��
			return hr;
		}
	}

	hr = m_pDInput->CreateDevice(GUID_SysKeyboard,&m_pDIDevKeyboard,NULL);

	if(FAILED(hr))
	{
		MessageBox(NULL,"DirectInput8�I�u�W�F�N�g�̍쐬�Ɏ��s","Direct Input Error",MB_OK);
		return hr;
	}

	hr=m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);

	if(FAILED(hr))
	{
			//
		MessageBox(NULL,"DirectInput8�I�u�W�F�N�g�̍쐬�Ɏ��s","Direct Input Error",MB_OK);
	}

	hr=m_pDIDevKeyboard->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if(FAILED(hr))
	{
		MessageBox(NULL,"DirectInput8�I�u�W�F�N�g�̍쐬�Ɏ��s","Direct Input Error",MB_OK);
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
	static int		m_RpeatKey_count[KEYNUM_MAX];				// ���s�[�g�L�[�p�J�E���g

	BYTE LastKeyState[KEYNUM_MAX];

	memcpy(LastKeyState,g_aKeyState,KEYNUM_MAX);

	if(SUCCEEDED(m_pDIDevKeyboard->GetDeviceState(sizeof(g_aKeyState),&g_aKeyState[0])))
	{
		for(int nkey=0;nkey<KEYNUM_MAX;nkey++)
		{
			//�g���K�[�L�[���쐬
			g_TringKeyState[nkey] = (LastKeyState[nkey] ^ g_aKeyState[nkey])&g_aKeyState[nkey];

			//�����[�X�L�[�[���쐬
			g_ReleseKeyState[nkey] = (LastKeyState[nkey] ^ g_aKeyState[nkey])&LastKeyState[nkey];

			//���s�[�g�L�[����
			if(g_aKeyState[nkey])											//��������Ă���L�[
			{
				if(RpeatKey_count[nkey] == 0)								// �J�E���g0�Ȃ�
				{
					g_RpeatKeyState[nkey] = g_aKeyState[nkey];				// ON
				}
				else
				{
					g_RpeatKeyState[nkey] = 0;
				}

				RpeatKey_count[nkey]++;

				if(RpeatKey_count[nkey] > RPEAT_TIME)						// ���t���[���������Ȃ�
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
}								// �L�[�X�V
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

	fp=fopen("�f��.bin","wb");

	if(fp==NULL)
	{
		printf("�I�[�v���G���[�ł�\n");
		exit(0);
	}
	char poi[3800];
	for(int i=0;i<TIME_MAX;i++)
	{
		poi[i] = Keycommand[i];
	}
	
	 fwrite(&poi[0], 1, TIME_MAX, fp);

	fclose(fp);*/

}								// �L�[�I��
bool CInputKeyboard::GetkeyboardPress(int key)
{
	if(g_aKeyState[key] & 0x80)
	{
		return true;
	}
	return false;
}						// �L�[�`�F�b�N
bool CInputKeyboard::tringkeyboardPress(int key)
{
	if(g_TringKeyState[key] & 0x80)
	{
		g_TringKeyState[key] = 0;
		return true;
	}
	return false;
}						// �g���K�[�L�[�`�F�b�N
bool CInputKeyboard::ReleasekeyboardPress(int key)
{
	if(g_TringKeyState[key] & 0x80)
	{
		return true;
	}
	return false;
}					// �����[�X�L�[�`�F�b�N
bool CInputKeyboard::RepeatkeyboardPress(int key)
{
	if(g_RpeatKeyState[key] & 0x80)
	{
		return true;
	}
	return false;
}						// ���s�[�g�L�[�`�F�b�N


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

	fp=fopen("data\\DEMO\\�f��.bin","rb");

	if(fp==NULL)
	{
		printf("�I�[�v���G���[�ł�\n");
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
	//���͖���
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
				//���̃L�[������
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
// �Q�[���p�b�h
//=============================================================================
BOOL CALLBACK CInputPad::SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID )
{
	HRESULT result;
	result = m_pDInputPad->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}
CInputPad::CInputPad(void)
{
}
CInputPad::~CInputPad(void)
{
}

//=============================================================================
// �p�b�h������
//=============================================================================
HRESULT CInputPad::InitPad(HINSTANCE hInstance,HWND hWnd)
{
	HRESULT hr;

	if(m_pDInputPad == NULL)
	{
		hr = DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void **)&m_pDInputPad,NULL);

		if(FAILED(hr))
		{
			//���b�Z�[�W�{�b�N�X�Ŏ��s�\��
			return hr;
		}
	}

	HRESULT		result;
	int i;

	padCount = 0;
	// �W���C�p�b�h��T��
	m_pDInputPad->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for ( i=0 ; i<padCount ; i++ ) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//�W���C�X�e�B�b�N���͐���J�n
		pGamePad[i]->Acquire();
	}
		
	return true;

}
//=============================================================================
// �p�b�h�I������
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
// �p�b�h�X�V
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
		padState[i] = 0x00000000l;	// ������

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if ( FAILED(result) )
		{
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��

		if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
		{
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
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

		//* �`�{�^��
		if ( dijs.rgbButtons[0] & 0x80 )
		{
				padState[i] |= BUTTON_A;
		}
		//* �a�{�^��
		if ( dijs.rgbButtons[1] & 0x80 )
		{
				padState[i] |= BUTTON_B;
		}
		//* X�{�^��
		if ( dijs.rgbButtons[2] & 0x80 )
		{
				padState[i] |= BUTTON_X;
		}
		//* Y�{�^��
		if ( dijs.rgbButtons[3] & 0x80 )
		{
				padState[i] |= BUTTON_Y;
		}
		//* L�{�^��
		if ( dijs.rgbButtons[4] & 0x80 )
		{
				padState[i] |= BUTTON_L;
		}
		//* R�{�^��
		if ( dijs.rgbButtons[5] & 0x80 )
		{
				padState[i] |= BUTTON_R;
		}
		//* SELECT�{�^��
		if ( dijs.rgbButtons[6] & 0x80 )
		{
				padState[i] |= BUTTON_L2;
		}
		//* START�{�^��
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

		// Trigger�ݒ�
		padTrigger[i] = ((lastPadState ^ padState[i])     // �O��ƈ���Ă���
					        & padState[i]);				  // ��������ON�̂��
		
	}

}
//=============================================================================
// �p�b�h��������Ă��邩�̃`�F�b�N
//=============================================================================
bool CInputPad::PresspadCheak(int padNo,DWORD button)
{
	return (button & padState[padNo]);
}
//=============================================================================
// �g���K�[�p�b�h�`�F�b�N
//=============================================================================
bool CInputPad::tringpadPress(int padNo,DWORD button)
{
	return (button & padTrigger[padNo]);
}