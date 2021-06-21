//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 長岡　和真
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_W	(4)
#define TEX_H	(10)
#define DRIVE	(4)
#define WALL_LIMIT   (9949)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT SetVtxData(void);
void LoadTexture(void);
float RotCheck(float rot);
void WallCheck(void);
void LoadmodelTexture(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureModel;		// テクスチャへのポインタ
LPD3DXMESH g_pD3DXMeshModel;
LPD3DXBUFFER g_pD3DXBuffMatModel;
DWORD g_nNumMatModel;

D3DXVECTOR3 g_posModel;
D3DXVECTOR3 g_rotModel;
D3DXVECTOR3 g_sclModel;
D3DXVECTOR3 g_rot;
D3DXVECTOR3 g_rotDestModel;




static D3DXMATRIX g_mtxWorld;								// ワールドマトリックス


static float g_fLength;										//ポリゴン対角線の長さ
static float g_fAngle;										//ポリゴン対角線の角度
static float g_W = 0;											//
static float g_H = 0;
static float g_X = 0;
static float g_Y = 0;
static float g_Z = 0;
static float g_SpeedX = 0;
static float g_SpeedY = 0;
static float g_SpeedZ = 0;
static bool g_flag = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModel(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    g_posModel = D3DXVECTOR3(1.0f, 1.0f, 0.0f);

    g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    LoadmodelTexture();								// テクスチャの読み込み

    if (FAILED(D3DXLoadMeshFromX("data\\MODEL\\robo.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pD3DXBuffMatModel, NULL, &g_nNumMatModel, &g_pD3DXMeshModel)))
    {
        return E_FAIL;
    }



    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
    //---テクスチャの解放---//
    if (g_pD3DTextureModel != NULL)
    {
        g_pD3DTextureModel->Release();
        g_pD3DTextureModel = NULL;
    }
    if (g_pD3DXMeshModel != NULL)
    {
        g_pD3DXMeshModel->Release();
        g_pD3DXMeshModel = NULL;
    }
    if (g_pD3DXBuffMatModel != NULL)
    {
        g_pD3DXBuffMatModel->Release();
        g_pD3DXBuffMatModel = NULL;
    }

    g_posModel = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    g_rot.y = 0;
    g_rotDestModel.y = 0;
    g_SpeedX = 0;
    g_SpeedZ = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
    g_rot = GetRotCamera();

    if (tringkeyboardPress(DIK_F3))
    {
        g_flag = TRUE;
    }
    if (tringkeyboardPress(DIK_F4))
    {
        g_flag = FALSE;
    }

    if (GetkeyboardPress(DIK_W) || PresspadCheak(0, BUTTON_UP))
    {
        g_SpeedX += sinf(g_rot.y) * DRIVE;
        g_SpeedZ += cosf(g_rot.y) * DRIVE;
        g_rotDestModel.y = 3.14f;


        if (GetkeyboardPress(DIK_A))
        {
            g_rotDestModel.y = 2.36f;
        }

    }
    if (GetkeyboardPress(DIK_S) || PresspadCheak(0, BUTTON_DOWN))
    {
        g_SpeedX -= sinf(g_rot.y) * DRIVE;
        g_SpeedZ -= cosf(g_rot.y) * DRIVE;
        g_rotDestModel.y = 0;

    }
    if (GetkeyboardPress(DIK_D) || PresspadCheak(0, BUTTON_RIGHT))
    {
        g_SpeedX += sinf(g_rot.y + 90 * D3DX_PI / 180) * DRIVE;
        g_SpeedZ += cosf(g_rot.y + 90 * D3DX_PI / 180) * DRIVE;

        g_rotDestModel.y = -1.57f;

        if (GetkeyboardPress(DIK_W) || PresspadCheak(0, BUTTON_UR))
        {
            g_rotDestModel.y = -2.36f;
        }
        if (GetkeyboardPress(DIK_S) || PresspadCheak(0, BUTTON_DR))
        {
            g_rotDestModel.y = -0.79f;
        }
    }
    if (GetkeyboardPress(DIK_A) || PresspadCheak(0, BUTTON_LEFT))
    {
        g_SpeedX -= sinf(g_rot.y + 90 * D3DX_PI / 180) * DRIVE;
        g_SpeedZ -= cosf(g_rot.y + 90 * D3DX_PI / 180) * DRIVE;
        g_rotDestModel.y = 1.57f;

        if (GetkeyboardPress(DIK_W) || PresspadCheak(0, BUTTON_UL))
        {
            g_rotDestModel.y = 2.36f;
        }
        if (GetkeyboardPress(DIK_S) || PresspadCheak(0, BUTTON_DL))
        {
            g_rotDestModel.y = 0.79f;
        }
    }

    float fDiftRotY;


    if (g_rotDestModel.y - g_rotModel.y <= -D3DX_PI)
    {

        fDiftRotY = g_rotModel.y - g_rotDestModel.y;

        fDiftRotY = RotCheck(fDiftRotY);

        g_rotModel.y -= fDiftRotY * 0.1f;

        g_rotModel.y = RotCheck(g_rotModel.y);
    }
    else if (g_rotDestModel.y - g_rotModel.y >= D3DX_PI)
    {

        fDiftRotY = g_rotModel.y - g_rotDestModel.y;

        fDiftRotY = RotCheck(fDiftRotY);

        g_rotModel.y -= fDiftRotY * 0.1f;

        g_rotModel.y = RotCheck(g_rotModel.y);
    }
    else
    {
        fDiftRotY = g_rotDestModel.y - g_rotModel.y;

        fDiftRotY = RotCheck(fDiftRotY);

        g_rotModel.y += fDiftRotY * 0.1f;

        g_rotModel.y = RotCheck(g_rotModel.y);
    }

    g_posModel.x += g_SpeedX;
    g_posModel.z += g_SpeedZ;

    WallCheck();

    g_SpeedX *= 0.85f;
    g_SpeedZ *= 0.85f;


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    D3DXMATERIAL* pD3DXMat;
    D3DMATERIAL9 matDef;


    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;


    // World Matrix
    D3DXMatrixIdentity(&g_mtxWorld);

    D3DXMatrixScaling(&mtxScl, g_sclModel.x, g_sclModel.y, g_sclModel.z);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

    D3DXMatrixTranslation(&mtxTranslate, g_posModel.x, g_posModel.y, g_posModel.z);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

    pDevice->GetMaterial(&matDef);

    pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();

    for (int nCntMat = 0; nCntMat < g_nNumMatModel; nCntMat++)
    {

        pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

        if (g_flag == TRUE)
        {
            pDevice->SetTexture(0, g_pD3DTextureModel);
        }
        else
        {
            pDevice->SetTexture(0, NULL);
        }

        g_pD3DXMeshModel->DrawSubset(nCntMat);
    }

    pDevice->SetMaterial(&matDef);								// マテリアルを元に戻す

}

D3DXVECTOR3 GetPos(void)
{
    return g_posModel;
}
D3DXVECTOR3 GetRot(void)
{
    return g_rotModel;

}
float GetRotbullet(void)
{
    return g_rotModel.y;

}
float GetPosX(void)
{
    return g_posModel.x;

}
float GetPosZ(void)
{
    return g_posModel.z;

}
float RotCheck(float rot)
{
    if (D3DX_PI > rot)
    {
        rot -= D3DX_PI * 2;
    }
    if (-D3DX_PI <= rot)
    {
        rot += D3DX_PI * 2;
    }

    return rot;

}
void WallCheck(void)
{
    if (g_posModel.x > WALL_LIMIT)
    {
        g_posModel.x = WALL_LIMIT;
    }
    if (g_posModel.x < -WALL_LIMIT)
    {
        g_posModel.x = -WALL_LIMIT;
    }
    if (g_posModel.z < -WALL_LIMIT)
    {
        g_posModel.z = -WALL_LIMIT;
    }
    if (g_posModel.z > WALL_LIMIT)
    {
        g_posModel.z = WALL_LIMIT;
    }
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void LoadmodelTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Untitled1.jpg", &g_pD3DTextureModel);
}
void SetHitPosition(D3DXVECTOR3 pos)
{
    g_posModel.x = pos.x;
    g_posModel.y = pos.y;
    g_posModel.z = pos.z;

}
void RollModel(void)
{
    g_rotModel.y += 0.5f;
}