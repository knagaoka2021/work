//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 長岡　和真
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
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"ROBOSHOOT"	// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
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
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
bool WindowMode = false;
#ifdef _DEBUG
LPD3DXFONT			g_pD3DXFPS = NULL;		// フォントへのポインタ
int					g_nCountFPS;			// FPSカウンタ
#endif
MODE g_mode;
MODE Last_mode;
static bool pause_flag = false;
//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
    UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

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

    // ウィンドウクラスの登録
    RegisterClassEx(&wcex);

    // ウィンドウの作成
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

    // 初期化処理(ウィンドウを作成してから行う)
    if (FAILED(Init(hInstance, hWnd, TRUE)))
    {
        return -1;
    }
    // キーボード初期化
    if (FAILED(InitKeyboard(hInstance, hWnd)))
    {
        return -1;
    }
    // パッド初期化
    InitPad();
    //フレームカウント初期化
    timeBeginPeriod(1);				// 分解能を設定
    dwExecLastTime =
        dwFPSLastTime = timeGetTime();
    dwCurrentTime =
        dwFrameCount = 0;

    // ウインドウの表示(初期化処理の後に呼ばないと駄目)
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // メッセージループ
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {// PostQuitMessage()が呼ばれたらループ終了
                break;
            }
            else
            {
                // メッセージの翻訳とディスパッチ
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            dwCurrentTime = timeGetTime();
            if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
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

                // 更新処理
                Update();

                // 描画処理
                Draw();

                dwFrameCount++;
            }
        }
    }

    // ウィンドウクラスの登録を解除
    UnregisterClass(CLASS_NAME, wcex.hInstance);

    // 終了処理
    Uninit();

    timeEndPeriod(1);				// 分解能を戻す

    return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

    // Direct3Dオブジェクトの作成
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (g_pD3D == NULL)
    {
        return E_FAIL;
    }

    // 現在のディスプレイモードを取得
    if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return E_FAIL;
    }

    // デバイスのプレゼンテーションパラメータの設定
    ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
    d3dpp.BackBufferCount = 1;						// バックバッファの数
    d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
    d3dpp.BackBufferFormat = d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
    d3dpp.Windowed = bWindow;					// ウィンドウモード
    d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// デプスバッファとして16bitを使う

    if (bWindow)
    {// ウィンドウモード
        d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
    }
    else
    {// フルスクリーンモード
        d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
    }

    // デバイスオブジェクトの生成
    // [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp, &g_pD3DDevice)))
    {
        // 上記の設定が失敗したら
        // [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp, &g_pD3DDevice)))
        {
            // 上記の設定が失敗したら
            // [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
            if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                D3DDEVTYPE_REF,
                hWnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                &d3dpp, &g_pD3DDevice)))
            {
                // 初期化失敗
                return E_FAIL;
            }
        }
    }

    // レンダーステートパラメータの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
    g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
    g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
    g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
    g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

    // サンプラーステートパラメータの設定
    g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
    g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
    g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
    g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

    // テクスチャステージステートの設定
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

#ifdef _DEBUG
    // 情報表示用フォントを設定
    D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFPS);
#endif
    InitDebugProc();
    // カメラの初期化
    InitCamera();

    InitLight();
    InitFade();
    InitSound(hWnd);
    InitTitle();
    InitPause();
    InitScore();

    // 地面の初期化処理
    //InitField();

    // メッシュフィールドの初期化処理
    InitMeshField(2, 2, 2, 2);

    InitMeshWall(2, 2, 2, 2, 1);

    // モデルの初期化処理
    InitModel();

    // 弾の初期化処理
    InitBullet();
    // 影の初期化処理
    InitShadow();

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
    if (g_pD3DXFPS != NULL)
    {// 情報表示用フォントの開放
        g_pD3DXFPS->Release();
        g_pD3DXFPS = NULL;
    }
