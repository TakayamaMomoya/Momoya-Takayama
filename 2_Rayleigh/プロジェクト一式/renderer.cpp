//*****************************************************
//
// レンダラーの処理[renderer.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "object.h"
#include <stdio.h>
#include "collision.h"
#include "fade.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
bool CRenderer::m_bFog = false;	// フォグをかけるかどうか
CRenderer *CRenderer::m_pRenderer = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CRenderer::CRenderer()
{
	// 変数のクリア
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CRenderer::~CRenderer()
{

}

//=====================================================
// 生成処理
//=====================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;

		if (m_pRenderer != nullptr)
		{
			m_pRenderer->Init(hWnd, bWindow);
		}
	}

	return m_pRenderer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;						// ディスプレイモードの設定
	D3DPRESENT_PARAMETERS d3dpp;						//ディスプレイモードの設定

	// Direct3Dオブジェクト生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメーターの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// パラメーターのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面の幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// 高さ
	d3dpp.BackBufferFormat = d3ddm.Format;							// 形式
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// 映像信号に同期
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファを8bitで使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	
	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプステートの設定
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_MINFILTER,
		D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_MAGFILTER,
		D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_ADDRESSU,
		D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_ADDRESSV,
		D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAOP,
		D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG1,
		D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG2,
		D3DTA_CURRENT);

	// オブジェクトのリリース処理
	CObject::ReleaseAll();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRenderer::Uninit(void)
{
	// オブジェクトのリリース
	CObject::ReleaseAll();

	if (m_pD3DDevice != nullptr)
	{// Direct3Dデバイス破棄
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	if (m_pD3D != nullptr)
	{// Direct3Dオブジェクトの破棄
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CRenderer::Update(void)
{
	// オブジェクトの更新
	CObject::UpdateAll();
}

//=====================================================
// 描画処理
//=====================================================
void CRenderer::Draw(void)
{
	// フェード取得
	CFade *pFade = CFade::GetInstance();

	// 画面クリア
	m_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//アルファテストの有効化
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	float fStart = 500;
	float fEnd = 15000;

	// フォグを有効化
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, m_bFog);

	// フォグの色を設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(0.53f, 0.26f, 0.10f, 1.00f));

	// フォグの状態を設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	// フォグの発生範囲を設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fStart));
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fEnd));

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画処理
		// FPS表示
		DrawFPS();
		
		// オブジェクトの描画
		CObject::DrawAll();

		if (pFade != nullptr)
		{// フェード描画
			pFade->Draw();
		}

		CDebugProc::GetInstance()->Draw();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// アルファテストの無効化
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// バック・フロントバッファを入れ替える
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//==================================================================================================
//デバッグ表示処理
//==================================================================================================
void CRenderer::DrawFPS(void)
{
	//文字列に代入
	CDebugProc::GetInstance()->Print("FPS:%d\n", GetFPS());
	CDebugProc::GetInstance()->Print("オブジェクト総数:[%d]\n", CObject::GetNumAll());
	CDebugProc::GetInstance()->Print("保存スコア:[%d]\n", CManager::GetScore());
}