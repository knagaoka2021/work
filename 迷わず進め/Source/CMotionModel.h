//=============================================================================
//
//  [CMotionModel.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CMOTIONMODEL_H
#define _CMOTIONMODEL_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "CCamera.h"
#include "CMeshField.h"
#include "CList.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �\���̒�`
//*****************************************************************************


#define PART_MAX (10)
#define KEY_MAX (10)
class CRenderer;
class CMotionModel : public CScene
{
	public:
		CMotionModel::CMotionModel(int Priority,OBJTYPE objType):CScene(6,objType)
		{
			m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);


			m_fLength = 0;																//�Ίp���̒���
			m_fAngle = 0;																//�Ίp���̊p�x

			m_pD3DXBuffMatModel = NULL;
			m_pD3DXMeshModel = NULL;
		};
		~CMotionModel(void){};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	void SetMatrix(D3DXMATRIX mtx);
	D3DXVECTOR3 GetPos(void);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);
	bool GetMoveSpeedchek(void);

	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};

	static CMotionModel *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,CMotionModel *p);

	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);
	D3DXMATRIX GetMatrix(void);
	CList *m_list;
	private:
	void	SetVertexPolygon(void);



	LPDIRECT3DTEXTURE9	*m_pD3DTextureModel;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH m_pD3DXMeshModel;
	LPD3DXBUFFER m_pD3DXBuffMatModel;
	DWORD m_nNumMatModel;
	D3DMATERIAL9* m_pMeshMaterials;	// �}�e���A�����


	D3DXMATRIX	m_mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �n�ʂ̈ʒu
	D3DXVECTOR3 m_posDest;
	D3DXVECTOR3 m_rot;								// �n�ʂ̌����i��]�j
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_scl;								// �n�ʂ̑傫���i�X�P�[���j
	LPDIRECT3DDEVICE9 m_pMotionModel;

	float m_xSpeed;									// x�ړ����x
	float m_ySpeed;									// y�ړ����x
	float m_zSpeed;									// z�ړ����x

	float m_fLength;								// �Ίp���̒���
	float m_fAngle;									// �Ίp���̊p�x
	float m_fvecFrontY;								// �J��������̐���
	float m_fvecRightY;
	bool m_jumpFlag;								// �W�����v�t���O
	CMeshField *m_pMeshFieldDummy;
	CMotionModel *m_parent;
};
struct PART
{
	CMotionModel *data;
	PART	*parent;
	D3DXMATRIX matrix;
};
struct KEY_DATA
{
	D3DXVECTOR3 position[PART_MAX];
	D3DXVECTOR3	rotation[PART_MAX];
};
struct ANM_DATA
{
	KEY_DATA anim[PART_MAX];
	int parts_num;
	int frame;
};
D3DXVECTOR3 GetPartsRot(int num);
D3DXVECTOR3 DebugMotionModelPos(void);
int GetPartsNum(void);
float FremeTime(void);
#endif
// EOF