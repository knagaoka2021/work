//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "bullet.h"
#include "camera.h"
#include "model.h"
#include"input.h"
#include "score.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_W	(50.0f)
#define BULLET_H	(50.0f)
#define BULLET_MAX	(200)
#define EXPLLOSION_MAX	(200)
#define WALLCHK		(9950)


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
    D3DXVECTOR3 g_posBullet;								// 地面の位置
    D3DXVECTOR3 g_rotBullet;								// 地面の向き（回転）
    bool flag;
    float y;
    float g_SpeedX;
    float g_SpeedZ;
}BULLET;
typedef struct
{
    D3DXVECTOR3 g_posExp;
    bool flag;
    float y;
    int expllosion_time;
    float texuv;

}EXPLLOSION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT SetBulletVtxData(void);
void LoadbulletTexture(void);
void SetEfectE(float x, float y, float z);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBullet[BULLET_MAX] = { NULL };	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureExp = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExp[EXPLLOSION_MAX] = { NULL };	// 頂点バッファインターフェースへのポインタ

BULLET g_Bullet[BULLET_MAX];
EXPLLOSION g_Expllosion[EXPLLOSION_MAX];

static int ccc = 0;
int qwe = 0.0f;

static D3DXMATRIX g_mtxWorld;								// ワールドマトリックス
static D3DXVECTOR3 g_sclBullet;								// 地面の大きさ（スケール）



static float g_fLength;										//ポリゴン対角線の長さ
static float g_fAngle;										//ポリゴン対角線の角度

bool flag_L = false;

static float g_SpeedZ = 0;
bool exploosion_flag = FALSE;
static int	 AutoCount = 0;

