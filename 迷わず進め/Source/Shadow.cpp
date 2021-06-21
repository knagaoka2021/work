//=============================================================================
//
//  [Shadow.cpp]
// Author : �����a�^
//
//=============================================================================
#include "CRenderer.h"
#include "Shadow.h"
#include "CInput.h"
#include "CMeshWall.h"
#include "CMotionModel.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOWPOSH (6.25f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �\����
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
VERTEX_2D vertex[4];

Shadow *Shadow::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	Shadow *pShadow;
	pShadow = new Shadow();
	pShadow->Init(m_pD3DDevice,fileName);
	m_pos.y = 2.0f;
	pShadow->SetPos(m_pos);
	pShadow->SetRot(m_rot);
	return pShadow;
}
HRESULT Shadow::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	LoadTexture(m_pD3DDevice);
	m_pShadowDevice = m_pD3DDevice;
	CSceneX::Init(m_pD3DDevice,fileName);

	vertex[0].vtx.x=0.0f;
	vertex[0].vtx.y=0.0f;

	vertex[1].vtx.x=SCREEN_WIDTH;
	vertex[1].vtx.y=0.0f;

	vertex[2].vtx.x=0.0f;
	vertex[2].vtx.y=SCREEN_HEIGHT;

	vertex[3].vtx.x=SCREEN_WIDTH;
	vertex[3].vtx.y=SCREEN_HEIGHT;

	vertex[0].diffuse = D3DCOLOR_RGBA(0,0,0,255);
	vertex[1].diffuse = D3DCOLOR_RGBA(0,0,0,255);
	vertex[2].diffuse = D3DCOLOR_RGBA(0,0,0,255);
	vertex[3].diffuse = D3DCOLOR_RGBA(0,0,0,255);
	vertex[0].rhw=1.0f;
	vertex[1].rhw=1.0f;
	vertex[2].rhw=1.0f;
	vertex[3].rhw=1.0f;
	vertex[0].tex=D3DXVECTOR2(0,0);

	vertex[1].tex=D3DXVECTOR2(1,0);

	vertex[2].tex=D3DXVECTOR2(0,1);

	vertex[3].tex=D3DXVECTOR2(1,1);

	return S_OK;
}
void Shadow::Uninit(void)
{
	//---�e�N�X�`���̉��---//


	CSceneX::Uninit();

}
void Shadow::Update(void)
{
	m_pos.y+=-SHADOWPOSH;

	// ���W�X�V
	CSceneX::SetPos(m_pos);
	CSceneX::SetRot(m_rot);

}
void Shadow::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

// �X�e���V���L��
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	// �y�o�b�t�@�ւ̏������ݖ���
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// �J���[�o�b�t�@�ւ̏������ݖ���(sample�ł�F2�Ő؂�ւ�)
	m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	// ���������č��i�H ALWAYS�������i
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	// �X�e���V���e�X�g���i�EZ�e�X�g���i-0
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
	// �X�e���V���e�X�g���i�EZ�e�X�g�s���i-+=1
	m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
	// �X�e���V���e�X�g�s���i�EZ�e�X�g�s���i-
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);

	// �J�����O���t�ɂ���
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	CSceneX::Draw(m_pD3DDevice);

	// �X�e���V���Q�ƒl1
	m_pD3DDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	// �X�e���V���l��1�ȏ�ō��i
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);
	// �X�e���V���e�X�g���i�EZ�e�X�g���i-0
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	// �X�e���V���e�X�g���i�EZ�e�X�g�s���i-+=1
	m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);
	// �X�e���V���e�X�g�s���i�EZ�e�X�g�s���i-
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	CSceneX::Draw(m_pD3DDevice);

	// �X�e���V���Q�ƒl2
	m_pD3DDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	// �X�e���V���l��1�ȏ�ō��i
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	// �X�e���V���e�X�g���i�EZ�e�X�g���i-0
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	// �X�e���V���e�X�g���i�EZ�e�X�g�s���i-+=1
	m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	// �X�e���V���e�X�g�s���i�EZ�e�X�g�s���i-
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	//// �J�����O�����Ƃɖ߂�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_ALPHA));

	//m_pD3DDevice->SetRenderState( D3DRS_LIGHTING , FALSE );


	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	m_pD3DDevice->SetTexture(0,NULL);//�e�N�X�`���̐ݒ�

	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2, &vertex[0],sizeof(VERTEX_2D));//2�̓|���S����

	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE,FALSE);


}
void Shadow::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 Shadow::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 Shadow::GetPosDest(void)
{

	return m_pos;
}
void Shadow::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 Shadow::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void Shadow::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	//--------�e�N�X�`���̓ǂݍ���----------//
	D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\white.jpg",&m_pD3DTextureModel);
}