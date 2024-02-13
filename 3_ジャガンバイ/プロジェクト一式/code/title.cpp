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
	const char* PLAYER_BODY_PATH[NUM_PLAYER] =
	{// 敵の体のパス
		"data\\MOTION\\motionTitle01.txt",
		"data\\MOTION\\motionTitle02.txt",
		"data\\MOTION\\motionTitle03.txt",
		"data\\MOTION\\motionTitle04.txt",
	};
	const D3DXVECTOR3 PLAYER_POS[NUM_PLAYER] =
	{// プレイヤーの位置
		D3DXVECTOR3(   0.0f, 0.0f, -150.0f),
		D3DXVECTOR3(-125.0f, 0.0f, -100.0f),
		D3DXVECTOR3( 100.0f, 0.0f,  -50.0f),
		D3DXVECTOR3(-75.0f, 0.0f,  100.0f),
	};
	const D3DXVECTOR3 PLAYER_ROT[NUM_PLAYER] =
	{// プレイヤーの向き
		D3DXVECTOR3(0.0f, D3DX_PI *  0.0f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * -0.25f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI *  0.25f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * -0.10f, 0.0f),
	};
	const D3DXVECTOR3 PLAYER_ESC_MOVE = D3DXVECTOR3(0.0f, 0.0f, 8.0f);		// プレイヤーの逃げるときの移動量 
	const D3DXVECTOR3 PLAYER_ESC_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// プレイヤーの逃げるときの向き

	const char* ENEMY_BODY_PATH[ENEMY::NUM_ENEMY] =
	{// 敵の体のパス
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
	};
	const D3DXVECTOR3 ENEMY_POS[ENEMY::NUM_ENEMY] =
	{// 敵の位置
		D3DXVECTOR3(-250.0f, 0.0f, -600.0f),
		D3DXVECTOR3(-150.0f,  0.0f, -450.0f),
		D3DXVECTOR3(-90.0f,  0.0f, -850.0f),
		D3DXVECTOR3(-30.0f, 0.0f, -600.0f),
		D3DXVECTOR3( 40.0f, 0.0f, -800.0f),
		D3DXVECTOR3( 80.0f,  0.0f, -950.0f),
		D3DXVECTOR3( 120.0f,  0.0f, -750.0f),
		D3DXVECTOR3( 250.0f, 0.0f, -500.0f),
	};
	const D3DXVECTOR3 ENEMY_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// 敵の向き
	const D3DXVECTOR3 ENEMY_MOVE = D3DXVECTOR3(0.0f, 0.0f, 7.5f);	// 敵の移動量

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

	const D3DXVECTOR3 CAMERA_POSV[CTitle::CAMERA_MAX] =
	{// 視点カメラの位置
		D3DXVECTOR3(0.0f, 75.0f, -400.0f),
		D3DXVECTOR3(75.0f, 75.0f, -250.0f),
		D3DXVECTOR3(-30.0f, 60.0f, -175.0f),
		D3DXVECTOR3(-30.0f, 60.0f, -105.0f),
		D3DXVECTOR3(0.0f, 30.0f, 0.0f),
	};
	const D3DXVECTOR3 CAMERA_POSR[CTitle::CAMERA_MAX] =
	{// 注視点カメラの位置
		D3DXVECTOR3(0.0f, 25.0f, 0.0f),
		D3DXVECTOR3(0.0f, 35.0f, 0.0f),
		D3DXVECTOR3(0.0f, 25.0f, 0.0f),
		D3DXVECTOR3(0.0f, 35.0f, 0.0f),
		D3DXVECTOR3(0.0f, 35.0f, 0.0f),
	};
	const D3DXVECTOR3 CAMERA_MOVE[CTitle::CAMERA_MAX] =
	{// カメラの移動量
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	};
	const int CAMERA_SWITCH_COUNT[CTitle::CAMERA_MAX] =
	{// カメラ切り替えのカウント
		720,
		360,
		360,
		360,
		360,
	};

	const D3DXVECTOR3 CAMERA_ESC_POS_V = D3DXVECTOR3(0.0f, 200.0f, -800.0f);
	const D3DXVECTOR3 CAMERA_ESC_POS_R = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	const D3DXVECTOR3 CAMERA_ESC_MOVE = D3DXVECTOR3(0.0f, 0.0f, 7.0f);
}

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{
	m_state = STATE_NONE;
	ZeroMemory(&m_cameraInfo, sizeof(m_cameraInfo));
	m_pLogo = nullptr;
	m_pLogoLate = nullptr;
	m_pStart = nullptr;
	ZeroMemory(&m_apModelPlayer[0], sizeof(m_apModelPlayer));
	ZeroMemory(&m_apModelEnemy[0], sizeof(m_apModelEnemy));
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
	// 情報取得
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		// カメラの設定
		pCamera->SetTitle(
			CAMERA_POSV[m_cameraInfo.num],
			CAMERA_POSR[m_cameraInfo.num]);
	}
	else if (pCamera == nullptr)
	{
		return E_FAIL;
	}

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
	}
	else if (pField == nullptr)
	{
		return E_FAIL;
	}

	// タイトルモデルの生成
	CObjectX* pTitleModel = CObjectX::Create();

	if (pTitleModel != nullptr)
	{
		int nIdx = CModel::Load("data\\MODEL\\title\\title_model.x");
		pTitleModel->BindModel(nIdx);
	}
	else if (pTitleModel == nullptr)
	{
		return E_FAIL;
	}

	// キャラクターの生成処理
	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		m_apModelPlayer[nCount] = CMotion::Create((char*)PLAYER_BODY_PATH[nCount]);

		if (m_apModelPlayer[nCount] != nullptr)
		{
			m_apModelPlayer[nCount]->SetPosition(PLAYER_POS[nCount]);
			m_apModelPlayer[nCount]->SetRot(PLAYER_ROT[nCount]);
			m_apModelPlayer[nCount]->SetMotion(TITLE_MOTION_PLAYER_NEUTRAL);
		}
		else if (m_apModelPlayer[nCount] == nullptr)
		{
			return E_FAIL;
		}
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

	CCamera* pCamera = CManager::GetCamera();

	// シーンの更新
	CScene::Update();

	if (pCamera != nullptr)
	{
		// カメラの更新
		UpdateCamera();
	}

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

	// プレイヤーの設定処理
	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (m_apModelPlayer[nCount] != nullptr)
		{
			D3DXVECTOR3 posPlayer = m_apModelPlayer[nCount]->GetPosition();
			m_apModelPlayer[nCount]->SetPosition(posPlayer + PLAYER_ESC_MOVE);
			m_apModelPlayer[nCount]->SetRot(PLAYER_ESC_ROT);
		}
	}

	// 敵の設定処理
	for (int nCount = 0; nCount < ENEMY::NUM_ENEMY; nCount++)
	{
		if (m_apModelEnemy[nCount] != nullptr)
		{
			D3DXVECTOR3 posEnemy = m_apModelEnemy[nCount]->GetPosition();
			m_apModelEnemy[nCount]->SetPosition(posEnemy + ENEMY_MOVE);
		}
	}

	if (m_nFadeCnt == FADE_COUNT)
	{
		// フェードの取得
		CFade* pFade = CFade::GetInstance();

		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_SELECT);
		}
	}
}

