//=============================================================================
//
//  [CScene2D.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CNumber.h"
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
static int g_numbuff;

CNumber *CNumber::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,int m_num)
{
	g_numbuff = m_num;
	CNumber *pNumber;
	pNumber = new CNumber();
	pNumber->Init(m_pD3DDevice,fileName);
	pNumber->SetPos(m_pos);


	return pNumber;


}
HRESULT CNumber::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	m_number = g_numbuff;

	if(FAILED(m_pD3DDevice->CreateVertexBuffer(
												sizeof(VERTEX_2D)*4,
												D3DUSAGE_WRITEONLY,
												FVF_VERTEX_2D,
												D3DPOOL_MANAGED,
												&m_pD3DVtxBuff,
												NULL
												)
			)
	)
	{
		return E_FAIL;
	}

	D3DXCreateTextureFromFile(m_pD3DDevice,fileName,&m_pD3DTex);

	m_rot = D3DXVECTOR3(0,0,-(D3DX_PI*0.5f));									//回転
	m_fLength = 0;																//対角線の長さ
	m_fAngle = 0;																//対角線の角度

	return S_OK;
}
void CNumber::Uninit(void)
{
	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff =NULL;
	}
	if(m_pD3DTex != NULL)
	{
		m_pD3DTex->Release();
		m_pD3DTex =NULL;
	}

	CScene::Relese();
}
void CNumber::Update(void)
{
	SetVertexPolygon();
}
void CNumber::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	if(m_pD3DVtxBuff!=NULL)
	{
		m_pD3DDevice->SetRenderState( D3DRS_LIGHTING , FALSE );

		m_pD3DDevice->SetStreamSource(0,m_pD3DVtxBuff,0,sizeof(VERTEX_2D));

		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		m_pD3DDevice->SetTexture(0,m_pD3DTex);//テクスチャの設定

		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);//2はポリゴン数

		m_pD3DDevice->SetRenderState( D3DRS_LIGHTING , TRUE );
	}

}

void CNumber::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CNumber::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CNumber::GetPosDest(void)
{

	return m_pos;
}

void CNumber::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CNumber::GetRot(void)
{
	return m_rot;
}
//数字を変える処理
void CNumber::SetNum(int num)
{
	m_number = num;
}


void CNumber::SetVertexPolygon(void)
{
	VERTEX_2D *pVtx;

	if(m_pD3DVtxBuff != NULL)
	{

		m_fLength = sqrtf(((TEXNUMBER_W) / 2)*((TEXNUMBER_W) / 2) + ((TEXNUMBER_H) / 2)*((TEXNUMBER_H) / 2));			//軸

		m_fAngle = atan2f((TEXNUMBER_W) / 2, (TEXNUMBER_H) / 2);												//アングル

		m_pD3DVtxBuff->Lock(0,0,(void **)&pVtx,0);

		pVtx[0].vtx.x=m_pos.x;
		pVtx[0].vtx.y=m_pos.y;

		pVtx[1].vtx.x = TEXNUMBER_W + m_pos.x;
		pVtx[1].vtx.y=m_pos.y;

		pVtx[2].vtx.x=m_pos.x;
		pVtx[2].vtx.y = TEXNUMBER_H + m_pos.y;

		pVtx[3].vtx.x = TEXNUMBER_W + m_pos.x;
		pVtx[3].vtx.y = TEXNUMBER_H + m_pos.y;



		pVtx[0].rhw=1.0f;

		pVtx[1].rhw=1.0f;

		pVtx[2].rhw=1.0f;

		pVtx[3].rhw=1.0f;

		pVtx[0].diffuse=D3DCOLOR_RGBA(255,255,255,255);

		pVtx[1].diffuse=D3DCOLOR_RGBA(255,255,255,255);

		pVtx[2].diffuse=D3DCOLOR_RGBA(255,255,255,255);

		pVtx[3].diffuse=D3DCOLOR_RGBA(255,255,255,255);

		pVtx[0].tex=D3DXVECTOR2(0.1f*m_number,0);

		pVtx[1].tex=D3DXVECTOR2(0.1f*m_number+0.1f,0);

		pVtx[2].tex=D3DXVECTOR2(0.1f*m_number,1.0f);

		pVtx[3].tex=D3DXVECTOR2(0.1f*m_number+0.1f,1.0f);

		m_pD3DVtxBuff->Unlock();
	}

	
}
