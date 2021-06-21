//=============================================================================
//
// タイトル [title.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

static LPDIRECT3DVERTEXBUFFER9 g_pD3DBGVtxBuff = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBG = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBGTex = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_pD3DBG2VtxBuff = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBG2Tex = NULL;
static VERTEX_2D* pVtx;
static int alpha = 0;

HRESULT InitTitle(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    pDevice = GetDevice();

    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pD3DBGVtxBuff, NULL)))
    {
        return E_FAIL;
    }

    g_pD3DBGVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].vtx = D3DXVECTOR3(SCREEN_WIDTH / 4, 0, 0.0f);

    pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4, 0, 0.0f);

    pVtx[2].vtx = D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8, 0.0f);

    pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8, 0.0f);

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

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title.png", &g_pD3DBGTex);

    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pD3DBG2VtxBuff, NULL)))
    {
        return E_FAIL;
    }

    g_pD3DBG2VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].vtx = D3DXVECTOR3(SCREEN_WIDTH / 4, 600, 0.0f);

    pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4, 600, 0.0f);

    pVtx[2].vtx = D3DXVECTOR3(SCREEN_WIDTH / 4, 800, 0.0f);

    pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4, 800, 0.0f);

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

    g_pD3DBG2VtxBuff->Unlock();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\push.png", &g_pD3DBG2Tex);

    PlaySound(SOUND_LABEL_BGM000, XAUDIO2_LOOP_INFINITE);


    return S_OK;
}
void UpdateTitle(void)
{

    if (tringkeyboardPress(DIK_RETURN) || tringpadPress(0, BUTTON_START))
    {
        PlaySound(SOUND_LABEL_SE_SYSTEM, 0);
        SetFade(FADE_IN);
    }

    if (GetFade() == FADE_NONE)
    {
        SetMode(MODE_TUTO);
        SetFade(FADE_OUT);
    }

    g_pD3DBG2VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);

    pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);

    pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);

    pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);


    g_pD3DBG2VtxBuff->Unlock();



    alpha += 6;
    if (alpha > 6000)
    {
        alpha = 0;
    }

}
void DrawTitle(void)
{
    LPDIRECT3DDEVICE9 pDevice;


    pDevice = GetDevice();

    pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    pDevice->SetStreamSource(0, g_pD3DBGVtxBuff, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->SetTexture(0, g_pD3DBGTex);//テクスチャの設定

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//2はポリゴン数


    pDevice->SetStreamSource(0, g_pD3DBG2VtxBuff, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->SetTexture(0, g_pD3DBG2Tex);//テクスチャの設定

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//2はポリゴン数

    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
void UninitTitle(void)
{
    StopSound(SOUND_LABEL_BGM000);

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

    if (g_pD3DBG2Tex != NULL)
    {
        g_pD3DBG2Tex->Release();
        g_pD3DBG2Tex = NULL;
    }
    if (g_pD3DBG2VtxBuff != NULL)
    {
        g_pD3DBG2VtxBuff->Release();
        g_pD3DBG2VtxBuff = NULL;
    }
    alpha = 0;
}