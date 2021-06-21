//=============================================================================
//
//  [CGimmick.cpp]
// Author : 長岡和真
////最大モデル数60000個
//=============================================================================
#include "CRenderer.h"
#include "CGimmick.h"
#include"CCamera.h"
#include "common.h"
#include "CSceneBoundingObj.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POS	(90.0f)
#define WIDTH	(50.0f)
#define CUBE_STARTPOSX (250)
#define CUBE_STARTPOSY (25)
#define CUBE_STARTPOS (CUBE_STARTPOSY*2)
#define CUBE_STARTPOSZ (250)
#define CUBE_INDEXNUM_MAX (24)
#define CUBE_SURFACE_MAX (32)
#define CUBE_UV_VALUE	((1.0f / OBJMODE_MAX))

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点バッファ
struct Vtx {
	float x, y, z;
	float u,v;
};
struct UV {	// チップのUVバッファ
	float u, v;
};
//*****************************************************************************
// グローバル変数
//*****************************************************************************
TYPE_OBJ TypeBox[CUBE_MAX] = {
	
	TYPE_ROCK, TYPE_ROCK, TYPE_ROCK, TYPE_ROCK, TYPE_ROCK, TYPE_NORMAL, TYPE_NORMAL, TYPE_NORMAL, TYPE_NORMAL, TYPE_NORMAL, TYPE_NORMAL, TYPE_DAMAGE, TYPE_DAMAGE, TYPE_DAMAGE, TYPE_GOAL
};
WORD index[34] = { 
0, 1, 2,3,3,4,4,5,6,7,7,8,8,9,10,11,11,12,12,13,14,15,15,16,16,17,18,19,19,20,20,21,22,23
 };
CCamera *g_pCameraGimmick = NULL;

