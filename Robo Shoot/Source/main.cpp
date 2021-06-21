//=============================================================================
//
// ���C������ [main.cpp]
// Author : �����@�a�^
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "field.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "shadow.h"
#include "wall.h"
#include "bullet.h"
#include "debugproc.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "Score.h"
#include "sound.h"
#include "pause.h"
#include "game.h"
#include "timer.h"
#include "enemy.h"
#include "status.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"ROBOSHOOT"	// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void Uninit(void);
void Update(void);
void Draw(void);
void ReInit(void);
int GetRand(int min, int max);
#ifdef _DEBUG
void DrawFPS(void);
#endif

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
bool WindowMode = false;
#ifdef _DEBUG
LPD3DXFONT			g_pD3DXFPS = NULL;		// �t�H���g�ւ̃|�C���^
int					g_nCountFPS;			// FPS�J�E���^
#endif
MODE g_mode;
MODE Last_mode;
static bool pause_flag = false;
//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
    UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

    DWORD dwExecLastTime;
    DWORD dwFPSLastTime;
    DWORD dwCurrentTime;
    DWORD dwFrameCount;

    WNDCLASSEX wcex =
    {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0,
        0,
        hInstance,
        NULL,
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)GetStockObject(GRAY_BRUSH),
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
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
        SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
        NULL,
        NULL,
        hInstance,
        NULL);

    // ����������(�E�B���h�E���쐬���Ă���s��)
    if (FAILED(Init(hInstance, hWnd, TRUE)))
    {
        return -1;
    }
    // �L�[�{�[�h������
    if (FAILED(InitKeyboard(hInstance, hWnd)))
    {
        return -1;
    }
    // �p�b�h������
    InitPad();
    //�t���[���J�E���g������
    timeBeginPeriod(1);				// ����\��ݒ�
    dwExecLastTime =
        dwFPSLastTime = timeGetTime();
    dwCurrentTime =
        dwFrameCount = 0;

    // �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // ���b�Z�[�W���[�v
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
                break;
            }
            else
            {
                // ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            dwCurrentTime = timeGetTime();
            if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
            {
#ifdef _DEBUG
                g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
                dwFPSLastTime = dwCurrentTime;
                dwFrameCount = 0;
            }

            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {
                dwExecLastTime = dwCurrentTime;

                Last_mode = GetMode();

                // �X�V����
                Update();

                // �`�揈��
                Draw();

                dwFrameCount++;
            }
        }
    }

    // �E�B���h�E�N���X�̓o�^������
    UnregisterClass(CLASS_NAME, wcex.hInstance);

    // �I������
    Uninit();

    timeEndPeriod(1);				// ����\��߂�

    return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam)
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

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

    // Direct3D�I�u�W�F�N�g�̍쐬
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (g_pD3D == NULL)
    {
        return E_FAIL;
    }

    // ���݂̃f�B�X�v���C���[�h���擾
    if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return E_FAIL;
    }

    // �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
    ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
    d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
    d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
    d3dpp.BackBufferFormat = d3ddm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
    d3dpp.Windowed = bWindow;					// �E�B���h�E���[�h
    d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

    if (bWindow)
    {// �E�B���h�E���[�h
        d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
    }
    else
    {// �t���X�N���[�����[�h
        d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
    }

    // �f�o�C�X�I�u�W�F�N�g�̐���
    // [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp, &g_pD3DDevice)))
    {
        // ��L�̐ݒ肪���s������
        // [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp, &g_pD3DDevice)))
        {
            // ��L�̐ݒ肪���s������
            // [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
            if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                D3DDEVTYPE_REF,
                hWnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                &d3dpp, &g_pD3DDevice)))
            {
                // ���������s
                return E_FAIL;
            }
        }
    }

    // �����_�[�X�e�[�g�p�����[�^�̐ݒ�
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
    g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
    g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
    g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
    g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

    // �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
    g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
    g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
    g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
    g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

    // �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

#ifdef _DEBUG
    // ���\���p�t�H���g��ݒ�
    D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFPS);
#endif
    InitDebugProc();
    // �J�����̏�����
    InitCamera();

    InitLight();
    InitFade();
    InitSound(hWnd);
    InitTitle();
    InitPause();
    InitScore();

    // �n�ʂ̏���������
    //InitField();

    // ���b�V���t�B�[���h�̏���������
    InitMeshField(2, 2, 2, 2);

    InitMeshWall(2, 2, 2, 2, 1);

    // ���f���̏���������
    InitModel();

    // �e�̏���������
    InitBullet();
    // �e�̏���������
    InitShadow();

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
    if (g_pD3DXFPS != NULL)
    {// ���\���p�t�H���g�̊J��
        g_pD3DXFPS->Release();
        g_pD3DXFPS = NULL;
    }
