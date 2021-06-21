//=============================================================================
//
//  [CEfectContoroller.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CEFECTCONTOROLLER_H
#define _CEFECTCONTOROLLER_H

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "common.h"
#include "CEfect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


class CEfectContoroller : public CEfect
{
	public:
	CEfectContoroller(int nPriority = 3):CEfect(nPriority){};
	~CEfectContoroller(void){};

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

	static CEfectContoroller *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot);

	private:
	void	SetVertexPolygon(void);

	float m_EfectTexuv;
	int m_EfectTime;
	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;				// 頂点バッファへのポインタ


	D3DXMATRIX	m_mtxWorld;								// ワールドマトリックス
	D3DXVECTOR3 m_pos;								// 地面の位置
	D3DXVECTOR3 m_rot;								// 地面の向き（回転）
	D3DXVECTOR3 m_scl;								// 地面の大きさ（スケール）
	int m_alpha;

	float m_fLength;									// 対角線の長さ
	float m_fAngle;										// 対角線の角度

	D3DXMATRIX m_mtxView;
};

#endif
// EOF