//=============================================================================
//
//  [CScene2D.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_CSCENE2D_H
#define _CSCENE2D_H
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
#define WIDTH (200)
#define TEX_W	(250)
#define TEX_H	(250)
//*****************************************************************************
//  列挙体
//*****************************************************************************
typedef enum
{
	TYPE2D_NOMAR = 0,
	TYPE2D_ANIMATION,

}TYPE2D;

//*****************************************************************************
//  前方宣言	
//*****************************************************************************
class CRenderer;
class CScene2D : public CScene
{
	public:
	CScene2D(int nPriority = 3):CScene(nPriority)
	{
		m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	};
	~CScene2D(void){};

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

	void SetColor(D3DXCOLOR color);
	void SetWidth(float width);
	void SetHeight(float height);
	void SetType2D(TYPE2D type2d);
	static CScene2D *Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,float wsize,float hsize,D3DXCOLOR color);

	private:
	void	SetVertexPolygon(void);
	void	SetVertexPolygonAnim(void);
	TYPE2D m_type2d;
	float m_Texuv;
	float m_AnimationTime;

	LPDIRECT3DTEXTURE9		m_pD3DTex;
	LPDIRECT3DVERTEXBUFFER9 m_pD3DVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_rot;									// 回転
	D3DXCOLOR m_color;

	float m_wsize;
	float m_hsize;
	float m_fLength;									// 対角線の長さ
	float m_fAngle;										// 対角線の角度
	int m_objType;
};

#endif
// EOF