#endif
    if (g_pD3DDevice != NULL)
    {// �f�o�C�X�̊J��
        g_pD3DDevice->Release();
        g_pD3DDevice = NULL;
    }

    if (g_pD3D != NULL)
    {// Direct3D�I�u�W�F�N�g�̊J��
        g_pD3D->Release();
        g_pD3D = NULL;
    }

    //�@key�I������
    Uninitkeyboard();
    //�@pad�I������
    UninitPad();


    // �e�̏I������
    UninitBullet();
    UninitCamera();
    UninitDebugProc();
    // ���f���̏I������
    UninitModel();
    UninitEnemy();
    // ���b�V���t�B�[���h�̏I������
    UninitMeshField();
    UninitMeshWall();
    // �e�̏I������
    UninitShadow();
    UninitTitle();
    UninitGame();
    UninitResult();
    UninitFade();
    UninitScore();
    UninitSound();
    UninitBullet();
    UninitPause();

    UninitStatus();
    UninitTutorial();
    UninitTimer();

}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
    // key �X�V
    UpdateKeyBoard();
    // pad �X�V
    UpdatePad();

    switch (g_mode)
    {

    case MODE_TITLE:

        UpdateTitle();
        UpdateCamera(1);
        break;
    case MODE_TUTO:
        UpdateTutorial();
        break;
    case MODE_GAME:

        if (pause_flag == true)
        {
            UpdatePause();
        }
        else
        {
            // �J�����X�V
            UpdateCamera(2);

            // ���f���̍X�V����
            UpdateModel();
            UpdateEnemy();

            // �e�̍X�V����
            UpdateBullet();

            // �e�̍X�V����
            UpdateShadow();
            UpdateScore();
            UpdateTimer();
            UpdateGame();
            UpdateBullet();
        }
        break;

    case MODE_RESULT:

        UpdateResult();
        break;
    }

    UpdateFade();


    // �ď���������
    if (g_mode != Last_mode)
    {
        ReInit();
    }
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
    D3DVIEWPORT9 ViewportDef;

    SetCamera(2);
    // �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
    g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    // Direct3D�ɂ��`��̊J�n
    if (SUCCEEDED(g_pD3DDevice->BeginScene()))
    {
        g_pD3DDevice->GetViewport(&ViewportDef);

        switch (g_mode)
        {
        case MODE_TITLE:

            DrawTitle();
            // �J�����̐ݒ菈��
            SetCamera(1);
            DrawModel();

            break;

        case MODE_TUTO:
            DrawTutorial();
            break;

        case MODE_GAME:

            // ���b�V���t�B�[���h�̕`�揈��
            DrawMeshField();
            // �Ǖ`�揈��
            DrawMeshWall();
            // �e�̕`�揈��
            DrawShadow();
            // ���f���̕`�揈��
            DrawModel();
            // �G�̕`�揈��
            DrawEnemy();
            // �e�̕`�揈��
            DrawBullet();

            // �^�C�}�[�`�揈��
            DrawTimer();

            // �X�R�A�`��
            DrawScore();

            DrawStatus();


            if (pause_flag == true)
            {
                DrawPause();
            }
            break;

        case MODE_RESULT:

            DrawResult();
            DrawScore();

            break;
        }

        DrawFade();

        g_pD3DDevice->SetViewport(&ViewportDef);




#ifdef _DEBUG
        // FPS�\��
        DrawFPS();
#endif

        // Direct3D�ɂ��`��̏I��
        g_pD3DDevice->EndScene();
    }

    // �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �ď���������
//=============================================================================
void ReInit(void)
{

    switch (g_mode)
    {
    case MODE_TITLE:
        UninitResult();
        UninitScore();
        InitTitle();
        InitModel();
        break;
    case MODE_TUTO:
        UninitTitle();
        UninitCamera();
        UninitModel();
        InitTutorial();
        break;
    case MODE_GAME:
        UninitTutorial();
        InitPause();
        InitStatus();
        InitTimer();
        InitScore();
        InitGame();
        InitBullet();
        // ���b�V���t�B�[���h�̏���������
        InitMeshField(2, 2, 2, 2);
        InitMeshWall(2, 2, 2, 2, 1);
        // ���f���̏���������
        InitModel();
        InitEnemy();

        for (int i = 0; i < START_ENEMY_NUM; i++)
        {
            EnemyPosSet((float)GetRand(-500,500), 0, (float)GetRand(-500, 500));
        }


        // �e�̏���������
        InitBullet();

        // �e�̏���������
        InitShadow();
        break;

    case MODE_RESULT:
        UninitGame();
        UninitShadow();
        UninitStatus();
        UninitTimer();
        // ���b�V���t�B�[���h�̏I������
        UninitMeshField();
        UninitMeshWall();

        // ���f���̏I������
        UninitModel();

        UninitEnemy();


        // �e�̏I������
        UninitBullet();
        InitResult();
        break;
    }

}
void SetMode(MODE mode)
{
    g_mode = mode;
}
MODE GetMode(void)
{
    return g_mode;
}
void SetPauseflag(bool flag)
{
    pause_flag = flag;
}
void PauseInitGame(void)
{
    InitPause();
    InitScore();
    InitStatus();
    InitTimer();
    InitGame();
    // ���b�V���t�B�[���h�̏���������
    InitMeshField(2, 2, 2, 2);

    InitMeshWall(2, 2, 2, 2, 1);

    // ���f���̏���������
    InitModel();
    InitEnemy();
    for (int i = 0; i < 50; i++)
    {
        EnemyPosSet(-9000 + i * 500, 0, -9000 + 300 * i);
    }

    // �e�̏���������
    InitBullet();
    // �e�̏���������
    InitShadow();
}
void PauseUninitGame(void)
{
    UninitGame();
    UninitBullet();
    // ���b�V���t�B�[���h�̏I������
    UninitMeshField();
    UninitMeshWall();

    // ���f���̏I������
    UninitModel();
    UninitEnemy();


    // �e�̏I������
    UninitBullet();
    UninitPause();
    UninitStatus();
    UninitTimer();
    UninitScore();
}
//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
    return g_pD3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// FPS�\��
//=============================================================================
void DrawFPS(void)
{
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    char str[256];

    wsprintf(str, "FPS:%d\n", g_nCountFPS);


    // �e�L�X�g�`��
    g_pD3DXFPS->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}
void SetWindowflag(bool flag)
{
    WindowMode = flag;
}
#endif

//=============================================================================
// �����l�̎擾
//=============================================================================
int GetRand(int min, int max) {
    srand((unsigned int)time(NULL));
    return min + rand() % (-min + max);
}

