//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : 長岡　和真
//
//=============================================================================

#include "pause.h"
#include "input.h"
#include "fade.h"
#include"sound.h"

#define ALFA	(40)
#define BLOCK	(5)
#define	FILE	("data\\TEXTURE\\MENU.png")

static LPDIRECT3DTEXTURE9 g_pD3DSCOTex[BLOCK] = { NULL };
static LPDIRECT3DVERTEXBUFFER9 g_pD3DSCOVtxBuff[BLOCK] = { NULL };
static VERTEX_2D* pVtx;
static LPCSTR     pSrcFile[BLOCK] = {
{"data\\TEXTURE\\pause_ui_01.png"},
{"data\\TEXTURE\\pause_ui_02.png"},
{"data\\TEXTURE\\pause_ui_03.png"}
};
static float g_ofsetPos = 0;


HRESULT InitPause(void)
{
    LPDIRECT3DDEVICE9 pDevice[BLOCK];
    g_ofsetPos = 30;


    for (int i = 0; i < BLOCK; i++)
    {
        pDevice[i] = GetDevice();

        if (FAILED(pDevice[i]->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * BLOCK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pD3DSCOVtxBuff[i], NULL)))
        {
            return E_FAIL;
        }

        g_pD3DSCOVtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);

        switch (i)
        {
        case 0:
            pVtx[0].vtx = D3DXVECTOR3(200, 0, 0.0f);

            pVtx[1].vtx = D3DXVECTOR3(1040, 0, 0.0f);

            pVtx[2].vtx = D3DXVECTOR3(200, 350, 0.0f);

            pVtx[3].vtx = D3DXVECTOR3(1040, 350, 0.0f);

            pVtx[0].rhw = 1.0f;

            pVtx[1].rhw = 1.0f;

            pVtx[2].rhw = 1.0f;

            pVtx[3].rhw = 1.0f;

            pVtx[0].diffuse = D3DCOLOR_RGBA(0, 0, 255, 85);

            pVtx[1].diffuse = D3DCOLOR_RGBA(10, 255, 255, 85);

            pVtx[2].diffuse = D3DCOLOR_RGBA(25, 0, 255, 85);

            pVtx[3].diffuse = D3DCOLOR_RGBA(25, 0, 255, 85);
            break;

        case 4:
            pVtx[0].vtx = D3DXVECTOR3(400, 105, 0.0f);

            pVtx[1].vtx = D3DXVECTOR3(460, 105, 0.0f);

            pVtx[2].vtx = D3DXVECTOR3(400, 130, 0.0f);

            pVtx[3].vtx = D3DXVECTOR3(460, 130, 0.0f);

            pVtx[0].rhw = 1.0f;

            pVtx[1].rhw = 1.0f;

            pVtx[2].rhw = 1.0f;

            pVtx[3].rhw = 1.0f;

            pVtx[0].diffuse = D3DCOLOR_RGBA(0, 0, 255, 255);

            pVtx[1].diffuse = D3DCOLOR_RGBA(10, 255, 255, 255);

            pVtx[2].diffuse = D3DCOLOR_RGBA(25, 0, 255, 255);

            pVtx[3].diffuse = D3DCOLOR_RGBA(25, 0, 255, 255);
            break;
        default:
            for (int nCntPlace = 0; nCntPlace < 1; nCntPlace++)
            {

                pVtx[0].vtx = D3DXVECTOR3(500 + ALFA * (float)nCntPlace, 0 + g_ofsetPos, 0.0f);

                pVtx[1].vtx = D3DXVECTOR3(700 + ALFA * (float)nCntPlace, 0 + g_ofsetPos, 0.0f);

                pVtx[2].vtx = D3DXVECTOR3(500 + ALFA * (float)nCntPlace, 35 + g_ofsetPos, 0.0f);

                pVtx[3].vtx = D3DXVECTOR3(700 + ALFA * (float)nCntPlace, 35 + g_ofsetPos, 0.0f);

                pVtx[0].rhw = 1.0f;

                pVtx[1].rhw = 1.0f;

                pVtx[2].rhw = 1.0f;

                pVtx[3].rhw = 1.0f;

                pVtx[0].diffuse = D3DCOLOR_RGBA(0, 0, 255, 255);

                pVtx[1].diffuse = D3DCOLOR_RGBA(10, 255, 255, 255);

                pVtx[2].diffuse = D3DCOLOR_RGBA(25, 0, 255, 255);

                pVtx[3].diffuse = D3DCOLOR_RGBA(25, 0, 255, 255);

                pVtx[0].tex = D3DXVECTOR2(0.0f, 0);

                pVtx[1].tex = D3DXVECTOR2(1, 0.0f);

                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

                pVtx[3].tex = D3DXVECTOR2(1, 1.0f);

                //pVtx+=4;												//４つ頂点で一つアドレスずらし
            }break;

        }


        g_pD3DSCOVtxBuff[i]->Unlock();

        //--------テクスチャの読み込み----------//

        g_ofsetPos += 70;

    }
    g_ofsetPos = 0;
    D3DXCreateTextureFromFile(pDevice[1], pSrcFile[0], &g_pD3DSCOTex[1]);
    D3DXCreateTextureFromFile(pDevice[2], pSrcFile[1], &g_pD3DSCOTex[2]);
    D3DXCreateTextureFromFile(pDevice[3], pSrcFile[2], &g_pD3DSCOTex[3]);
    return S_OK;
}
void UpdatePause(void)
{
    if (GetFade() == FADE_MAX)
    {
        g_pD3DSCOVtxBuff[4]->Lock(0, 0, (void**)&pVtx, 0);

        pVtx[0].vtx = D3DXVECTOR3(400, 105 + g_ofsetPos, 0.0f);

        pVtx[1].vtx = D3DXVECTOR3(460, 105 + g_ofsetPos, 0.0f);

        pVtx[2].vtx = D3DXVECTOR3(400, 130 + g_ofsetPos, 0.0f);

        pVtx[3].vtx = D3DXVECTOR3(460, 130 + g_ofsetPos, 0.0f);

        g_pD3DSCOVtxBuff[4]->Unlock();

        if (tringkeyboardPress(DIK_P))
        {
            SetPauseflag(false);
        }
        if (tringkeyboardPress(DIK_DOWNARROW))
        {
            g_ofsetPos += 70;
        }
        if (tringkeyboardPress(DIK_UPARROW))
        {
            g_ofsetPos -= 70;
        }
        if (g_ofsetPos < 0)
        {
            g_ofsetPos = 140;
        }
        else if (g_ofsetPos > 140)
        {
            g_ofsetPos = 0;
        }
        if (ReleasekeyboardPress(DIK_RETURN) && g_ofsetPos == 0)
        {
            SetPauseflag(false);
            PlaySound(SOUND_LABEL_SE_SYSTEM, 0);
        }
        if (ReleasekeyboardPress(DIK_RETURN) && g_ofsetPos == 70)
        {
            SetFade(FADE_IN);
            PlaySound(SOUND_LABEL_SE_SYSTEM, 0);
        }
        if (ReleasekeyboardPress(DIK_RETURN) && g_ofsetPos == 140)
        {
            SetFade(FADE_IN);
            PlaySound(SOUND_LABEL_SE_SYSTEM, 0);

        }
    }
    if (GetFade() == FADE_NONE)
    {
        PauseUninitGame();
        SetPauseflag(false);
        if (g_ofsetPos == 70)
        {
            PauseInitGame();
            SetFade(FADE_OUT);
        }
        else
        {
            SetMode(MODE_TITLE);
            SetFade(FADE_OUT);
        }
    }
}
void DrawPause(void)
{
    LPDIRECT3DDEVICE9 pDevice[BLOCK];

    for (int i = 0; i < BLOCK; i++)
    {
        pDevice[i] = GetDevice();

        pDevice[i]->SetStreamSource(0, g_pD3DSCOVtxBuff[i], 0, sizeof(VERTEX_2D));

        pDevice[i]->SetFVF(FVF_VERTEX_2D);

        pDevice[i]->SetTexture(0, g_pD3DSCOTex[i]);//テクスチャの設定

        for (int nCntPlace = 0; nCntPlace < BLOCK; nCntPlace++)
        {
            pDevice[i]->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlace * 4, 2);//2はポリゴン数
        }
    }
}
void UninitPause(void)
{
    for (int i = 0; i < BLOCK; i++)
    {
        //---テクスチャの解放---//
        if (g_pD3DSCOTex[i] != NULL)
        {
            g_pD3DSCOTex[i]->Release();
            g_pD3DSCOTex[i] = NULL;
        }
        if (g_pD3DSCOVtxBuff[i] != NULL)
        {
            g_pD3DSCOVtxBuff[i]->Release();
            g_pD3DSCOVtxBuff[i] = NULL;
        }
    }
}
