//=============================================================================
//
//  [CSceneSkinMesh.cpp]
// Author : �����a�^
//
//=============================================================================
#include "CRenderer.h"
#include "CSceneSkinMesh.h"
#include "CCamera.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STARTPOSX			(-250.0f)
#define	VIEW_ANGLE		(D3DX_PI/2.0f)									// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(1.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(6000.0f)										// �r���[���ʂ�FarZ�l
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �\����
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CSceneSkinMesh *g_p = NULL;
XFileAnimationMesh* g_model;

CSceneSkinMesh *CSceneSkinMesh::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	CSceneSkinMesh *pSceneSkinMesh;
	pSceneSkinMesh = new CSceneSkinMesh(3,objType);
	pSceneSkinMesh->Init(m_pD3DDevice,fileName);
	pSceneSkinMesh->SetPos(m_pos);
	
	return pSceneSkinMesh;


}
HRESULT CSceneSkinMesh::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	model = new XFileAnimationMesh(TEXT("data\\MODEL\\player.x"), m_pD3DDevice);
	m_motion = MOTION_WAIT;
	model->ChangeMotion((int)m_motion);
	m_parts = PARTS_MAX;
	return S_OK;
}
void CSceneSkinMesh::Uninit(void)
{
	if(model!=NULL)
	{
		delete model;
		model = NULL;
	}
	m_parts = PARTS_MAX;

	m_pos = D3DXVECTOR3(0.0f,1.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	CScene::Relese();
}
void CSceneSkinMesh::Update(void)
{
	if(model!=NULL)
	{
		if(model->GetResetMotion())
		{
			m_motion = MOTION_WAIT;
			model->SetResetMotion(FALSE);
		}

		model->ChangeMotion(m_motion);
		//if(CInputKeyboard::GetkeyboardPress(DIK_H))
		//{
		model->AdvanceTime(60.0 / 4800);
		//}
	}
}
void CSceneSkinMesh::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	D3DXMATRIX rotation;							// ��]
	D3DXMatrixRotationYawPitchRoll(&rotation,m_rot.y,m_rot.x,m_rot.z);

	if(model!=NULL)
	{
		model->Draw(D3DXVECTOR3(m_pos.x,m_pos.y,m_pos.z),rotation);
	}
}

void CSceneSkinMesh::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CSceneSkinMesh::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CSceneSkinMesh::GetPosDest(void)
{

	return m_pos;
}
void CSceneSkinMesh::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CSceneSkinMesh::GetRot(void)
{
	return m_rot;
}

D3DXMATRIX CSceneSkinMesh::GetMatrix(PARTS_MATRIX parts)
{
	D3DXMATRIX mat;
	switch(parts)
	{
		case PARTS_BODY:
			mat = model->GetPartsMatrix("NewJoint5_Joint");
			break;
		case PARTS_RIGHTARM:
			mat = model->GetPartsMatrix("NewJoint11_Joint");
			break;
	}
	
	return mat;
}
void CSceneSkinMesh::SetVertexPolygon(void)
{

}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CSceneSkinMesh::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	//--------�e�N�X�`���̓ǂݍ���----------//

	//D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\Untitled1.jpg",&m_pD3DTextureModel);
}
//=============================================================================
// ���[�V�����^�C�v�擾
//=============================================================================
MOTIONTYPE CSceneSkinMesh::GetSkinMotion(void)
{
	MOTIONTYPE motion;

	if(model!=NULL)
	{
		motion = (MOTIONTYPE)model->GetMotionType();
	}
	return motion;
}
//=============================================================================
// ���[�V�����^�C�v�ݒ�
//=============================================================================
void CSceneSkinMesh::SetSkinMotion(MOTIONTYPE motion)
{
	if(model!=NULL)
	{
		m_motion = motion;
		//model->SetMotionType((int)m_motion);
	}
}
//=============================================================================
// ���[�V�����^�C�v�ύX
//=============================================================================
void CSceneSkinMesh::ChangeSkinMotion(MOTIONTYPE motion)
{
	int motionNum = motion;
	if(model!=NULL)
	{
		model->ChangeMotion(motionNum);
	}
}
//=============================================================================
// ���[�V�������ԃ��Z�b�g
//=============================================================================
void CSceneSkinMesh::SkinMotionResetTime(MOTIONTYPE motion)
{
	int motionNum = motion;

	if(model!=NULL)
	{
		model->ResetTime(motionNum);
		model->AdvanceTime(0);
	}
}
double CSceneSkinMesh::Timeget(void)
{
	if(model!=NULL)
	{
		return	model->getloop();
	}
	return 0.0f;
}