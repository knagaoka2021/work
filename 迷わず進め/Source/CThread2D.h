//=============================================================================
//
//  [CScene2D.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CTHREAD2D_H
#define _CTHREAD2D_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "CScene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_W	(250)
#define TEX_H	(250)

//*****************************************************************************
// 構造体定義
//*****************************************************************************



class CRenderer;
class CThread2D : public CScene2D
{
	public:
	CThread2D(int nPriority = 0):CScene2D(nPriority)
	{			
		///m_pos = D3DXVECTOR3(0.0f,0.0f,1.0f);

		m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	};
	~CThread2D(void){};

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
	static 	TYPE2D m_type2d;

	void SetColor(D3DXCOLOR color);
	static CThread2D *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,float wsize,float hsize,D3DXCOLOR color,TYPE2D type2d);
	void SetVisibleflag(bool flag);
	bool GetVisibleflag(void);
	bool m_visble;
	static float m_wsize;
	static float m_hsize;
	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_rot;									// 回転
	D3DXCOLOR m_color;


	float m_fLength;									// 対角線の長さ
	float m_fAngle;										// 対角線の角度
	int m_objType;

};

#endif
// EOF