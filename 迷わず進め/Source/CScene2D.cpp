//=============================================================================
//
//  [CScene2D.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CScene2D.h"
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
float g_width = SCREEN_WIDTH;
float g_height =SCREEN_HEIGHT;

//=============================================================================
// クリエイト
//=============================================================================
CScene2D *CScene2D::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,float wsize,float hsize,D3DXCOLOR color)
{
	CScene2D *pScene2D;
	pScene2D = new CScene2D(2);
	g_width =wsize;
	g_height=hsize;
	pScene2D->SetColor(color);
	pScene2D->Init(m_pD3DDevice,fileName);
	pScene2D->SetPos(m_pos);

	return pScene2D;


}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CScene2D::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	m_pD3DTex = NULL;

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

	m_wsize = g_width;
	m_hsize = g_height;
	if(fileName!=NULL)
	{
		D3DXCreateTextureFromFile(m_pD3DDevice,fileName,&m_pD3DTex);
	}

	//m_pos = D3DXVECTOR3(SCREEN_WIDTH/4,SCREEN_HEIGHT/4,1.0f);					//位置
	//m_rot = D3DXVECTOR3(0,0,-(D3DX_PI*0.5f));									//回転
	m_color = D3DCOLOR_RGBA(255,255,255,255);

	m_fLength = 0;																//対角線の長さ
	m_fAngle = 0;																//対角線の角度
	m_type2d = TYPE2D_NOMAR;
	m_Texuv = 0.0f;
	m_AnimationTime = 1.0f;


	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	g_width = 0;
	g_height =0;
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
//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{
	switch(m_type2d)
	{
	case TYPE2D_NOMAR:
		SetVertexPolygon();
		break;
	case TYPE2D_ANIMATION:
		SetVertexPolygonAnim();
		break;
	}

}
//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DDevice->SetRenderState( D3DRS_LIGHTING , FALSE );

		m_pD3DDevice->SetStreamSource(0,m_pD3DVtxBuff,0,sizeof(VERTEX_2D));

		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		if(m_pD3DTex!=NULL)
		{
			m_pD3DDevice->SetTexture(0,m_pD3DTex);//テクスチャの設定
		}
		else
		{
			m_pD3DDevice->SetTexture(0,NULL);//テクスチャの設定
		}

		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);//2はポリゴン数

		m_pD3DDevice->SetRenderState( D3DRS_LIGHTING , TRUE );
	}


}

void CScene2D::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CScene2D::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CScene2D::GetPosDest(void)
{

	return m_pos;
}
void CScene2D::SetRot(D3DXVECTOR3 set_rot)
{
		m_pos = set_rot;
}
D3DXVECTOR3 CScene2D::GetRot(void)
{
	return m_rot;
}
void CScene2D::SetColor(D3DXCOLOR color)
{
	m_color = color;
}
void CScene2D::SetWidth(float width)
{
	m_wsize = width;
}
void CScene2D::SetHeight(float height)
{
	m_hsize = height;
}
void CScene2D::SetVertexPolygon(void)
{
	VERTEX_2D *pVtx;

	if(m_pD3DVtxBuff != NULL)
	{

	m_fLength=sqrtf(((TEX_W)/2)*((TEX_W)/2)+((TEX_H)/2)*((TEX_H)/2));			//軸

	m_fAngle=atan2f((TEX_W)/2,(TEX_H)/2);												//アングル

	m_pD3DVtxBuff->Lock(0,0,(void **)&pVtx,0);


	pVtx[0].vtx.x=m_pos.x;
	pVtx[0].vtx.y=m_pos.y;

	pVtx[1].vtx.x=m_wsize+m_pos.x;
	pVtx[1].vtx.y=m_pos.y;

	pVtx[2].vtx.x=m_pos.x;
	pVtx[2].vtx.y=m_hsize+m_pos.y;

	pVtx[3].vtx.x=m_wsize+m_pos.x;
	pVtx[3].vtx.y=m_hsize+m_pos.y;

	pVtx[0].rhw=1.0f;

	pVtx[1].rhw=1.0f;

	pVtx[2].rhw=1.0f;

	pVtx[3].rhw=1.0f;

	pVtx[0].diffuse = m_color;

	pVtx[1].diffuse = m_color;

	pVtx[2].diffuse = m_color;

	pVtx[3].diffuse = m_color;

	pVtx[0].tex=D3DXVECTOR2(0,0);

	pVtx[1].tex=D3DXVECTOR2(1,0);

	pVtx[2].tex=D3DXVECTOR2(0,1);

	pVtx[3].tex=D3DXVECTOR2(1,1);

	m_pD3DVtxBuff->Unlock();

	}

	
}
void CScene2D::SetVertexPolygonAnim(void)
{
	VERTEX_2D *pVtx;

	if(m_pD3DVtxBuff != NULL)
	{

	if((int)m_AnimationTime%12==0&&m_Texuv != 8)
	{
		m_Texuv++;
	}

	m_pD3DVtxBuff->Lock(0,0,(void **)&pVtx,0);

	pVtx[0].vtx.x=m_pos.x;
	pVtx[0].vtx.y=m_pos.y;

	pVtx[1].vtx.x=m_wsize+m_pos.x;
	pVtx[1].vtx.y=m_pos.y;

	pVtx[2].vtx.x=m_pos.x;
	pVtx[2].vtx.y=m_hsize+m_pos.y;

	pVtx[3].vtx.x=m_wsize+m_pos.x;
	pVtx[3].vtx.y=m_hsize+m_pos.y;

	pVtx[0].rhw=1.0f;

	pVtx[1].rhw=1.0f;

	pVtx[2].rhw=1.0f;

	pVtx[3].rhw=1.0f;

	pVtx[0].diffuse = m_color;

	pVtx[1].diffuse = m_color;

	pVtx[2].diffuse = m_color;

	pVtx[3].diffuse = m_color;

	pVtx[0].tex=D3DXVECTOR2(0.125f*m_Texuv,0);

	pVtx[1].tex=D3DXVECTOR2(0.125f*m_Texuv+0.125f,0);

	pVtx[2].tex=D3DXVECTOR2(0.125f*m_Texuv,1.0f);

	pVtx[3].tex=D3DXVECTOR2(0.125f*m_Texuv+0.125f,1.0f);

	if(m_Texuv == 8)
	{
		m_Texuv = 0.0f;
		m_AnimationTime = 1.0f;
	}

	m_pD3DVtxBuff->Unlock();


	}
	m_AnimationTime+=1.0f;
	
}
void CScene2D::SetType2D(TYPE2D type2d)
{
	m_type2d = type2d;
}