//=============================================================================
//
//  [CSceneBoundingObj.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CSceneBoundingObj.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VTXNUM_CUBE	(8)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
VERTEX_3D* pVtxBounding;
CMotionModel *g_pBounding = NULL;
CObjModel *g_pObjBounding = NULL;
CSceneSkinMesh *g_pSkinMesh = NULL;
PARTS_MATRIX g_parts = PARTS_MAX;
//=============================================================================
// クリエイト
//=============================================================================
CSceneBoundingObj *CSceneBoundingObj::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,CMotionModel *pMotion,CObjModel *pObj,CSceneSkinMesh *pSkin,float width, float height,float depth,PARTS_MATRIX parts)
{
	CSceneBoundingObj *pSceneBoundingObj;

	if(pMotion)
	g_pBounding = pMotion;
	if(pObj)
	g_pObjBounding = pObj;
	if(pSkin)
	g_pSkinMesh = pSkin;
	g_parts = parts;
	pSceneBoundingObj = new CSceneBoundingObj();
	pSceneBoundingObj->SetPos(m_pos);
	pSceneBoundingObj->Setwhd(D3DXVECTOR3(width,height,depth));
	pSceneBoundingObj->Init(m_pD3DDevice,fileName);




	return pSceneBoundingObj;


}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CSceneBoundingObj::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	
	m_pMotion = g_pBounding;
	m_pObj = g_pObjBounding;
	m_pSkin = g_pSkinMesh;
	g_pBounding = NULL;
	g_pObjBounding = NULL;
	g_pSkinMesh = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);

	
		 
		 
		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*VTXNUM_CUBE,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	
	VERTEX_3D Vertices[VTXNUM_CUBE] =
	{
		{ D3DXVECTOR3(-m_whd.x + m_pos.x, m_whd.y + m_pos.y, m_whd.z + m_pos.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(m_whd.x + m_pos.x, m_whd.y + m_pos.y, m_whd.z + m_pos.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(-m_whd.x + m_pos.x, m_whd.y + m_pos.y, -m_whd.z + m_pos.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(m_whd.x + m_pos.x, m_whd.y + m_pos.y, -m_whd.z + m_pos.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(-m_whd.x + m_pos.x, -m_whd.y + m_pos.y, m_whd.z + m_pos.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(m_whd.x + m_pos.x, -m_whd.y + m_pos.y, m_whd.z + m_pos.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(-m_whd.x + m_pos.x, -m_whd.y + m_pos.y, -m_whd.z + m_pos.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(m_whd.x + m_pos.x, -m_whd.y + m_pos.y, -m_whd.z + m_pos.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
	};
	
	
	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&pVtxBounding , 0 );

	memcpy( pVtxBounding , Vertices , sizeof( Vertices ) );

	m_pD3DVtxBuff->Unlock();

	// 最少頂点・最大頂点登録
	m_vtxMin = Vertices[6].vtx;
	m_vtxMax = Vertices[1].vtx;

	// インデックスバッファの生成
	if(FAILED(m_pD3DDevice->CreateIndexBuffer(sizeof(WORD)*((VTXNUM_CUBE*2)),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pD3DIndexBuffer,NULL)))
	{
		return E_FAIL;
	}
	WORD *pI;

	m_pD3DIndexBuffer->Lock(0,0,(void**)&pI,0);

	pI[0] = 5;
	pI[1] = 4;
	pI[2] = 7;
	pI[3] = 6;
	pI[4] = 7;
	pI[5] = 2;
	pI[6] = 3;
	pI[7] = 1;
	pI[8] = 7;
	pI[9] = 5;
	pI[10] = 4;
	pI[11] = 1;
	pI[12] = 0;
	pI[13] = 2;
	pI[14] = 4;
	pI[15] = 6;

	m_pD3DIndexBuffer->Unlock();


	m_parts = g_parts;
	g_parts = PARTS_MAX;

	m_pos = D3DXVECTOR3(0.0f,1.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	test.m_Pos = D3DXVECTOR3(0.0f,1.0f,0.0f);
 //方向ベクトル取得
	D3DXMATRIX matRot;
    D3DXMatrixRotationYawPitchRoll(&matRot, m_rot.y, m_rot.x, m_rot.z);
    test.m_NormaDirect[0] = D3DXVECTOR3(matRot._11,matRot._12,matRot._13);
    test.m_NormaDirect[1] = D3DXVECTOR3(matRot._21,matRot._22,matRot._23);
    test.m_NormaDirect[2] = D3DXVECTOR3(matRot._31,matRot._32,matRot._33);
    //長さ取得
    test.m_fLength[0] = fabsf(m_vtxMax.x - m_vtxMin.x) * 0.5f;
    test.m_fLength[1] = fabsf(m_vtxMax.y - m_vtxMin.y) * 0.5f;
    test.m_fLength[2] = fabsf(m_vtxMax.z - m_vtxMin.z) * 0.5f;

	

	m_hitFlag = false;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CSceneBoundingObj::Uninit(void)
{
	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff =NULL;
	}
	if(m_pD3DIndexBuffer != NULL)
	{
		m_pD3DIndexBuffer->Release();
		m_pD3DIndexBuffer=NULL;
	}
	if(m_pMotion!=NULL)
	{
		m_pMotion = NULL;
	}
	if(m_pObj!=NULL)
	{
		m_pObj = NULL;
	}
	if(m_pSkin!=NULL)
	{
		m_pSkin = NULL;
	}

	m_pos = D3DXVECTOR3(0.0f,1.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	CScene::Relese();
}
//=============================================================================
// 更新処理
//=============================================================================
void CSceneBoundingObj::Update(void)
{
	SetVertexPolygon();
	test.m_Pos = D3DXVECTOR3(m_mtxWorld._41,m_mtxWorld._42,m_mtxWorld._43);
}
//=============================================================================
// 描画処理
//=============================================================================
void CSceneBoundingObj::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	// World Matrix
	D3DXMatrixIdentity(&m_mtxWorld);															// 初期化


	//D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	if(m_pMotion)
	{
		D3DXMatrixMultiply( &m_mtxWorld, &m_mtxWorld, &m_pMotion->GetMatrix() );
	}
	if(m_pObj)
	{
		D3DXMatrixMultiply( &m_mtxWorld, &m_mtxWorld, &m_pObj->GetMatrix() );
	}
	if(m_pSkin)
	{
		D3DXMatrixMultiply( &m_mtxWorld, &m_mtxWorld, &m_pSkin->GetMatrix(m_parts) );
	}



	if (GetDebug())
	{
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ワイヤーフレーム描画

		m_pD3DDevice->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		m_pD3DDevice->SetIndices(m_pD3DIndexBuffer);

		m_pD3DDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

		m_pD3DDevice->SetTexture(0, NULL);										// テクスチャの設定

		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 8, 0, 14);

		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	}
}
//=============================================================================
// 処理
//=============================================================================
void CSceneBoundingObj::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CSceneBoundingObj::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CSceneBoundingObj::GetPosDest(void)
{

	return m_pos;
}
//=============================================================================
// 処理
//=============================================================================
void CSceneBoundingObj::SetRot(D3DXVECTOR3 set_rot)
{
	m_rot = set_rot;
}
D3DXVECTOR3 CSceneBoundingObj::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// 
//=============================================================================
void CSceneBoundingObj::SetVertexPolygon(void)
{

}
//=============================================================================
// 幅-高さ-奥行き設定
//=============================================================================
void CSceneBoundingObj::Setwhd(D3DXVECTOR3 whd)
{
	m_whd = whd;
}
D3DXVECTOR3 CSceneBoundingObj::GetVertexMin(void)
{
	return m_vtxMin;
}
D3DXVECTOR3 CSceneBoundingObj::GetVertexMax(void)
{
	return m_vtxMax;
}
D3DXMATRIX CSceneBoundingObj::GetMatrix()
{
	return m_mtxWorld;
}
void CSceneBoundingObj::SetHitflag(bool flag)
{
	m_hitFlag = flag;;
}
bool CSceneBoundingObj::GetHitflag(void)
{
	return m_hitFlag;
}
OBB CSceneBoundingObj::Getobb(void)
{
	return test;
}