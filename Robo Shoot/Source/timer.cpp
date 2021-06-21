//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : 長岡　和真
//
//=============================================================================
#include "timer.h"
#include "fade.h"
static LPDIRECT3DTEXTURE9 g_pD3DTimerTex = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_pD3DTimerVtxBuff = NULL;
static VERTEX_2D* pVtx;
static int g_nTimer = 60;
static int count_T = 0;
bool g_nTime_flag = FALSE;
#define ALFA	(40)

HRESULT InitTimer(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    g_nTimer = 60;
    g_nTime_flag = TRUE;

    pDevice = GetDevice();

    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * KETA_T, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pD3DTimerVtxBuff, NULL)))
    {
        return E_FAIL;
    }

    g_pD3DTimerVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPlace = 0; nCntPlace < KETA_T; nCntPlace++)
    {

        pVtx[0].vtx = D3DXVECTOR3(300 + ALFA * (float)nCntPlace, 0, 0.0f);

        pVtx[1].vtx = D3DXVECTOR3(350 + ALFA * (float)nCntPlace, 0, 0.0f);

        pVtx[2].vtx = D3DXVECTOR3(300 + ALFA * (float)nCntPlace, 50, 0.0f);

        pVtx[3].vtx = D3DXVECTOR3(350 + ALFA * (float)nCntPlace, 50, 0.0f);

        pVtx[0].rhw = 1.0f;

        pVtx[1].rhw = 1.0f;

        pVtx[2].rhw = 1.0f;

        pVtx[3].rhw = 1.0f;

        pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[0].tex = D3DXVECTOR2(0.0f, 0);

        pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);

        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

        pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

        pVtx += 4;												//４つ頂点で一つアドレスずらし
    }
    g_pD3DTimerVtxBuff->Unlock();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pD3DTimerTex);

    return S_OK;
}
void UpdateTimer(void)
{
    int work = 10;
    int workbox[KETA_T];


    g_pD3DTimerVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPlace = 0; nCntPlace < KETA_T; nCntPlace++)
    {
        workbox[nCntPlace] = (g_nTimer / work) % 10;

        work /= 10;

        pVtx[0].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace], 0);

        pVtx[1].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace] + 0.1f, 0);

        pVtx[2].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace], 1.0f);

        pVtx[3].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace] + 0.1f, 1.0f);

        pVtx += 4;
    }
    g_pD3DTimerVtxBuff->Unlock();

    if (g_nTimer != 0)
    {
        if (count_T % 60 == 0)
        {
            COUNTDOWN();
            //count_T=0;
        }
        count_T++;
    }
    else if (g_nTime_flag == TRUE)
    {
        SetFade(FADE_IN);
        g_nTime_flag = FALSE;
    }
    if (GetFade() == FADE_NONE)
    {
        SetMode(MODE_RESULT);
        SetFade(FADE_OUT);
    }


}
void DrawTimer(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    pDevice = GetDevice();

    pDevice->SetStreamSource(0, g_pD3DTimerVtxBuff, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->SetTexture(0, g_pD3DTimerTex);//テクスチャの設定

    for (int nCntPlace = 0; nCntPlace < KETA_T; nCntPlace++)
    {
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlace * 4, 2);//2はポリゴン数
    }
}
void UninitTimer(void)
{
    //---テクスチャの解放---//
    if (g_pD3DTimerTex != NULL)
    {
        g_pD3DTimerTex->Release();
        g_pD3DTimerTex = NULL;
    }
    if (g_pD3DTimerVtxBuff != NULL)
    {
        g_pD3DTimerVtxBuff->Release();
        g_pD3DTimerVtxBuff = NULL;
    }
    g_nTimer = 0;
    g_nTime_flag = TRUE;
}
void COUNTDOWN(void)
{
    g_nTimer--;
}
