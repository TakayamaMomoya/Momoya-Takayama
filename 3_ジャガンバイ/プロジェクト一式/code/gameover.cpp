//=========================================================
//
// ゲームオーバー処理 [gameover.cpp]
// Author = 阿部翔大郎
//
//=========================================================

//===============================================
// インクルード
//===============================================
#include "gameover.h"
#include "enemyManager.h"
#include "playerManager.h"
#include "particle.h"
#include "scene.h"
#include "fade.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "camera.h"
#include "manager.h"
#include "goalTimer.h"
#include "UIManager.h"
#include "goal.h"
#include "texture.h"
#include "game.h"
#include "gimmickManager.h"
#include "block.h"

//===============================================
// 定数定義
//===============================================
namespace
{
	// プレイヤーの体のパス
	const char* PLAYER_BODY_PATH[NUM_PLAYER] =
	{
		"data\\MOTION\\motionPotatoman01.txt",
		"data\\MOTION\\motionPotatoman02.txt",
		"data\\MOTION\\motionPotatoman03.txt",
		"data\\MOTION\\motionPotatoman04.txt"
	};

	// プレイヤーの位置
	const D3DXVECTOR3 PLAYER_POS[NUM_PLAYER] =
	{
		D3DXVECTOR3(-35.0f, 0.0f, -215.0f),
		D3DXVECTOR3(35.0f, 0.0f, -215.0f),
		D3DXVECTOR3(90.0f, 0.0f, -215.0f),
		D3DXVECTOR3(-90.0f, 0.0f, -215.0f)
	};

	// プレイヤーの向き
	const D3DXVECTOR3 PLAYER_ROT[NUM_PLAYER] =
	{
		D3DXVECTOR3(0.0f, D3DX_PI * -0.10f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.10f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.15f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * -0.15f, 0.0f)
	};

	// 敵の体のパス
	const char* ENEMY_BODY_PATH = "data\\MOTION\\motionEnemy00.txt";

	// 敵の位置
	const D3DXVECTOR3 ENEMY_POS[NUM_ENEMY] =
	{
		D3DXVECTOR3(-140.0f, 0.0f, -160.0f),
		D3DXVECTOR3(-70.0f, 0.0f, -130.0f),
		D3DXVECTOR3(0.0f, 0.0f, -100.0f),
		D3DXVECTOR3(70.0f, 0.0f, -130.0f),
		D3DXVECTOR3(140.0f, 0.0f, -160.0f)
	};

	// 敵の向き
	const D3DXVECTOR3 ENEMY_ROT[NUM_ENEMY] =
	{
		D3DXVECTOR3(0.0f, D3DX_PI * -0.2f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * -0.1f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.1f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.2f, 0.0f)
	};

	// 視点カメラの位置
	const D3DXVECTOR3 CAMERA_POSV = D3DXVECTOR3(0.0f, 110.0f, -340.0f);

	// 注視点カメラの位置
	const D3DXVECTOR3 CAMERA_POSR = D3DXVECTOR3(0.0f, -50.0f, 50.0f);

	// ゲームオーバーのテクスチャのパス
	const char* LOGO_PATH = "data\\TEXTURE\\UI\\gameover.png";

	// ゲームオーバーの横幅
	const float LOGO_WIDTH = 480.0f;

	// ゲームオーバーの縦幅
	const float LOGO_HEIGHT = 270.0f;

	// ゲームオーバーの位置
	const D3DXVECTOR3 LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f, 0.0f);

	// コンティニューのテクスチャのパス
	const char* CONTINY_PATH = "data\\TEXTURE\\UI\\pressAbutton.png";

	// コンティニューの横幅
	const float CONTINY_WIDTH = 322.0f;

	// コンティニューの縦幅
	const float CONTINY_HEIGHT = 30.0f;

	// コンティニューの位置
	const D3DXVECTOR3 CONTINY_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 250.0f, 0.0f);

	// コンティニューの表示タイミング
	const float CONTINY_DISPTIMING = 0.7f;
}

//===============================================
// 静的メンバ変数宣言
//===============================================
CGameover* CGameover::m_pGameover = nullptr;	// 自身のポインタ

//===============================================
// コンストラクタ
//===============================================
CGameover::CGameover()
{
	// 値のクリア
	ZeroMemory(&m_apModelPlayer[0], sizeof(m_apModelPlayer));
	ZeroMemory(&m_apModelEnemy[0], sizeof(m_apModelEnemy));
	m_pBlack = nullptr;
	m_pLogo = nullptr;
	m_pContiny = nullptr;
	m_bDeathPlayer[0] = {};
	m_bFlash = false;
	m_fFadeCounter = 0.0f;
	m_fFlashCounter = 0.0f;
}

