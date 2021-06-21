	//=============================================================================
//
//  [CGame.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CGame.h"
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
#define GAMEOBJ_MAX (400)
#define OBJ_MAX (245)
#define STARTTITLEOBJPOS (525.0f)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct CObjPaty
{
	CObjModel *pObj;
	int Objnum;
};
//*****************************************************************************
// グローバル変数
//*****************************************************************************
CMeshField *g_pMeshField=NULL;
CPlayerMotion *p_Motion =NULL;
CObjPaty g_pObj[OBJ_MAX] = { NULL, 0 };
CList *g_pList = NULL;
CLife *g_pLife = NULL;
CGimmick *g_pGimmick = NULL;
CScene *pGameObj[GAMEOBJ_MAX];
int CGame::ObjCount = 0;
int objmove;
float speed = 1.0f;
static GAMEJUDG GameJudg= GAME_NONE;

CGame::CGame(void)
{
	objmove =100.0f;
};
CGame::~CGame(void)
{
};

//=============================================================================
// ゲームの初期化処理
//=============================================================================
HRESULT CGame::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,CCamera *p)
{
	ObjCount = 0;
	g_pList = new CList;
	g_pMeshField = NULL;
	//CGAMEInit
	
	pGameObj[0] = g_pGimmick = CGimmick::Create(m_pD3DDevice, "data\\TEXTURE\\wood.jpg", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0, 0, 0));
	pGameObj[1] = CTimer::Create(m_pD3DDevice,"data\\TEXTURE\\counter.jpg",D3DXVECTOR3(1000,0,1.0f),180);
	pGameObj[2] = g_pMeshField = CMeshField::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0,0,0),100,100,40.0f,40.0f);
	pGameObj[3] = CSkydome::Create(m_pD3DDevice,"data\\TEXTURE\\sora.jpg",D3DXVECTOR3(0.0f,-1000.0f,0.0f),D3DXVECTOR3(0,0,0),120.0f, 36,8,50.0f,6000.0f);

	pGameObj[4] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),18,2,0,-2000,2000);
	pGameObj[5] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),18,2,90,-2000,-2000);
	pGameObj[6] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),18,2,180,2000,-2000);
	pGameObj[7] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),18,2,270,2000,2000);
	
	pGameObj[9] = g_pLife = CLife::Create(m_pD3DDevice,"data\\TEXTURE\\life.png",D3DXVECTOR3(300,0,1.0f),111);
	pGameObj[10] = p_Motion = CPlayerMotion::Create(m_pD3DDevice,"data\\MODEL\\body.x",p,D3DXVECTOR3(450.0f,1000.0f,500.0f),D3DXVECTOR3(0,(D3DX_PI),0),g_pMeshField,g_pList);
	

	pGameObj[11] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,0.3f),18,2,0,-2000,-2000);
	pGameObj[12] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,0.3f),18,2,90,2000,-2000);
	pGameObj[13] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,0.3f),18,2,180,2000,2000);
	pGameObj[14] = CMeshWall::Create(m_pD3DDevice,"null",D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,0),D3DXCOLOR(1.0f,1.0f,1.0f,0.3f),18,2,270,-2000,2000);


	int count =0;
	for(int y=0;y<7;y++)
	{
		for(int x=0;x<7;x++)
		{
			if(y<2||4<y)
			{
				pGameObj[17 + count] = CObjBillboard::Create(m_pD3DDevice, "data\\TEXTURE\\tree000.png", D3DXVECTOR3(1645.0f - (x*STARTTITLEOBJPOS), 160.0f, -1645.0f + (y*STARTTITLEOBJPOS)), D3DXVECTOR3(0, 0, 0));
				count++;
			}
			if(1<y&&y<5&&x<2||5<x)
			{
				pGameObj[17 + count] = CObjBillboard::Create(m_pD3DDevice, "data\\TEXTURE\\tree000.png", D3DXVECTOR3(1645.0f - (x*STARTTITLEOBJPOS), 160.0f, -1645.0f + (y*STARTTITLEOBJPOS)), D3DXVECTOR3(0, 0, 0));
				count++;
			}

		}
	}



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	g_pLife = NULL;
	GameJudg = GAME_NONE;

	for (int num = 0; num<OBJ_MAX; num++)
	{
		g_pObj[num].pObj = NULL;
	}

	if(g_pList!=NULL)
	{
		g_pList->Uninit();
		delete g_pList;
		g_pList = NULL;
	}

	for (int i = 0; i<GAMEOBJ_MAX; i++)
	{
		if(pGameObj[i] != NULL)
		{
			pGameObj[i]->Uninit();
			pGameObj[i]=NULL;
		}
	}
	ObjCount = 0;
	g_pGimmick = NULL;
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CGame::Update(void)
{

}
//=============================================================================
// プレイヤーアドレス参照用
//=============================================================================
CPlayerMotion* CGame::addrPresent(void)
{
	return p_Motion;
}
//=============================================================================
// プレイヤーアドレス参照用
//=============================================================================
void CGame::SetPresent(void)
{
	p_Motion = NULL;
}
//=============================================================================
// プレイヤーライフアドレス参照用
//=============================================================================
CLife* CGame::CLifeAddrPresent(void)
{
	return g_pLife;
}
CObjModel *CGame::CObjModelAddrPresent(int num)
{
	return g_pObj[num].pObj;
}
CGimmick *CGame::CTestAddrPresentAddrPresent(int num)
{
	return g_pGimmick;
}
void CGame::CObjModelAddrDlete(int num)
{
	pGameObj[g_pObj[num].Objnum] = NULL;
	g_pObj[num].pObj = NULL;
	g_pObj[num].Objnum = 0;
	if(ObjCount!=0)
	{
	}
}
int CGame::GetCObjModelNum(void)
{
	return ObjCount;
}
GAMEJUDG GetJudg(void)
{
	return GameJudg;
}
void SetJudg(GAMEJUDG judg)
{
	GameJudg = judg;
}
