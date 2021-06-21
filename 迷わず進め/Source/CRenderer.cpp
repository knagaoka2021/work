//=============================================================================
//
//  [CRenderer.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CScene.h"
#include "CSceneBillboard.h"
#include "CDebugproc.h"
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

//=============================================================================
// クリエイト
//=============================================================================
CRenderer::CRenderer(void)
{
	m_nCountScene = 0;
}
CRenderer::~CRenderer(void)
{
}
HRESULT CRenderer::Init(HWND hWnd , BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	if( m_pD3D == NULL ){
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory( &d3dpp, sizeof(d3dpp) );						// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.BackBufferFormat           = D3DFMT_R5G6B5;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED), 
									&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &m_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}



	m_pD3D->Release();

	//--------------レンダーステートパラメータの設定--------------------//


	// レンダーステートパラメータの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}
void CRenderer::Uninit(void)
{



	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice =NULL;
	}

}
void CRenderer::Update(void)
{	

	CScene::UpdateAll();
}
void CRenderer::Draw(void)
{

	m_pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),D3DCOLOR_ARGB(255,0,0,0),1.0f,0);

	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{

		CScene::DrawAll(GetDevice());


// デバッグモード時　デバックプロシージャー
#ifdef _DEBUG
		D3DXVECTOR3 debugPlayerPos;
		//debugPlayerPos = DebugPlayerPos();
		CDebugproc::DrawDebugProc(0,0);
		CDebugproc::PrintDebugProc("デバッグモード：\n");
		CDebugproc::PrintDebugProc("長岡和真\n");
#endif
		//CDebugproc::DrawDebugProc(350,0);
		//CDebugproc::PrintDebugProc("S:後移動\n");
		//CDebugproc::PrintDebugProc("L or K:左右回転\n");
		m_pD3DDevice->EndScene();
	}


	m_pD3DDevice->Present(NULL,NULL,NULL,NULL);
}
void CRenderer::AddScene(CScene* pScene)
{

	m_apScenes[m_nCountScene] = pScene;


	m_nCountScene++;

	if(SCENE_MAX < m_nCountScene)
	{
		m_nCountScene =  SCENE_MAX;
	}
}



//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

void CRenderer::DrawFPS(void)
{
}

//D3DXMATRIX GetMtxView(void)
//{
//	return g_FieldmtxView;
//}
//float GetView(void)
//{
//	return g_fLengZ;
//}

//D3DXVECTOR3 CRenderer::GetRotCamera(void)
//{
//	return g_rotCamera;
//
//}
//D3DXMATRIX CRenderer::SetMtxView(void)
//{
//	return g_FieldmtxView;
//}
