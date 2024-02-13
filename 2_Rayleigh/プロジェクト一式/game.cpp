//*****************************************************
//
// ゲーム処理[game.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "object.h"
#include "universal.h"
#include "particle.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "fade.h"
#include "camera.h"
#include "effect3D.h"
#include "sound.h"
#include "scene.h"
#include "player.h"
#include "field.h"
#include "block.h"
#include "edit.h"
#include "enemy.h"
#include "skybox.h"
#include "enemyManager.h"
#include "animEffect3D.h"
#include "debugproc.h"
#include <stdio.h>
#include "UIManager.h"
#include "anim3D.h"
#include "texture.h"
#include "timer.h"
#include "pause.h"
#include "inputManager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define TRANS_TIME	(60)	// 終了までの余韻のフレーム数
#define RESULT_TIME	(5)	// リザルト画面表示までのラグ
#define BOSS_LINE	(2737.0f)	// ボス戦に突入するライン
#define CHECKPOINT_PATH "data\\MAP\\checkpoint.txt"	// チェックポイントデータのパス

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CGame::STATE CGame::m_state = STATE_NONE;	// 状態
int CGame::m_nProgress = 0;	// 進行状況
CGame *CGame::m_pGame = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CGame::CGame()
{
	m_pPosCheckPoint = nullptr;
	m_nNumCheckPoint = 0;
	m_nCntState = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CGame::~CGame()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGame::Init(void)
{
	m_pGame = this;

	m_state = STATE_NORMAL;

	// カメラ距離の設定
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetDist(180.0f);
	}

	// リスポーン地点設定
	LoadCheckPoint();

	// スカイボックス
	CSkybox *pSkybox = CSkybox::Create();
	pSkybox->SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	// ブロック配置読込
	CBlock::Load("data\\MAP\\map01.bin");

	// UIマネージャー
	CUIManager::Create();

	// プレイヤー生成
	CPlayer *pPlayer = CPlayer::Create();

	if (pPlayer != nullptr && m_pPosCheckPoint != nullptr)
	{// スポーン地点の設定
		pPlayer->SetPosition(m_pPosCheckPoint[m_nProgress]);
	}

	// 敵マネージャー生成
	CEnemyManager::Create();

	// ３Dエフェクト管理生成
	CAnimEffect3D::Create();

	// チュートリアルの生成
	CreateTutorial();

#ifdef _DEBUG
	//CEdit::Create();
#endif

	m_bStop = false;

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_BGM_BATTLE);
	}

	return S_OK;
}

//=====================================================
// チェックポイントの読込
//=====================================================
void CGame::LoadCheckPoint(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntPos = 0;

	// ファイルから読み込む
	FILE *pFile = fopen(CHECKPOINT_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "NUM_CHECKPOINT") == 0)
			{// チェックポイント数読込
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_nNumCheckPoint);

				if (m_pPosCheckPoint == nullptr)
				{// 判定情報の生成
					m_pPosCheckPoint = new D3DXVECTOR3[m_nNumCheckPoint];

					for (int i = 0; i < m_nNumCheckPoint; i++)
					{// 情報のクリア
						m_pPosCheckPoint[i] = { 0.0f,0.0f,0.0f };
					}
				}
				else
				{
					break;
				}

				if (m_pPosCheckPoint == nullptr)
				{// 生成できなかった場合
					break;
				}

				while (true)
				{
					// 文字読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "CHECKPOINTSET") == 0)
					{// パラメーター読込開始
						while (true)
						{
							// 文字読み込み
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								for (int i = 0; i < 3; i++)
								{
									fscanf(pFile, "%f", &m_pPosCheckPoint[nCntPos][i]);
								}
							}

							if (strcmp(cTemp, "END_CHECKPOINTSET") == 0)
							{// パラメーター読込終了
								nCntPos++;

								break;
							}
						}
					}

					if (m_nNumCheckPoint <= nCntPos)
					{
						break;
					}
				}
			}
			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
}

