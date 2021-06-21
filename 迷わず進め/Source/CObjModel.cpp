//=============================================================================
//
//  [CObjModel.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CObjModel.h"
#include "common.h"
#define NUMMESHFIELDX	(101)
#define NUMMESHFIELDZ	(100)
#define GURAVITY	(0.65f)
#define DRIVE	(2)
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
CObjModel *CObjModel::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,D3DXVECTOR3 vtxMax,D3DXVECTOR3 vtxMin)
{

	CObjModel *pObjmodel;


	pObjmodel = new CObjModel(3,objType);

	pObjmodel->Init(m_pD3DDevice,fileName,vtxMax,vtxMin);

	pObjmodel->SetPos(m_pos);

	
	return pObjmodel;
}
HRESULT CObjModel::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 vtxMax,D3DXVECTOR3 vtxMin)
{
	//p_Player = CPlayer::GetCPlayerAddress();
	//LoadTexture(m_pD3DDevice);

	CSceneX::Init(m_pD3DDevice,fileName);
	m_pObb = CSceneBoundingObj::Create(m_pD3DDevice,"data\\TEXTURE\\systemwall.jpg",D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0,0,0),NULL,this,NULL,30.0f,45.0f,30.0f,PARTS_MAX);


	m_vtxMin = m_pObb->GetVertexMin();
	m_vtxMax = m_pObb->GetVertexMax();


	return S_OK;
}
void CObjModel::Uninit(void)
{
//---テクスチャの解放---//

	if(m_pObb!=NULL)
	{
		m_pObb->Uninit();
		m_pObb=NULL;
	}

	CSceneX::Uninit();

	CScene::Relese();
}
void CObjModel::Update(void)
{

	m_posDest = m_pos;
	m_rotDest = m_rot;

	


	//m_rot.y += 0.010f;
	// 座標更新
	CSceneX::SetPos(m_pos);
	CSceneX::SetRot(m_rot);



}
void CObjModel::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	CSceneX::Draw(m_pD3DDevice);
}
void CObjModel::SetPos(D3DXVECTOR3 set_pos)
{
	m_posDest =set_pos;
	m_pos = set_pos;
}
D3DXVECTOR3 CObjModel::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CObjModel::GetPosDest(void)
{

	return m_posDest;
}
void CObjModel::SetRot(D3DXVECTOR3 set_rot)
{
		m_pos = set_rot;
}
D3DXVECTOR3 CObjModel::GetRot(void)
{
	return m_rot;
}
D3DXMATRIX CObjModel::GetMatrix(void)
{
	m_mtxWorld = CSceneX::GetMatrix();
	return m_mtxWorld;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CObjModel::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

}
CSceneBoundingObj *CObjModel::GetOBB(void)
{
	return m_pObb;
}
// デバッグモード時　プレイヤー座標観測用



