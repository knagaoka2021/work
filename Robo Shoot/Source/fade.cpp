//=============================================================================
//
// フェード [fade.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "fade.h"
#include "input.h"
static LPDIRECT3DVERTEXBUFFER9 g_pD3DBGVtxBuff = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBG = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBGTex = NULL;
static VERTEX_2D* pVtx;
static int alpha = 255;
FADE g_fade;

void FADEIN(void);
void FADEOUT(void);

HRESULT InitFade(void)
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


    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\white.jpg", &g_pD3DBGTex);

    g_fade = FADE_OUT;

    return S_OK;
}

void UpdateFade(void)
{
    switch (g_fade)
    {
    case FADE_NONE:
        alpha = 0;

        break;
    case FADE_IN:
        if (alpha < 255)
        {
            FADEIN();
        }
        else
        {
            g_fade = FADE_NONE;
        }
        break;
    case FADE_OUT:
        if (alpha > 0)
        {
            FADEOUT();
        }
        else
        {
            g_fade = FADE_MAX;
        }
        break;
    case FADE_MAX:
        break;
    }

    g_pD3DBGVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);

    pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);

    pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);

    pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);


    g_pD3DBGVtxBuff->Unlock();
}

void DrawFade(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    pDevice = GetDevice();

    pDevice->SetStreamSource(0, g_pD3DBGVtxBuff, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->SetTexture(0, g_pD3DBGTex);//テクスチャの設定

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//2はポリゴン数
}
void UninitFade(void)
{

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
}
void SetFade(FADE fade)
{
    g_fade = fade;
}
FADE GetFade(void)
{
    return g_fade;
}
void FADEIN(void)
{
    alpha += 3;
}
void FADEOUT(void)
{
    alpha -= 3;
}