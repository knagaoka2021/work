//=============================================================================
//
// �ǂ̏��� [wall.cpp]
// Author : �����a�^
//
//=============================================================================
#include "wall.h"
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
HRESULT SetWallVtxData(void);
void LoadWallTexture(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureWall;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffWall;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBufferWall;	// �C���f�b�N�X�o�b�t�@

D3DXMATRIX g_mtxWorldWall;					// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posWall;						// �ʒu
D3DXVECTOR3 g_rotWall;						// ����
VERTEX_3D* pWallVtx;

int g_nNumWallBlockX, g_nNumWallBlockZ;				// �u���b�N��
int g_nNumWallVertex;							// �����_��
int g_nNumWallPolygon;							// ���|���S����
float g_fSizeWallBlockX, g_fSizeWallBlockZ;			// �u���b�N�T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshWall(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, int ste)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    LPDIRECT3DDEVICE9 pIndexDevice = GetDevice();

    g_nNumWallVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

    g_nNumWallPolygon = g_nNumWallVertex + ((fSizeBlockX) * (fSizeBlockZ)) + 1;

    g_posWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    g_rotWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


    SetWallVtxData();								// ���_���̐ݒ�

    LoadWallTexture();								// �e�N�X�`���̓ǂݍ���


    // �C���f�b�N�X�o�b�t�@�̐���
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
// �I������
//=============================================================================
void UninitMeshWall(void)
{
    //---�e�N�X�`���̉��---//
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
// �X�V����
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// �`�揉��
//=============================================================================
void DrawMeshWall(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;


    // World Matrix
    D3DXMatrixIdentity(&g_mtxWorldWall);															// ������

    D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);

    D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxScl);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotWall.y, g_rotWall.x, g_rotWall.z);

    D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

    D3DXMatrixTranslation(&mtxTranslate, g_posWall.x, g_posWall.y, g_posWall.z);

    D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTranslate);

    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

    pDevice->SetStreamSource(0, g_pD3DVtxBuffWall, 0, sizeof(VERTEX_3D));

    pDevice->SetIndices(g_pD3DIndexBufferWall);

    pDevice->SetFVF(FVF_VERTEX_3D);													// ���_�t�H�[�}�b�g�Z�b�g

    pDevice->SetTexture(0, g_pD3DTextureWall);										// �e�N�X�`���̐ݒ�

    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumWallVertex, 0, 12);			// 2�̓|���S����(���̓|���S�������R�Ŋ���)


}
//=============================================================================
// ���_���̐ݒ�
//=============================================================================
HRESULT SetWallVtxData(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    // ���_�o�b�t�@�̐���
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
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void LoadWallTexture(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //--------�e�N�X�`���̓ǂݍ���----------//

    D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Wall001.jpg", &g_pD3DTextureWall);
}

//bool Collision(float ax,float ay,float aw,float ah,float bx,float by,float bw,float bh)
//{
//	//return;
//}
