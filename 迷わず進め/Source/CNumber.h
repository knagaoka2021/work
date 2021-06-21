//=============================================================================
//
//  [CNumber.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CNUMBER_H
#define _CNUMBER_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXNUMBER_W	(50)
#define TEXNUMBER_H	(50)

//*****************************************************************************
// 構造体定義
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
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_rot;									// 回転

	float m_fLength;									// 対角線の長さ
	float m_fAngle;										// 対角線の角度
	int m_number;
	OBJTYPE m_objType;
};

#endif
// EOF