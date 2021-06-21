//=============================================================================
//
//  [CObjModel.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_COBJMODEL_H
#define _COBJMODEL_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "CSceneX.h"
#include "CSceneBoundingObj.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************


class CSceneBoundingObj;
class CObjModel : public CSceneX
{
	public:
		CObjModel::CObjModel(int Priority,OBJTYPE objType):CSceneX(3,objType)
		{
			m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_rotDest = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
			m_objType = objType;

			m_fLength = 0;																//対角線の長さ
			m_fAngle = 0;																//対角線の角度
			m_xSpeed = 0;									// x移動速度
			m_ySpeed = 0.0f;									// y移動速度
			m_zSpeed = 0;									// z移動速度
			m_jumpFlag = FALSE;
		}
		~CObjModel(void)
		{
			this->Uninit();
		};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 vtxMax,D3DXVECTOR3 vtxMin);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetPosDest(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };

	D3DXVECTOR3 GetVertexMin(){return m_vtxMin;};
	D3DXVECTOR3 GetVertexMax(){return m_vtxMax;};

	D3DXMATRIX GetMatrix(void);

	static CObjModel *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,D3DXVECTOR3 vtxMax,D3DXVECTOR3 vtxMin);
	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);


	static CSceneX* m_pSceneXDummy;
	void SetDebugPlayerPos(void);
	void SetDebugFieldPos(void);


	float GetHeight(D3DXVECTOR3 pos,D3DXVECTOR3* pNomal,VERTEX_3D * m_HitCheckMap);
	float GetHeightPolygon(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,D3DXVECTOR3& pos,D3DXVECTOR3* pNomal);
	CSceneBoundingObj *m_pObb;
	CSceneBoundingObj *GetOBB(void);
	private:
	void	SetVertexPolygon(void);
	CPlayer* p_Player;


	LPDIRECT3DTEXTURE9	m_pD3DTextureModel;		// テクスチャへのポインタ
	LPD3DXMESH m_pD3DXMeshModel;
	LPD3DXBUFFER m_pD3DXBuffMatModel;
	DWORD m_nNumMatModel;


	D3DXMATRIX	m_mtxWorld;							// ワールドマトリックス
	D3DXVECTOR3 m_pos;								// 地面の位置
	D3DXVECTOR3 m_rot;								// 地面の向き（回転）
	D3DXVECTOR3 m_posDest;
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_scl;								// 地面の大きさ（スケール）
	D3DXVECTOR3 m_vtxMin;
	D3DXVECTOR3 m_vtxMax;

	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
	float m_xSpeed;									// x移動速度
	float m_ySpeed;									// y移動速度
	float m_zSpeed;									// z移動速度
	bool m_jumpFlag;								// ジャンプフラグ
	float m_yPrev;


	VERTEX_3D * m_HitCheckMap;						// 起伏当たり判定用
	OBJTYPE m_objType;

};



#endif
// EOF