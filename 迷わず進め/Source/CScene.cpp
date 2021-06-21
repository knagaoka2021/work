//=============================================================================
//
//  [CScene.cpp]
// Author : �����a�^
//
//=============================================================================
#include "CRenderer.h"
#include "CScene.h"
#include "CList.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �\����
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CScene* CScene::m_ptop[NUM_PRIORRITY]={NULL};									// ���X�g�̐擪�A�h���X
CScene* CScene::m_pCur[NUM_PRIORRITY]={NULL};									// ���X�g�̌��A�h���X
int CScene::m_nPriority = 0;

//=============================================================================
// �N���G�C�g
//=============================================================================
CScene::CScene( int nPriority,OBJTYPE objType)
{
	m_nPriority = nPriority;
	m_objType = objType;
	type = 0;
	m_bDelete = false;
	// �擪�����Ȃ��ꍇ
	if( m_ptop[nPriority] == NULL )
	{
		m_ptop[nPriority] = this;
	}
	// �I�[�����Ȃ��ꍇ
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
// �S�X�V
//=============================================================================
void CScene::UpdateAll(void)
{
	// �v���C�I���e�B�ɂ�鏈��
	for(int i=NUM_PRIORRITY;i>0;i--)
	{
		if(m_ptop[i-1] != NULL)
		{
			CScene *pScene = m_ptop[i-1];

			while(pScene)
			{

				CScene *pSceneNext = pScene->m_pNext;

				pScene->Update();

				// �f���[�g�t���O��������
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
// �S�`��
//=============================================================================
void CScene::DrawAll(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	// �v���C�I���e�B�ɂ�鏈��
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
// �����[�X�폜
//=============================================================================
void CScene::Relese(void)
{
	//�f���[�g�t���OON
	m_bDelete = true;

}
//=============================================================================
// �S�폜
//=============================================================================
void CScene::ReleseAll(void)
{
	// �v���C�I���e�B�ɂ�鏈��
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
// �O�㌋��
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