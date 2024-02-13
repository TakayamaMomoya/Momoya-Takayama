//*****************************************************
//
// タイトル処理[title.cpp]
// Author:酒井南勝
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "title.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "manager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "renderer.h"
#include "sound.h"

#include "object3D.h"
#include "objectX.h"
#include "skybox.h"

//*****************************************************
// マクロ定義
//*****************************************************

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float FIELD_WIDTH = 10000.0f;		// フィールドの幅
	const float FIELD_HEIGHT = 10000.0f;	// フィールドの高さ

	const D3DXVECTOR3 LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 150.0f, 0.0f);	// ロゴの位置
	const float LOGO_WIDTH = 875.0f * 0.35f;	// ロゴの幅
	const float LOGO_HEIGHT = 320.0f * 0.35f;	// ロゴの高さ
	const char* LOGO_PATH = "data\\TEXTURE\\UI\\logo000.png";	// ロゴのパス

	const D3DXVECTOR3 STATE_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f, 0.0f);	// スタート表示の位置
	const float START_WIDTH = 200.0f;	// スタート表示の幅
	const float START_HEIGHT = 50.0f;	// スタート表示の高さ
	const float ADD_WIDTH = 16.0f;		// 幅の拡大化
	const float ADD_HEIGHT = 10.0f;		// 高さの拡大化  
	const char* START_PATH = "data\\TEXTURE\\UI\\gamestart.png";	// スタート表示のパス

	const int FADE_COUNT = 120;			// フェードまでの時間

	const float ALPHA_UPPER = 1.0f;			// α値の上限量
	const float ALPHA_LOWER = 0.25f;		// α値の下限量
	const float ALPHA_CHANGE = 0.1f;		// α値の変化量
	const float ALPHA_CHANGE_LOGO = 0.01f;	// α値の変化量
}

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{
	m_state = STATE_NONE;
	m_pLogo = nullptr;
	m_pLogoLate = nullptr;
	m_pStart = nullptr;
	m_nFadeCnt = 0;
	m_bIsAlphaChange = false;
	m_fSizeX = LOGO_WIDTH;
	m_fSizeY = LOGO_HEIGHT;
}

//=====================================================
// デストラクタ
//=====================================================
CTitle::~CTitle()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTitle::Init(void)
{
	// ロゴの生成
	m_pLogo = CObject2D::Create(7);

	if (m_pLogo != nullptr)
	{
		m_pLogo->SetSize(LOGO_WIDTH, LOGO_HEIGHT);
		m_pLogo->SetPosition(LOGO_POS);
		int nIdx = CTexture::GetInstance()->Regist(LOGO_PATH);
		m_pLogo->SetIdxTexture(nIdx);
		m_pLogo->SetVtx();
	}
	if (m_pLogo == nullptr)
	{
		return E_FAIL;
	}

	// スタート表示の生成
	m_pStart = CObject2D::Create(7);

	if (m_pStart != nullptr)
	{
		m_pStart->SetSize(START_WIDTH, START_HEIGHT);
		m_pStart->SetPosition(STATE_POS);
		int nIdx = CTexture::GetInstance()->Regist(START_PATH);
		m_pStart->SetIdxTexture(nIdx);
		m_pStart->SetVtx();
	}
	else if (m_pStart == nullptr)
	{
		return E_FAIL;
	}

	// スカイボックスの生成
	CSkybox* pSkyBox = CSkybox::Create();

	if (pSkyBox == nullptr)
	{
		return E_FAIL;
	}

	// 地面の生成
	CObject3D* pField = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pField != nullptr)
	{
		pField->SetSize(FIELD_WIDTH, FIELD_HEIGHT);
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\field00.jpg");
		pField->SetIdxTexture(nIdx);
		pField->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f));
		pField->SetVtx();
	}
	else if (pField == nullptr)
	{
		return E_FAIL;
	}

	// フォグをかける
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
	}
	else if (pRenderer == nullptr)
	{
		return E_FAIL;
	}

	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		// BGMの再生
		pSound->Play(pSound->LABEL_BGM_TITLE);
	}
	else if (pSound == nullptr)
	{
		return E_FAIL;
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTitle::Uninit(void)
{
	// オブジェクト全破棄
	CObject::ReleaseAll();

	// フォグをかける
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(false);
	}
}

