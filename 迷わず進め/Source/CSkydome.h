//=============================================================================
//
//  [CSkydome.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CSKYDOME_H
#define _CSKYDOME_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "common.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �\���̒�`
//*****************************************************************************



class CRenderer;
class CSkydome : public CScene
{
	public:
	CSkydome(int nPriority = 5):CScene(nPriority){};
	~CSkydome(void){};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);

	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};

	static CSkydome *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *m_fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,float m_Rad ,int m_edge, int m_split,float Width,float Height);

	static float mWidth;
	static float mHeight;

	static int m_nNumVertexEdge;							// �����_��
	static int m_nNumVertexSplit;							// �����_��
	static float Start_PosX;
	static float Start_PosZ;
	static float m_Radius;
	static int m_NumVecPolygon;
	static char* m_FileName;
	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBufft;
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBufferField;	// �C���f�b�N�X�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBufferFieldt;	// �C���f�b�N�X�o�b�t�@


	D3DXMATRIX	m_mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �n�ʂ̈ʒu
	D3DXVECTOR3 m_rot;								// �n�ʂ̌����i��]�j
	D3DXVECTOR3 m_scl;								// �n�ʂ̑傫���i�X�P�[���j

	float m_fLength;								// �Ίp���̒���
	float m_fAngle;									// �Ίp���̊p�x

	int m_nNumVertex;							// �����_��
	int m_nNumPolygon;							// ���|���S����
	int m_nNumTopPolygon;							// ���|���S����
	int m_nNumIndex;								// ���C���f�b�N�X��
	int m_nNumTopIndex;								// ���C���f�b�N�X��
	int mNum_Vtx_X;
	int mNum_Vtx_Z;


};

#endif
// EOF