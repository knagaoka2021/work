//=============================================================================
//
//  [CFade.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CFADE_H
#define _CFADE_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "common.h"
#include "CScene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************


class CFade : public CScene2D
{
	public:
	CFade(int nPriority = 0):CScene2D(nPriority)
	{
		m_fLength =0.0f;
	};
	~CFade(void){};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	void SetColor(D3DXCOLOR m_color);
	D3DXVECTOR3 GetPos(void);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);

	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };
	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};
	void StartFade(MODE_FADE mode,int m_time,D3DXCOLOR m_color);
	MODE_FADE GetModeFade(void);
	static CFade *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,MODE_FADE mode,int time,D3DXCOLOR color);

	private:
	void	SetVertexPolygon(void);




	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_rot;									// 回転

	float m_fLength;									// 対角線の長さ
	float m_fAngle;										// 対角線の角度

	int fadeTime;
	float fadeAlpha;
	D3DXCOLOR m_color;
	MODE_FADE m_mode;
};

#endif
// EOF