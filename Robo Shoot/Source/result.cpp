//=============================================================================
//
// リザルト [result.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "result.h"
#include "Score.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "debugproc.h"

static LPDIRECT3DVERTEXBUFFER9 g_pD3DBGVtxBuff = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_pD3DBGVtxBuff1 = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBGTex = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBG1Tex = NULL;
static VERTEX_2D* pVtx;
static int Cnt;

void LoadJTexture(void);

HRESULT InitResult(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    pDevice = GetDevice();

    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pD3DBGVtxBuff, NULL)))
    {
        return E_FAIL;
    }

    g_pD3DBGVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].vtx = D3DXVECTOR3(0, 0, 0.0f);

    pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

    pVtx[2].vtx = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);

    pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    pVtx[0].rhw = 1.0f;

    pVtx[1].rhw = 1.0f;

    pVtx[2].rhw = 1.0f;

    pVtx[3].rhw = 1.0f;

    pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[0].tex = D3DXVECTOR2(0, 0);

    pVtx[1].tex = D3DXVECTOR2(1, 0);

    pVtx[2].tex = D3DXVECTOR2(0, 1);

    pVtx[3].tex = D3DXVECTOR2(1, 1);

    g_pD3DBGVtxBuff->Unlock();

    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pD3DBGVtxBuff1, NULL)))
    {
        return E_FAIL;
    }

    g_pD3DBGVtxBuff1->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].vtx = D3DXVECTOR3(0, 0, 0.0f);

    pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

    pVtx[2].vtx = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);

    pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    pVtx[0].rhw = 1.0f;

    pVtx[1].rhw = 1.0f;

    pVtx[2].rhw = 1.0f;

    pVtx[3].rhw = 1.0f;

    pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[0].tex = D3DXVECTOR2(0, 0);

    pVtx[1].tex = D3DXVECTOR2(1, 0);

    pVtx[2].tex = D3DXVECTOR2(0, 1);

    pVtx[3].tex = D3DXVECTOR2(1, 1);

    g_pD3DBGVtxBuff1->Unlock();

    Cnt = 0;

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameclear.png", &g_pD3DBGTex);
    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameover.png", &g_pD3DBG1Tex);

    PlaySound(SOUND_LABEL_BGM004, XAUDIO2_LOOP_INFINITE);

    return S_OK;
}
void UpdateResult(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    bool flag = false;

    pDevice = GetDevice();
    int test = GetScore();

    if (tringkeyboardPress(DIK_RETURN) || tringpadPress(0, BUTTON_START) || Cnt > 600)				// Enterキーによる画面切り替え(フェード)
    {
        SetFade(FADE_IN);
        Cnt = 0;
    }
    if (GetFade() == FADE_NONE)
    {
        SetMode(MODE_TITLE);
        SetFade(FADE_OUT);
    }

    Result_Scoredisp();								// スコア結果表示

    if (test >= 50000)
    {

        g_pD3DBGVtxBuff1->Lock(0, 0, (void**)&pVtx, 0);


        pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

        pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

        pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

        pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

        g_pD3DBGVtxBuff1->Unlock();


    }
    else
    {
        g_pD3DBGVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


        pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

        pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

        pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

        pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

        g_pD3DBGVtxBuff->Unlock();


    }

    Cnt++;



}
void DrawResult(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    pDevice = GetDevice();

    pDevice->SetStreamSource(0, g_pD3DBGVtxBuff, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->SetTexture(0, g_pD3DBGTex);//テクスチャの設定

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//2はポリゴン数

    pDevice->SetStreamSource(0, g_pD3DBGVtxBuff1, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->SetTexture(0, g_pD3DBG1Tex);//テクスチャの設定

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//2はポリゴン数
}
void UninitResult(void)
{
    StopSound(SOUND_LABEL_BGM004);

    //---テクスチャの解放---//
    if (g_pD3DBGTex != NULL)
    {
        g_pD3DBGTex->Release();
        g_pD3DBGTex = NULL;
    }
    if (g_pD3DBGVtxBuff != NULL)
    {
        g_pD3DBGVtxBuff->Release();
        g_pD3DBGVtxBuff = NULL;
    }

    //---テクスチャの解放---//
    if (g_pD3DBG1Tex != NULL)
    {
        g_pD3DBG1Tex->Release();
        g_pD3DBG1Tex = NULL;
    }
    if (g_pD3DBGVtxBuff1 != NULL)
    {
        g_pD3DBGVtxBuff1->Release();
        g_pD3DBGVtxBuff1 = NULL;
    }
    Cnt = 0;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void LoadJTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameover.png", &g_pD3DBGTex);
}