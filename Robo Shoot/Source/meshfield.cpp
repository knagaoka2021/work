//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : �����a�^
//
//=============================================================================
#include "meshfield.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define VTX_MAX	(9)						// ���_�ő吔
#define INDXVTX_MAX	(14)				// �C���f�b�N�X�ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT SetVtxData(void);
void LoadTexture(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureField;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBufferField;	// �C���f�b�N�X�o�b�t�@

D3DXMATRIX g_mtxWorldField;					// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posField;						// �ʒu
D3DXVECTOR3 g_rotField;						// ����
VERTEX_3D* pVtx;

int g_nNumBlockX, g_nNumBlockZ;				// �u���b�N��
int g_nNumVertex;							// �����_��
int g_nNumPolygon;							// ���|���S����
float g_fSizeBlockX, g_fSizeBlockZ;			// �u���b�N�T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    LPDIRECT3DDEVICE9 pIndexDevice = GetDevice();

    g_nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

    g_nNumPolygon = g_nNumVertex + ((fSizeBlockX) * (fSizeBlockZ)) + 1;

    g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


    SetVtxData();								// ���_���̐ݒ�

    LoadTexture();								// �e�N�X�`���̓ǂݍ���


    // �C���f�b�N�X�o�b�t�@�̐���
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
// �I������
//=============================================================================
void UninitMeshField(void)
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
    if (g_pD3DIndexBufferField != NULL)
    {
        g_pD3DIndexBufferField->Release();
        g_pD3DIndexBufferField = NULL;
    }
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{
}

//=============================================================================
// �`�揉��
//=============================================================================
void DrawMeshField(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;


    // World Matrix
    D3DXMatrixIdentity(&g_mtxWorldField);															// ������

    D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);

    D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);

    D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

    D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);

    D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

    pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

    pDevice->SetIndices(g_pD3DIndexBufferField);

    pDevice->SetFVF(FVF_VERTEX_3D);													// ���_�t�H�[�}�b�g�Z�b�g

    pDevice->SetTexture(0, g_pD3DTextureField);										// �e�N�X�`���̐ݒ�

    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertex, 0, 12);			// 2�̓|���S����(���̓|���S�������R�Ŋ���)


}
//=============================================================================
// ���_���̐ݒ�
//=============================================================================
HRESULT SetVtxData(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    // ���_�o�b�t�@�̐���
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
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void LoadTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------�e�N�X�`���̓ǂݍ���----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fgy.jpg", &g_pD3DTextureField);
}