//=====================================================
// カメラの更新処理
//=====================================================
void CTitle::UpdateCamera(void)
{
	CCamera* pCamera = CManager::GetCamera();

	if (m_state == STATE_NONE)
	{
		if (pCamera != nullptr)
		{
			// タイトルのカメラ更新
			pCamera->UpdateTitle(CAMERA_MOVE[m_cameraInfo.num]);
		}
	}
	else if (m_state == STATE_OUT)
	{
		if (pCamera != nullptr)
		{
			// タイトルの逃げるときのカメラ更新
			pCamera->UpdateTitleEsc(CAMERA_ESC_MOVE);
		}
	}

	if (m_cameraInfo.nCount >= CAMERA_SWITCH_COUNT[m_cameraInfo.num])
	{
		m_cameraInfo.nCount = 0;

		// カメラ番号を進める
		m_cameraInfo.num = (CAMERA)(m_cameraInfo.num + 1);

		if (m_cameraInfo.num >= CAMERA_MAX)
		{
			// カメラ番号初期化
			m_cameraInfo.num = (CAMERA)0;
		}

		D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (m_cameraInfo.num >= CAMERA_PLAYER_ONE &&
			m_cameraInfo.num <= CAMERA_PLAYER_FOUR)
		{
			int nPlayerNum = m_cameraInfo.num + ((CAMERA_PLAYER_ONE - 1) - 1);

			if (m_apModelPlayer[nPlayerNum] != nullptr)
			{
				// プレイヤーの位置を取得
				posPlayer = m_apModelPlayer[nPlayerNum]->GetPosition();	
			}
		}

		if (pCamera != nullptr)
		{
			if (m_cameraInfo.num <= CAMERA_MAX)
			{
				// カメラの目的値設定
				pCamera->SetTitleDest(
					CAMERA_POSV[m_cameraInfo.num],
					posPlayer + CAMERA_POSR[m_cameraInfo.num]);
			}
		}
	}
	else
	{
		m_cameraInfo.nCount++;
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

	if (pCamera != nullptr)
	{
		// 逃げる時のカメラの目的値設定
		pCamera->SetTitleDest(
			CAMERA_ESC_POS_V, 
			CAMERA_ESC_POS_R);
	}

	// フェードアウトに設定
	m_state = STATE_OUT;

	// プレイヤーのモーション設定処理（移動）
	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (m_apModelPlayer[nCount] != nullptr)
		{
			m_apModelPlayer[nCount]->SetMotion(TITLE_MOTION_PLAYER_MOVE);
		}
	}

	// 敵の生成処理
	for (int nCount = 0; nCount < ENEMY::NUM_ENEMY; nCount++)
	{
		m_apModelEnemy[nCount] = CMotion::Create((char*)ENEMY_BODY_PATH[nCount]);

		if (m_apModelEnemy[nCount] != nullptr)
		{
			m_apModelEnemy[nCount]->SetPosition(ENEMY_POS[nCount]);
			m_apModelEnemy[nCount]->SetRot(ENEMY_ROT);
			m_apModelEnemy[nCount]->SetMotion(1);
		}
	}
}