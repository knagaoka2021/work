//=============================================================================
//
//  [CCylinder.cpp]
// Author : �����a�^
//
//=============================================================================
#include "CRenderer.h"
#include "CCylinder.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STRATPOSCYLINDER	(50.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
char* CCylinder::m_FileName=NULL;
int CCylinder::m_nNumVertex = 0;							// �����_��
int CCylinder::m_nNumVertexEdge = 0;							// �����_��
int CCylinder::m_nNumVertexSplit = 0;							// �����_��
int CCylinder::m_nNumPolygon = 0;							// ���|���S����
int CCylinder::m_nNumIndex = 0;								// ���C���f�b�N�X��
int CCylinder::mNum_Vtx_X = 0;
int CCylinder::mNum_Vtx_Z = 0;
int CCylinder::m_NumVecPolygon = 0;
float CCylinder::Start_PosX = 0.0f;
float CCylinder::Start_PosZ = 0.0f;
float CCylinder::mWidth = 0.0f;
float CCylinder::mHeight = 0.0f;
float CCylinder::m_Radius = 0.0f;


//=============================================================================
// �N���G�C�g
//=============================================================================
CCylinder *CCylinder::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *m_fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,float m_Rad ,int m_edge, int m_split,float Width,float Height)
{
	CCylinder *pCCylinder;
	pCCylinder = new CCylinder();

	m_FileName = m_fileName;

	mWidth = Width;
	mHeight = Height;

	m_nNumVertexEdge = m_edge;															// �p�̒��_��(X)
	m_nNumVertexSplit = m_split + 1;														// �������_��(Y)
	m_nNumVertex = (m_nNumVertexEdge * m_nNumVertexSplit) + (m_edge * 2) + m_edge;					// �����_����
	m_nNumPolygon = ((m_nNumVertexEdge * 2) * m_split) + (4 * (m_split-1)) + (m_edge - 2) + 12;	// ���|���S����
	m_nNumIndex = ((m_nNumPolygon) + m_edge);												// ���C���f�b�N�X��
	m_Radius = m_Rad;

	pCCylinder->Init(m_pD3DDevice,m_fileName);
	pCCylinder->SetPos(m_pos);
	//pIndexDevice = m_pD3DDevice;


	return pCCylinder;


}
//=============================================================================
// ������
//=============================================================================
HRESULT CCylinder::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	// ���_�o�b�t�@�̐���
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*m_nNumVertex,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	D3DXCreateTextureFromFile(m_pD3DDevice,m_FileName,&m_pD3DTex);
	// �C���f�b�N�X�o�b�t�@�̐���
	if(FAILED(m_pD3DDevice->CreateIndexBuffer(sizeof(WORD)*m_nNumIndex,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pD3DIndexBufferField,NULL)))
	{
		return E_FAIL;
	}


	// VRAM�֒��_�f�[�^�]��
	VERTEX_3D* pV;	// ���_�i�[����
	VERTEX_3D pVt[1];	// ���_�i�[����
	float tex =0.1f;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pV, 0);
	{
		// ���_�f�[�^��ݒ�
		for(int y = 0; y < m_nNumVertexSplit; y++)
		{
			for(int x = 0; x < m_nNumVertexEdge; x++)
			{
				//pVt[0].vtx = 
				pV[y*m_nNumVertexEdge+x].vtx = D3DXVECTOR3(sin(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x) * m_Radius,m_Radius * y, cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x) * m_Radius);
				pV[y*m_nNumVertexEdge+x].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pV[y*m_nNumVertexEdge+x].tex= D3DXVECTOR2((float)(x%2), (float)(y%2));
			}

			// ��̂ӂ�
			if((y+1) >= m_nNumVertexSplit)
			{
				for(int x = 0; x < m_nNumVertexEdge; x++)
				{
					pV[(y+1)*m_nNumVertexEdge + x].vtx = D3DXVECTOR3(sin(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x) * m_Radius, m_Radius * y, cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x) * m_Radius);
					pV[(y+1)*m_nNumVertexEdge + x].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pV[(y+1)*m_nNumVertexEdge + x].tex= D3DXVECTOR2(((m_Radius * 2) / 320.0f) + pV[(y+1)*m_nNumVertexEdge + x].vtx.x, ((m_nNumVertexEdge * 2) / 320.0f) + pV[(y+1)*m_nNumVertexEdge + x].vtx.z);
				}
			}
		}
		int x,y;

		for( y = 0; y < m_nNumVertexSplit; y++)
		{
			for( x = 0; x < m_nNumVertexEdge; x++)
			{
				commonCross(&pV[y*m_nNumVertexEdge+x].nor, &pV[y*m_nNumVertexEdge+x].vtx, &pV[y*m_nNumVertexEdge+x+m_nNumVertexEdge].vtx, &pV[y*m_nNumVertexEdge+x+(m_nNumVertexEdge-1)].vtx);
			}
		}

		for(int nor = y*m_nNumVertexEdge+x-20;nor < m_nNumVertex;nor++)
		{
			pV[nor].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		}

	}
	m_pD3DVtxBuff->Unlock();

	// �C���f�b�N�X��VRAM�֓]��
	LPWORD pI;	// �C���f�b�N�X�f�[�^����

	m_pD3DIndexBufferField->Lock(0, 0, (void**)&pI, 0);
	{
		int n = 0;
		int a = 0;
		int sy = 0;

		//Index�w�� STRIPver 
		for(sy = 0; sy < m_nNumVertexSplit-1; sy++)
		{
			for(int vx = 0; vx < m_nNumVertexEdge; vx++)
			{
				a = sy * m_nNumVertexEdge + vx;

				pI[n] = a + m_nNumVertexEdge;
				pI[n+1] = a;
				n += 2;
			}

			// ����̃��X�g�̕�
			pI[n] = m_nNumVertexEdge * (sy + 1);
			pI[n+1] =  pI[n] - m_nNumVertexEdge;

			// �k��
			if(sy < m_nNumVertexSplit-1)
			{
				pI[n+2] = pI[n+1];
				pI[n+3] = (m_nNumVertexEdge * 2);
				n += 4;
			}

			// ��ӂ�
			if((sy + 1) >= m_nNumVertexSplit-1)
			{
				pI[n] = (m_nNumVertexEdge * (m_nNumVertexSplit-1 + 1));
				pI[n+1] = (m_nNumVertexEdge * (m_nNumVertexSplit-1 + 1));

				for(int nLoop = 0; nLoop < m_nNumVertexEdge / 2; nLoop++)
				{
					pI[n+2] = (m_nNumVertexEdge - (nLoop + 1)) + (m_nNumVertexEdge * (m_nNumVertexSplit-1 + 1));
					pI[n+3] = (nLoop + 1) + (m_nNumVertexEdge * (m_nNumVertexSplit-1 + 1));
					n+=2;
				}
			}
		}
	}
	m_pD3DIndexBufferField->Unlock();

	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_fLength = 0;																//�Ίp���̒���
	m_fAngle = 0;																//�Ίp���̊p�x


	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CCylinder::Uninit(void)
{

	if(m_pD3DTex != NULL)
	{
		m_pD3DTex->Release();
		m_pD3DTex =NULL;
	}

	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff =NULL;
	}
	
	if(m_pD3DIndexBufferField != NULL)
	{
		m_pD3DIndexBufferField->Release();
		m_pD3DIndexBufferField=NULL;
	}

	CScene::Relese();
}
//=============================================================================
// �X�V����
//=============================================================================
void CCylinder::Update(void)
{
	SetVertexPolygon();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CCylinder::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	if(m_pD3DVtxBuff!=NULL)
	{
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

		m_pD3DDevice->SetIndices( m_pD3DIndexBufferField );

		m_pD3DDevice->SetFVF(FVF_VERTEX_3D);													// ���_�t�H�[�}�b�g�Z�b�g

		m_pD3DDevice->SetTexture(0,m_pD3DTex);										// �e�N�X�`���̐ݒ�

		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,m_nNumVertex,0,m_nNumPolygon);			// 2�̓|���S����(���̓|���S�������R�Ŋ���)
	}


}
//=============================================================================
// ����
//=============================================================================
void CCylinder::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CCylinder::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CCylinder::GetPosDest(void)
{

	return m_pos;
}
//=============================================================================
// ����
//=============================================================================
void CCylinder::SetRot(D3DXVECTOR3 set_rot)
{
		m_pos = set_rot;
}
D3DXVECTOR3 CCylinder::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// 
//=============================================================================
void CCylinder::SetVertexPolygon(void)
{

}