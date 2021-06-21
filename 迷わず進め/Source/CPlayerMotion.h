//=============================================================================
//
//  [CPlayerMotion.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CPLAYERMOTION_H
#define _CPLAYERMOTION_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "CMotionModel.h"
#include "CMeshField.h"
#include "CCamera.h"
#include "Shadow.h"
#include "CList.h"
#include "CSceneBoundingObj.h"
#include "CGame.h"
#include "CSceneSkinMesh.h"

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
	PLAYERM_NOMAL,
	PLAYERM_DAMEGE,
	PLAYERM_INVINCIBLE,
	PLAYERM_STOP,
	PLAYERM_MAX
}PLAYERSTATEM;


class CPlayerMotion : public CMotionModel
{
	public:
		CPlayerMotion():CMotionModel(0,OBJTYPE_X)
		{
			m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_rotDest = D3DXVECTOR3(0.0f,0.0f,0.0f);

			m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);


			m_fLength = 0;																//対角線の長さ
			m_fAngle = 0;																//対角線の角度
			m_xSpeed = 0;									// x移動速度
			m_ySpeed = 0.0f;									// y移動速度
			m_zSpeed = 0;									// z移動速度
			m_jumpFlag = FALSE;
			m_fDiftRotY = 0.0f;
			m_numGimic = 0;
			m_evtTime = 0;
			m_pGimic = NULL;
			m_flagMove = TRUE;
		}
		~CPlayerMotion(void)
		{
			m_plyerState = PLAYERM_STOP;
		};

	HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
	void	Uninit(void);
	void	Update(void);
	void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetPos(D3DXVECTOR3 m_pos);
	D3DXVECTOR3 GetPos(void);
	bool GetMoveSpeedchek(void);

	void SetRot(D3DXVECTOR3 m_rot);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotDest(void){ return D3DXVECTOR3(0.0f,0.0f,0.0f); };

	D3DXVECTOR3 GetPosDest(void);

	D3DXVECTOR3 GetVertexMin(){return D3DXVECTOR3(-100.00000f,-10.00000f,-100.00000f);};
	D3DXVECTOR3 GetVertexMax(){return D3DXVECTOR3(100.00000f,20.00000f,100.00000f);};
	CList *m_list;
	static CPlayerMotion *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,CCamera *p_Camera,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,CMeshField *pCMeshFieldDummy,CList *plist);
	void LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice);

	void SetDebugPlayerPos(void);
	void SetDebugFieldPos(void);

	void SetMatrix(D3DXMATRIX mtx);
	float GetHeight(D3DXVECTOR3 pos,D3DXVECTOR3* pNomal,VERTEX_3D * m_HitCheckMap);
	float GetHeightPolygon(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,D3DXVECTOR3& pos,D3DXVECTOR3* pNomal);
	static CMeshField *g_pMeshFieldDummy;
	static CPlayerMotion* m_pPlayer;
	static CPlayerMotion* GetCPlayerAddress(void);
	CSceneBoundingObj *GetOBB(void);
	Shadow *m_pShadow;
	private:
	void	SetVertexPolygon(void);
	LPDIRECT3DDEVICE9 m_pPlayerDevice;

	float RollCheak(float rot);

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

	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
	float m_xSpeed;									// x移動速度
	float m_ySpeed;									// y移動速度
	float m_zSpeed;									// z移動速度
	float m_fvecFrontY;								// カメラからの垂直
	float m_fvecRightY;
	bool m_jumpFlag;								// ジャンプフラグ
	float m_yPrev;
	float m_fDiftRotY;
	int m_evtTime;
	CScene *m_pGimic;
	int m_numGimic;
	VERTEX_3D * m_HitCheckMap;						// 起伏当たり判定用
	CMeshField *m_pMeshFieldDummy;
	PLAYERSTATEM m_plyerState;
	int damagePoint;
	bool m_flagMove;
	CSceneBoundingObj *m_pObb;
	CSceneBoundingObj *m_pWeponObb;
	CMotionModel *m_pWepon;
	CSceneSkinMesh *m_pSkin;

};


D3DXVECTOR3 DebugMotionPlayerPos(void);
float DebugFieldMotionPlayerPos(void);
bool IncheckMotionPlayer(void);


#endif
// EOF