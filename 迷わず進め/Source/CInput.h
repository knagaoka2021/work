//=============================================================================
//
//  [CInput.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CINPUT_H
#define _CINPUT_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#define DIRECTINPUT_VERSION (0x0800)    // �x���΍��p
#include <dinput.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define					KEYNUM_MAX			(256)				// �L�[�o�b�t�@�T�C�Y
#define					RPEAT_TIME			(30)				// ���s�[�g����Ԋu

#define BUTTON_UP		0x00000001l	// �����L�[��(.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.IX>0)
#define BUTTON_UR		0x10000000l	// �����L�[�E��
#define BUTTON_DR		0x20000000l	// �����L�[�E��
#define BUTTON_UL		0x30000000l	// �����L�[����
#define BUTTON_DL		0x40000000l	// �����L�[����
#define BUTTON_A		0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_X		0x00000040l	// X�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_Y		0x00000080l	// Y�{�^��(.rgbButtons[3]&0x80)
#define BUTTON_L		0x00000100l	// L�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_R		0x00000200l	// R�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L2		0x00000400l	// L2�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R2		0x00000800l	// R2�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_PAUSE	0x00008000l	// PAUSE�{�^��(.rgbButtons[10]&0x80)
#define BUTTON_START	0x00020000l	// START�{�^��(.rgbButtons[11]&0x80)

#define GAMEPADMAX		2			// �ڑ�����W���C�p�b�h�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

class CInput
{
	public:
	CInput(void);
	virtual ~CInput(void);


	virtual HRESULT Init(HINSTANCE hInstance,HWND hWnd);

	virtual void Uninit(void);
	virtual void Update(void) = 0;

	static LPDIRECTINPUT8			m_pDInput;						// DirectInput�I�u�W�F�N�g�̃|�C���^
		   LPDIRECTINPUTDEVICE8	m_pDIDevKeyboard;				// �f�o�C�X�I�u�W�F�N�g�̃|�C���^(�L�[�{�[�h�p)
	int m_count;

};

class CInputKeyboard:public CInput
{
	public :
	HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	void Update(void);								// �L�[�X�V
	void Uninit(void);								// �L�[�I��
	static bool GetkeyboardPress(int key);						// �L�[�`�F�b�N
	static bool tringkeyboardPress(int key);						// �g���K�[�L�[�`�F�b�N
	static bool ReleasekeyboardPress(int key);					// �����[�X�L�[�`�F�b�N
	static bool RepeatkeyboardPress(int key);						// ���s�[�g�L�[�`�F�b�N
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
	static LPDIRECTINPUT8			m_pDInputPad;						// DirectInput�I�u�W�F�N�g�̃|�C���^

	static bool PresspadCheak(int padNo,DWORD button);
	static bool tringpadPress(int padNo,DWORD button);
};

#endif
// EOF