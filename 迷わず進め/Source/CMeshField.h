//=============================================================================
//
//  [CMeshField.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CMESHFIELD_H
#define _CMESHFIELD_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************



class CRenderer;
class CMeshField : public CScene
{
	public:
	CMeshField(int nPriority = 5):CScene(nPriority){};
	~CMeshField(void){};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };
	static CMeshField *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,int m_nNumBlockX, int m_nNumBlockZ,float Width,float Height);
	VERTEX_3D *SetMapVtx(void);
	static CMeshField * addressPresent(void);
	static float mWidth;
	static float mHeight;
	static int m_nNumVertex;							// 総頂点数
	static int m_nNumPolygon;							// 総ポリゴン数
	static int m_nNumIndex;								// 総インデックス数
	static int mNum_Vtx_X;
	static int mNum_Vtx_Z;
	static float Start_PosX;
	static float Start_PosZ;
	static int m_NumVecPolygon;
	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBufferField;	// インデックスバッファ


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