//=============================================================================
//
//  [CSceneSkinMesh.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CSCENESKINMESH_H
#define _CSCENESKINMESH_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "common.h"
#include "skinanime.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
//  列挙体
//*****************************************************************************
typedef enum
{
	PARTS_BODY = 0,
	PARTS_RIGHTARM,
	PARTS_MAX,
}PARTS_MATRIX;


class CRenderer;
class CSceneSkinMesh : public CScene
{
	public:
		CSceneSkinMesh::CSceneSkinMesh(int Priority,OBJTYPE objType):CScene(3,objType)
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
		~CSceneSkinMesh(void){};

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
	D3DXMATRIX GetMatrix(PARTS_MATRIX parts);
	static CSceneSkinMesh *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot);
	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);
	MOTIONTYPE GetSkinMotion(void);
	void ChangeSkinMotion(MOTIONTYPE motion);
	void SetSkinMotion(MOTIONTYPE motion);
	void SkinMotionResetTime(MOTIONTYPE motion);
	MOTIONTYPE m_motion;
	double Timeget(void);
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
	XFileAnimationMesh* model;
	PARTS_MATRIX m_parts;


};

#endif
// EOF