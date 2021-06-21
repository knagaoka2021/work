//=============================================================================
//
//  [CEfect.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CEFECT_H
#define _CEFECT_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define WIDTH (200)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


class CRenderer;
class CCamera;
class CEfect : public CScene
{
	public:
	CEfect(int nPriority=1):CScene(4)
	{
		m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

		m_scl = D3DXVECTOR3(5.0f,50.0f,1.0f);


		m_fLength = 0;																//�Ίp���̒���
		m_fAngle = 0;																//�Ίp���̊p�x

	};
	~CEfect(void){};

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
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};

	static CEfect *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot);

	private:
	void	SetVertexPolygon(void);

	float m_EfectTexuv;
	CCamera *m_pEfectCamera;

	int m_EfectTime;
	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^


	D3DXMATRIX	m_mtxWorld;								// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �n�ʂ̈ʒu
	D3DXVECTOR3 m_rot;								// �n�ʂ̌����i��]�j
	D3DXVECTOR3 m_scl;								// �n�ʂ̑傫���i�X�P�[���j
	int m_alpha;

	float m_fLength;									// �Ίp���̒���
	float m_fAngle;										// �Ίp���̊p�x

	D3DXMATRIX m_mtxView;
};

#endif
// EOF