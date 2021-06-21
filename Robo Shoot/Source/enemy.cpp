//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 長岡　和真
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "bullet.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_W	(4)
#define TEX_H	(10)
#define ENEMY_MAX	(50)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT SetVtxData(void);
void LoadTexture(void);
float EnemyRotCheck(float rot);
void EnemyWallCheck(int Enemynum);



//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
    D3DXVECTOR3 g_posEnemy;								// 地面の位置
    D3DXVECTOR3 g_rotEnemy;								// 地面の向き（回転）
    bool flag;
    float y;
    float g_SpeedX;
    float g_SpeedZ;
    int g_nEnemy_count;
    bool g_nMinusX_flag;
    bool g_nMinusZ_flag;
    int livecount;
    bool Deadflag;
    bool Rebornflag;

}ENEMY;
//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMY g_Enemy[ENEMY_MAX];
LPDIRECT3DTEXTURE9	g_pD3DTextureEnemy;		// テクスチャへのポインタ
LPD3DXMESH g_pD3DXMeshEnemy;
LPD3DXBUFFER g_pD3DXBuffMatEnemy[ENEMY_MAX];
DWORD g_nNumMatEnemy;

D3DXVECTOR3 g_posEnemy;
D3DXVECTOR3 get_posBullet;
D3DXVECTOR3 g_posPlayer;
D3DXVECTOR3 g_rotEnemy;
D3DXVECTOR3 g_sclEnemy;
D3DXVECTOR3 g_rotE;
D3DXVECTOR3 g_rotDestEnemy;

static D3DXMATRIX g_mtxWorld;								// ワールドマトリックス


static float g_fLength;										//ポリゴン対角線の長さ
static float g_fAngle;										//ポリゴン対角線の角度
static float g_W = 0;											//
static float g_H = 0;
static float g_X = 0;
static float g_Y = 0;
static float g_Z = 0;
static float g_EnemySpeedX = 0;
static float g_EnemySpeedY = 0;
static float g_EnemySpeedZ = 0;
bool g_nMinusX_flag = FALSE;
bool g_nMinusZ_flag = FALSE;
static int g_NumEnemy = 0;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    g_posEnemy = D3DXVECTOR3(0.0f, 1.0f, 100.0f);

    g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_sclEnemy = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (FAILED(D3DXLoadMeshFromX("data\\MODEL\\enemy.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pD3DXBuffMatEnemy[i], NULL, &g_nNumMatEnemy, &g_pD3DXMeshEnemy)))
        {
            return E_FAIL;
        }

        g_Enemy[i].g_nMinusX_flag = FALSE;
        g_Enemy[i].g_nMinusZ_flag = FALSE;
        g_Enemy[i].Deadflag = FALSE;
        g_Enemy[i].Rebornflag = FALSE;
        g_Enemy[i].livecount = 0;
    }





    return S_OK;
}

void EnemyPosSet(float x, float y, float z)
{
    g_Enemy[g_NumEnemy].g_posEnemy.x = x;
    g_Enemy[g_NumEnemy].g_posEnemy.y = y;
    g_Enemy[g_NumEnemy].g_posEnemy.z = z;

    g_NumEnemy++;

    if (g_NumEnemy == ENEMY_MAX)
    {
        g_NumEnemy = ENEMY_MAX - 1;
    }

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
    //---テクスチャの解放---//
    if (g_pD3DTextureEnemy != NULL)
    {
        g_pD3DTextureEnemy->Release();
        g_pD3DTextureEnemy = NULL;
    }
    if (g_pD3DXMeshEnemy != NULL)
    {
        g_pD3DXMeshEnemy->Release();
        g_pD3DXMeshEnemy = NULL;
    }
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_pD3DXBuffMatEnemy[i] != NULL)
        {
            g_pD3DXBuffMatEnemy[i]->Release();
            g_pD3DXBuffMatEnemy[i] = NULL;
        }
    }

    g_posEnemy = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_sclEnemy = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < ENEMY_MAX; i++)
    {

        g_Enemy[i].g_nMinusX_flag = FALSE;
        g_Enemy[i].g_nMinusZ_flag = FALSE;
        g_Enemy[i].Deadflag = FALSE;
        g_Enemy[i].Rebornflag = FALSE;
    }

    g_rotE.y = 0;
    g_rotDestEnemy.y = 0;
    g_EnemySpeedX = 0;
    g_EnemySpeedZ = 0;
    g_posEnemy.z = 5;


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
    g_posPlayer = GetPos();


    for (int Enemynum = 0; Enemynum < ENEMY_MAX; Enemynum++)
    {
        for (int i = 0; i < 100; i++)
        {
            get_posBullet = GetBulletPos(i);

            if (Getflag(i))
            {
                if (CollsionCheck(g_Enemy[Enemynum].g_posEnemy.x, g_Enemy[Enemynum].g_posEnemy.y, g_Enemy[Enemynum].g_posEnemy.z, 100, get_posBullet.x, get_posBullet.y, get_posBullet.z, 250))
                {
                    SetEfect(i);
                    PlaySound(SOUND_LABEL_SE_EXPLOSION, 0);
                    g_Enemy[Enemynum].g_posEnemy.x = 20000;
                    g_Enemy[Enemynum].g_posEnemy.y = 10000;
                    g_Enemy[Enemynum].g_posEnemy.z = 10000;
                    g_Enemy[Enemynum].Deadflag = TRUE;
                }
            }
        }

        if (g_Enemy[Enemynum].Deadflag == FALSE)
        {

            if (g_Enemy[Enemynum].g_nEnemy_count % (Enemynum + 6) == 0)
            {
                g_Enemy[Enemynum].g_SpeedX = 300.0f;

                if (g_Enemy[Enemynum].g_nMinusX_flag == TRUE)
                {
                    g_Enemy[Enemynum].g_SpeedX = -g_Enemy[Enemynum].g_SpeedX;
                }
                else
                {
                    g_Enemy[Enemynum].g_SpeedX = g_Enemy[Enemynum].g_SpeedX;
                }
            }
            if (g_Enemy[Enemynum].g_nEnemy_count % (Enemynum + 6) == 0)
            {
                g_Enemy[Enemynum].g_SpeedZ = 265.0f;

                if (g_Enemy[Enemynum].g_nMinusZ_flag == TRUE)
                {
                    g_Enemy[Enemynum].g_SpeedZ = -g_Enemy[Enemynum].g_SpeedZ;
                }
                else
                {
                    g_Enemy[Enemynum].g_SpeedZ = g_Enemy[Enemynum].g_SpeedZ;
                }
            }

        }

        if (g_Enemy[Enemynum].Deadflag == TRUE)
        {
            g_Enemy[Enemynum].livecount++;
        }
        if (g_Enemy[Enemynum].livecount > 180)
        {
            g_Enemy[Enemynum].livecount = 0;
            g_Enemy[Enemynum].Rebornflag = TRUE;
            g_Enemy[Enemynum].Deadflag = FALSE;
        }


        if (g_Enemy[Enemynum].Rebornflag == TRUE)
        {
            g_Enemy[Enemynum].g_posEnemy.x = Enemynum * 10;
            g_Enemy[Enemynum].g_posEnemy.y = 0;
            g_Enemy[Enemynum].g_posEnemy.z = 10;
            g_Enemy[Enemynum].Rebornflag = FALSE;
            g_Enemy[Enemynum].g_SpeedX = 0;
            g_Enemy[Enemynum].g_SpeedZ = 0;
        }



        g_Enemy[Enemynum].g_posEnemy.x += g_Enemy[Enemynum].g_SpeedX;
        g_Enemy[Enemynum].g_posEnemy.z += g_Enemy[Enemynum].g_SpeedZ;

        EnemyWallCheck(Enemynum);

        g_Enemy[Enemynum].g_SpeedX *= 0.85f;
        g_Enemy[Enemynum].g_SpeedZ *= 0.85f;

        g_Enemy[Enemynum].g_nEnemy_count++;

    }

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    D3DXMATERIAL* pD3DXMat;
    D3DMATERIAL9 matDef;

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

    for (int i = 0; i < ENEMY_MAX; i++)
    {

        // World Matrix
        D3DXMatrixIdentity(&g_mtxWorld);															// 初期化

        D3DXMatrixScaling(&mtxScl, g_sclEnemy.x, g_sclEnemy.y, g_sclEnemy.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

        D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].g_rotEnemy.y, g_Enemy[i].g_rotEnemy.x, g_Enemy[i].g_rotEnemy.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

        D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].g_posEnemy.x, g_Enemy[i].g_posEnemy.y, g_Enemy[i].g_posEnemy.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

        pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

        pDevice->GetMaterial(&matDef);

        pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy[i]->GetBufferPointer();

        for (int nCntMat = 0; nCntMat < g_nNumMatEnemy; nCntMat++)
        {
            pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

            pDevice->SetTexture(0, NULL);

            g_pD3DXMeshEnemy->DrawSubset(nCntMat);
        }

    }

    pDevice->SetMaterial(&matDef);								// マテリアルを元に戻す

}

