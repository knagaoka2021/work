//=============================================================================
//
//  [CCylinder.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CSkydome.h"
#include "CScene3D.h"
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
char* CSkydome::m_FileName=NULL;
int CSkydome::m_nNumVertexEdge = 0;							// 総頂点数
int CSkydome::m_nNumVertexSplit = 0;							// 総頂点数
int CSkydome::m_NumVecPolygon = 0;
float CSkydome::Start_PosX = 0.0f;
float CSkydome::Start_PosZ = 0.0f;
float CSkydome::mWidth = 0.0f;
float CSkydome::mHeight = 0.0f;
float CSkydome::m_Radius = 0.0f;
VERTEX_3D vertex3D[4];

//=============================================================================
// クリエイト
//=============================================================================
CSkydome *CSkydome::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *m_fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,float m_Rad ,int m_edge, int m_split,float Width,float Height)
{
	CSkydome *pCSkydome;
	pCSkydome = new CSkydome();

	m_FileName = m_fileName;

	mWidth = Width;
	mHeight = Height;

	m_nNumVertexEdge = m_edge;															// 角の頂点数(X)
	m_nNumVertexSplit = m_split + 1;														// 分割頂点数(Y)

	m_Radius = m_Rad;

	pCSkydome->Init(m_pD3DDevice,m_fileName);
	pCSkydome->SetPos(m_pos);
	//pIndexDevice = m_pD3DDevice;
	//CScene3D::Create( m_pD3DDevice,"data\\TEXTURE\\mysphere.jpg", D3DXVECTOR3(0.0f,Height+650.0f,0.0f), m_rot);

	return pCSkydome;


}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CSkydome::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{

	m_nNumVertex = (m_nNumVertexEdge * m_nNumVertexSplit) + (m_nNumVertexEdge * 2) + m_nNumVertexEdge;					// 総頂点数数
	m_nNumPolygon = ((m_nNumVertexEdge * 2) * (m_nNumVertexSplit-1)) + (4 * (m_nNumVertexSplit-2)) + (m_nNumVertexEdge - 2) + 12;	// 総ポリゴン数
	m_nNumIndex = ((m_nNumPolygon) + m_nNumVertexEdge);												// 総インデックス数
	m_nNumTopPolygon = m_nNumVertexEdge;
	m_nNumTopIndex = m_nNumVertexEdge+2;
		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*m_nNumVertex,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}
	D3DXCreateTextureFromFile(m_pD3DDevice,m_FileName,&m_pD3DTex);
	// インデックスバッファの生成
	if(FAILED(m_pD3DDevice->CreateIndexBuffer(sizeof(WORD)*m_nNumIndex,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pD3DIndexBufferField,NULL)))
	{
		return E_FAIL;
	}

		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*m_nNumTopIndex,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pD3DVtxBufft,NULL)))
	{
		return E_FAIL;
	}
	D3DXCreateTextureFromFile(m_pD3DDevice,m_FileName,&m_pD3DTex);
	// インデックスバッファの生成
	if(FAILED(m_pD3DDevice->CreateIndexBuffer(sizeof(WORD)*m_nNumTopIndex,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pD3DIndexBufferFieldt,NULL)))
	{
		return E_FAIL;
	}


	// VRAMへ頂点データ転送
	VERTEX_3D* pV;	// 頂点格納生成
	VERTEX_3D* pVt;	// 頂点格納生成
	float tex =0.0f;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pV, 0);
	{
		// 頂点データを設定
		for(int y = 0; y < m_nNumVertexSplit; y++)
		{
			for(int x = 0; x < m_nNumVertexEdge; x++)
			{
				tex=cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * y)*m_Radius;
				pV[y*m_nNumVertexEdge+x].vtx = D3DXVECTOR3(mWidth*cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x)*tex,sin((D3DX_PI * 2.0f) / m_nNumVertexEdge* y)*mHeight, mWidth*sin(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x)*tex );
				pV[y*m_nNumVertexEdge+x].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pV[y*m_nNumVertexEdge+x].nor = D3DXVECTOR3(1.0f,1.0f,1.0f);
				//pV[y*m_nNumVertexEdge+x].tex= D3DXVECTOR2((float)(x % 2), (float)(y % 2));
				pV[y*m_nNumVertexEdge+x].tex= D3DXVECTOR2((float)x/(m_nNumVertexEdge), (float)(m_nNumVertexSplit-1-y)/(m_nNumVertexSplit-1));
			}

			//// 上のふた
			//if((y+1) >= m_nNumVertexSplit)
			//{
			//	for(int x = 0; x < m_nNumVertexEdge; x++)
			//	{
			//		pV[(y+1)*m_nNumVertexEdge + x].vtx = D3DXVECTOR3(sin(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x) * m_Radius, m_Radius * y, cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x) * m_Radius);
			//		pV[(y+1)*m_nNumVertexEdge + x].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//		pV[(y+1)*m_nNumVertexEdge + x].tex= D3DXVECTOR2(((m_Radius * 2) / 320.0f) + pV[(y+1)*m_nNumVertexEdge + x].vtx.x, ((m_nNumVertexEdge * 2) / 320.0f) + pV[(y+1)*m_nNumVertexEdge + x].vtx.z);
			//	}
			//}

		}


		//for( y = 0; y < m_nNumVertexSplit; y++)
		//{
		//	for( x = 0; x < m_nNumVertexEdge; x++)
		//	{
		//		//commonCross(&pV[y*m_nNumVertexEdge+x].nor, &pV[y*m_nNumVertexEdge+x].vtx, &pV[y*m_nNumVertexEdge+x+m_nNumVertexEdge].vtx, &pV[y*m_nNumVertexEdge+x+(m_nNumVertexEdge-1)].vtx);
		//	}
		//}

		// 頂点データを設定
		for(int y = 0; y < m_nNumVertexSplit; y++)
		{
			int x = 0;
			tex=cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * y)*m_Radius;
			pV[m_nNumVertexSplit*m_nNumVertexEdge+y].vtx = D3DXVECTOR3(mWidth*cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x)*tex,sin(((D3DX_PI * 2.0f) / m_nNumVertexEdge)* y)*mHeight, mWidth*sin(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x)*tex );
			pV[m_nNumVertexSplit*m_nNumVertexEdge+y].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pV[m_nNumVertexSplit*m_nNumVertexEdge+y].tex= D3DXVECTOR2( (float)1.0f, (float)(m_nNumVertexSplit-1-y)/(m_nNumVertexSplit-1) );
			pV[m_nNumVertexSplit*m_nNumVertexEdge+y].nor=D3DXVECTOR3(1.0f,1.0f,1.0f);
		}


	}


	m_pD3DVtxBuff->Unlock();


	m_pD3DVtxBufft->Lock(0, 0, (void**)&pVt, 0);

	tex=cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * (m_nNumVertexSplit-1))*m_Radius;
	pVt[0].vtx= D3DXVECTOR3(0.0f,sin((D3DX_PI * 2.0f) / m_nNumVertexEdge* (m_nNumVertexSplit-1))*mHeight+50.0f, 0.0f);
	pVt[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVt[0].nor =D3DXVECTOR3(1.0f,1.0f,1.0f);
	pVt[0].tex =  D3DXVECTOR2(0.0f,0.01f);

	// 頂点データを設定
	for(int y = m_nNumVertexSplit-1; y < m_nNumVertexSplit; y++)
	{
		for(int x = 0; x < m_nNumVertexEdge; x++)
		{
			tex=cos(((D3DX_PI * 20.0f) / m_nNumVertexEdge) * y)*m_Radius;
			pVt[x+1].vtx = D3DXVECTOR3(mWidth*cos(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x)*tex,sin((D3DX_PI * 2.0f) / m_nNumVertexEdge* y)*mHeight, mWidth*sin(((D3DX_PI * 2.0f) / m_nNumVertexEdge) * x)*tex );
			pVt[x+1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVt[x+1].nor=D3DXVECTOR3(1.0f,1.0f,1.0f);
			pVt[x+1].tex=  D3DXVECTOR2(0.0f,0.01f);
		}
	}

	pVt[m_nNumVertexEdge+1].vtx= pVt[1].vtx;
	pVt[m_nNumVertexEdge+1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVt[m_nNumVertexEdge+1].nor =D3DXVECTOR3(1.0f,1.0f,1.0f);
	pVt[m_nNumVertexEdge+1].tex = D3DXVECTOR2(0.0f,0.02f);

	m_pD3DVtxBufft->Unlock();



	// インデックスをVRAMへ転送
	LPWORD pI;	// インデックスデータ生成
	LPWORD pIt;	// ふた用インデックスデータ生成

	m_pD3DIndexBufferField->Lock(0, 0, (void**)&pI, 0);
	{
		int n = 0;
		int a = 0;
		int sy = 0;
		int work=0;

		//Index指定 STRIPver 
		for(sy = 0; sy < m_nNumVertexSplit-1; sy++)
		{
			for(int vx = 0; vx < m_nNumVertexEdge; vx++)
			{
				a = sy * m_nNumVertexEdge + vx;

				pI[n] = a + m_nNumVertexEdge;
				pI[n+1] = a;

				n += 2;
			}


			pI[n] = m_nNumVertexSplit*m_nNumVertexEdge+work+1;
			pI[n+1] = m_nNumVertexSplit*m_nNumVertexEdge+work;
			pI[n+2] = m_nNumVertexEdge * (sy+1);
			pI[n+3] = m_nNumVertexEdge * (sy+1);
			//// 周りのラストの閉じ
			////pI[n] = m_nNumVertexEdge * (sy + 1);
			//pI[n] =  m_nNumVertexSplit*m_nNumVertexEdge+work+1;
			////pI[n+1] =  pI[n] - m_nNumVertexEdge;
			//pI[n+1] =  m_nNumVertexSplit*m_nNumVertexEdge+work;

			//// 周りのラストの閉じ
			////pI[n] = m_nNumVertexEdge * (sy + 1);

			//pI[n+1] =  108;
			//pI[n+2] =  m_nNumVertexEdge * (sy + 1);
			//pI[n+3] =  m_nNumVertexEdge * (sy + 1)+1;
			//pI[n+4] =  36;

		n+=4;

			// 縮退
			if(sy < m_nNumVertexSplit-1)
			{


			//pI[n+1]  =  m_nNumVertexEdge * (sy+1);
			//pI[n+2]  =  m_nNumVertexEdge * (sy+2);
			//pI[n+3]  =  m_nNumVertexEdge * (sy+2);
	
				//pI[n+1] = m_nNumVertexEdge * (sy + 1);
				////pI[n+2] = pI[n+2];
				//n += 1;
			}

			work++;
			//// 上ふた
			//if((sy + 1) >= m_nNumVertexSplit-1)
			//{
			//	pI[n] = (m_nNumVertexEdge * (m_nNumVertexSplit-1 + 1));
			//	pI[n+1] = (m_nNumVertexEdge * (m_nNumVertexSplit-1 + 1));

			//	for(int nLoop = 0; nLoop < m_nNumVertexEdge / 2; nLoop++)
			//	{
			//		pI[n] = (m_nNumVertexEdge - (nLoop + 1)) + (m_nNumVertexEdge * (m_nNumVertexSplit-1 + 1));
			//		pI[n+1] = (nLoop + 1) + (m_nNumVertexEdge * (m_nNumVertexSplit-1 + 1));
			//		n+=2;
			//	}
			//}
		}
	}
	m_pD3DIndexBufferField->Unlock();

	m_pD3DIndexBufferFieldt->Lock(0, 0, (void**)&pIt, 0);

	for(int i=0;i<m_nNumTopIndex;i++)
	{
		pIt[i] = i;
	}

	m_pD3DIndexBufferFieldt->Unlock();


	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_fLength = 0;																//対角線の長さ
	m_fAngle = 0;																//対角線の角度


	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CSkydome::Uninit(void)
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
	
	if(m_pD3DIndexBufferField != NULL)
	{
		m_pD3DIndexBufferField->Release();
		m_pD3DIndexBufferField=NULL;
	}
	if(m_pD3DVtxBufft != NULL)
	{
		m_pD3DVtxBufft->Release();
		m_pD3DVtxBufft =NULL;
	}
	
	if(m_pD3DIndexBufferFieldt != NULL)
	{
		m_pD3DIndexBufferFieldt->Release();
		m_pD3DIndexBufferFieldt=NULL;
	}

	CScene::Relese();
}
//=============================================================================
// 更新処理
//=============================================================================
void CSkydome::Update(void)
{
	//if(CInputKeyboard::GetkeyboardPress(DIK_A))
	//{
	//	this->Uninit();
	//}
	m_rot.y-=D3DXToRadian(0.01f);
	SetVertexPolygon();
}
//=============================================================================
// 描画処理
//=============================================================================
void CSkydome::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	if(m_pD3DVtxBufft!=NULL)
	{

	// World Matrix
	D3DXMatrixIdentity(&m_mtxWorld);															// 初期化

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxScl);

	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y,m_rot.x,m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxRot);

	D3DXMatrixTranslation(&mtxTranslate,m_pos.x,m_pos.y,m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&mtxTranslate);

	m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);

	m_pD3DDevice->SetStreamSource( 0 , m_pD3DVtxBufft , 0 , sizeof( VERTEX_3D ) );

	m_pD3DDevice->SetIndices( m_pD3DIndexBufferFieldt );

	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

	m_pD3DDevice->SetTexture(0,m_pD3DTex);										// テクスチャの設定

	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN,0,0,m_nNumTopIndex-1,0,m_nNumTopPolygon);			// 2はポリゴン数(又はポリゴン数を３で割る)

		m_pD3DDevice->SetStreamSource( 0 , m_pD3DVtxBuff , 0 , sizeof( VERTEX_3D ) );

	m_pD3DDevice->SetIndices( m_pD3DIndexBufferField );

	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);													// 頂点フォーマットセット

	m_pD3DDevice->SetTexture(0,m_pD3DTex);										// テクスチャの設定

	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,m_nNumIndex,0,m_nNumPolygon);			// 2はポリゴン数(又はポリゴン数を３で割る)
	}


}
//=============================================================================
// 処理
//=============================================================================
void CSkydome::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CSkydome::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CSkydome::GetPosDest(void)
{

	return m_pos;
}
//=============================================================================
// 処理
//=============================================================================
void CSkydome::SetRot(D3DXVECTOR3 set_rot)
{
		m_pos = set_rot;
}
D3DXVECTOR3 CSkydome::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// 
//=============================================================================
void CSkydome::SetVertexPolygon(void)
{

}