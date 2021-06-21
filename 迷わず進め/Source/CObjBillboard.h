//=============================================================================
//
//  [CobjBillbord.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_COBJBILLBOARD_H
#define _COBJBILLBOARD_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "CSceneBillboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �\���̒�`
//*****************************************************************************




class CObjBillboard : public CSceneBillboard
{
	public:
		CObjBillboard::CObjBillboard():CSceneBillboard(3)
		{
			m_pos = D3DXVECTOR3(1.0f,1.0f,0.0f);

			m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);


			m_fLength = 0;																//�Ίp���̒���
			m_fAngle = 0;																//�Ίp���̊p�x
		}
		~CObjBillboard(void)
		{
		// �����̃N���G�C�g
			//CExplotion::Create(m_pBulletDevice,m_pos,m_rot);
		};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);

	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };

	static CObjBillboard *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot);
	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);


	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DDEVICE9 m_pBulletDevice;
	LPDIRECT3DTEXTURE9	m_pD3DTextureModel;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH m_pD3DXMeshModel;
	LPD3DXBUFFER m_pD3DXBuffMatModel;
	DWORD m_nNumMatModel;


	D3DXMATRIX	m_mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �n�ʂ̈ʒu
	D3DXVECTOR3 m_rot;								// �n�ʂ̌����i��]�j
	D3DXVECTOR3 m_scl;								// �n�ʂ̑傫���i�X�P�[���j

	float m_fLength;								// �Ίp���̒���
	float m_fAngle;									// �Ίp���̊p�x
	float m_BulletSpX;
	float m_BulletSpZ;
	int m_work[4];


};

#endif
// EOF