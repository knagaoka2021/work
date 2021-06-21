//=============================================================================
//	DirectX9 [���킸�i�߁I]
//	[main.cpp]
//	HAL�����Q�[������w��3�N_�����a�^
// Author : �����a�^
//
//=============================================================================
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "main.h"
#include "CManager.h"
#include "CRenderer.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"���킸�����߁I"		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#ifdef _DEBUG
void DrawFPS(void);
#endif

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
#ifdef _DEBUG
LPD3DXFONT				g_pD3DXFont = NULL;
int						g_nCountFPS = 0;			// FPS�J�E���^
#endif
CManager *g_pManager=NULL;
CRenderer *render=NULL;

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �������[���[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);



	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						(WS_OVERLAPPEDWINDOW) & ~(WS_THICKFRAME|WS_MAXIMIZEBOX),
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
						(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
						NULL,
						NULL,
						hInstance,
						NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	g_pManager = new CManager;

	g_pManager->Init(hInstance, hWnd, TRUE);
	

// �t���[���J�E���g������
	timeBeginPeriod(1);					// ����\��ݒ�
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime =
	dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɍs��)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	
	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();		// �V�X�e���������擾

			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPS�𑪒�
#endif
			    
				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �J�E���g���N���A
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;		// ��������������ۑ�

				// �X�V����
				g_pManager->Update();

				// �`�揈��
				g_pManager->Draw();
//#ifdef _DEBUG
//		// FPS�\��
//		DrawFPS();
//#endif

				dwFrameCount++;			// �J�E���g�����Z
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������

//#ifdef _DEBUG
//	if(g_pD3DXFont != NULL)
//	{// ���\���p�t�H���g�̊J��
//		g_pD3DXFont->Release();
//		g_pD3DXFont = NULL;
//	}
//#endif
	g_pManager->Uninit();

	delete g_pManager;



	timeEndPeriod(1);				// ����\��߂�


	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LPDIRECT3DDEVICE9 GetDevice(void)
{
	// �f�o�C�X�̃|�C���^��Ԃ�
	return g_pD3DDevice;
}
#ifdef _DEBUG
//=============================================================================
// FPS�\��
//=============================================================================
void DrawFPS(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char str[256];

	wsprintf(str, "FPS:%d\n", g_nCountFPS);


	// �e�L�X�g�`��
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}
#endif