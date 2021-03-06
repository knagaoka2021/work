//=============================================================================
//
//  [CobjBillbord.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_COBJBILLBOARD_H
#define _COBJBILLBOARD_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "CSceneBillboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************




class CObjBillboard : public CSceneBillboard
{
	public:
		CObjBillboard::CObjBillboard():CSceneBillboard(3)
		{
			m_pos = D3DXVECTOR3(1.0f,1.0f,0.0f);

			m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);


			m_fLength = 0;																//対角線の長さ
			m_fAngle = 0;																//対角線の角度
		}
		~CObjBillboard(void)
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

	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };

	static CObjBillboard *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot);
	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);


	private:
	void	SetVertexPolygon(void);


	LPDIRECT3DDEVICE9 m_pBulletDevice;
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
	float m_BulletSpX;
	float m_BulletSpZ;
	int m_work[4];


};

#endif
// EOF