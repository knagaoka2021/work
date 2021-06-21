//=============================================================================
//
//  [CSceneBoundingObj.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CSCENEBOUNDINGOBJ_H
#define _CSCENEBOUNDINGOBJ_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "CMotionModel.h"
#include "CObjModel.h"
#include "CSceneSkinMesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �\���̒�`
//*****************************************************************************




class CRenderer;
class CMotionModel;
class CObjModel;
class CObjEmpty;
class CSceneBoundingObj : public CScene
{
	public:
	CSceneBoundingObj(int nPriority = 3):CScene(nPriority){};
	~CSceneBoundingObj(void){};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };

	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 m_vtxMin;
	D3DXVECTOR3 m_vtxMax;
	D3DXVECTOR3 GetVertexMin();
	D3DXVECTOR3 GetVertexMax();
	D3DXMATRIX GetMatrix();
	void Setwhd(D3DXVECTOR3 whd);
	bool GetHitflag(void);
	void SetHitflag(bool flag);
	static CSceneBoundingObj *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,CMotionModel *p,CObjModel *pObj,CSceneSkinMesh *pSkin,float width, float height,float depth,PARTS_MATRIX parts);

	OBB test;
	OBB Getobb(void);
	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuffer;	// �C���f�b�N�X�o�b�t�@


	D3DXMATRIX	m_mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �n�ʂ̈ʒu
	D3DXVECTOR3 m_rot;								// �n�ʂ̌����i��]�j
	D3DXVECTOR3 m_scl;								// �n�ʂ̑傫���i�X�P�[���j
	D3DXVECTOR3 m_whd;

	float m_fLength;								// �Ίp���̒���
	float m_fAngle;									// �Ίp���̊p�x
	OBJTYPE m_objType;
	CMotionModel *m_pMotion;
	CObjModel *m_pObj;
	CSceneSkinMesh *m_pSkin;
	bool m_hitFlag;
	PARTS_MATRIX m_parts;


};

#endif
// EOF