//=============================================================================
// クリエイト
//=============================================================================
CGimmick *CGimmick::Create(LPDIRECT3DDEVICE9 m_pD3DDevice, char *fileName, D3DXVECTOR3 m_pos, D3DXVECTOR3 m_rot)
{	

	CGimmick *pScene3D;
	pScene3D = new CGimmick();
	pScene3D->SetPos(m_pos);
	pScene3D->Init(m_pD3DDevice,fileName);

	g_pCameraGimmick = CCamera::GetTypeClass();
	return pScene3D;


}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CGimmick::Init(LPDIRECT3DDEVICE9 m_pD3DDevice, char *fileName)
{

	posBuffer = NULL;
	indexBuf = 0;
	m_pD3DTex = NULL;
	D3DXVECTOR3 m_whd = D3DXVECTOR3(25.0f,25.0f,25.0f);

	Vtx vtx[CUBE_INDEXNUM_MAX] =
	{
		{ -m_whd.x, m_whd.y, m_whd.z, 0.0f, 0.0f },
		{ m_whd.x, m_whd.y, m_whd.z, CUBE_UV_VALUE, 0.0f },
		{ -m_whd.x, m_whd.y, -m_whd.z, 0.0f, 1.0f },
		{ m_whd.x, m_whd.y, -m_whd.z, CUBE_UV_VALUE, 1.0f },

		{ -m_whd.x, m_whd.y, -m_whd.z, 0.0f, 0.0f },
		{ m_whd.x, m_whd.y, -m_whd.z, CUBE_UV_VALUE, 0.0f },
		{ -m_whd.x, -m_whd.y, -m_whd.z, 0.0f, 1.0f },
		{ m_whd.x, -m_whd.y, -m_whd.z, CUBE_UV_VALUE, 1.0f },

		{ m_whd.x, m_whd.y, -m_whd.z, 0.0f, 0.0f },
		{ m_whd.x, m_whd.y, m_whd.z, CUBE_UV_VALUE, 0.0f },
		{ m_whd.x, -m_whd.y, -m_whd.z, 0.0f, 1.0f },
		{ m_whd.x, -m_whd.y, m_whd.z, CUBE_UV_VALUE, 1.0f },

		{ m_whd.x, m_whd.y, m_whd.z, 0.0f, 0.0f },
		{ -m_whd.x, m_whd.y, m_whd.z, CUBE_UV_VALUE, 0.0f },
		{ m_whd.x, -m_whd.y, m_whd.z, 0.0f, 1.0f },
		{ -m_whd.x, -m_whd.y, m_whd.z, CUBE_UV_VALUE, 1.0f },

		{ -m_whd.x, m_whd.y, m_whd.z, 0.0f, 0.0f },
		{ -m_whd.x, m_whd.y, -m_whd.z, CUBE_UV_VALUE, 0.0f },
		{ -m_whd.x, -m_whd.y, m_whd.z, 0.0f, 1.0f },
		{ -m_whd.x, -m_whd.y, -m_whd.z, CUBE_UV_VALUE, 1.0f },

		{ m_whd.x, -m_whd.y, m_whd.z, 0.0f, 0.0f },
		{ -m_whd.x, -m_whd.y, m_whd.z, CUBE_UV_VALUE, 0.0f },
		{ m_whd.x, -m_whd.y, -m_whd.z, 0.0f, 1.0f },
		{ -m_whd.x, -m_whd.y, -m_whd.z, CUBE_UV_VALUE, 1.0f },
	};

		// 頂点バッファの生成
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof( vtx ),0,0 ,D3DPOOL_MANAGED,&m_pD3DVtxBuff,NULL)))
	{
		return E_FAIL;
	}

	copyBuf( sizeof( vtx ), vtx, m_pD3DVtxBuff );


	WorldPos *worldPos = new WorldPos[ CUBE_MAX ];

	for (int i = 0; i < CUBE_MAX; i++)
	{
		worldPos[i].x = CUBE_STARTPOS * GetRandom(-5, 5);
		worldPos[i].y = CUBE_STARTPOSY * (GetRandom(0, 5)*2+1);
		worldPos[i].z = CUBE_STARTPOS * GetRandom(-5, 5);
	}

		worldPos[0].x = CUBE_STARTPOS * -2;
		worldPos[0].y = CUBE_STARTPOSY * 1;
		worldPos[0].z = CUBE_STARTPOS * 0;

		worldPos[1].x = CUBE_STARTPOS * -1;
		worldPos[1].y = CUBE_STARTPOSY * 1;
		worldPos[1].z = CUBE_STARTPOS * 0;

		worldPos[2].x = CUBE_STARTPOS * -1;
		worldPos[2].y = CUBE_STARTPOSY * 3;
		worldPos[2].z = CUBE_STARTPOS * 0;

		worldPos[3].x = CUBE_STARTPOS * 0;
		worldPos[3].y = CUBE_STARTPOSY * 5;
		worldPos[3].z = CUBE_STARTPOS * 0;

		worldPos[4].x = CUBE_STARTPOS * 0;
		worldPos[4].y = CUBE_STARTPOSY * 13;
		worldPos[4].z = CUBE_STARTPOS * 3;

		worldPos[5].x = CUBE_STARTPOS * 0;
		worldPos[5].y = CUBE_STARTPOSY * 1;
		worldPos[5].z = CUBE_STARTPOS * 0;

		worldPos[6].x = CUBE_STARTPOS * 1;
		worldPos[6].y = CUBE_STARTPOSY * 3;
		worldPos[6].z = CUBE_STARTPOS * 0;

		worldPos[7].x = CUBE_STARTPOS * 1;
		worldPos[7].y = CUBE_STARTPOSY * 1;
		worldPos[7].z = CUBE_STARTPOS * 0;

		worldPos[8].x = CUBE_STARTPOS * 2;
		worldPos[8].y = CUBE_STARTPOSY * 1;
		worldPos[8].z = CUBE_STARTPOS * 0;

		worldPos[9].x = CUBE_STARTPOS * 0;
		worldPos[9].y = CUBE_STARTPOSY * 1;
		worldPos[9].z = CUBE_STARTPOS * 1;

		worldPos[10].x = CUBE_STARTPOS * 0;
		worldPos[10].y = CUBE_STARTPOSY * 1;
		worldPos[10].z = CUBE_STARTPOS * -1;

		worldPos[11].x = CUBE_STARTPOS * 0;
		worldPos[11].y = CUBE_STARTPOSY * 3;
		worldPos[11].z = CUBE_STARTPOS * 1;

		worldPos[12].x = CUBE_STARTPOS * 0;
		worldPos[12].y = CUBE_STARTPOSY * 5;
		worldPos[12].z = CUBE_STARTPOS * -1;

		worldPos[13].x = CUBE_STARTPOS * 3;
		worldPos[13].y = CUBE_STARTPOSY * 3;
		worldPos[13].z = CUBE_STARTPOS * 0;

		worldPos[14].x = CUBE_STARTPOS * 0;
		worldPos[14].y = CUBE_STARTPOSY * 12;
		worldPos[14].z = CUBE_STARTPOS * 5;

		if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(WorldPos) * CUBE_MAX, 0, 0, D3DPOOL_MANAGED, &posBuffer, NULL)))
		{
			return E_FAIL;
		}

		copyBuf(sizeof(WorldPos) * CUBE_MAX, worldPos, posBuffer);



		delete[] worldPos;



	UV *uv = new UV[CUBE_MAX]; // チップUVバッファ
	for (int i = 0; i < CUBE_MAX; i++) 
	{
		uv[i].u = (unsigned int)TypeBox[i] * CUBE_UV_VALUE;
		uv[i].v = 0.0f;
	}
	m_pD3DDevice->CreateVertexBuffer(sizeof(UV) * CUBE_MAX, 0, 0, D3DPOOL_MANAGED, &uvBuf, 0);

	copyBuf(sizeof(UV) * CUBE_MAX, uv, uvBuf);

	delete[] uv;



	// インデックスバッファ
	m_pD3DDevice->CreateIndexBuffer( sizeof( index ), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuf, 0 );
	void *p = 0;
	indexBuf->Lock( 0, 0, &p, 0 );
	memcpy( p, index, sizeof( index ) );
	indexBuf->Unlock();

	D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\GimixBox.jpg",&m_pD3DTex);

		// 頂点宣言作成
		D3DVERTEXELEMENT9 declElems[] = {
	{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},  // XYZ
	{0,  12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},  // UV
	{1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},  // WORLD
	{2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2}, // 個々のUV
	D3DDECL_END()
};
		m_pD3DDevice->CreateVertexDeclaration( declElems, &decl );
		
		// シェーダ作成
		ID3DXBuffer *errBuf = 0;
		D3DXCreateEffectFromFileA( m_pD3DDevice, "data\\SH\\instance.fx", 0, 0, 0, 0, &effect, &errBuf );
		if ( errBuf ) 
		{
			const char *errMsg = ( const char* )errBuf->GetBufferPointer();
			OutputDebugStringA( errMsg );
		}

	m_pos = D3DXVECTOR3(0.0f,1.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_fLength = 0;																//対角線の長さ
	m_fAngle = 0;																//対角線の角度


	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CGimmick::Uninit(void)
{
	if(m_pD3DVtxBuff != NULL)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff =NULL;
	}
	if(posBuffer != NULL)
	{
		posBuffer->Release();
		posBuffer =NULL;
	}
	if (uvBuf != NULL)
	{
		uvBuf->Release();
		uvBuf = NULL;
	}
	if(m_pD3DTex != NULL)
	{
		m_pD3DTex->Release();
		m_pD3DTex =NULL;
	}

	if(decl != NULL)
	{
		decl->Release();
		decl =NULL;
	}
	if(effect != NULL)
	{
		effect->Release();
		effect =NULL;
	}
	if(indexBuf != NULL)
	{
		indexBuf->Release();
		indexBuf =NULL;
	}
	CScene::Relese();
}
//=============================================================================
// 更新処理
//=============================================================================
void CGimmick::Update(void)
{
	SetVertexPolygon();
}
//=============================================================================
// 描画処理
//=============================================================================
void CGimmick::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ワイヤーフレーム描画

	D3DXMATRIX mtxScl,mtxRot,mtxTranslate,view,proj;
	if (g_pCameraGimmick != NULL)
	{
		CCamera *pol = g_pCameraGimmick;
		view = pol->GetCameraMtxView();
		proj = pol->GetCameraMtxProjection();
	}

	if(effect!=NULL && decl!=NULL)
	{

		UINT numPass=0;
		// NORMALBOX
		D3DXMatrixIdentity(&m_mtxWorld);															// 初期化
	
		m_pD3DDevice->SetStreamSourceFreq( 0, D3DSTREAMSOURCE_INDEXEDDATA  | (CUBE_MAX) );
		m_pD3DDevice->SetStreamSourceFreq( 1, D3DSTREAMSOURCE_INSTANCEDATA | 1 );
		m_pD3DDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
		

		m_pD3DDevice->SetVertexDeclaration( decl );
		m_pD3DDevice->SetStreamSource( 0, m_pD3DVtxBuff, 0, sizeof( Vtx ) );
		m_pD3DDevice->SetStreamSource( 1, posBuffer, 0, sizeof( WorldPos ) );
		m_pD3DDevice->SetStreamSource(2, uvBuf, 0, sizeof(UV));

		m_pD3DDevice->SetIndices( indexBuf );
		effect->SetMatrix("gWVPMatrix", &m_mtxWorld );
		effect->SetMatrix("gWVPVMatrix", &view );
		effect->SetMatrix("gWVPPMatrix", &proj );
		effect->SetTexture("Tex", m_pD3DTex);// テクスチャの設定
		effect->SetTechnique("tech");
	
		effect->Begin( &numPass, 0 );
		effect->BeginPass(0);
		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (CUBE_INDEXNUM_MAX-1), 0, CUBE_SURFACE_MAX);
		effect->EndPass();
		effect->End();

		
		// 後始末
		m_pD3DDevice->SetStreamSourceFreq( 0, 1 );
		m_pD3DDevice->SetStreamSourceFreq( 1, 1 );




		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング

	}

}
//=============================================================================
// 処理
//=============================================================================
void CGimmick::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CGimmick::GetPos(void)
{

	return m_pos;
}
D3DXVECTOR3 CGimmick::GetPosDest(void)
{

	return m_pos;
}
//=============================================================================
// 処理
//=============================================================================
void CGimmick::SetRot(D3DXVECTOR3 set_rot)
{
	m_rot = set_rot;
}
D3DXVECTOR3 CGimmick::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// 
//=============================================================================
void CGimmick::SetVertexPolygon(void)
{
}

WorldPos *CGimmick::GetPosObj(WorldPos *pOut)
{
	WorldPos *pV;

	if(type ==0)
	{
		if(posBuffer)
		{
			posBuffer->Lock(0,0,(void**)&pV,0);
			for(int i=0;i<CUBE_MAX;i++)
			{
				pOut[i] = pV[i];
			}

			posBuffer->Unlock();
		}
	}

	return &pOut[0];
}
TYPE_OBJ *CGimmick::GetTypeObj(TYPE_OBJ *pOut)
{
	for (int i = 0; i<CUBE_MAX; i++)
	{
		pOut[i] = TypeBox[i];
	}
	return &pOut[0];
}
void CGimmick::SetPosObj(D3DXVECTOR3 pOut, int no)
{
	WorldPos *pV;



		if(posBuffer)
		{
			posBuffer->Lock(0,0,(void**)&pV,0);

			pV[no].x = pOut.x;
			pV[no].y = pOut.y;
			pV[no].z = pOut.z;

			posBuffer->Unlock();
		}
	}
