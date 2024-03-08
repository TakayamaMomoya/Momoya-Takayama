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
#include "inputManager.h"
#include "manager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "cameraBehavior.h"
#include "renderer.h"
#include "sound.h"
#include "object3D.h"
#include "objectX.h"
#include "skybox.h"
#include "player.h"
#include "smoke.h"

//*****************************************************
// マクロ定義
//*****************************************************

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const D3DXVECTOR3 LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.27f, 150.0f, 0.0f);	// ロゴの位置
	const float LOGO_WIDTH = 875.0f * 0.35f;	// ロゴの幅
	const float LOGO_HEIGHT = 320.0f * 0.35f;	// ロゴの高さ
	const char* LOGO_PATH = "data\\TEXTURE\\UI\\logo000.png";	// ロゴのパス

	const D3DXVECTOR3 STATE_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f, 0.0f);	// スタート表示の位置
	const float START_WIDTH = 400.0f;	// スタート表示の幅
	const float START_HEIGHT = 100.0f;	// スタート表示の高さ
	const char* START_PATH = "data\\TEXTURE\\UI\\gamestart.png";	// スタート表示のパス
	const float SPEED_AFTER_EXPAND = 2.0f;	// 残像の膨らむ速度
	const float SPEED_START_AVOID = 0.03f;	// スタート表示の消える速度
	D3DXVECTOR2 SIZE_MENU = { 200.0f,60.0f };	//　メニューのサイズ
	const D3DXCOLOR COL_INITIAL_MENU = { 0.4f,0.4f,0.4f,1.0f };	// メニュー項目の初期色
	const D3DXCOLOR COL_CURRENT_MENU = { 1.0f,1.0f,1.0f,1.0f };	// メニュー項目の選択色
	const D3DXVECTOR3 POS_PLAYER = { -154.31f, 82.62f, 600.51f };	// プレイヤーモデルの位置
}

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{
	m_state = STATE_NONE;
	m_pLogo = nullptr;
	m_pMotion = nullptr;
	m_pBehavior = nullptr;
	m_fTImerSmoke = 0.0f;
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
	ChangeBehavior(new CTitleStart);

	// フォグをかけない
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(false);
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

	// カメラ位置の設定
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posV = { 45.38f,84.71f,270.10f };
	pInfoCamera->posR = { -454.28f,331.03f,878.09f };

	// 背景オブジェクトの生成
	CObjectX* pArsenal = CObjectX::Create();

	if (pArsenal != nullptr)
	{
		int nIdx = CModel::Load("data\\MODEL\\other\\arsenal.x");
		pArsenal->BindModel(nIdx);
	}

	// プレイヤーモデルの設置
	m_pMotion = CMotion::Create("data\\MOTION\\motionArms01.txt");

	if (m_pMotion != nullptr)
	{
		m_pMotion->SetPosition(POS_PLAYER);
		m_pMotion->SetMotion(CPlayer::MOTION::MOTION_NEUTRAL_TITLE);
		m_pMotion->InitPose(CPlayer::MOTION::MOTION_NEUTRAL_TITLE);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTitle::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pLogo);

	if (m_pBehavior != nullptr)
	{
		delete m_pBehavior;
		m_pBehavior = nullptr;
	}

	// オブジェクト全破棄
	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CTitle::Update(void)
{
	// シーンの更新
	CScene::Update();

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Update(this);
	}

	m_fTImerSmoke += CManager::GetDeltaTime();

	if (m_fTImerSmoke >= 0.3f)
	{
		D3DXVECTOR3 pos = POS_PLAYER;

		pos +=
		{
			(float)universal::RandRange(500, -500),
			0.0f,
			(float)universal::RandRange(500, -500),
		};

		CSmoke::Create(pos);

		m_fTImerSmoke = 0.0f;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CTitle::Draw(void)
{

}

//=====================================================
// ビヘイビアの変更
//=====================================================
void CTitle::ChangeBehavior(CTitleBehavior *pBehavior)
{
	if (m_pBehavior != nullptr)
	{
		delete m_pBehavior;
	}

	m_pBehavior = pBehavior;
}

//=====================================================================
// Behavior
//=====================================================================
//=====================================================
// 基底Behavior
//=====================================================
CTitleBehavior::CTitleBehavior()
{// コンストラクタ

}

CTitleBehavior::~CTitleBehavior()
{// デストラクタ

}

//=====================================================
// スタート表示状態
//=====================================================
CTitleStart::CTitleStart()
{// コンストラクタ
	m_pStart = nullptr;
	m_pAfter = nullptr;

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
}

CTitleStart::~CTitleStart()
{// デストラクタ
	Object::DeleteObject((CObject**)&m_pStart);
	Object::DeleteObject((CObject**)&m_pAfter);
}

void CTitleStart::Update(CTitle *pTitle)
{// 更新
	if (m_pAfter == nullptr)
	{
		CInputManager *pInput = CInputManager::GetInstance();

		if (pInput == nullptr)
			return;

		if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
		{
			// 残像の生成
			m_pAfter = CObject2D::Create(6);

			if (m_pAfter != nullptr)
			{
				m_pAfter->SetSize(START_WIDTH, START_HEIGHT);
				m_pAfter->SetPosition(STATE_POS);
				int nIdx = CTexture::GetInstance()->Regist(START_PATH);
				m_pAfter->SetIdxTexture(nIdx);
				m_pAfter->SetVtx();
			}

			// サウンド再生
			Sound::Play(CSound::LABEL_SE_START_GAME);
		}
	}
	else
	{
		// サイズの変更
		float fWidth = m_pAfter->GetWidth();
		float fHeight = m_pAfter->GetHeight();

		fWidth += SPEED_AFTER_EXPAND;
		fHeight += SPEED_AFTER_EXPAND;

		m_pAfter->SetSize(fWidth, fHeight);
		m_pAfter->SetVtx();

		// 色の変更
		D3DXCOLOR col = m_pAfter->GetCol();

		col.a -= SPEED_START_AVOID;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;

			pTitle->ChangeBehavior(new CTitleMenu);

			return;
		}

		if (m_pStart != nullptr)
		{
			m_pStart->SetCol(col);
		}

		m_pAfter->SetCol(col);
	}
}

//=====================================================
// スタート表示状態
//=====================================================
CTitleMenu::CTitleMenu()
{// コンストラクタ
	const char* aPath[MENU_MAX] =
	{// メニューのテクスチャパス
		"data\\TEXTURE\\UI\\menu00.png",
		"data\\TEXTURE\\UI\\menu01.png",
	};

	ZeroMemory(&m_apMenu[0], sizeof(m_apMenu));
	m_menu = MENU_GAME;
	m_pCursor = nullptr;

	for (int i = 0; i < MENU_MAX; i++)
	{
		int nCnt = i - 1;

		m_apMenu[i] = CObject2D::Create(6);

		if (m_apMenu[i] != nullptr)
		{
			D3DXVECTOR3 pos = SCRN_MID;
			pos.x = SIZE_MENU.x + 20.0f;
			pos.y += SIZE_MENU.y * i * 2 + 40.0f;

			m_apMenu[i]->SetPosition(pos);
			m_apMenu[i]->SetSize(SIZE_MENU.x, SIZE_MENU.y);
			m_apMenu[i]->SetCol(COL_INITIAL_MENU);
			m_apMenu[i]->SetVtx();

			int nIdx = CTexture::GetInstance()->Regist(aPath[i]);
			m_apMenu[i]->SetIdxTexture(nIdx);
		}
	}

	// カーソルの生成
	m_pCursor = CObject2D::Create(6);

	if (m_pCursor != nullptr)
	{
		m_pCursor->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		m_pCursor->SetVtx();

		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\cursorMenu.png");
		m_pCursor->SetIdxTexture(nIdx);
	}
}

CTitleMenu::~CTitleMenu()
{// デストラクタ
	Object::DeleteObject((CObject**)&m_apMenu, MENU_MAX);
}

void CTitleMenu::Update(CTitle *pTitle)
{// 更新
	// 入力
	Input();

	// カーソルの制御
	ManageCursor();

	// 色の管理
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (m_apMenu[i] != nullptr)
		{
			D3DXCOLOR colDest = COL_INITIAL_MENU;

			if (i == m_menu)
			{
				colDest = COL_CURRENT_MENU;
			}

			D3DXCOLOR col = m_apMenu[i]->GetCol();

			col += (colDest - col) * 0.3f;

			m_apMenu[i]->SetCol(col);
		}
	}
}

