//=============================================================================
//
// 影の処理 [shadow.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "shadow.h"
#include "input.h"
#include "model.h"
#include "bullet.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
    D3DXVECTOR3 g_posShadow;								// 地面の位置
    D3DXVECTOR3 g_rotShadow;								// 地面の向き（回転）

}SHADOW;
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_W	(4)
#define SHADOW_MAX	(201)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT SetShadowVtxData(void);
void LoadShadowTexture(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureShadow = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffShadow[SHADOW_MAX];	// 頂点バッファインターフェースへのポインタ

SHADOW g_Shadow[SHADOW_MAX];


static D3DXMATRIX g_mtxWorld;								// ワールドマトリックス
static D3DXVECTOR3 g_posShadow;								// 地面の位置
static D3DXVECTOR3 g_rotShadow;								// 地面の向き（回転）
static D3DXVECTOR3 g_sclShadow;								// 地面の大きさ（スケール）

static float g_fLength;										//ポリゴン対角線の長さ
static float g_fAngle;										//ポリゴン対角線の角度
static float g_W = 0;											//
static float g_H = 0;
static float g_X = 0;
static float g_Y = 0;
static float g_Z = 0;
static float g_MoveUP = 1;
static float g_MoveDOWN = 1;
static float g_MoveRIGHT = 0;
static float g_MoveLEFT = 0;

VERTEX_3D* pShadowVtx;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    for (int i = 0; i < SHADOW_MAX; i++)
    {
        g_Shadow[i].g_posShadow = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);
        g_Shadow[i].g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }

    g_posShadow = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_sclShadow = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    SetShadowVtxData();								// 頂点情報の設定

    LoadShadowTexture();								// テクスチャの読み込み

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
    //---テクスチャの解放---//
    if (g_pD3DTextureShadow != NULL)
    {
        g_pD3DTextureShadow->Release();
        g_pD3DTextureShadow = NULL;
    }
    for (int i = 0; i < SHADOW_MAX; i++)
    {
        if (g_pD3DVtxBuffShadow[i] != NULL)
        {
            g_pD3DVtxBuffShadow[i]->Release();
            g_pD3DVtxBuffShadow[i] = NULL;
        }
    }
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{
    g_Shadow[0].g_posShadow = GetPos();
    g_Shadow[0].g_posShadow.y = 0;

    for (int i = 1; i < SHADOW_MAX; i++)
    {
        g_Shadow[i].g_posShadow = GetBulletPos(i - 1);

        g_Shadow[i].g_posShadow.y = 0;
    }


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);

    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

    for (int i = 0; i < SHADOW_MAX; i++)
    {

        // World Matrix
        D3DXMatrixIdentity(&g_mtxWorld);															// 初期化

        D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

        D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[i].g_rotShadow.y, g_Shadow[i].g_rotShadow.x, g_Shadow[i].g_rotShadow.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

        D3DXMatrixTranslation(&mtxTranslate, g_Shadow[i].g_posShadow.x, g_Shadow[i].g_posShadow.y, g_Shadow[i].g_posShadow.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

        pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

        pDevice->SetStreamSource(0, g_pD3DVtxBuffShadow[i], 0, sizeof(VERTEX_3D));

        pDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

        pDevice->SetTexture(0, g_pD3DTextureShadow);										// テクスチャの設定

        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);								// 2はポリゴン数(又はポリゴン数を３で割る)

    }

    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);



}
//=============================================================================
// 頂点情報の設定
//=============================================================================
HRESULT SetShadowVtxData(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    for (int i = 0; i < SHADOW_MAX; i++)
    {
        // 頂点バッファの生成
        if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pD3DVtxBuffShadow[i], NULL)))
        {
            return E_FAIL;
        }

        VERTEX_3D Vertices[] = {
            { D3DXVECTOR3(-50.0f , 0.0f , 50.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//0
            { D3DXVECTOR3(50.0f , 0.0f , 50.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) } ,//2
            { D3DXVECTOR3(-50.0f , 0.0f , -50.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,//6
            { D3DXVECTOR3(50.0f , 0.0f , -50.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(1.0f,1.0f)} ,//8
        };




        g_pD3DVtxBuffShadow[i]->Lock(0, sizeof(pShadowVtx), (void**)&pShadowVtx, 0);

        memcpy(pShadowVtx, Vertices, sizeof(Vertices));

        g_pD3DVtxBuffShadow[i]->Unlock();

    }



    return S_OK;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void LoadShadowTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_pD3DTextureShadow);
}
