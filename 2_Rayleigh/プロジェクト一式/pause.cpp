//*****************************************************
//
// ポーズの処理[pause.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "pause.h"
#include "manager.h"
#include "object2D.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MENU_WIDTH	(200.0f)	// 項目の幅
#define MENU_HEIGHT	(50.0f)	// 項目の高さ
#define MOVE_FACT	(0.15f)	// 移動速度
#define LINE_ARRIVAL	(90.0f)	// 到着したとされるしきい値
#define LINE_UNINIT	(3.0f)	// 終了するまでのしきい値
#define SPEED_FADE	(0.03f)	// 背景のフェード速度
#define ALPHA_BG	(0.5f)	// 背景の不透明度

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPause *CPause::m_pPause = nullptr;	// 自身のポインタ

//====================================================
// コンストラクタ
//====================================================
CPause::CPause()
{
	m_menu = MENU_RESUME;
	m_state = STATE_NONE;
	ZeroMemory(&m_apMenu[0], sizeof(m_apMenu));
	ZeroMemory(&m_aPosDest[0], sizeof(D3DXVECTOR3) * MENU_MAX);
}

//====================================================
// デストラクタ
//====================================================
CPause::~CPause()
{

}

//====================================================
// 生成処理
//====================================================
CPause *CPause::Create(void)
{
	if (m_pPause == nullptr)
	{
		m_pPause = new CPause;

		if (m_pPause != nullptr)
		{
			m_pPause->Init();
		}
	}

	return m_pPause;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CPause::Init(void)
{
	// ゲームを停止する
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		pGame->EnableStop(true);
	}

	// 背景の生成
	m_pBg = CObject2D::Create(7);

	if (m_pBg != nullptr)
	{
		m_pBg->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f));

		m_pBg->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		m_pBg->SetVtx();
	}

	char *pPath[MENU_MAX] = 
	{// メニュー項目のパス
		"data\\TEXTURE\\UI\\menu_resume.png",
		"data\\TEXTURE\\UI\\menu_retry.png",
		"data\\TEXTURE\\UI\\menu_return.png",
		"data\\TEXTURE\\UI\\menu_quit.png",
	};

	int nIdxTexture;

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{// メニュー項目のポリゴンを生成
		if (m_apMenu[nCntMenu] == nullptr)
		{
			m_apMenu[nCntMenu] = CObject2D::Create(7);

			if (m_apMenu[nCntMenu] != nullptr)
			{
				// ポリゴンの設定
				m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(-MENU_WIDTH, SCREEN_HEIGHT * 0.1f + MENU_HEIGHT * nCntMenu * 2, 0.0f));
				m_aPosDest[nCntMenu] = m_apMenu[nCntMenu]->GetPosition();

				m_apMenu[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);

				m_apMenu[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

				// テクスチャの設定
				nIdxTexture = CTexture::GetInstance()->Regist(pPath[nCntMenu]);

				m_apMenu[nCntMenu]->SetIdxTexture(nIdxTexture);

				m_apMenu[nCntMenu]->SetVtx();
			}
		}
	}

	m_state = STATE_IN;

	m_aPosDest[0].x = MENU_WIDTH;

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CPause::Uninit(void)
{
	// ゲームを再開する
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		pGame->EnableStop(false);
	}

	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{// メニュー項目の破棄
		if (m_apMenu[nCnt] != nullptr)
		{
			m_apMenu[nCnt]->Uninit();

			m_apMenu[nCnt] = nullptr;
		}
	}

	m_pPause = nullptr;

	Release();
}

//====================================================
// 更新処理
//====================================================
void CPause::Update(void)
{
	// 状態管理
	ManageState();
}

//====================================================
// 状態管理
//====================================================
void CPause::ManageState(void)
{
	if (m_state != STATE_OUT)
	{
		Input();
	}

	// 背景の管理
	ManageBg();

	// 終了フラグ用
	int nEnd = 0;

	// ポリゴンを目標位置に向かわせる
	for (int i = 0;i < MENU_MAX;i++)
	{
		if (m_apMenu[i] != nullptr)
		{
			D3DXVECTOR3 pos = m_apMenu[i]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[i] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;

			vecDiff += pos;

			m_apMenu[i]->SetPosition(vecDiff);

			m_apMenu[i]->SetVtx();

			float fDiff = m_aPosDest[i].x - vecDiff.x;

			if (fDiffOld * fDiffOld >= LINE_ARRIVAL * LINE_ARRIVAL &&
				fDiff * fDiff < LINE_ARRIVAL * LINE_ARRIVAL &&
				i < MENU_MAX - 1)
			{// 差分がしきい値より下になったら下のものを動かす
				if (m_state == STATE_IN)
				{
					m_aPosDest[i + 1].x = MENU_WIDTH;
				}
				else if (m_state == STATE_OUT)
				{
					m_aPosDest[i + 1].x = -MENU_WIDTH;
				}
			}

			if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
				m_state == STATE_OUT)
			{// 終了のライン
				nEnd++;
			}
		}
	}

	if (nEnd == MENU_MAX &&
		m_state == STATE_OUT)
	{
		Uninit();
	}
}

//====================================================
// 背景の管理
//====================================================
void CPause::ManageBg(void)
{
	if (m_pBg == nullptr)
	{
		return;
	}

	D3DXCOLOR col = m_pBg->GetCol();

	switch (m_state)
	{
	case CPause::STATE_IN:

		col.a += SPEED_FADE;

		if (col.a >= ALPHA_BG)
		{
			col.a = ALPHA_BG;
		}

		break;
	case CPause::STATE_OUT:

		col.a -= SPEED_FADE;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;
		}

		break;
	default:
		break;
	}

	m_pBg->SetCol(col);
}

//====================================================
// 操作処理
//====================================================
void CPause::Input(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}
	else
	{
		if (pFade->GetState() != CFade::FADE_NONE)
		{
			return;
		}
	}

	if (pInputManager == nullptr)
	{
		return;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE) || 
		pInputManager->GetTrigger(CInputManager::BUTTON_BACK))
	{// ポーズ解除、以降の操作を受け付けない
		m_state = STATE_OUT;
		m_aPosDest[0].x = -MENU_WIDTH;

		return;
	}

	if (m_apMenu[m_menu] != nullptr)
	{// 以前に選択してた項目を非選択色にする
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));
	}

	// 項目切り替え
	if (pInputManager->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{
		m_menu = (MENU)((m_menu + 1) % MENU_MAX);
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_AXIS_UP))
	{
		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);
	}

	if (m_apMenu[m_menu] != nullptr)
	{// 選択している項目の色変更
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
	{// 選択項目にフェードする
		Fade(m_menu);
	}
}

//====================================================
// フェードする処理
//====================================================
void CPause::Fade(MENU menu)
{
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}

	switch (menu)
	{
	case CPause::MENU_RESUME:

		m_state = STATE_OUT;
		m_aPosDest[0].x = -MENU_WIDTH;

		break;
	case CPause::MENU_RESTART:

		CGame::SetProgress(0);
		CGame::SetState(CGame::STATE_END);
		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CPause::MENU_CHECKPOINT:

		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CPause::MENU_QUIT:

		pFade->SetFade(CScene::MODE_TITLE);

		break;
	default:
		break;
	}
}

//====================================================
// 描画処理
//====================================================
void CPause::Draw(void)
{

}