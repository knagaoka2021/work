//=============================================================================
//
// 壁の処理 [wall.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "wall.h"
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
HRESULT SetWallVtxData(void);
void LoadWallTexture(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureWall;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffWall;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBufferWall;	// インデックスバッファ

D3DXMATRIX g_mtxWorldWall;					// ワールドマトリックス
D3DXVECTOR3 g_posWall;						// 位置
D3DXVECTOR3 g_rotWall;						// 向き
VERTEX_3D* pWallVtx;

int g_nNumWallBlockX, g_nNumWallBlockZ;				// ブロック数
int g_nNumWallVertex;							// 総頂点数
int g_nNumWallPolygon;							// 総ポリゴン数
float g_fSizeWallBlockX, g_fSizeWallBlockZ;			// ブロックサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshWall(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, int ste)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    LPDIRECT3DDEVICE9 pIndexDevice = GetDevice();

    g_nNumWallVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

    g_nNumWallPolygon = g_nNumWallVertex + ((fSizeBlockX) * (fSizeBlockZ)) + 1;

    g_posWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_rotWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


    SetWallVtxData();								// 頂点情報の設定

    LoadWallTexture();								// テクスチャの読み込み


    // インデックスバッファの生成
    if (FAILED(pIndexDevice->CreateIndexBuffer(sizeof(WORD) * 10, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pD3DIndexBufferWall, NULL)))
    {
        return E_FAIL;
    }

    WORD* pIndex;

    g_pD3DIndexBufferWall->Lock(0, 0, (void**)&pIndex, 0);

    pIndex[0] = 5;
    pIndex[1] = 0;
    pIndex[2] = 6;
    pIndex[3] = 1;
    pIndex[4] = 7;
    pIndex[5] = 2;
    pIndex[6] = 8;
    pIndex[7] = 3;
    pIndex[8] = 9;
    pIndex[9] = 4;
    //pIndex[10] = 7;
    //pIndex[11] = 4;
    //pIndex[12] = 8;
    //pIndex[13] = 5;

    g_pD3DIndexBufferWall->Unlock();


    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{
    //---テクスチャの解放---//
    if (g_pD3DTextureWall != NULL)
    {
        g_pD3DTextureWall->Release();
        g_pD3DTextureWall = NULL;
    }
    if (g_pD3DVtxBuffWall != NULL)
    {
        g_pD3DVtxBuffWall->Release();
        g_pD3DVtxBuffWall = NULL;
    }
    if (g_pD3DIndexBufferWall != NULL)
    {
        g_pD3DIndexBufferWall->Release();
        g_pD3DIndexBufferWall = NULL;
    }
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// 描画初理
//=============================================================================
void DrawMeshWall(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;


    // World Matrix
    D3DXMatrixIdentity(&g_mtxWorldWall);															// 初期化

    D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);

    D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxScl);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotWall.y, g_rotWall.x, g_rotWall.z);

    D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

    D3DXMatrixTranslation(&mtxTranslate, g_posWall.x, g_posWall.y, g_posWall.z);

    D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTranslate);

    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

    pDevice->SetStreamSource(0, g_pD3DVtxBuffWall, 0, sizeof(VERTEX_3D));

    pDevice->SetIndices(g_pD3DIndexBufferWall);

    pDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

    pDevice->SetTexture(0, g_pD3DTextureWall);										// テクスチャの設定

    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumWallVertex, 0, 12);			// 2はポリゴン数(又はポリゴン数を３で割る)


}
//=============================================================================
// 頂点情報の設定
//=============================================================================
HRESULT SetWallVtxData(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    // 頂点バッファの生成
    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 10, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pD3DVtxBuffWall, NULL)))
    {
        return E_FAIL;
    }

    VERTEX_3D Vertices[] =
    {
        { D3DXVECTOR3(-10000.0f , 10000.0f , 10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//0
        { D3DXVECTOR3(10000.0f , 10000.0f ,10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) } ,//1
        { D3DXVECTOR3(10000.0f , 10000.0f , -10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//2
        { D3DXVECTOR3(-10000.0f , 10000.0f , -10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) } ,//3
        { D3DXVECTOR3(-10000.0f , 10000.0f , 10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//4
        { D3DXVECTOR3(-10000.0f , 0.0f , 10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,//5
        { D3DXVECTOR3(10000.0f , 0.0f , 10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) } ,//6
        { D3DXVECTOR3(10000.0f , 0.0f , -10000.0f) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,//7
        { D3DXVECTOR3(-10000.0f , 0.0f , -10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) } ,//8
        { D3DXVECTOR3(-10000.0f , 0.0f , 10000.0f) , D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(0.0f,1.0f)} ,//9
    };

    g_pD3DVtxBuffWall->Lock(0, sizeof(Vertices), (void**)&pWallVtx, 0);

    memcpy(pWallVtx, Vertices, sizeof(Vertices));

    g_pD3DVtxBuffWall->Unlock();




    return S_OK;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void LoadWallTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Wall001.jpg", &g_pD3DTextureWall);
}

//bool Collision(float ax,float ay,float aw,float ah,float bx,float by,float bw,float bh)
//{
//	//return;
//}
