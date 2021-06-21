//=============================================================================
//
//  [CSceneX.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CSceneX.h"
#include "common.h"
#include "CCamera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STARTPOSX	(-250.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
#define POSX	(-250.0f)
CSceneX *g_p = NULL;


CSceneX *CSceneX::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,OBJTYPE objType,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,CSceneX *p)
{
	CSceneX *pSceneX;
	pSceneX = new CSceneX(3,objType);
	g_p = p;
	pSceneX->Init(m_pD3DDevice,fileName);
	pSceneX->SetPos(m_pos);

	return pSceneX;


}
HRESULT CSceneX::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	m_parent = g_p;

	if(FAILED(D3DXLoadMeshFromX(fileName,D3DXMESH_SYSTEMMEM,m_pD3DDevice,NULL,&m_pD3DXBuffMatModel,NULL,&m_nNumMatModel,&m_pD3DXMeshModel)))
	{
		return E_FAIL;
	}

	// pD3DXMtrlBuffer から、質感やテクスチャーの情報を読み取る
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)m_pD3DXBuffMatModel->GetBufferPointer();

	m_pMeshMaterials = new D3DMATERIAL9[m_nNumMatModel];
	m_pD3DTextureModel  = new LPDIRECT3DTEXTURE9[m_nNumMatModel];

	for( DWORD i=0; i < m_nNumMatModel; i++ )
	{
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;// 質感のコピー

		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;// マテリアルの環境色を設定する

		if(d3dxMaterials[i].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile( m_pD3DDevice, d3dxMaterials[i].pTextureFilename, &m_pD3DTextureModel[i] );
		}
		else
		{
			m_pD3DTextureModel[i] = NULL;
		}
	}

	
	for( DWORD i=0; i < m_nNumMatModel; i++ )
	{
		d3dxMaterials[i].pTextureFilename;
		m_pD3DTextureModel[i];
	}

	return S_OK;
}
void CSceneX::Uninit(void)
{
	//---テクスチャの解放---//
	if( m_pMeshMaterials != NULL ) 
	{
		delete m_pMeshMaterials;
	}

	if( m_pMeshMaterials != NULL)
	{
		for( DWORD i = 0; i < m_nNumMatModel; i++ )
		{
			if(m_pD3DTextureModel[i]!=NULL)
			{
				m_pD3DTextureModel[i]->Release();
			}
		}
		delete m_pD3DTextureModel;
		m_pMeshMaterials = NULL;
	}

	if(m_pD3DXMeshModel != NULL)
	{
		m_pD3DXMeshModel->Release();
		m_pD3DXMeshModel=NULL;
	}
	if(m_pD3DXBuffMatModel != NULL)
	{
		m_pD3DXBuffMatModel->Release();
		m_pD3DXBuffMatModel=NULL;
	}

	m_pos = D3DXVECTOR3(0.0f,1.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	CScene::Relese();
}
void CSceneX::Update(void)
{
	
	SetVertexPolygon();
}
void CSceneX::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	if(m_pD3DXMeshModel != NULL)
	{


			
	/*D3DXPLANE p;

	D3DXPlaneFromPointNormal(&p,&D3DXVECTOR3(0.0f,-1.0f,0.0f),&D3DXVECTOR3(0.0f,1.0f,0.0f));*/

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate,mtxRefrect;
	//this->GetPos();
	//m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);				// 裏面をカリング

	// World Matrix
	D3DXMatrixIdentity(&m_mtxWorld);															// 初期化



	D3DXMatrixScaling(&mtxScl,m_scl.x,m_scl.y,m_scl.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y,m_rot.x,m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRot);

	D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxTranslate);





	//親が存在する場合は親マトリクスを合成
	if( m_parent )
	{
		D3DXMatrixMultiply( &m_mtxWorld, &m_mtxWorld, &m_parent->m_mtxWorld );

	}
	else
	{

		//D3DXMatrixReflect(&mtxRefrect,&p);

		//D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRefrect);

	}

	m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);



	//m_pD3DDevice->GetMaterial(m_pMeshMaterials);

	for(DWORD nCntMat=0;nCntMat<m_nNumMatModel;nCntMat++)
	{
		//m_pD3DDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		m_pD3DDevice->SetMaterial(&m_pMeshMaterials[nCntMat]);
		m_pD3DDevice->SetTexture(0,m_pD3DTextureModel[nCntMat]);
		m_pD3DXMeshModel->DrawSubset(nCntMat);
	}

	//m_pD3DDevice->SetMaterial(m_pMeshMaterials);								// マテリアルを元に戻す
	//m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	}

}

void CSceneX::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CSceneX::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CSceneX::GetPosDest(void)
{

	return m_pos;
}
void CSceneX::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CSceneX::GetRot(void)
{
	return m_rot;
}

D3DXMATRIX CSceneX::GetMatrix(void)
{
	return m_mtxWorld;
}
void CSceneX::SetVertexPolygon(void)
{

}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CSceneX::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	//--------テクスチャの読み込み----------//

	//D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\Untitled1.jpg",&m_pD3DTextureModel);
}