D3DXVECTOR3 GetPosEnemy(void)
{
    return g_posEnemy;
}
D3DXVECTOR3 GetRotdateEnemy(void)
{
    return g_rotEnemy;

}
float GetRotEnemy(void)
{
    return g_rotEnemy.y;

}
float GetPosEnemyX(void)
{
    return g_posEnemy.x;

}
float GetPosEnemyZ(void)
{
    return g_posEnemy.z;

}
float EnemyRotCheck(float rot)
{
    if (D3DX_PI < rot)
    {
        rot -= D3DX_PI * 2;
    }
    if (-D3DX_PI >= rot)
    {
        rot += D3DX_PI * 2;
    }

    return rot;

}
void EnemyWallCheck(int Enemynum)
{
    if (g_Enemy[Enemynum].g_posEnemy.x > 9500)
    {
        g_Enemy[Enemynum].g_posEnemy.x = 9500;
        g_Enemy[Enemynum].g_nMinusX_flag = TRUE;
    }
    if (g_Enemy[Enemynum].g_posEnemy.x < -9500)
    {
        g_Enemy[Enemynum].g_posEnemy.x = -9500;
        g_Enemy[Enemynum].g_nMinusX_flag = FALSE;
    }
    if (g_Enemy[Enemynum].g_posEnemy.z < -9500)
    {
        g_Enemy[Enemynum].g_posEnemy.z = -9500;
        g_Enemy[Enemynum].g_nMinusZ_flag = FALSE;
    }
    if (g_Enemy[Enemynum].g_posEnemy.z > 9500)
    {
        g_Enemy[Enemynum].g_posEnemy.z = 9500;
        g_Enemy[Enemynum].g_nMinusZ_flag = TRUE;
    }
}
bool CollsionCheck(float x1, float y1, float z1, float r1, float x2, float y2, float z2, float r2)
{
    if ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1) <= (r1 - r2) * (r1 - r2))
    {
        return TRUE;
    }
    return FALSE;
}