//=====================================================
// 更新処理
//=====================================================
void CTitle::Update(void)
{
	// 情報取得
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	// シーンの更新
	CScene::Update();

	if (m_state == STATE_NONE)
	{
		if (pKeyboard != nullptr && pMouse != nullptr && pJoypad != nullptr)
		{
			if (pKeyboard->GetTrigger(DIK_RETURN) ||
				pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
				pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
			{// フェード開始

				// サウンドインスタンスの取得
				CSound* pSound = CSound::GetInstance();

				if (pSound != nullptr)
				{
					pSound->Play(pSound->LABEL_SE_START_GAME);
				}

				// フェード設定
				SetFadeIn();
			}
		}
	}
	else if (m_state == STATE_OUT)
	{
		// フェード更新処理
		UpdateFade();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CTitle::Draw(void)
{

}

//=====================================================
// スタート表示の管理
//=====================================================
void CTitle::ManageStart(void)
{
	// 色の情報取得
	D3DXCOLOR colStart = m_pStart->GetCol();
	D3DXCOLOR colLogo = m_pLogo->GetCol();
	D3DXCOLOR colLogoLate = m_pLogoLate->GetCol();

	if (m_pStart != nullptr && m_pLogo != nullptr)
	{
		if (m_bIsAlphaChange == false)
		{
			colStart.a -= ALPHA_CHANGE;
		}
		else if (m_bIsAlphaChange == true)
		{
			colStart.a += ALPHA_CHANGE;
		}

		if (colStart.a <= ALPHA_LOWER)
		{
			m_bIsAlphaChange = true;
		}
		else if (colStart.a >= ALPHA_UPPER)
		{
			m_bIsAlphaChange = false;
		}

		if (colLogo.a >= 0.0f)
		{
			// ロゴのα値を減少
			colLogo.a -= ALPHA_CHANGE_LOGO;
		}

		if (colLogoLate.a >= 0.0f)
		{
			// ロゴのα値を減少
			colLogoLate.a -= 0.06f;
		}

		// サイズの肥大化
		m_fSizeX += ADD_WIDTH;
		m_fSizeY += ADD_HEIGHT;

		// 設定処理
		m_pStart->SetCol(colStart);
		m_pStart->SetVtx();
		m_pLogo->SetCol(colLogo);
		m_pLogo->SetVtx();
		m_pLogoLate->SetSize(m_fSizeX, m_fSizeY);
		m_pLogoLate->SetCol(colLogoLate);
		m_pLogoLate->SetVtx();
	}
}

//=====================================================
// フェードの更新処理
//=====================================================
void CTitle::UpdateFade(void)
{
	// スタート表示の管理
	ManageStart();

	// フェードカウント進める
	m_nFadeCnt++;

	if (m_nFadeCnt == FADE_COUNT)
	{
		// フェードの取得
		CFade* pFade = CFade::GetInstance();

		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_GAME);
		}
	}
}

//=====================================================
// フェード設定
//=====================================================
void CTitle::SetFadeIn(void)
{
	// 情報取得
	CCamera* pCamera = CManager::GetCamera();
	
	if (m_pLogoLate == nullptr)
	{
		m_pLogoLate = CObject2D::Create(7);

		if (m_pLogoLate != nullptr)
		{
			m_pLogoLate->SetSize(LOGO_WIDTH, LOGO_HEIGHT);
			m_pLogoLate->SetPosition(LOGO_POS);
			int nIdx = CTexture::GetInstance()->Regist(LOGO_PATH);
			m_pLogoLate->SetIdxTexture(nIdx);
			m_pLogoLate->SetVtx();
		}
	}

	// フェードアウトに設定
	m_state = STATE_OUT;
}