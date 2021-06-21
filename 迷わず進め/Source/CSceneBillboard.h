//=============================================================================
//
//  [CSceneBillboard.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CSCENEBILLBOARD_H
#define _CSCENEBILLBOARD_H
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
class CSceneBillboard : public CScene
{
	public:
	CSceneBillboard(int nPriority = 5):CScene(nPriority){};
	~CSceneBillboard(void){};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);
	void SetSize(float width);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };
	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};

	static CSceneBillboard *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot);
	static float m_W;
	private:
	void	SetVertexPolygon(void);
	float m_Width;

	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;				// 頂点バッファへのポインタ


	D3DXMATRIX	m_mtxWorld;								// ワールドマトリックス
	D3DXVECTOR3 m_pos;								// 地面の位置
	D3DXVECTOR3 m_rot;								// 地面の向き（回転）
	D3DXVECTOR3 m_scl;								// 地面の大きさ（スケール）

	float m_fLength;									// 対角線の長さ
	float m_fAngle;										// 対角線の角度
	float m_fSize;										// 大きさ

	D3DXMATRIX m_mtxView;
		OBJTYPE m_objType;
};

#endif
// EOF