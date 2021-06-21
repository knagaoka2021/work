//=============================================================================
//
// �L�[���͏��� [input.h]
// Author : �����@�a�^
//
//=============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void UpdateKeyBoard(void);								// �L�[�X�V
void Uninitkeyboard(void);								// �L�[�I��
bool GetkeyboardPress(int key);						// �L�[�`�F�b�N
bool tringkeyboardPress(int key);						// �g���K�[�L�[�`�F�b�N
bool ReleasekeyboardPress(int key);					// �����[�X�L�[�`�F�b�N
bool RepeatkeyboardPress(int key);						// ���s�[�g�L�[�`�F�b�N

HRESULT InitPad(void);
BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID );
void UpdatePad(void);
void UninitPad(void);
bool PresskeyCheak(void);

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
#define BUTTON_SELECT		0x00000400l	// SELECT�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_START		0x00000800l	// START�{�^��(.rgbButtons[7]&0x80)
#define GAMEPADMAX		2			// �ڑ�����W���C�p�b�h�̍ő吔
bool PresspadCheak(int padNo,DWORD button);
bool tringpadPress(int padNo,DWORD button);



#endif