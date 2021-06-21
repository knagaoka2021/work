//=============================================================================
//
//  [CNumber.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CNUMBER_H
#define _CNUMBER_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXNUMBER_W	(50)
#define TEXNUMBER_H	(50)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************





class CRenderer;
class CNumber : public CScene
{
	public:
	CNumber(int nPriority = 1):CScene(nPriority){};
	~CNumber(void){};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);

	void SetRot(D3DXVECTOR3 m_rot);
	void SetNum(int num);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };
	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};

	static CNumber *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,int m_num);

	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_rot;									// ��]

	float m_fLength;									// �Ίp���̒���
	float m_fAngle;										// �Ίp���̊p�x
	int m_number;
	OBJTYPE m_objType;
};

#endif
// EOF