//=====================================================
// チュートリアル表記の生成
//=====================================================
void CGame::CreateTutorial(void)
{
	D3DXVECTOR3 aPos[5] =
	{
		{ 161.0f, 15.0f, 20.0f },
		{ 300.0f, 15.0f, 20.0f },
		{ 526.0f, 11.0f, 20.0f },
		{ 631.0f, 80.0f, 20.0f },
		{ 842.0f, 80.0f, 20.0f },
	};

	char *apPath[5] = 
	{
		"data\\TEXTURE\\UI\\tutorial00.png",
		"data\\TEXTURE\\UI\\tutorial01.png",
		"data\\TEXTURE\\UI\\tutorial02.png",
		"data\\TEXTURE\\UI\\tutorial03.png",
		"data\\TEXTURE\\UI\\tutorial04.png",
	};

	for (int i = 0; i < 5; i++)
	{
		// インスタンス生成
		CAnim3D *pAnim3D = CAnim3D::Create(aPos[i], 2, 40, true);

		if (pAnim3D != nullptr)
		{
			// 色の設定
			CTexture *pTexture = CTexture::GetInstance();

			if (pTexture != nullptr)
			{// テクスチャの設定
				int nIdx = pTexture->Regist(apPath[i]);
				pAnim3D->SetIdxTexture(nIdx);
				pAnim3D->SetSize(35.0f, 25.0f);
			}
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CGame::Uninit(void)
{
	if (m_pPosCheckPoint != nullptr)
	{
		delete[] m_pPosCheckPoint;
		m_pPosCheckPoint = nullptr;
	}

	// ブロック情報削除
	CBlock::DeleteAll();

	// オブジェクト全棄
	CObject::ReleaseAll();

	m_pGame = nullptr;
}

//=====================================================
// 更新処理
//=====================================================
void CGame::Update(void)
{
	CFade *pFade = CFade::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (m_bStop == false)
	{
		// シーンの更新
		CScene::Update();

		if (pInputManager != nullptr)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
			{
				CPause::Create();
			}
		}
	}
	else
	{
#ifdef DEBUG
		CEdit *pEdit = CEdit::GetInstatnce();

		if (pEdit != nullptr)
		{
			pEdit->Update();
		}
#endif // DEBUG

		CPause *pPause = CPause::GetInstance();

		if (pPause != nullptr)
		{
			pPause->Update();
		}
	}

	// カメラ更新
	UpdateCamera();

	// 状態管理
	ManageState();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// カメラの更新
//=====================================================
void CGame::UpdateCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	if (m_bStop == false)
	{
		if (m_state == STATE_NORMAL)
		{
			// プレイヤーの追従
			pCamera->FollowPlayer();
		}
		else if (m_state == STATE_BOSS)
		{
			// ボス戦の動き
			pCamera->BossBattle();
		}
	}
	else
	{
		// 操作
		pCamera->Control();
	}
}

//=====================================================
// 状態管理
//=====================================================
void CGame::ManageState(void)
{
	CFade *pFade = CFade::GetInstance();

	switch (m_state)
	{
	case CGame::STATE_NORMAL:
	{
		// プレイヤー取得
		CPlayer *pPlayer = nullptr;
		pPlayer = CPlayer::GetInstance();

		if (pPlayer != nullptr)
		{
			D3DXVECTOR3 pos = pPlayer->GetPosition();

			if (pos.x >= BOSS_LINE)
			{// ボス戦へ移行
				CEnemyManager *pEnemyManager = nullptr;

				pEnemyManager = CEnemyManager::GetInstance();

				if (pEnemyManager != nullptr)
				{
					pEnemyManager->CreateEnemy(D3DXVECTOR3(BOSS_LINE,190.0f,0.0f), CEnemy::TYPE_BOSS);
				}

				m_state = STATE_BOSS;
			}

			if (m_nProgress < m_nNumCheckPoint - 1)
			{
				if (pos.x >= m_pPosCheckPoint[m_nProgress + 1].x)
				{// チェックポイントの設定
					m_nProgress++;
				}
			}
		}
	}
		break;
	case CGame::STATE_BOSS:
		break;
	case CGame::STATE_RESULT:
		break;
	case CGame::STATE_END:

		m_nCntState++;

		if (m_nCntState >= 300 && pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_RANKING);
		}

		break;
	default:
		break;
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CGame::Debug(void)
{
	// 入力取得
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_F))
	{
		m_bStop = m_bStop ? false : true;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CGame::Draw(void)
{
	CDebugProc::GetInstance()->Print("\n進行状況：[%d]", m_nProgress);
}