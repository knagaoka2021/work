//=============================================================================
//
//  [CGimmick.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CGIMMICK_H
#define _CGIMMICK_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CUBE_MAX (60)

//*****************************************************************************
// 列挙体定義
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
// 構造体定義
//*****************************************************************************
// 板ポリのワールド座標位置バッファ
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
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;			// 頂点バッファへのポインタ


	D3DXMATRIX	m_mtxWorld;							// ワールドマトリックス
	D3DXVECTOR3 m_pos;								// 地面の位置
	D3DXVECTOR3 m_rot;								// 地面の向き（回転）
	D3DXVECTOR3 m_scl;								// 地面の大きさ（スケール）

	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
	OBJTYPE m_objType;

};

#endif
// EOF