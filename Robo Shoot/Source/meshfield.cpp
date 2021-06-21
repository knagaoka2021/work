//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "meshfield.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VTX_MAX	(9)						// 頂点最大数
#define INDXVTX_MAX	(14)				// インデックス最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT SetVtxData(void);
void LoadTexture(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureField;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBufferField;	// インデックスバッファ

D3DXMATRIX g_mtxWorldField;					// ワールドマトリックス
D3DXVECTOR3 g_posField;						// 位置
D3DXVECTOR3 g_rotField;						// 向き
VERTEX_3D* pVtx;

int g_nNumBlockX, g_nNumBlockZ;				// ブロック数
int g_nNumVertex;							// 総頂点数
int g_nNumPolygon;							// 総ポリゴン数
float g_fSizeBlockX, g_fSizeBlockZ;			// ブロックサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    LPDIRECT3DDEVICE9 pIndexDevice = GetDevice();

    g_nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

    g_nNumPolygon = g_nNumVertex + ((fSizeBlockX) * (fSizeBlockZ)) + 1;

    g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


    SetVtxData();								// 頂点情報の設定

    LoadTexture();								// テクスチャの読み込み


    // インデックスバッファの生成
    if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumPolygon, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pD3DIndexBufferField, NULL)))
    {
        return E_FAIL;
    }

    WORD* pIndex;

    g_pD3DIndexBufferField->Lock(0, 0, (void**)&pIndex, 0);

    pIndex[0] = 3;
    pIndex[1] = 0;
    pIndex[2] = 4;
    pIndex[3] = 1;
    pIndex[4] = 5;
    pIndex[5] = 2;
    pIndex[6] = 2;
    pIndex[7] = 6;
    pIndex[8] = 6;
    pIndex[9] = 3;
    pIndex[10] = 7;
    pIndex[11] = 4;
    pIndex[12] = 8;
    pIndex[13] = 5;

    g_pD3DIndexBufferField->Unlock();


    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
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
    if (g_pD3DIndexBufferField != NULL)
    {
        g_pD3DIndexBufferField->Release();
        g_pD3DIndexBufferField = NULL;
    }
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{
}

//=============================================================================
// 描画初理
//=============================================================================
void DrawMeshField(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;


    // World Matrix
    D3DXMatrixIdentity(&g_mtxWorldField);															// 初期化

    D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);

    D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);

    D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

    D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);

    D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

    pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

    pDevice->SetIndices(g_pD3DIndexBufferField);

    pDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

    pDevice->SetTexture(0, g_pD3DTextureField);										// テクスチャの設定

    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertex, 0, 12);			// 2はポリゴン数(又はポリゴン数を３で割る)


}
//=============================================================================
// 頂点情報の設定
//=============================================================================
HRESULT SetVtxData(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    // 頂点バッファの生成
    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pD3DVtxBuffField, NULL)))
    {
        return E_FAIL;
    }

    VERTEX_3D Vertices[] =
    {
        { D3DXVECTOR3(-10000.0f , 0.0f , 10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//0
        { D3DXVECTOR3(0.0f , 0.0f , 10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) } ,//1
        { D3DXVECTOR3(10000.0f , 0.0f , 10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//2
        { D3DXVECTOR3(-10000.0f , 0.0f , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,//3
        { D3DXVECTOR3(0.0f , 0.0f , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) } ,//4
        { D3DXVECTOR3(10000.0f , 0.0f , 0.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,//5
        { D3DXVECTOR3(-10000.0f , 0.0f , -10000.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//6
        { D3DXVECTOR3(0.0f , 0.0f , -10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) } ,//7
        { D3DXVECTOR3(10000.0f , 0.0f , -10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(0.0f,0.0f)} ,//8
    };

    g_pD3DVtxBuffField->Lock(0, sizeof(Vertices), (void**)&pVtx, 0);

    memcpy(pVtx, Vertices, sizeof(Vertices));

    g_pD3DVtxBuffField->Unlock();




    return S_OK;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void LoadTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fgy.jpg", &g_pD3DTextureField);
}