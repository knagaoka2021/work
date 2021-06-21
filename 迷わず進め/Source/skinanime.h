//=============================================================================
//
//  [skinanime.h]
// Author : �����a�^
//
//=============================================================================
#ifndef	_SKINANIM_H
#define _SKINANIM_H
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

/// X�t�@�C������Ǎ��񂾃��b�V��
class XFileMesh sealed {
private:
	LPDIRECT3DDEVICE9	pDevice;		// Direct3D�f�o�C�X�I�u�W�F�N�g
	LPD3DXMESH			pMesh;			// ���b�V��
	D3DMATERIAL9*		pMeshMaterials;	// �}�e���A���z��
	LPDIRECT3DTEXTURE9*	pMeshTextures;	// �e�N�X�`���z��
	DWORD				dwNumMaterials;	// �}�e���A���E�e�N�X�`���z��̑傫��
public:
	/// �R���X�g���N�^
	/// @param filename �摜�t�@�C����
	/// @param pDev		Direct3D�f�o�C�X�I�u�W�F�N�g
	XFileMesh(LPCTSTR filename, LPDIRECT3DDEVICE9 pDev);
	~XFileMesh(void);

	void Draw(D3DXVECTOR3 position, D3DXMATRIX rotation);
	LPD3DXMESH GetMesh();
};

/// ���b�V���̃r���[�ϊ��A�ˉe�ϊ����s�Ȃ��J����
class MeshCamera sealed {
private:
	LPDIRECT3DDEVICE9 pDevice;

public:
	/// �R���X�g���N�^
	/// @param pDev Direct3D�f�o�C�X�I�u�W�F�N�g
	MeshCamera(LPDIRECT3DDEVICE9 pDev);

	~MeshCamera(void);
	void Look(D3DXVECTOR3 eyePoint, D3DXVECTOR3 lookAtPoint);
};


/// ���b�V���p���C�g
class MeshLight sealed {
private:
	LPDIRECT3DDEVICE9 pDevice;
	D3DLIGHT9 light;

public:
	/// �R���X�g���N�^
	/// @param pDev Direct3D�f�o�C�X�I�u�W�F�N�g
	MeshLight(LPDIRECT3DDEVICE9 pDev);

	~MeshLight(void);
	void Illume(D3DXVECTOR3 direction);
};


/// X�t�@�C������Ǎ��񂾃A�j���[�V�������b�V��
class XFileAnimationMesh sealed {
private:
	struct MYFRAME: public D3DXFRAME {
		D3DXMATRIX CombinedTransformationMatrix;
	};
	struct MYMESHCONTAINER: public D3DXMESHCONTAINER {
		LPDIRECT3DTEXTURE9*  ppTextures;
		DWORD dwWeight;							// �d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
		DWORD dwBoneAmt;						// �{�[���̐�
		LPD3DXBUFFER pBoneBuffer;				// �{�[���E�e�[�u��
		D3DXMATRIX** ppBoneMatrix;				// �S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
		D3DXMATRIX* pBoneOffsetMatrices;		// �t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
	};
	class MyHierarchy : public ID3DXAllocateHierarchy {
	private:
		void CreateTexture(MYMESHCONTAINER* pMeshContainer, LPDIRECT3DDEVICE9 pDevice, DWORD NumMaterials);
		void SetDefaultMaterial(MYMESHCONTAINER* pMeshContainer);
	public:
		MyHierarchy(){};
		~MyHierarchy(){};
		STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
		STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*, CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD*, LPD3DXSKININFO, LPD3DXMESHCONTAINER*);
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
	};
	LPDIRECT3DDEVICE9			pDevice;										// Direct3D�f�o�C�X�I�u�W�F�N�g
	MyHierarchy					cHierarchy;										// �A���P�[�g�q�G�����L�[�I�u�W�F�N�g
	LPD3DXFRAME					pFrameRoot;										// �t���[��
	LPD3DXANIMATIONCONTROLLER	pAnimController;								// �A�j���[�V�����R���g���[��
	LPD3DXANIMATIONSET			pAnimSet[100];									// �A�j���[�V�����Z�b�g
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER);							// ���b�V���ɍs�������U��
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME);								// �ċA�R�[�h�F�t���[���K�w��W�J���Ċe���b�V���ɍs�������U��
	void UpdateFrameMatrices(LPD3DXFRAME , LPD3DXMATRIX );						// �ċA�R�[�h�F�t���[���̃��[���h�ϊ�
	void RenderMeshContainer(LPDIRECT3DDEVICE9, MYMESHCONTAINER* ,MYFRAME* );	// ���b�V���R���e�i�̃����_�����O
	void DrawFrame(LPDIRECT3DDEVICE9,LPD3DXFRAME );								// �ċA�R�[�h�F�t���[���̃����_�����O
	void FreeAnim(LPD3DXFRAME pF);												// �ċA�R�[�h�F�S�Ẵ��b�V���R���e�i�������[�X
public:
	/// �R���X�g���N�^
	/// @param filename �摜�t�@�C����
	/// @param pDevice	Direct3D�f�o�C�X�I�u�W�F�N�g
	XFileAnimationMesh(LPCTSTR filename, LPDIRECT3DDEVICE9 pDevice);

	/// �f�X�g���N�^
	~XFileAnimationMesh(void);

	// �`��
	void Draw(D3DXVECTOR3 position, D3DXMATRIX rotation);

	/// ���Ԃ�i�s
	void AdvanceTime(double time);
	/// ���Ԃ�i�s
	void ResetTime(int motion_number);

	/// ������؂�ւ���
	bool ChangeMotion(int motion_number);
	/// ���[�v���Ԑݒ�
	bool SetLoopTime( int animID, double time );
	/// �A�j���[�V�����ڍs���Ԑݒ�
	bool SetShiftTime( int animID, double interval );
	/// �A�j���[�V�����ۊ�
	LPD3DXANIMATIONSET animationSet[256];
	int m_MotionNumber;
	int m_lastMotionNumber;
	int GetMotionType(void);
	void SetMotionType(int motionNum);
	void SearchMatrix(LPD3DXFRAME pFrameBase);
	bool GetResetMotion(void);
	void SetResetMotion(bool resetMotion);
	bool resetMotion;
	D3DXMATRIX ArmMatrix;
	D3DXMATRIX GetPartsMatrix(char *partsName);
	double getloop(void);
};

#endif
// EOF