VERTEX_3D* pBulletVtx;
VERTEX_3D* pExpVtx;
D3DXMATRIX g_BulletmtxView;
D3DXMATRIX g_ExpView;
D3DXVECTOR3 g_rotbullet;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    for (int i = 0; i < BULLET_MAX; i++)
    {
        g_Bullet[i].g_posBullet = D3DXVECTOR3(0.0f, 10000.0f, 1000.0f);
        g_Bullet[i].g_rotBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Bullet[i].g_SpeedX = 0;
        g_Bullet[i].g_SpeedZ = 0;
    }
    for (int i = 0; i < EXPLLOSION_MAX; i++)
    {
        g_Expllosion[i].g_posExp = D3DXVECTOR3(0.0f, 10000.0f, 5000.0f);
        g_Expllosion[i].expllosion_time = 0;
        g_Expllosion[i].texuv = 0;
        g_Expllosion[i].flag = FALSE;

    }
    //g_Expllosion[].g_posExp = D3DXVECTOR3(0.0f,10.0f,10.0f);




    g_sclBullet = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    SetBulletVtxData();								// 頂点情報の設定

    LoadbulletTexture();								// テクスチャの読み込み

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
    //---テクスチャの解放---//
    if (g_pD3DTextureBullet != NULL)
    {
        g_pD3DTextureBullet->Release();
        g_pD3DTextureBullet = NULL;
    }
    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (g_pD3DVtxBuffBullet[i] != NULL)
        {
            g_pD3DVtxBuffBullet[i]->Release();
            g_pD3DVtxBuffBullet[i] = NULL;
        }
    }

    //---テクスチャの解放---//
    if (g_pD3DTextureExp != NULL)
    {
        g_pD3DTextureExp->Release();
        g_pD3DTextureExp = NULL;
    }

    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (g_pD3DVtxBuffExp[i] != NULL)
        {
            g_pD3DVtxBuffExp[i]->Release();
            g_pD3DVtxBuffExp[i] = NULL;
            g_Expllosion[i].expllosion_time = 0;
        }
    }

    for (int i = 0; i < BULLET_MAX; i++)
    {
        g_Bullet[i].g_posBullet = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);
        g_Bullet[i].g_rotBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Bullet[i].g_SpeedX = 0;
        g_Bullet[i].g_SpeedZ = 0;
    }

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
    D3DXVECTOR3 vtx;

    //g_rotbullet = GetRotCamera();
    g_rotbullet.y = GetRotbullet();

    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (g_Bullet[i].flag == false)
        {
            vtx = GetPos();
            g_Bullet[i].g_posBullet.x = vtx.x;
            g_Bullet[i].g_posBullet.y = vtx.y + 10000;
            g_Bullet[i].g_posBullet.z = vtx.z;

        }

    }

    if (RepeatkeyboardPress(DIK_SPACE) || tringpadPress(0, BUTTON_A))
    {
        PlaySound(SOUND_LABEL_SE_SHOT, 0);
        flag_L = TRUE;
    }


    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (AutoCount % 6 == 0 && flag_L == TRUE && g_Bullet[i].flag == false)
        {

            g_Bullet[i].y = g_rotbullet.y;
            g_Bullet[i].flag = true;
            flag_L = FALSE;
            g_Bullet[i].g_SpeedX = sinf(g_Bullet[i].y) * 60;
            g_Bullet[i].g_SpeedZ = cosf(g_Bullet[i].y) * 60;
            g_Bullet[i].g_posBullet.x = GetPosX();
            g_Bullet[i].g_posBullet.y = vtx.y + 100;
            g_Bullet[i].g_posBullet.z = GetPosZ();
            break;
        }

    }


    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (g_Bullet[i].flag == true && g_Bullet[i].g_posBullet.z<-WALLCHK || g_Bullet[i].g_posBullet.z>WALLCHK || g_Bullet[i].g_posBullet.x<-WALLCHK || g_Bullet[i].g_posBullet.x>WALLCHK)
        {
            g_Bullet[i].flag = false;
            g_Bullet[i].g_SpeedX = 0;
            g_Bullet[i].g_SpeedZ = 0;

            SetEfectE(g_Bullet[i].g_posBullet.x, g_Bullet[i].g_posBullet.y, g_Bullet[i].g_posBullet.z);
            PlaySound(SOUND_LABEL_SE_EXPLOSION, 0);

            vtx = GetPos();
            g_Bullet[i].g_posBullet.x = vtx.x;
            g_Bullet[i].g_posBullet.y = vtx.y + 10000;
            g_Bullet[i].g_posBullet.z = vtx.z;
            COUNT_UP();
        }
    }

    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (g_Bullet[i].flag == true)
        {
            g_Bullet[i].g_posBullet.x -= g_Bullet[i].g_SpeedX;
            g_Bullet[i].g_posBullet.z -= g_Bullet[i].g_SpeedZ;
        }
    }
    AutoCount++;

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    for (int i = 0; i < EXPLLOSION_MAX; i++)
    {
        if (g_Expllosion[i].flag == TRUE)
        {
            if (g_Expllosion[i].expllosion_time % 5 == 0 && g_Expllosion[i].texuv != 10)
            {
                g_Expllosion[i].texuv++;
            }

            g_pD3DVtxBuffExp[i]->Lock(0, 0, (void**)&pExpVtx, 0);
            pExpVtx[0].tex = D3DXVECTOR2(0.1f * g_Expllosion[i].texuv, 0);

            pExpVtx[1].tex = D3DXVECTOR2(0.1f * g_Expllosion[i].texuv + 0.1f, 0);

            pExpVtx[2].tex = D3DXVECTOR2(0.1f * g_Expllosion[i].texuv, 1.0f);

            pExpVtx[3].tex = D3DXVECTOR2(0.1f * g_Expllosion[i].texuv + 0.1f, 1.0f);

            pExpVtx += 4;
            g_pD3DVtxBuffExp[i]->Unlock();

            if (g_Expllosion[i].texuv == 10)
            {
                g_Expllosion[i].flag = FALSE;
                g_Expllosion[i].texuv = 0;
                g_Expllosion[i].g_posExp.x = 0;
                g_Expllosion[i].g_posExp.y = 10000;
            }
            g_Expllosion[i].expllosion_time++;
        }
    }



    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;




    //// World Matrix


    for (int i = 0; i < BULLET_MAX; i++)
    {

        g_BulletmtxView = GetMtxView();

        D3DXMatrixIdentity(&g_mtxWorld);

        D3DXMatrixInverse(&g_mtxWorld, NULL, &g_BulletmtxView);

        g_mtxWorld._41 = 0.0f;
        g_mtxWorld._42 = 0.0f;
        g_mtxWorld._43 = 0.0f;

        D3DXMatrixTranslation(&mtxTranslate, g_Bullet[i].g_posBullet.x, g_Bullet[i].g_posBullet.y, g_Bullet[i].g_posBullet.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

        pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);



        pDevice->SetStreamSource(0, g_pD3DVtxBuffBullet[i], 0, sizeof(VERTEX_3D));


        pDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

        pDevice->SetTexture(0, g_pD3DTextureBullet);										// テクスチャの設定

        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);								// 2はポリゴン数(又はポリゴン数を３で割る)

    }

    for (int i = 0; i < EXPLLOSION_MAX; i++)
    {

        g_BulletmtxView = GetMtxView();

        D3DXMatrixIdentity(&g_mtxWorld);

        D3DXMatrixInverse(&g_mtxWorld, NULL, &g_BulletmtxView);

        g_mtxWorld._41 = 0.0f;
        g_mtxWorld._42 = 0.0f;
        g_mtxWorld._43 = 0.0f;

        D3DXMatrixTranslation(&mtxTranslate, g_Expllosion[i].g_posExp.x, g_Expllosion[i].g_posExp.y, g_Expllosion[i].g_posExp.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

        pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);



        pDevice->SetStreamSource(0, g_pD3DVtxBuffExp[i], 0, sizeof(VERTEX_3D));


        pDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

        pDevice->SetTexture(0, g_pD3DTextureExp);										// テクスチャの設定

        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);								// 2はポリゴン数(又はポリゴン数を３で割る)
    }



    //pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);

}
//=============================================================================
// 頂点情報の設定
//=============================================================================
HRESULT SetBulletVtxData(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    VERTEX_3D Vertices[] =
    {
        { D3DXVECTOR3(-BULLET_W , BULLET_W , 0.0f) , D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//0
        { D3DXVECTOR3(BULLET_W , BULLET_W , 0.0f) , D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) } ,//2
        { D3DXVECTOR3(-BULLET_W , -BULLET_W , 0.0f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,//6
        { D3DXVECTOR3(BULLET_W , -BULLET_W , 0.0f) , D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(1.0f,1.0f)} ,//8
    };

    for (int i = 0; i < BULLET_MAX; i++)
    {
        // 頂点バッファの生成
        if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pD3DVtxBuffBullet[i], NULL)))
        {
            return E_FAIL;
        }




        g_pD3DVtxBuffBullet[i]->Lock(0, sizeof(pBulletVtx), (void**)&pBulletVtx, 0);

        memcpy(pBulletVtx, Vertices, sizeof(Vertices));

        g_pD3DVtxBuffBullet[i]->Unlock();
    }

    VERTEX_3D Vertices2[] =
    {
        { D3DXVECTOR3(-250.0f , 250.0f , 0.0f) , D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) } ,//0
        { D3DXVECTOR3(250.0f , 250.0f , 0.0f) , D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.1f,0.0f) } ,//2
        { D3DXVECTOR3(-250.0f , 0.0f , 0.0f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) } ,//6
        { D3DXVECTOR3(250.0f , 0.0f , 0.0f) , D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(0.1f,1.0f)} ,//8
    };

    for (int i = 0; i < EXPLLOSION_MAX; i++)
    {


        // 頂点バッファの生成
        if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pD3DVtxBuffExp[i], NULL)))
        {
            return E_FAIL;
        }


        g_pD3DVtxBuffExp[i]->Lock(0, sizeof(pExpVtx), (void**)&pExpVtx, 0);

        memcpy(pExpVtx, Vertices2, sizeof(Vertices2));

        g_pD3DVtxBuffExp[i]->Unlock();
    }



    return S_OK;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void LoadbulletTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------テクスチャの読み込み----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\001.png", &g_pD3DTextureBullet);
    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\exp.png", &g_pD3DTextureExp);
}
D3DXVECTOR3 GetBulletPos(int id)
{
    return g_Bullet[id].g_posBullet;
}
void SetEfect(int id)
{
    g_Bullet[id].flag = false;
    g_Bullet[id].g_SpeedX = 0;
    g_Bullet[id].g_SpeedZ = 0;
    SetEfectE(g_Bullet[id].g_posBullet.x, g_Bullet[id].g_posBullet.y, g_Bullet[id].g_posBullet.z);
    COUNT_UP2();
}
bool Getflag(int id)
{
    if (g_Bullet[id].flag == TRUE)
    {
        return TRUE;
    }
    return FALSE;
}
void SetEfectE(float x, float y, float z)
{
    for (int i = 0; i < EXPLLOSION_MAX; i++)
    {
        if (g_Expllosion[i].flag == false)
        {

            g_Expllosion[i].flag = TRUE;
            g_Expllosion[i].g_posExp.x = x;
            g_Expllosion[i].g_posExp.y = y;
            g_Expllosion[i].g_posExp.z = z;
            break;
        }

    }
}