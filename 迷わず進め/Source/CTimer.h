//=============================================================================
//
//  [CTimer.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CTIME_H
#define _CTIME_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "CNumber.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TIMEDIGITS	(3)

//*****************************************************************************
// 構造体定義
//*****************************************************************************



class CTimer : public CNumber
{
	public:
		CTimer::CTimer():CNumber(3)
		{
			m_pos = D3DXVECTOR3(1.0f,1.0f,0.0f);

			m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);


			m_fLength = 0;																//対角線の長さ
			m_fAngle = 0;																//対角線の角度
		}
		~CTimer(void)
		{
		// 爆発のクリエイト
			//CExplotion::Create(m_pBulletDevice,m_pos,m_rot);
		};

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

	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};

	static CTimer *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,int m_num);
	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);


	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DDEVICE9 m_pShadowDevice;
	LPDIRECT3DTEXTURE9	m_pD3DTextureModel;		// テクスチャへのポインタ
	LPD3DXMESH m_pD3DXMeshModel;
	LPD3DXBUFFER m_pD3DXBuffMatModel;
	DWORD m_nNumMatModel;


	D3DXMATRIX	m_mtxWorld;							// ワールドマトリックス
	D3DXVECTOR3 m_pos;								// 地面の位置
	D3DXVECTOR3 m_rot;								// 地面の向き（回転）
	D3DXVECTOR3 m_scl;								// 地面の大きさ（スケール）

	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
	int m_number;

};
bool timerflag(void);
#endif
// EOF