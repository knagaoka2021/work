//=============================================================================
//
// 地面処理 [field.cpp]
// Author : 長岡　和真
//
//=============================================================================
#include "field.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_W	(4)
#define TEX_H	(10)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureField = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// 頂点バッファインターフェースへのポインタ

static D3DXVECTOR3 g_posCameraP;							// カメラの視点
static D3DXVECTOR3 g_posCameraR;							// カメラの注視点
static D3DXVECTOR3 g_vecCameraU;							// カメラの上方向ベクトル


static D3DXMATRIX g_mtxWorld;								// ワールドマトリックス
static D3DXVECTOR3 g_posField;								// 地面の位置
static D3DXVECTOR3 g_rotField;								// 地面の向き（回転）
static D3DXVECTOR3 g_sclField;								// 地面の大きさ（スケール）

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


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    //SetVtxData();								// 頂点情報の設定

    //LoadTexture();								// テクスチャの読み込み

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
    //---テクスチャの解放---//
    if (g_pD3DTextureField != NULL)
    {
        g_pD3DTextureField->Release();
        g_pD3DTextureField = NULL;
    }
    if (g_pD3DVtxBuffField != NULL)
    {
        g_pD3DVtxBuffField->Release();
        g_pD3DVtxBuffField = NULL;
    }
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;


    // World Matrix
    D3DXMatrixIdentity(&g_mtxWorld);															// 初期化

    D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

    D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

    pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

    pDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

    pDevice->SetTexture(0, g_pD3DTextureField);										// テクスチャの設定

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 12);								// 2はポリゴン数(又はポリゴン数を３で割る)


}


