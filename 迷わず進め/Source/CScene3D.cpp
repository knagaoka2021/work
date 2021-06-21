//=============================================================================
//
//  [CScene3D.cpp]
// Author : �����a�^
//
//=============================================================================
#include "CRenderer.h"
#include "CScene3D.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POS	(900.0f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �\����
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
VERTEX_3D* pVtx;

//=============================================================================
// �N���G�C�g
//=============================================================================
CScene3D *CScene3D::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot)
{
	CScene3D *pScene3D;
	pScene3D = new CScene3D();
	pScene3D->Init(m_pD3DDevice,fileName);
	pScene3D->SetPos(m_pos);

	return pScene3D;


}
//=============================================================================
// ������
//=============================================================================
HRESULT CScene3D::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	

		// ���_�o�b�t�@�̐���
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*4,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	
	VERTEX_3D Vertices[4] =
	{

 		{ D3DXVECTOR3( POS , 0.0f , POS) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.01) },
		{ D3DXVECTOR3( -POS , 0.0f , POS) , D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.01) } ,
		{ D3DXVECTOR3( POS , 0.0f , -POS) , D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255), D3DXVECTOR2(0.0f,0.01)} ,
		{ D3DXVECTOR3( -POS ,  0.0f , -POS) ,D3DXVECTOR3(0.0f,1.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.01) } ,
	};
	
	
	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&pVtx , 0 );

	memcpy( pVtx , Vertices , sizeof( Vertices ) );

	m_pD3DVtxBuff->Unlock();

	D3DXCreateTextureFromFile(m_pD3DDevice,fileName,&m_pD3DTex);

	m_pos = D3DXVECTOR3(0.0f,1.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_fLength = 0;																//�Ίp���̒���
	m_fAngle = 0;																//�Ίp���̊p�x


	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CScene3D::Uninit(void)
{
	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff =NULL;
	}
	if(m_pD3DTex != NULL)
	{
		m_pD3DTex->Release();
		m_pD3DTex =NULL;
	}

	CScene::Relese();
}
//=============================================================================
// �X�V����
//=============================================================================
void CScene3D::Update(void)
{
	//if(CInputKeyboard::GetkeyboardPress(DIK_A))
	//{
	//	this->Uninit();
	//}
	SetVertexPolygon();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CScene3D::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;


	// World Matrix
	D3DXMatrixIdentity(&m_mtxWorld);															// ������

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y,m_rot.x,m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRot);

	D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxTranslate);

	m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);


	m_pD3DDevice->SetStreamSource( 0 , m_pD3DVtxBuff , 0 , sizeof( VERTEX_3D ) );

	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);													// ���_�t�H�[�}�b�g�Z�b�g

	m_pD3DDevice->SetTexture(0,m_pD3DTex);										// �e�N�X�`���̐ݒ�

	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);								// 2�̓|���S����(���̓|���S�������R�Ŋ���)


}
//=============================================================================
// ����
//=============================================================================
void CScene3D::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CScene3D::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CScene3D::GetPosDest(void)
{

	return m_pos;
}
//=============================================================================
// ����
//=============================================================================
void CScene3D::SetRot(D3DXVECTOR3 set_rot)
{
	m_rot = set_rot;
}
D3DXVECTOR3 CScene3D::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// 
//=============================================================================
void CScene3D::SetVertexPolygon(void)
{

}
