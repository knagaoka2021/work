//
//  [CScene.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_CSCENE_H
#define _CSCENE_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_PRIORRITY	(8)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
//  �񋓑�
//*****************************************************************************
typedef enum
{
	OBJTYPE_NONE = 0,
	OBJTYPE_2D,
	OBJTYPE_3D,
	OBJTYPE_X,
	OBJTYPE_BILLBOARD,
	OBJTYPE_GIMMICK
}OBJTYPE;

class CRenderer;

class CScene
{
	public:
		CScene( int nPriority,OBJTYPE objType = OBJTYPE_NONE);
		virtual ~CScene(void)=0{};

		virtual HRESULT Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)= 0;
		virtual void	Uninit(void) = 0;
		virtual void	Update(void) = 0;
		virtual void	Draw(LPDIRECT3DDEVICE9 m_pD3DDevice) = 0;

		virtual void SetPos(D3DXVECTOR3 m_pos) = 0;
		virtual D3DXVECTOR3 GetPos(void) = 0;

		virtual void SetRot(D3DXVECTOR3 m_rot) = 0;
		virtual D3DXVECTOR3 GetRot(void) = 0;

		virtual D3DXVECTOR3 GetPosDest(void) = 0;
		virtual D3DXVECTOR3 GetRotDest(void) = 0;


		virtual D3DXVECTOR3 GetVertexMin() = 0;
		virtual D3DXVECTOR3 GetVertexMax() = 0;



		static void UpdateAll(void);
		static void DrawAll(LPDIRECT3DDEVICE9 m_pD3DDevice);
		static void ReleseAll(void);
		void Relese(void);
		OBJTYPE m_objType;
		int type;

	private:
		static CScene* m_ptop[NUM_PRIORRITY];					// ���X�g�̐擪�A�h���X
		static CScene* m_pCur[NUM_PRIORRITY];					// ���X�g�̌��A�h���X
		CScene* m_pPrev;										// �O�̃I�u�W�F�N�g�̃A�h���X
		CScene* m_pNext;										// ���̂̃I�u�W�F�N�g�̃A�h���X
		static int m_nPriority;
		void UnLinkList(int numPriority);
		bool m_bDelete;											// �I�u�W�F�N�g�폜�t���O







};



#endif