//=============================================================================
//
//  [CSceneX.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CSCENEX_H
#define _CSCENEX_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "CSceneX.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************



class CRenderer;
class CSceneX : public CScene
{
	public:
		CSceneX::CSceneX(int Priority,OBJTYPE objType):CScene(3,objType)
		{
			m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

			m_objType = objType;

			m_fLength = 0;																//対角線の長さ
			m_fAngle = 0;																//対角線の角度

			m_pD3DXBuffMatModel = NULL;
			m_pD3DXMeshModel = NULL;
		};
		~CSceneX(void){};

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
	D3DXMATRIX GetMatrix(void);
	static CSceneX *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,CSceneX *p);
	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);

	private:
	void	SetVertexPolygon(void);



	LPDIRECT3DTEXTURE9	*m_pD3DTextureModel;		// テクスチャへのポインタ
	LPD3DXMESH m_pD3DXMeshModel;
	LPD3DXBUFFER m_pD3DXBuffMatModel;
	DWORD m_nNumMatModel;
	D3DMATERIAL9 *m_pMeshMaterials;	// マテリアル情報


	D3DXMATRIX	m_mtxWorld;							// ワールドマトリックス
	D3DXVECTOR3 m_pos;								// 地面の位置
	D3DXVECTOR3 m_rot;								// 地面の向き（回転）
	D3DXVECTOR3 m_scl;								// 地面の大きさ（スケール）

	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
	OBJTYPE m_objType;
	CSceneX *m_parent;

};

#endif
// EOF