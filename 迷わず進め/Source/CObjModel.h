//=============================================================================
//
//  [CObjModel.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_COBJMODEL_H
#define _COBJMODEL_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "CSceneX.h"
#include "CSceneBoundingObj.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �\���̒�`
//*****************************************************************************


class CSceneBoundingObj;
class CObjModel : public CSceneX
{
	public:
		CObjModel::CObjModel(int Priority,OBJTYPE objType):CSceneX(3,objType)
		{
			m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_rotDest = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
			m_objType = objType;

			m_fLength = 0;																//�Ίp���̒���
			m_fAngle = 0;																//�Ίp���̊p�x
			m_xSpeed = 0;									// x�ړ����x
			m_ySpeed = 0.0f;									// y�ړ����x
			m_zSpeed = 0;									// z�ړ����x
			m_jumpFlag = FALSE;
		}
		~CObjModel(void)
		{
			this->Uninit();
		};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 vtxMax,D3DXVECTOR3 vtxMin);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };

	D3DXVECTOR3 GetVertexMin(){return m_vtxMin;};
	D3DXVECTOR3 GetVertexMax(){return m_vtxMax;};

	D3DXMATRIX GetMatrix(void);

	static CObjModel *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,D3DXVECTOR3 vtxMax,D3DXVECTOR3 vtxMin);
	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);


	static CSceneX* m_pSceneXDummy;
	void SetDebugPlayerPos(void);
	void SetDebugFieldPos(void);


	float GetHeight(D3DXVECTOR3 pos,D3DXVECTOR3* pNomal,VERTEX_3D * m_HitCheckMap);
	float GetHeightPolygon(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,D3DXVECTOR3& pos,D3DXVECTOR3* pNomal);
	CSceneBoundingObj *m_pObb;
	CSceneBoundingObj *GetOBB(void);
	private:
	void	SetVertexPolygon(void);
	CPlayer* p_Player;


	LPDIRECT3DTEXTURE9	m_pD3DTextureModel;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH m_pD3DXMeshModel;
	LPD3DXBUFFER m_pD3DXBuffMatModel;
	DWORD m_nNumMatModel;


	D3DXMATRIX	m_mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �n�ʂ̈ʒu
	D3DXVECTOR3 m_rot;								// �n�ʂ̌����i��]�j
	D3DXVECTOR3 m_posDest;
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_scl;								// �n�ʂ̑傫���i�X�P�[���j
	D3DXVECTOR3 m_vtxMin;
	D3DXVECTOR3 m_vtxMax;

	float m_fLength;								// �Ίp���̒���
	float m_fAngle;									// �Ίp���̊p�x
	float m_xSpeed;									// x�ړ����x
	float m_ySpeed;									// y�ړ����x
	float m_zSpeed;									// z�ړ����x
	bool m_jumpFlag;								// �W�����v�t���O
	float m_yPrev;


	VERTEX_3D * m_HitCheckMap;						// �N�������蔻��p
	OBJTYPE m_objType;

};



#endif
// EOF