//=============================================================================
//
//  [CMeshWall.cpp]
// Author : 長岡和真
//
//原因:インデックス数足りなくインデックスをVRAMへ転送 原因場所:// 縮退 現象:ヒープ壊れる 14_7/29
//=============================================================================
#include "CRenderer.h"
#include "CMeshWall.h"
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
#define POS	(50.0f)
#define W_SIZE	(100.0f)
#define Wall_SIZE	(500.0f)
#define H_SIZE	(50.0f)
int CMeshWall::m_nNumVertex = 0;							// 総頂点数
int CMeshWall::m_nNumPolygon = 0;							// 総ポリゴン数
int CMeshWall::m_nNumIndex = 0;								// 総インデックス数
int CMeshWall::mNum_Vtx_X=0;
int CMeshWall::mNum_Vtx_Z=0;
float CMeshWall::Start_PosX = 0.0f;
float CMeshWall::Start_PosY = 0.0f;
float CMeshWall::Start_PosZ = 0.0f;
float CMeshWall::m_radian = 0.0f;


CMeshWall* pMeshW;
//=============================================================================
// クリエイト
//=============================================================================
CMeshWall *CMeshWall::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,D3DXCOLOR m_color,int m_nNumBlockX, int m_nNumBlockZ,int rad,float Width,float Hight)
{
	CMeshWall *pMeshWall;
	pMeshWall = new CMeshWall(5);
	pMeshW = pMeshWall;
	mNum_Vtx_X = m_nNumBlockX+1;
	mNum_Vtx_Z = m_nNumBlockZ+1;
	m_nNumVertex = (mNum_Vtx_X) * (mNum_Vtx_Z);
	m_nNumPolygon = ((m_nNumBlockX*2) * m_nNumBlockZ) + (m_nNumBlockZ-1)*4;
	m_nNumIndex = (m_nNumPolygon)+4;
	Start_PosX = ((m_nNumBlockX / 2.0f) * W_SIZE);
	Start_PosY = ((m_nNumBlockZ / 2.0f) * W_SIZE);
	m_radian = (float)rad;
	Start_PosX = Width;
	Start_PosZ = Hight;
	pMeshWall->Init(m_pD3DDevice,fileName);
	pMeshWall->SetPos(m_pos);

	return pMeshWall;


}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CMeshWall::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	m_rad = m_radian;
	m_start_PosX = Start_PosX;
	m_start_PosZ = Start_PosZ;
	
	m_sinX=sinf(m_rot.y)*2;
	m_cosZ=cosf(m_rot.y)*2;;

	m_Wallpos[0].y=0.0f;
	m_Wallpos[1].y=0.0f;
	m_Wallpos[2].y=0.0f;
	m_Wallpos[3].y=0.0f;

	VERTEX_3D* pWVtx;

		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*m_nNumVertex,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	
	D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\wall_01.png",&m_pD3DTex);

	m_pD3DVtxBuff->Lock( 0 , 0, ( void** )&pWVtx , 0 );

		// 頂点データを設定
		for(int y = 0; y < mNum_Vtx_Z; y++)
		{
			for(int x = 0; x < mNum_Vtx_X; x++)
			{
				pWVtx[y*mNum_Vtx_X+x].vtx = D3DXVECTOR3(m_start_PosX + (x * W_SIZE*cos(D3DXToRadian(m_rad))), Start_PosY + (y * -W_SIZE), m_start_PosZ+(x * W_SIZE*sin(D3DXToRadian(m_rad))));
				pWVtx[y*mNum_Vtx_X+x].tex = D3DXVECTOR2((float)(x % 2), (float)(y % 2));
				pWVtx[y*mNum_Vtx_X+x].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			}
		}

	m_pD3DVtxBuff->Unlock();


	// インデックスバッファの生成
	if(FAILED(m_pD3DDevice->CreateIndexBuffer(sizeof(WORD)*m_nNumIndex,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pD3DIndexBufferWall,NULL)))
	{
		return E_FAIL;
	}

	WORD *pI;

	m_pD3DIndexBufferWall->Lock(0,0,(void**)&pI,0);

		int n = 0;
		int a;

		//Index指定
		for(int gz = 0; gz < mNum_Vtx_Z-1; gz++)
		{
			for(int vx = 0; vx < mNum_Vtx_X; vx++)
			{
				a = gz*mNum_Vtx_X+vx;

				pI[n+0] = a+mNum_Vtx_X;
				pI[n+1] = a;
				n+=2;
			}

			// 縮退
			if(gz < mNum_Vtx_X-2)
			{
				pI[n] = pI[n-1];
				pI[n+1] = pI[n] + mNum_Vtx_X + 1;
				n+=2;
			}
		}


	m_pD3DIndexBufferWall->Unlock();



	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_fLength=sqrtf(((200.0f)/2)*((200.0f)/2)+((200.0f)/2)*((200.0f)/2));			//軸

	m_fAngle=atan2f((200.0f)/2,(200.0f)/2);												//アングル

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CMeshWall::Uninit(void)
{

	if(m_pD3DTex != NULL)
	{
		m_pD3DTex->Release();
		m_pD3DTex =NULL;
	}
	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff =NULL;
	}
	if(m_pD3DIndexBufferWall != NULL)
	{
		m_pD3DIndexBufferWall->Release();
		m_pD3DIndexBufferWall=NULL;
	}
	CScene::Relese();
}
//=============================================================================
// 更新処理
//=============================================================================
void CMeshWall::Update(void)
{
	SetVertexPolygon();
}
//=============================================================================
// 描画処理
//=============================================================================
void CMeshWall::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	if(m_pD3DVtxBuff!=NULL)
	{
		// World Matrix
		D3DXMatrixIdentity(&m_mtxWorld);															// 初期化

		D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);

		D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

		D3DXMatrixRotationYawPitchRoll(&mtxRot,-m_rot.y,m_rot.x,m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRot);

		D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxTranslate);

	}


}
//=============================================================================
// 処理
//=============================================================================
void CMeshWall::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CMeshWall::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CMeshWall::GetPosDest(void)
{

	return m_pos;
}
D3DXVECTOR3 *CMeshWall::GetPosWall(void)
{
	m_Wallpos[0].x = -Wall_SIZE*cosf(m_rot.y)-sinf(m_rot.y)*Wall_SIZE+m_pos.x;
	m_Wallpos[0].z =  -Wall_SIZE*sinf(m_rot.y)+cosf(m_rot.y)*Wall_SIZE+m_pos.z;

	m_Wallpos[1].x = Wall_SIZE*cosf(m_rot.y)-sinf(m_rot.y)*Wall_SIZE+m_pos.x;
	m_Wallpos[1].z = Wall_SIZE*sinf(m_rot.y)+cosf(m_rot.y)*Wall_SIZE+m_pos.z;

	m_Wallpos[2].x = Wall_SIZE*cosf(m_rot.y)-sinf(m_rot.y)*-Wall_SIZE+m_pos.x;
	m_Wallpos[2].z = Wall_SIZE*sinf(m_rot.y)+cosf(m_rot.y)*-Wall_SIZE+m_pos.z;

	m_Wallpos[3].x = -Wall_SIZE*cosf(m_rot.y)-sinf(m_rot.y)*-Wall_SIZE+m_pos.x;
	m_Wallpos[3].z = -Wall_SIZE*sinf(m_rot.y)+cosf(m_rot.y)*-Wall_SIZE+m_pos.z;

	return &m_Wallpos[0];
}

//=============================================================================
// 処理
//=============================================================================
void CMeshWall::SetRot(D3DXVECTOR3 set_rot)
{
	m_rot = set_rot;
}
D3DXVECTOR3 CMeshWall::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// 
//=============================================================================
void CMeshWall::SetVertexPolygon(void)
{

}
CMeshWall* CMeshWall::PointerGet(void)
{
	return pMeshW;
}