#endif
    if (g_pD3DDevice != NULL)
    {// デバイスの開放
        g_pD3DDevice->Release();
        g_pD3DDevice = NULL;
    }

    if (g_pD3D != NULL)
    {// Direct3Dオブジェクトの開放
        g_pD3D->Release();
        g_pD3D = NULL;
    }

    //　key終了処理
    Uninitkeyboard();
    //　pad終了処理
    UninitPad();


    // 弾の終了処理
    UninitBullet();
    UninitCamera();
    UninitDebugProc();
    // モデルの終了処理
    UninitModel();
    UninitEnemy();
    // メッシュフィールドの終了処理
    UninitMeshField();
    UninitMeshWall();
    // 影の終了処理
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
// 更新処理
//=============================================================================
void Update(void)
{
    // key 更新
    UpdateKeyBoard();
    // pad 更新
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
            // カメラ更新
            UpdateCamera(2);

            // モデルの更新処理
            UpdateModel();
            UpdateEnemy();

            // 弾の更新処理
            UpdateBullet();

            // 影の更新処理
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


    // 再初期化処理
    if (g_mode != Last_mode)
    {
        ReInit();
    }
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
    D3DVIEWPORT9 ViewportDef;

    SetCamera(2);
    // バックバッファ＆Ｚバッファのクリア
    g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    // Direct3Dによる描画の開始
    if (SUCCEEDED(g_pD3DDevice->BeginScene()))
    {
        g_pD3DDevice->GetViewport(&ViewportDef);

        switch (g_mode)
        {
        case MODE_TITLE:

            DrawTitle();
            // カメラの設定処理
            SetCamera(1);
            DrawModel();

            break;

        case MODE_TUTO:
            DrawTutorial();
            break;

        case MODE_GAME:

            // メッシュフィールドの描画処理
            DrawMeshField();
            // 壁描画処理
            DrawMeshWall();
            // 影の描画処理
            DrawShadow();
            // モデルの描画処理
            DrawModel();
            // 敵の描画処理
            DrawEnemy();
            // 弾の描画処理
            DrawBullet();

            // タイマー描画処理
            DrawTimer();

            // スコア描画
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
        // FPS表示
        DrawFPS();
#endif

        // Direct3Dによる描画の終了
        g_pD3DDevice->EndScene();
    }

    // バックバッファとフロントバッファの入れ替え
    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// 再初期化処理
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
        // メッシュフィールドの初期化処理
        InitMeshField(2, 2, 2, 2);
        InitMeshWall(2, 2, 2, 2, 1);
        // モデルの初期化処理
        InitModel();
        InitEnemy();

        for (int i = 0; i < START_ENEMY_NUM; i++)
        {
            EnemyPosSet((float)GetRand(-500,500), 0, (float)GetRand(-500, 500));
        }


        // 弾の初期化処理
        InitBullet();

        // 影の初期化処理
        InitShadow();
        break;

    case MODE_RESULT:
        UninitGame();
        UninitShadow();
        UninitStatus();
        UninitTimer();
        // メッシュフィールドの終了処理
        UninitMeshField();
        UninitMeshWall();

        // モデルの終了処理
        UninitModel();

        UninitEnemy();


        // 弾の終了処理
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
    // メッシュフィールドの初期化処理
    InitMeshField(2, 2, 2, 2);

    InitMeshWall(2, 2, 2, 2, 1);

    // モデルの初期化処理
    InitModel();
    InitEnemy();
    for (int i = 0; i < 50; i++)
    {
        EnemyPosSet(-9000 + i * 500, 0, -9000 + 300 * i);
    }

    // 弾の初期化処理
    InitBullet();
    // 影の初期化処理
    InitShadow();
}
void PauseUninitGame(void)
{
    UninitGame();
    UninitBullet();
    // メッシュフィールドの終了処理
    UninitMeshField();
    UninitMeshWall();

    // モデルの終了処理
    UninitModel();
    UninitEnemy();


    // 弾の終了処理
    UninitBullet();
    UninitPause();
    UninitStatus();
    UninitTimer();
    UninitScore();
}
//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
    return g_pD3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void DrawFPS(void)
{
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    char str[256];

    wsprintf(str, "FPS:%d\n", g_nCountFPS);


    // テキスト描画
    g_pD3DXFPS->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}
void SetWindowflag(bool flag)
{
    WindowMode = flag;
}
#endif

//=============================================================================
// 乱数値の取得
//=============================================================================
int GetRand(int min, int max) {
    srand((unsigned int)time(NULL));
    return min + rand() % (-min + max);
}

