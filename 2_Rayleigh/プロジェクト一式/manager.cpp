//*****************************************************
//
// マネージャー処理[manager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "bg.h"
#include "debugproc.h"
#include "effect2D.h"
#include "effect3D.h"
#include "sound.h"
#include "score.h"
#include "timer.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectX.h"
#include "billboard.h"
#include "model.h"
#include "texture.h"
#include "universal.h"
#include "particle.h"
#include "fade.h"
#include "block.h"
#include "inputManager.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CCamera *CManager::m_pCamera = nullptr;	// カメラのポインタ
CLight *CManager::m_pLight = nullptr;	// ライトのポインタ
CScene *CManager::m_pScene = nullptr;	// シーンへのポインタ
CScene::MODE CManager::m_mode = CScene::MODE_TITLE;	// 現在のモード
int CManager::m_nScore = 0;	// スコア保存用

//=====================================================
// コンストラクタ
//=====================================================
CManager::CManager()
{
	m_pScene = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CManager::~CManager()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダラーの生成
	CRenderer::Create(hWnd, bWindow);

	// 入力マネージャー生成
	CInputManager::Create(hInstance, hWnd);

	// デバッグ表示の生成
	CDebugProc::Create();

	// サウンド生成
	CSound::Create(hWnd);

	if (m_pCamera == nullptr)
	{// カメラ生成
		m_pCamera = new CCamera;

		if (m_pCamera != nullptr)
		{
			// 初期化
			if (FAILED(m_pCamera->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pLight == nullptr)
	{// ライト生成
		m_pLight = new CLight;

		if (m_pLight != nullptr)
		{
			// 初期化
			if (FAILED(m_pLight->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	// テクスチャ管理の生成
	CTexture::Create();

	// フェードの生成
	CFade::Create();

	// 汎用処理生成
	CUniversal::Create();

	// パーティクルの読込
	CParticle::Load();

	// ブロック番号読込
	CBlock::LoadModel();

	SetMode(m_mode);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CManager::Uninit(void)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
	}

	// フェード終了
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->Uninit();
	}

	// パーティクル情報破棄
	CParticle::Unload();

	// ブロック情報削除
	CBlock::DeleteAll();

	// ブロック番号削除
	CBlock::DeleteIdx();

	// レンダラー終了
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->Uninit();
	}

	// 汎用処理終了
	CUniversal *pUniversal = CUniversal::GetInstance();

	if (pUniversal != nullptr)
	{
		pUniversal->Uninit();
	}

	// 入力マネージャー終了
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		pInputManager->Uninit();
	}

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// サウンドの終了・破棄
		pSound->Uninit();
		pSound = nullptr;
	}

	// デバッグプロシージャの終了
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc != nullptr)
	{
		pDebugProc->Uninit();
	}

	if (m_pLight != nullptr)
	{// ライトの終了・破棄
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pCamera != nullptr)
	{// カメラの終了・破棄
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// テクスチャの終了・破棄
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture != nullptr)
	{
		pTexture->Unload();

		delete pTexture;
		pTexture = nullptr;
	}

	// モデル破棄
	CModel::Unload();
}

//=====================================================
// 更新処理
//=====================================================
void CManager::Update(void)
{
	// フェード更新
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->Update();
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	// 入力マネージャー更新
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		pInputManager->Update();
	}

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// サウンドの更新
		pSound->Update();
	}

	// デバッグプロシージャの更新
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc != nullptr)
	{
		pDebugProc->Update();
	}

	if (m_pLight != nullptr)
	{
		// ライトの更新
		m_pLight->Update();
	}
	
	if (CInputKeyboard::GetInstance() != nullptr)
	{
		if (CInputKeyboard::GetInstance()->GetTrigger(DIK_F3))
		{// 強制リセット
			CManager::SetMode(CScene::MODE_TITLE);
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CManager::Draw(void)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}

	// レンダラー描画
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->Draw();
	}
}

//=====================================================
// モード設定
//=====================================================
void CManager::SetMode(CScene::MODE mode)
{
	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// 音の停止
		pSound->Stop();
	}

	if (m_pCamera != nullptr)
	{
		m_pCamera->Init();
	}

	// シーンを破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = nullptr;
	}

	// シーン生成
	m_pScene = CScene::Create(mode);

	// モード設定
	m_mode = mode;
}