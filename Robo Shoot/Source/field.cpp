//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : �����@�a�^
//
//=============================================================================
#include "field.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_W	(4)
#define TEX_H	(10)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureField = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

static D3DXVECTOR3 g_posCameraP;							// �J�����̎��_
static D3DXVECTOR3 g_posCameraR;							// �J�����̒����_
static D3DXVECTOR3 g_vecCameraU;							// �J�����̏�����x�N�g��


static D3DXMATRIX g_mtxWorld;								// ���[���h�}�g���b�N�X
static D3DXVECTOR3 g_posField;								// �n�ʂ̈ʒu
static D3DXVECTOR3 g_rotField;								// �n�ʂ̌����i��]�j
static D3DXVECTOR3 g_sclField;								// �n�ʂ̑傫���i�X�P�[���j

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


//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    //SetVtxData();								// ���_���̐ݒ�

    //LoadTexture();								// �e�N�X�`���̓ǂݍ���

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
    //---�e�N�X�`���̉��---//
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
// �X�V����
//=============================================================================
void UpdateField(void)
{


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;


    // World Matrix
    D3DXMatrixIdentity(&g_mtxWorld);															// ������

    D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

    D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);

    D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

    pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

    pDevice->SetFVF(FVF_VERTEX_3D);													// ���_�t�H�[�}�b�g�Z�b�g

    pDevice->SetTexture(0, g_pD3DTextureField);										// �e�N�X�`���̐ݒ�

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 12);								// 2�̓|���S����(���̓|���S�������R�Ŋ���)


}


