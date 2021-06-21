//=============================================================================
//
//  [CRenderer.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CRENDERER_H
#define _CRENDERER_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "CScene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCENE_MAX	(4)
//*****************************************************************************
// 構造体定義
//*****************************************************************************


class CRenderer
{
	public:
	CRenderer(void);
	~CRenderer(void);

	HRESULT Init(HWND hWnd , BOOL bWindow);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	void AddScene(CScene* pScene);				// シーンの追加
	LPDIRECT3DDEVICE9 GetDevice(void);


	D3DXVECTOR3 GetRotCamera(void);

	D3DXMATRIX SetMtxView(void);

	private:
	void	DrawFPS(void);

	CScene* m_apScenes[SCENE_MAX];
	int m_nCountScene;


	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pD3DDevice;


};

#endif
// EOF