void CTitleMenu::Input(void)
{// 入力
	CInputManager *pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	// 項目選択
	if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_UP))
	{
		Sound::Play(CSound::LABEL_SE_PAUSE_ARROW);

		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);
	}
	else if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{
		Sound::Play(CSound::LABEL_SE_PAUSE_ARROW);

		m_menu = (MENU)((m_menu + 1) % MENU_MAX);
	}

	if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
	{// フェード処理
		Fade();
	}
}

void CTitleMenu::Fade(void)
{// 各種フェード
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pFade->GetState() != CFade::FADE_NONE)
		return;

	Sound::Play(CSound::LABEL_SE_START_GAME);

	switch (m_menu)
	{
	case CTitleMenu::MENU_GAME:

		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CTitleMenu::MENU_TRANING:

		pFade->SetFade(CScene::MODE_TUTORIAL);

		break;
	default:
		break;
	}
}

void CTitleMenu::ManageCursor(void)
{// カーソルの制御
	if (m_pCursor == nullptr)
		return;

	D3DXVECTOR3 pos = m_pCursor->GetPosition();
	D3DXVECTOR3 posDest;

	posDest = SCRN_MID;
	posDest.x = 20.0f;
	posDest.y += SIZE_MENU.y * m_menu * 2 + SIZE_MENU.y * 1.5f;

	pos += (posDest - pos) * 0.4f;

	m_pCursor->SetPosition(pos);
	m_pCursor->SetVtx();
}