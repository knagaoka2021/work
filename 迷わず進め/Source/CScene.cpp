//=============================================================================
//
//  [CScene.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CScene.h"
#include "CList.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CScene* CScene::m_ptop[NUM_PRIORRITY]={NULL};									// リストの先頭アドレス
CScene* CScene::m_pCur[NUM_PRIORRITY]={NULL};									// リストの後ろアドレス
int CScene::m_nPriority = 0;

//=============================================================================
// クリエイト
//=============================================================================
CScene::CScene( int nPriority,OBJTYPE objType)
{
	m_nPriority = nPriority;
	m_objType = objType;
	type = 0;
	m_bDelete = false;
	// 先頭がいない場合
	if( m_ptop[nPriority] == NULL )
	{
		m_ptop[nPriority] = this;
	}
	// 終端がいない場合
	if( m_pCur[nPriority] == NULL )
	{
		m_pCur[nPriority] = this;
	}

	m_pCur[nPriority]->m_pNext = this;

	if( m_pCur[nPriority] == this )
	{
		m_pPrev = NULL;
	}
	else
	{
		m_pPrev = m_pCur[nPriority];
	}




	m_pCur[nPriority] = this;

	m_pNext = NULL;
}
//=============================================================================
// 全更新
//=============================================================================
void CScene::UpdateAll(void)
{
	// プライオリティによる処理
	for(int i=NUM_PRIORRITY;i>0;i--)
	{
		if(m_ptop[i-1] != NULL)
		{
			CScene *pScene = m_ptop[i-1];

			while(pScene)
			{

				CScene *pSceneNext = pScene->m_pNext;

				pScene->Update();

				// デリートフラグたったら
				if(pScene->m_bDelete)
				{
					pScene->UnLinkList(i-1);
					delete pScene;
				}

				pScene = pSceneNext;
			}
		}
	}
}
//=============================================================================
// 全描画
//=============================================================================
void CScene::DrawAll(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	// プライオリティによる処理
	for(int i=NUM_PRIORRITY;i>0;i--)
	{
		if(m_ptop[i-1] != NULL)
		{
			CScene *pScene = m_ptop[i-1];

			while(pScene)
			{
				CScene *pSceneNext = pScene->m_pNext;

				pScene->Draw(m_pD3DDevice);

				pScene = pSceneNext;
			}
		}
	}
}
//=============================================================================
// リリース削除
//=============================================================================
void CScene::Relese(void)
{
	//デリートフラグON
	m_bDelete = true;

}
//=============================================================================
// 全削除
//=============================================================================
void CScene::ReleseAll(void)
{
	// プライオリティによる処理
	for(int i=NUM_PRIORRITY;i>0;i--)
	{
		if(m_ptop[i-1] != NULL)
		{
			CScene *pScene = m_ptop[i-1];
	
			while(pScene)
			{
				CScene *pSceneNext = pScene->m_pNext;
				delete pScene;
				pScene = pSceneNext;
			}
		}
	}

}
//=============================================================================
// 前後結合
//=============================================================================
void CScene::UnLinkList(int numPriority)
{
	CScene *pScene, *pSceneNext, *pScenePrev;

	pScene = m_ptop[numPriority];

	while(pScene)
	{
		pSceneNext = pScene->m_pNext;

		if(pScene == this)
		{
			if(pScene->m_pPrev)
			{
				pScene->m_pPrev->m_pNext = pScene->m_pNext;
			}

			if(pScene->m_pNext)
			{
				pScene->m_pNext->m_pPrev = pScene->m_pPrev;
			}

			if(pScene == m_ptop[numPriority])
			{
				m_ptop[numPriority] = pSceneNext;
			}

		}

			pScene = pSceneNext;
	}

	pScene = m_pCur[numPriority];

	while(pScene)
	{
		pScenePrev = pScene->m_pPrev;

		if(pScene == this)
		{
			if(pScene->m_pPrev)
			{
				pScene->m_pPrev->m_pNext = pScene->m_pNext;
			}

			if(pScene->m_pNext)
			{
				pScene->m_pNext->m_pPrev = pScene->m_pPrev;
			}

			if(pScene == m_pCur[numPriority])
			{
				m_pCur[numPriority] = pScenePrev;
			}

		}

		pScene = pScenePrev;
	}

}