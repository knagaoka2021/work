//=============================================================================
//
// ステータス処理 [status.cpp]
// Author : 長岡　和真
//
//=============================================================================
#include "timer.h"
#include "fade.h"
static LPDIRECT3DTEXTURE9 g_pD3DStatusTex = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_pD3DStatusVtxBuff = NULL;
static VERTEX_2D* pVtx;

HRESULT InitStatus(void)
{
    LPDIRECT3DDEVICE9 pDevice;


    pDevice = GetDevice();

    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pD3DStatusVtxBuff, NULL)))
    {
        return E_FAIL;
    }

    g_pD3DStatusVtxBuff->Lock(0, 0, (void**)&pVtx, 0);



    pVtx[0].vtx = D3DXVECTOR3(200, 0, 0.0f);

    pVtx[1].vtx = D3DXVECTOR3(300, 0, 0.0f);

    pVtx[2].vtx = D3DXVECTOR3(200, 50, 0.0f);

    pVtx[3].vtx = D3DXVECTOR3(300, 50, 0.0f);

    pVtx[0].rhw = 1.0f;

    pVtx[1].rhw = 1.0f;

    pVtx[2].rhw = 1.0f;

    pVtx[3].rhw = 1.0f;

    pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[0].tex = D3DXVECTOR2(0.0f, 0);

    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    pVtx += 4;												//４つ頂点で一つアドレスずらし
    g_pD3DStatusVtxBuff->Unlock();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\time.png", &g_pD3DStatusTex);

    return S_OK;
}
void UpdateStatus(void)
{



}
void DrawStatus(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    pDevice = GetDevice();

    pDevice->SetStreamSource(0, g_pD3DStatusVtxBuff, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->SetTexture(0, g_pD3DStatusTex);//テクスチャの設定

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//2はポリゴン数
}
void UninitStatus(void)
{
    //---テクスチャの解放---//
    if (g_pD3DStatusTex != NULL)
    {
        g_pD3DStatusTex->Release();
        g_pD3DStatusTex = NULL;
    }
    if (g_pD3DStatusVtxBuff != NULL)
    {
        g_pD3DStatusVtxBuff->Release();
        g_pD3DStatusVtxBuff = NULL;
    }

}
