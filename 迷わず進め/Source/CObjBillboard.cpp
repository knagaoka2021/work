//=============================================================================
//
//  [CObjBillbord.cpp]
// Author : �����a�^
//
//=============================================================================
#include "CRenderer.h"
#include "CObjBillboard.h"
#include "CInput.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �\����
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

CObjBillboard *CObjBillboard::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	CObjBillboard *pObjBillboard;
	pObjBillboard = new CObjBillboard();
	pObjBillboard->Init(m_pD3DDevice,fileName);
	m_pos.y += 5.0f;
	pObjBillboard->SetPos(m_pos);
	pObjBillboard->SetRot(m_rot);


	return pObjBillboard;
}
HRESULT CObjBillboard::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	//LoadTexture(m_pD3DDevice);
	//�|���S���T�C�Y�w��
	CSceneBillboard::SetSize(180.0f);
	m_pBulletDevice = m_pD3DDevice;
	CSceneBillboard::Init(m_pD3DDevice,fileName);


	return S_OK;
}
void CObjBillboard::Uninit(void)
{
	//---�e�N�X�`���̉��---//


	CSceneBillboard::Uninit();

	//CScene::Relese();
}
void CObjBillboard::Update(void)
{
	// ���W�X�V
	CSceneBillboard::SetPos(m_pos);
	CSceneBillboard::SetRot(m_rot);
}
void CObjBillboard::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,0);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);

	CSceneBillboard::Draw(m_pD3DDevice);

	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
}
void CObjBillboard::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CObjBillboard::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CObjBillboard::GetPosDest(void)
{

	return m_pos;
}
void CObjBillboard::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CObjBillboard::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CObjBillboard::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
}