//===============================================
// デストラクタ
//===============================================
CGameover::~CGameover()
{
	
}

//===============================================
// 生成処理
//===============================================
CGameover *CGameover::Create(int nPriority)
{
	if (m_pGameover == nullptr)
	{
		// ゲームオーバーの生成
		m_pGameover = new CGameover;

		if (m_pGameover != nullptr)
		{
			// 初期化処理
			m_pGameover->Init();
		}
		else if (m_pGameover == nullptr)
		{
			return nullptr;
		}
	}

	return m_pGameover;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CGameover::Init()
{
	// 値の初期化
	m_pBlack = nullptr;
	m_pLogo = nullptr;
	m_pContiny = nullptr;
	m_bDeathPlayer[0] = {};
	m_bFlash = false;
	m_fFadeCounter = 0.0f;
	m_fFlashCounter = 0.0f;

	// インスタンスを取得
	CEnemyManager* pEnemyManager = CEnemyManager::GetInstance();
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CGame* pGame = CGame::GetInstance();
	CUIManager* pUIManager = CUIManager::GetInstance();
	CGoal* pGoal = CGoal::GetInstance();
	CGimmickManager* pGimmickManager = CGimmickManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		// 先頭オブジェクトを代入
		CEnemy* pEnemy = pEnemyManager->GetHead();

		// 現在出現している敵を削除
		while (pEnemy != nullptr)
		{
			// 次のアドレスを保存
			CEnemy* pEnemyNext = pEnemy->GetNext();

			// 終了処理
			pEnemy->Uninit();

			// 次のアドレスを代入
			pEnemy = pEnemyNext;
		}

		// ゲームオーバー状態にして敵のスポーンを停止する
		if (pGame != nullptr)
		{
			pGame->SetState(CGame::STATE_GAMEOVER);
		}

		// 敵を生成
		for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
		{
			m_apModelEnemy[nCntEnemy] = CMotion::Create((char*)ENEMY_BODY_PATH);

			// 敵の情報の設定
			m_apModelEnemy[nCntEnemy]->SetPosition(ENEMY_POS[nCntEnemy]);	// 位置
			m_apModelEnemy[nCntEnemy]->SetRot(ENEMY_ROT[nCntEnemy]);		// 向き
			m_apModelEnemy[nCntEnemy]->SetMotion(0);						// モーション
		}
	}
	else if (pEnemyManager == nullptr)
	{
		return E_FAIL;
	}

	// ギミックの削除
	if (pGimmickManager != nullptr)
	{
		pGimmickManager->DeleteAllGimmick();
	}
	else if (pGimmickManager == nullptr)
	{
		return E_FAIL;
	}

	// 死亡モーションのプレイヤーの生成
	if (pPlayerManager != nullptr)
	{
		int nNumPlayer = 4;

		for (int nCntPlayer = 0; nCntPlayer < nNumPlayer; nCntPlayer++)
		{
			bool bDeathPlayer = pPlayerManager->GetDeathPlayer(nCntPlayer);		// 死んだプレイヤーを取得

			if (bDeathPlayer == true)
			{
				m_apModelPlayer[nCntPlayer] = CMotion::Create((char*)PLAYER_BODY_PATH[nCntPlayer]);	// 生成

				// プレイヤーの情報の設定
				m_apModelPlayer[nCntPlayer]->SetPosition(PLAYER_POS[nCntPlayer]);	// 位置
				m_apModelPlayer[nCntPlayer]->SetRot(PLAYER_ROT[nCntPlayer]);		// 向き
				m_apModelPlayer[nCntPlayer]->SetMotion(3);							// モーション
			}
		}
	}
	else if (pPlayerManager == nullptr)
	{
		return E_FAIL;
	}

	// 黒ポリゴンの表示
	if (m_pBlack == nullptr)
	{
		m_pBlack = CObject2D::Create(7);
	}

	if (m_pBlack != nullptr)
	{// 黒ポリゴンの情報を設定
		m_pBlack->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		m_pBlack->SetPosition({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f });
		m_pBlack->SetVtx();
		m_pBlack->SetCol({ 0.0, 0.0, 0.0, 0.0 });
	}
	else if (m_pBlack == nullptr)
	{
		return E_FAIL;
	}


	// ゲームオーバーの表示
	if (m_pLogo == nullptr)
	{
		m_pLogo = CObject2D::Create(7);
	}

	if (m_pLogo != nullptr)
	{// ゲームオーバーの情報を設定
		m_pLogo->SetSize(LOGO_WIDTH, LOGO_HEIGHT);
		m_pLogo->SetPosition(LOGO_POS);
		int nIdx = CTexture::GetInstance()->Regist(LOGO_PATH);
		m_pLogo->SetIdxTexture(nIdx);
		m_pLogo->SetVtx();
		m_pLogo->SetCol({1.0, 1.0, 1.0, 0.0});
	}
	else if (m_pLogo == nullptr)
	{
		return E_FAIL;
	}

	// コンティニューの表示
	if (m_pContiny == nullptr)
	{
		m_pContiny = CObject2D::Create(7);
	}

	if (m_pContiny != nullptr)
	{// コンティニューの情報を設定
		m_pContiny->SetSize(CONTINY_WIDTH, CONTINY_HEIGHT);
		m_pContiny->SetPosition(CONTINY_POS);
		int nIdx = CTexture::GetInstance()->Regist(CONTINY_PATH);
		m_pContiny->SetIdxTexture(nIdx);
		m_pContiny->SetVtx();
		m_pContiny->SetCol({ 1.0, 1.0, 1.0, 0.0 });
	}
	else if (m_pContiny == nullptr)
	{
		return E_FAIL;
	}

	// UIを非表示
	if (pUIManager != nullptr)
	{
		pUIManager->EnableDisp(false);
	}
	else if (pUIManager == nullptr)
	{
		return E_FAIL;
	}
	if (pGoal != nullptr)
	{
		pGoal->Uninit();
	}
	else if (pGoal == nullptr)
	{
		return E_FAIL;
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGameover::Uninit(void)
{
	m_pGameover = nullptr;
	m_pBlack = nullptr;
	m_pLogo = nullptr;
	m_pContiny = nullptr;

	delete this;
}

//===============================================
// 更新処理
//===============================================
void CGameover::Update(void)
{
	// インスタンスを取得
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CCamera* pCamera = CManager::GetCamera();
	
	// カメラをロケット付近へ移動
	if (pCamera != nullptr)
	{
		// カメラの設定
		pCamera->UpdateGameover(CAMERA_POSV, CAMERA_POSR);
	}

	int nNumBlock = CBlock::GetNumAll();

	if (nNumBlock > 0)
	{
		CBlock** pBlock = CBlock::GetBlock();

		for (int nCntBlock = 0; nCntBlock < NUM_OBJECT; nCntBlock++)
		{
			if (pBlock[nCntBlock] != nullptr)
			{
				pBlock[nCntBlock]->Hit(100);
			}
		}
	}

	bool bFinish = m_apModelPlayer[0]->IsFinish();		// 死亡モーションが終了したか

	if (bFinish == true)
	{
		// 暗転処理
		BlackOut();
	}
}

//===============================================
// 描画処理
//===============================================
void CGameover::Draw(void)
{
	
}

//===============================================
// 暗転処理
//===============================================
void CGameover::BlackOut(void)
{
	// インスタンスを取得
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	CFade* pFade = CFade::GetInstance();

	// カウントを更新
	m_fFadeCounter += 0.01f;

	if (m_pBlack != nullptr)
	{// 黒ポリゴンを少しずつ表示する
		m_pBlack->SetCol({ 0.0, 0.0, 0.0, m_fFadeCounter });
	}

	if (m_pLogo != nullptr)
	{// ゲームオーバーを少しずつ表示する
		m_pLogo->SetCol({ 1.0, 1.0, 1.0, m_fFadeCounter - 0.1f });

		if (m_fFadeCounter - 0.1f >= 0.7f)
		{// 移動量を減衰させる
			m_pLogo->DicMove(0.1f);
			m_pLogo->SetVtx();
		}
		else
		{// 文字を下へ移動
			m_pLogo->SetMove({ 0.0f, 0.8f, 0.0f });
			m_pLogo->SetVtx();
		}
	}

	if (m_fFadeCounter >= CONTINY_DISPTIMING)
	{
		if (m_pContiny != nullptr)
		{// コンティニューを少しずつ表示する
			if (m_fFlashCounter > 1.0f || m_fFlashCounter < 0.0f)
			{// 透明度が最小か最大になった
				m_bFlash = m_bFlash ? false : true;		// 状態を切り替える
			}

			if (m_bFlash == false)
			{
				m_fFlashCounter += 0.02f;
			}
			else if(m_bFlash == true)
			{
				m_fFlashCounter -= 0.02f;
			}

			m_pContiny->SetCol({ 1.0, 1.0, 1.0, m_fFlashCounter });
		}
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
	{//ENTERキーが押されたら
		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_RANKING);	// ランキングに遷移

			// プレイヤーマネージャーの終了
			CPlayerManager* pPlayerManger = CPlayerManager::GetInstance();

			if (pPlayerManger != nullptr)
			{
				pPlayerManger->Uninit();
			}
		}
	}
}