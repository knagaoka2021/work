//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : �����a�^
//
//=============================================================================
#include "light.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static D3DLIGHT9 g_aLight[4];								// ���C�g�̏��
static D3DXVECTOR3 vec0, vec1, vec2, vec3;
static float x, y, z;

//=============================================================================
// ���C�g�̏���������
//=============================================================================
HRESULT InitLight(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    D3DXVECTOR3 vecDir0, vecDir1, vecDir2, vecDir3;

    // �P�ڂ̃��C�g�ݒ�

    ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

    g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

    g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    vecDir0 = D3DXVECTOR3(10.0f, -10.0f, -10.0f);

    D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir0);

    pDevice->SetLight(0, &g_aLight[0]);

    pDevice->LightEnable(0, TRUE);

    // 2�ڂ̃��C�g�ݒ�

    ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

    g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

    g_aLight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    vecDir1 = D3DXVECTOR3(-20.0f, 20.0f, 0.0f);

    D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir1);

    pDevice->SetLight(1, &g_aLight[1]);

    pDevice->LightEnable(1, TRUE);

    // 3�ڂ̃��C�g�ݒ�

    ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

    g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

    g_aLight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    vecDir2 = D3DXVECTOR3(0.0f, -5.0f, 0.0f);

    D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[2].Direction, &vecDir2);

    pDevice->SetLight(2, &g_aLight[2]);

    pDevice->LightEnable(2, TRUE);

    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    // 4�ڂ̃��C�g�ݒ�

    ZeroMemory(&g_aLight[3], sizeof(D3DLIGHT9));

    g_aLight[3].Type = D3DLIGHT_DIRECTIONAL;

    g_aLight[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    vecDir3 = D3DXVECTOR3(-10.0f, 50.0f, 100.0f);

    D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[3].Direction, &vecDir3);

    pDevice->SetLight(3, &g_aLight[3]);

    pDevice->LightEnable(3, TRUE);


    return S_OK;
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLight(void)
{

    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    if (GetkeyboardPress(DIK_X))
    {
        x += 0.1f;
    }
    if (GetkeyboardPress(DIK_Y))
    {
        y += 0.1f;
    }
    if (GetkeyboardPress(DIK_Z))
    {
        z += 0.1f;
    }
    if (GetkeyboardPress(DIK_I))
    {
        x -= 0.1f;
    }
    if (GetkeyboardPress(DIK_O))
    {
        y -= 0.1f;
    }
    if (GetkeyboardPress(DIK_P))
    {
        z -= 0.1f;
    }
}

