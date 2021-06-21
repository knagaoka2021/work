//=============================================================================
//
//  [CTitle.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CTitle.h"
#include "CScene.h"
#include "CScene2D.h"
#include "CScene3D.h"
#include "CSceneX.h"
#include "CSceneBillboard.h"
#include "CMeshField.h"
#include "CMeshWall.h"
#include "CCylinder.h"
#include "CSkydome.h"
#include "CObjModel.h"
#include "CObjBillboard.h"
#include "CScore.h"
#include "CTimer.h"
#include "CList.h"
#include "CLife.h"
#include "CMotionModel.h"
#include "CPlayerMotion.h"
#include "CGimmick.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLEOBJ_MAX (120)
#define STARTTITLEOBJPOS (500.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CScene *pTitleObj[TITLEOBJ_MAX];
CMeshField *g_pMeshFieldDemo=NULL;
D3DXVECTOR3 g_pos = D3DXVECTOR3(SCREEN_WIDTH/2-(SCREEN_WIDTH/8),350.0f,1.0f);
CPlayerMotion *p_DemoMotion =NULL;
int y = 0;

CTitle::CTitle(void)
{
	g_pos = D3DXVECTOR3(SCREEN_WIDTH/2-(SCREEN_WIDTH/8),4800,1.0f);
};
CTitle::~CTitle(void)
{
};

//=============================================================================
// タイトルの初期化処理
//=============================================================================
HRESULT CTitle::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,CCamera *p)
{
	pTitleObj[0] =CScene2D::Create(m_pD3DDevice,"data\\TEXTURE\\tilogo.png",D3DXVECTOR3(SCREEN_WIDTH/2-(SCREEN_WIDTH/8),0,1.0f),D3DXVECTOR3(0,0,0),SCREEN_WIDTH/4,SCREEN_HEIGHT/6,D3DCOLOR_RGBA(255,0,0,255));
	pTitleObj[1] =CScene2D::Create(m_pD3DDevice,"data\\TEXTURE\\push.png",D3DXVECTOR3(0,40,0.0f),D3DXVECTOR3(0,0,0),SCREEN_WIDTH/4,SCREEN_HEIGHT/9,D3DCOLOR_RGBA(255,0,0,0));

	pTitleObj[3] = g_pMeshFieldDemo = CMeshField::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0,0,0),100,100,40.0f,40.0f);
	pTitleObj[4] = CSkydome::Create(m_pD3DDevice,"data\\TEXTURE\\sora.jpg",D3DXVECTOR3(0.0f,-1000.0f,0.0f),D3DXVECTOR3(0,0,0),120.0f, 36,8,50.0f,1600.0f);

	pTitleObj[5] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),18,2,0,-2000,2000);
	pTitleObj[6] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),18,2,90,-2000,-2000);
	pTitleObj[7] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),18,2,180,2000,-2000);
	pTitleObj[8] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),18,2,270,2000,2000);
	pTitleObj[9] = p_DemoMotion = CPlayerMotion::Create(m_pD3DDevice,"data\\MODEL\\body.x",p,D3DXVECTOR3(0.0f,0.0f,450.0f),D3DXVECTOR3(0,(D3DX_PI),0),g_pMeshFieldDemo,NULL);
	pTitleObj[10] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,0.3f),18,2,0,-2000,-2000);
	pTitleObj[11] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,0.3f),18,2,90,2000,-2000);
	pTitleObj[12] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,0.3f),18,2,180,2000,2000);
	pTitleObj[13] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,0.3f),18,2,270,-2000,2000);

	int count =0;
	for(int y=0;y<7;y++)
	{
		for(int x=0;x<7;x++)
		{
			if(y<2||4<y)
			{
				pTitleObj[15 + count] = CObjBillboard::Create(m_pD3DDevice, "data\\TEXTURE\\tree000.png", D3DXVECTOR3(1645.0f - (x*STARTTITLEOBJPOS), 160.0f, -1645.0f + (y*STARTTITLEOBJPOS)), D3DXVECTOR3(0, 0, 0));
				count++;
			}
			if(1<y&&y<5&&x<2||5<x)
			{
				pTitleObj[15 + count] = CObjBillboard::Create(m_pD3DDevice, "data\\TEXTURE\\tree000.png", D3DXVECTOR3(1645.0f - (x*STARTTITLEOBJPOS), 160.0f, -1645.0f + (y*STARTTITLEOBJPOS)), D3DXVECTOR3(0, 0, 0));
				count++;
			}

		}
	}
	return S_OK;
}

//=============================================================================
// タイトルの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	for(int i=0;i<TITLEOBJ_MAX;i++)
	{
		if(pTitleObj[i] != NULL)
		{
			pTitleObj[i]->Uninit();
			pTitleObj[i]=NULL;
		}
	}
	
}

//=============================================================================
// タイトルの更新処理
//=============================================================================
void CTitle::Update(void)
{
	pTitleObj[1]->SetPos(g_pos);
	if(g_pos.y>600.0f)
	{
		g_pos.y = 580.0f;
	}
	g_pos.y++;

}
CPlayerMotion* CTitle::addrPresent(void)
{
	return p_DemoMotion;
}