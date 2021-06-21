//=============================================================================
//
//  [CMotionModel.cpp]
// Author : �����a�^
//
//=============================================================================
#include "common.h"
#include "CRenderer.h"
#include "CMotionModel.h"
#include "CManager.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POSX	(-250.0f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �\����
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int AutoCount = 0;
CMotionModel *g_p = NULL;

CMotionModel *CMotionModel::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,CMotionModel *p)
{

	CMotionModel *pMotion;

	pMotion = new CMotionModel(3,objType);

	g_p = p;


	pMotion->Init(m_pD3DDevice,fileName);

	pMotion->SetPos(m_pos);


	pMotion->SetRot(m_rot);




	return pMotion;
}
HRESULT CMotionModel::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	m_pMotionModel = m_pD3DDevice;


	m_parent = g_p;

	if(FAILED(D3DXLoadMeshFromX(fileName,D3DXMESH_SYSTEMMEM,m_pD3DDevice,NULL,&m_pD3DXBuffMatModel,NULL,&m_nNumMatModel,&m_pD3DXMeshModel)))
	{
		return E_FAIL;
	}

	// pD3DXMtrlBuffer ����A������e�N�X�`���[�̏���ǂݎ��
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)m_pD3DXBuffMatModel->GetBufferPointer();

	m_pMeshMaterials = new D3DMATERIAL9[m_nNumMatModel];
	m_pD3DTextureModel  = new LPDIRECT3DTEXTURE9[m_nNumMatModel];

	for( DWORD i=0; i < m_nNumMatModel; i++ )
	{
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;// �����̃R�s�[

		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;// �}�e���A���̊��F��ݒ肷��

		if(d3dxMaterials[i].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile( m_pD3DDevice, d3dxMaterials[i].pTextureFilename, &m_pD3DTextureModel[i] );
		}
		else
		{
			m_pD3DTextureModel[i] = NULL;
		}
	}

	
	for( DWORD i=0; i < m_nNumMatModel; i++ )
	{
		d3dxMaterials[i].pTextureFilename;
		m_pD3DTextureModel[i];
	}


	

	return S_OK;
}
void CMotionModel::Uninit(void)
{
	
	//---�e�N�X�`���̉��---//
	if( m_pMeshMaterials != NULL ) 
	{
		delete m_pMeshMaterials;
	}

	if( m_pMeshMaterials != NULL)
	{
		for( DWORD i = 0; i < m_nNumMatModel; i++ )
		{
			if(m_pD3DTextureModel[i]!=NULL)
			{
				m_pD3DTextureModel[i]->Release();
			}
		}
		delete m_pD3DTextureModel;
		m_pMeshMaterials = NULL;
	}

	if(m_pD3DXMeshModel != NULL)
	{
		m_pD3DXMeshModel->Release();
		m_pD3DXMeshModel=NULL;
	}
	if(m_pD3DXBuffMatModel != NULL)
	{
		m_pD3DXBuffMatModel->Release();
		m_pD3DXBuffMatModel=NULL;
	}

	m_pos = D3DXVECTOR3(0.0f,1.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	CScene::Relese();

}
void CMotionModel::Update(void)
{
	
}
void CMotionModel::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	if(m_pD3DXMeshModel != NULL)
	{

		D3DXMATRIX mtxScl,mtxRot,mtxTranslate,mtxRefrect;

		// World Matrix
		D3DXMatrixIdentity(&m_mtxWorld);															// ������


		D3DXMatrixScaling(&mtxScl,m_scl.x,m_scl.y,m_scl.z);

		D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

		D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y,m_rot.x,m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRot);

		D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxTranslate);


		//�e�����݂���ꍇ�͐e�}�g���N�X������
		if( m_parent )
		{
			D3DXMatrixMultiply( &m_mtxWorld, &m_mtxWorld, &m_parent->m_mtxWorld );

		}
	
		m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);


		for(DWORD nCntMat=0;nCntMat<m_nNumMatModel;nCntMat++)
		{
			m_pD3DDevice->SetMaterial(&m_pMeshMaterials[nCntMat]);
			m_pD3DDevice->SetTexture(0,m_pD3DTextureModel[nCntMat]);
			m_pD3DXMeshModel->DrawSubset(nCntMat);
		}

	}

}

void CMotionModel::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CMotionModel::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CMotionModel::GetPosDest(void)
{

	return m_pos;
}
void CMotionModel::SetRot(D3DXVECTOR3 set_rot)
{
	m_rot = set_rot;
}
D3DXVECTOR3 CMotionModel::GetRot(void)
{
	return m_rot;
}
D3DXMATRIX CMotionModel::GetMatrix(void)
{
	return m_mtxWorld;
}
void CMotionModel::SetMatrix(D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}


void CMotionModel::SetVertexPolygon(void)
{
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CMotionModel::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
}
