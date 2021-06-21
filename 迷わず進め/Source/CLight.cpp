//=============================================================================
//
//  [CLight.cpp]
// Author : �����a�^
//
//=============================================================================
#include "CLight.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

static D3DLIGHT9 g_aLight[4];								// ���C�g�̏��

CLight::CLight(void)
{
};
CLight::~CLight(void)
{
};

//=============================================================================
// ���C�g�̏���������
//=============================================================================
HRESULT CLight::InitLight(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXVECTOR3 vecDir0,vecDir1,vecDir2,vecDir3;

	// �P�ڂ̃��C�g�ݒ�

	ZeroMemory(&g_aLight[0],sizeof(D3DLIGHT9));

	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	g_aLight[0].Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vecDir0 = D3DXVECTOR3(10.0f,-10.0f,-10.0f);

	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction,&vecDir0);

	m_pD3DDevice->SetLight(0,&g_aLight[0]);

	m_pD3DDevice->LightEnable(0,TRUE);

	// 2�ڂ̃��C�g�ݒ�

	ZeroMemory(&g_aLight[1],sizeof(D3DLIGHT9));

	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	g_aLight[1].Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vecDir1 = D3DXVECTOR3(-20.0f,20.0f,0.0f);

	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction,&vecDir1);

	m_pD3DDevice->SetLight(1,&g_aLight[1]);

	m_pD3DDevice->LightEnable(1,TRUE);

	// 3�ڂ̃��C�g�ݒ�

	ZeroMemory(&g_aLight[2],sizeof(D3DLIGHT9));

	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	g_aLight[2].Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vecDir2 = D3DXVECTOR3(0.0f,-50.0f,0.0f);

	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[2].Direction,&vecDir2);
	
	m_pD3DDevice->SetLight(2,&g_aLight[2]);

	m_pD3DDevice->LightEnable(2,TRUE);


	// 4�ڂ̃��C�g�ݒ�

	ZeroMemory(&g_aLight[3],sizeof(D3DLIGHT9));

	g_aLight[3].Type = D3DLIGHT_DIRECTIONAL;

	g_aLight[3].Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	vecDir3 = D3DXVECTOR3(-10.0f,50.0f,100.0f);

	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[3].Direction,&vecDir3);
	
	m_pD3DDevice->SetLight(3,&g_aLight[3]);

	m_pD3DDevice->LightEnable(3,TRUE);

	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING , FALSE );


	return S_OK;
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void CLight::UninitLight(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void CLight::UpdateLight(void)
{
}