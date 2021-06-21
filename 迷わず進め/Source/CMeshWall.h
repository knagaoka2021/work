//=============================================================================
//
//  [CMeshWall.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CMESHWALL_H
#define _CMESHWALL_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �\���̒�`
//*****************************************************************************


class CRenderer;
class CMeshWall : public CScene
{
	public:
	CMeshWall(int nPriority = 4):CScene(nPriority){};
	~CMeshWall(void){};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 *GetPosWall(void);


	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);

	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};

	static CMeshWall *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,D3DXCOLOR m_color,int m_nNumBlockX, int m_nNumBlockZ,int rad,float Width,float Hight);


	static int m_nNumVertex;							// �����_��
	static int m_nNumPolygon;							// ���|���S����
	static int m_nNumIndex;								// ���C���f�b�N�X��
	static float m_radian;
	static int mNum_Vtx_X;
	static int mNum_Vtx_Z;
	static float Start_PosX;
	static float Start_PosY;
	static float Start_PosZ;
	//static D3DXCOLOR m_col;

	static CMeshWall* PointerGet(void);

	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBufferWall;	// �C���f�b�N�X�o�b�t�@


	D3DXMATRIX	m_mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �n�ʂ̈ʒu
	D3DXVECTOR3 m_rot;								// �n�ʂ̌����i��]�j
	D3DXVECTOR3 m_scl;								// �n�ʂ̑傫���i�X�P�[���j

	float m_fLength;								// �Ίp���̒���
	float m_fAngle;									// �Ίp���̊p�x

	D3DXVECTOR3 m_Wallpos[4];								// �n�ʂ̈ʒu

	float m_sinX;								// 
	float m_cosZ;								//
	float m_rad;
	float m_start_PosX;
	float m_start_PosY;
	float m_start_PosZ;
	D3DXCOLOR m_color;
	OBJTYPE m_objType;

};

#endif
// EOF