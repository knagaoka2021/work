//=============================================================================
//
//  [CCamera.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CCAMERA_H
#define _CCAMERA_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "CScene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// 構造体定義
//*****************************************************************************


class CPlayer;
class CPlayerMotion;

class CCamera
{
	public:
	CCamera(void);
	~CCamera(void);


	HRESULT InitCamera(void);
	void UninitCamera(void);
	void UpdateCamera(int nIndex,CPlayerMotion *p_player);

	void SetCamera(int nIndex,LPDIRECT3DDEVICE9 m_pD3DDevice);

	D3DXVECTOR3 GetRotCamera(void);
	D3DXVECTOR3 GetCameraLength(void);
	D3DXMATRIX GetCameraMtxView(void);
	D3DXMATRIX GetCameraMtxProjection(void);
	void SetClass(CCamera* classtype);
	static CCamera* GetTypeClass(void);
	D3DXVECTOR3 m_vecCameraLength;


};
D3DXMATRIX GetMatrixV(void);
#endif
// EOF