//=============================================================================
//
// �e�̏��� [shadow.cpp]
// Author : �����a�^
//
//=============================================================================
#include "shadow.h"
#include "input.h"
#include "model.h"
#include "bullet.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
    D3DXVECTOR3 g_posShadow;								// �n�ʂ̈ʒu
    D3DXVECTOR3 g_rotShadow;								// �n�ʂ̌����i��]�j

}SHADOW;
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_W	(4)
#define SHADOW_MAX	(201)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT SetShadowVtxData(void);
void LoadShadowTexture(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureShadow = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffShadow[SHADOW_MAX];	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

SHADOW g_Shadow[SHADOW_MAX];


static D3DXMATRIX g_mtxWorld;								// ���[���h�}�g���b�N�X
static D3DXVECTOR3 g_posShadow;								// �n�ʂ̈ʒu
static D3DXVECTOR3 g_rotShadow;								// �n�ʂ̌����i��]�j
static D3DXVECTOR3 g_sclShadow;								// �n�ʂ̑傫���i�X�P�[���j

static float g_fLength;										//�|���S���Ίp���̒���
static float g_fAngle;										//�|���S���Ίp���̊p�x
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
// ����������
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

    SetShadowVtxData();								// ���_���̐ݒ�

    LoadShadowTexture();								// �e�N�X�`���̓ǂݍ���

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
    //---�e�N�X�`���̉��---//
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
// �X�V����
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
// �`�揈��
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
        D3DXMatrixIdentity(&g_mtxWorld);															// ������

        D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

        D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[i].g_rotShadow.y, g_Shadow[i].g_rotShadow.x, g_Shadow[i].g_rotShadow.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

        D3DXMatrixTranslation(&mtxTranslate, g_Shadow[i].g_posShadow.x, g_Shadow[i].g_posShadow.y, g_Shadow[i].g_posShadow.z);

        D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

        pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

        pDevice->SetStreamSource(0, g_pD3DVtxBuffShadow[i], 0, sizeof(VERTEX_3D));

        pDevice->SetFVF(FVF_VERTEX_3D);													// ���_�t�H�[�}�b�g�Z�b�g

        pDevice->SetTexture(0, g_pD3DTextureShadow);										// �e�N�X�`���̐ݒ�

        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);								// 2�̓|���S����(���̓|���S�������R�Ŋ���)

    }

    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);



}
//=============================================================================
// ���_���̐ݒ�
//=============================================================================
HRESULT SetShadowVtxData(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    for (int i = 0; i < SHADOW_MAX; i++)
    {
        // ���_�o�b�t�@�̐���
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
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void LoadShadowTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------�e�N�X�`���̓ǂݍ���----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_pD3DTextureShadow);
}
