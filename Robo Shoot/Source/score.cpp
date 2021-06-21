//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 長岡　和真
//
//=============================================================================
#include "Score.h"
static LPDIRECT3DTEXTURE9 g_pD3DSCOTex = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_pD3DSCOVtxBuff = NULL;
static VERTEX_2D* pVtx;
static int g_nScore = 0;
#define ALFA	(40)

HRESULT InitScore(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    g_nScore = 0;

    pDevice = GetDevice();

    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * KETA, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pD3DSCOVtxBuff, NULL)))
    {
        return E_FAIL;
    }

    g_pD3DSCOVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPlace = 0; nCntPlace < KETA; nCntPlace++)
    {

        pVtx[0].vtx = D3DXVECTOR3(600 + ALFA * (float)nCntPlace, 0, 0.0f);

        pVtx[1].vtx = D3DXVECTOR3(650 + ALFA * (float)nCntPlace, 0, 0.0f);

        pVtx[2].vtx = D3DXVECTOR3(600 + ALFA * (float)nCntPlace, 50, 0.0f);

        pVtx[3].vtx = D3DXVECTOR3(650 + ALFA * (float)nCntPlace, 50, 0.0f);

        pVtx[0].rhw = 1.0f;

        pVtx[1].rhw = 1.0f;

        pVtx[2].rhw = 1.0f;

        pVtx[3].rhw = 1.0f;

        pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 0, 255);

        pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 0, 255);

        pVtx[2].diffuse = D3DCOLOR_RGBA(25, 255, 0, 255);

        pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 0, 255);

        pVtx[0].tex = D3DXVECTOR2(0.0f, 0);

        pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);

        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

        pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

        pVtx += 4;												//４つ頂点で一つアドレスずらし
    }
    g_pD3DSCOVtxBuff->Unlock();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pD3DSCOTex);

    return S_OK;
}
void UpdateScore(void)
{
    int work = 10000000;
    int workbox[KETA];

    g_pD3DSCOVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPlace = 0; nCntPlace < KETA; nCntPlace++)
    {
        workbox[nCntPlace] = (g_nScore / work) % 10;

        work /= 10;

        pVtx[0].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace], 0);

        pVtx[1].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace] + 0.1f, 0);

        pVtx[2].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace], 1.0f);

        pVtx[3].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace] + 0.1f, 1.0f);

        pVtx += 4;
    }
    g_pD3DSCOVtxBuff->Unlock();
}
void DrawScore(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    pDevice = GetDevice();

    pDevice->SetStreamSource(0, g_pD3DSCOVtxBuff, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->SetTexture(0, g_pD3DSCOTex);//テクスチャの設定

    for (int nCntPlace = 0; nCntPlace < KETA; nCntPlace++)
    {
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlace * 4, 2);//2はポリゴン数
    }
}
void UninitScore(void)
{
    //---テクスチャの解放---//
    if (g_pD3DSCOTex != NULL)
    {
        g_pD3DSCOTex->Release();
        g_pD3DSCOTex = NULL;
    }
    if (g_pD3DSCOVtxBuff != NULL)
    {
        g_pD3DSCOVtxBuff->Release();
        g_pD3DSCOVtxBuff = NULL;
    }
    g_nScore = 0;
}
void COUNT_UP(void)
{
    g_nScore++;
}
void COUNT_UP2(void)
{
    g_nScore += 500;
}
void COUNT_DOWN(void)
{
    g_nScore--;
}
void Result_Scoredisp(void)
{
    int work = 10000000;
    int workbox[KETA];

    g_pD3DSCOVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPlace = 0; nCntPlace < KETA; nCntPlace++)
    {
        workbox[nCntPlace] = (g_nScore / work) % 10;

        work /= 10;

        pVtx[0].vtx = D3DXVECTOR3(450 + ALFA * (float)nCntPlace, 300, 0.0f);

        pVtx[1].vtx = D3DXVECTOR3(500 + ALFA * (float)nCntPlace, 300, 0.0f);

        pVtx[2].vtx = D3DXVECTOR3(450 + ALFA * (float)nCntPlace, 390, 0.0f);

        pVtx[3].vtx = D3DXVECTOR3(500 + ALFA * (float)nCntPlace, 390, 0.0f);

        pVtx[0].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace], 0);

        pVtx[1].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace] + 0.1f, 0);

        pVtx[2].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace], 1.0f);

        pVtx[3].tex = D3DXVECTOR2(0.1f * workbox[nCntPlace] + 0.1f, 1.0f);

        pVtx += 4;
    }
    g_pD3DSCOVtxBuff->Unlock();
}
int GetScore(void)
{
    return g_nScore;
}