//=============================================================================
//
//  [CGimmick.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CGIMMICK_H
#define _CGIMMICK_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CUBE_MAX (60)

//*****************************************************************************
// �񋓑̒�`
//*****************************************************************************
typedef enum
{
	TYPE_NORMAL = 0,
	TYPE_ROCK,
	TYPE_DAMAGE,
	TYPE_GOAL,
	TYPE_START,
	OBJMODE_MAX
}TYPE_OBJ;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �|���̃��[���h���W�ʒu�o�b�t�@
struct WorldPos {
	float x, y, z,w;
};


class CRenderer;
class CGimmick : public CScene
{
	public:
		CGimmick(int nPriority = 6) :CScene(nPriority){};
		~CGimmick(void){};

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
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-25.00000f,-25.00000f,-25.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(25.00000f,25.00000f,25.00000f);};
	WorldPos *GetPosObj(WorldPos *pOut);

	TYPE_OBJ *GetTypeObj(TYPE_OBJ *pOut);
	void CGimmick::SetPosObj(D3DXVECTOR3 pOut, int no);

	static CGimmick *Create(LPDIRECT3DDEVICE9 m_pD3DDevice, char *fileName, D3DXVECTOR3 m_pos, D3DXVECTOR3 m_rot);

	private:
	void	SetVertexPolygon(void);
	ID3DXEffect *effect;
	IDirect3DVertexDeclaration9 *decl;
	LPDIRECT3DVERTEXBUFFER9 posBuffer;
	LPDIRECT3DVERTEXBUFFER9 uvBuf;
	IDirect3DIndexBuffer9 *indexBuf;

	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^


	D3DXMATRIX	m_mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �n�ʂ̈ʒu
	D3DXVECTOR3 m_rot;								// �n�ʂ̌����i��]�j
	D3DXVECTOR3 m_scl;								// �n�ʂ̑傫���i�X�P�[���j

	float m_fLength;								// �Ίp���̒���
	float m_fAngle;									// �Ίp���̊p�x
	OBJTYPE m_objType;

};

#endif
// EOF