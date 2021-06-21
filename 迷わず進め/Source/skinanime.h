//=============================================================================
//
//  [skinanime.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_SKINANIM_H
#define _SKINANIM_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

/// Xファイルから読込んだメッシュ
class XFileMesh sealed {
private:
	LPDIRECT3DDEVICE9	pDevice;		// Direct3Dデバイスオブジェクト
	LPD3DXMESH			pMesh;			// メッシュ
	D3DMATERIAL9*		pMeshMaterials;	// マテリアル配列
	LPDIRECT3DTEXTURE9*	pMeshTextures;	// テクスチャ配列
	DWORD				dwNumMaterials;	// マテリアル・テクスチャ配列の大きさ
public:
	/// コンストラクタ
	/// @param filename 画像ファイル名
	/// @param pDev		Direct3Dデバイスオブジェクト
	XFileMesh(LPCTSTR filename, LPDIRECT3DDEVICE9 pDev);
	~XFileMesh(void);

	void Draw(D3DXVECTOR3 position, D3DXMATRIX rotation);
	LPD3DXMESH GetMesh();
};

/// メッシュのビュー変換、射影変換を行なうカメラ
class MeshCamera sealed {
private:
	LPDIRECT3DDEVICE9 pDevice;

public:
	/// コンストラクタ
	/// @param pDev Direct3Dデバイスオブジェクト
	MeshCamera(LPDIRECT3DDEVICE9 pDev);

	~MeshCamera(void);
	void Look(D3DXVECTOR3 eyePoint, D3DXVECTOR3 lookAtPoint);
};


/// メッシュ用ライト
class MeshLight sealed {
private:
	LPDIRECT3DDEVICE9 pDevice;
	D3DLIGHT9 light;

public:
	/// コンストラクタ
	/// @param pDev Direct3Dデバイスオブジェクト
	MeshLight(LPDIRECT3DDEVICE9 pDev);

	~MeshLight(void);
	void Illume(D3DXVECTOR3 direction);
};


/// Xファイルから読込んだアニメーションメッシュ
class XFileAnimationMesh sealed {
private:
	struct MYFRAME: public D3DXFRAME {
		D3DXMATRIX CombinedTransformationMatrix;
	};
	struct MYMESHCONTAINER: public D3DXMESHCONTAINER {
		LPDIRECT3DTEXTURE9*  ppTextures;
		DWORD dwWeight;							// 重みの個数（重みとは頂点への影響。）
		DWORD dwBoneAmt;						// ボーンの数
		LPD3DXBUFFER pBoneBuffer;				// ボーン・テーブル
		D3DXMATRIX** ppBoneMatrix;				// 全てのボーンのワールド行列の先頭ポインター
		D3DXMATRIX* pBoneOffsetMatrices;		// フレームとしてのボーンのワールド行列のポインター
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
	LPDIRECT3DDEVICE9			pDevice;										// Direct3Dデバイスオブジェクト
	MyHierarchy					cHierarchy;										// アロケートヒエラルキーオブジェクト
	LPD3DXFRAME					pFrameRoot;										// フレーム
	LPD3DXANIMATIONCONTROLLER	pAnimController;								// アニメーションコントローラ
	LPD3DXANIMATIONSET			pAnimSet[100];									// アニメーションセット
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER);							// メッシュに行列を割り振る
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME);								// 再帰コード：フレーム階層を展開して各メッシュに行列を割り振る
	void UpdateFrameMatrices(LPD3DXFRAME , LPD3DXMATRIX );						// 再帰コード：フレームのワールド変換
	void RenderMeshContainer(LPDIRECT3DDEVICE9, MYMESHCONTAINER* ,MYFRAME* );	// メッシュコンテナのレンダリング
	void DrawFrame(LPDIRECT3DDEVICE9,LPD3DXFRAME );								// 再帰コード：フレームのレンダリング
	void FreeAnim(LPD3DXFRAME pF);												// 再帰コード：全てのメッシュコンテナをリリース
public:
	/// コンストラクタ
	/// @param filename 画像ファイル名
	/// @param pDevice	Direct3Dデバイスオブジェクト
	XFileAnimationMesh(LPCTSTR filename, LPDIRECT3DDEVICE9 pDevice);

	/// デストラクタ
	~XFileAnimationMesh(void);

	// 描画
	void Draw(D3DXVECTOR3 position, D3DXMATRIX rotation);

	/// 時間を進行
	void AdvanceTime(double time);
	/// 時間を進行
	void ResetTime(int motion_number);

	/// 動きを切り替える
	bool ChangeMotion(int motion_number);
	/// ループ時間設定
	bool SetLoopTime( int animID, double time );
	/// アニメーション移行時間設定
	bool SetShiftTime( int animID, double interval );
	/// アニメーション保管
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