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
#include "game.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputManager.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "debugproc.h"
#include <stdio.h>
#include "UIManager.h"
#include "object3D.h"
#include "playerManager.h"
#include "texture.h"
#include "skybox.h"
#include "itemWeapon.h"
#include "itemRepair.h"
#include "weaponManager.h"
#include "enemyManager.h"
#include "rocket.h"
#include "edit.h"
#include "goal.h"
#include "block.h"
#include "gimmickManager.h"
#include "renderer.h"
#include "animEffect3D.h"
#include "pause.h"
#include "box.h"
#include "gameover.h"
#include "enemyThief.h"
#include "ghost.h"
#include "record.h"
#include "containerManager.h"
#include "enemyEvent.h"
#include "repairStatus.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define TRANS_TIME	(60)	// 終了までの余韻のフレーム数

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CGame::STATE CGame::m_state = STATE_NONE;	// 状態
CGame *CGame::m_pGame = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CGame::CGame()
{
	m_nCntState = 0;
	m_bStop = false;
	m_bGameover = false;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGame::Init(void)
{
	m_pGame = this;

	m_state = STATE_NORMAL;
	m_bStop = false;
	m_bGameover = false;

	// UIマネージャーの追加
	CUIManager::Create();

	// 床の生成
	CObject3D *pObject = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pObject != nullptr)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\wood001.jpg");
		pObject->SetIdxTexture(nIdx);
		pObject->SetTex(D3DXVECTOR2(10.0f,10.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	// 外の地面生成
	CObject3D *pObjectOut = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pObjectOut != nullptr)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\field00.jpg");
		pObjectOut->SetPosition(D3DXVECTOR3(0.0f, -5.0f, 0.0f));
		pObjectOut->SetIdxTexture(nIdx);
		pObjectOut->SetTex(D3DXVECTOR2(10.0f, 10.0f), D3DXVECTOR2(0.0f, 0.0f));
		pObjectOut->SetSize(5000.0f, 5000.0f);
	}

	// スカイボックスの生成
	CSkybox::Create();

	// ブロックの読み込み
	CBlock::Load("data\\MAP\\map01.bin");

	// プレイヤーの生成
	CPlayerManager *pPlayerManger = CPlayerManager::GetInstance();

	if (pPlayerManger != nullptr)
	{
		pPlayerManger->CreatePlayer();
	}

	// 武器マネージャーの生成
	CWeaponManager::Create();

	// 敵マネージャーの生成
	CEnemyManager *pEnemyManager = CEnemyManager::Create();

	// ロケットの生成
	CRocket::Create();

	// 修理状況の生成
	CRepairStatus::Create();

	// ギミックマネージャーの生成
	CGimmickManager::Create();

	// ３Dアニメーション管理の生成
	CAnimEffect3D::Create();

	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_BGM_GAME);
	}

	CRecord* pRecord = CRecord::GetInstance();

	// 戦績の生成
	if (pRecord == nullptr)
	{
		pRecord = CRecord::Create();
	}
	else if (pRecord != nullptr)
	{
		// 戦績の終了処理（リトライの記録維持防止）
		pRecord->Uninit();

		// 戦績の生成処理
		pRecord = CRecord::Create();
	}

	if (pRecord == nullptr)
	{
		return E_FAIL;
	}

#ifdef _DEBUG
	// エディットの生成
	//CEdit::Create();
#endif

	// フォグをかける
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
	}

	// コンテナマネージャーの生成
	CContainerManager::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGame::Uninit(void)
{
	// オブジェクト全棄
	CObject::ReleaseAll();

	// ブロックの破棄
	CBlock::DeleteAll();

	// ゲームオーバーの終了
	CGameover* pGameover = CGameover::GetInstance();

	if (pGameover != nullptr)
	{
		pGameover->Uninit();
	}

	m_pGame = nullptr;
}

//=====================================================
// 更新処理
//=====================================================
void CGame::Update(void)
{
	CFade *pFade = CFade::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();

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
		// 停止しないオブジェクトの更新
		CObject::UpdateNotStop();

		// エディットの更新
		CEdit* pEdit = CEdit::GetInstatnce();

		if (pEdit != nullptr)
		{
			pEdit->Update();
		}

		CPause *pPause = CPause::GetInstance();

		if (pPause != nullptr)
		{
			pPause->Update();
		}
	}

	// 戦績の取得
	CRecord* pRecord = CRecord::Create();

	if (pRecord != nullptr)
	{
		pRecord->Update();
	}

	CGameover* pGameover = CGameover::GetInstance();

	if (pGameover != nullptr)
	{
		pGameover->Update();
	}

	// カメラ更新
	UpdateCamera();

	// 状態管理
	ManageState();

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{
		int nNumPlayer = pPlayerManager->GetNumPlayer();

		if (nNumPlayer <= 0 && m_bGameover == false)
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				CGameover::Create();
				m_bGameover = true;

				if (pSound != nullptr)
				{
					pSound->Play(pSound->LABEL_SE_GAMEOVER);
				}

				//pFade->SetFade(CScene::MODE_RANKING);
			}
		}
	}

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
		if (m_state == STATE_ESCAPE || m_state == STATE_RESULT)
		{
			pCamera->UpdateResult();
		}
	}
	else
	{
		//// 操作
		//pCamera->Control();
	}

	pCamera->MoveDist(0.05f);
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
		break;
	case CGame::STATE_ESCAPE:

		m_nCntState++;

		if (m_nCntState >= TRANS_TIME)
		{
			m_nCntState = 0;
			SetState(STATE_RESULT);

			// ゴールでリザルトの表示
			CGoal *pGoal = CGoal::GetInstance();

			if (pGoal != nullptr)
			{
				pGoal->SetResult();
			}
		}

		break;
	case CGame::STATE_END:

		m_nCntState++;

		if (m_nCntState >= TRANS_TIME && pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_RANKING);
		}

		break;
	default:
		break;
	}
}

//=====================================================
// イベントカメラの設定
//=====================================================
void CGame::SetEventCamera(float fTime, D3DXVECTOR3 posRDest, D3DXVECTOR3 posVDest)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetEventTimer(fTime);
		pCamera->SetPosRDest(posRDest);
		pCamera->SetPosVDest(posVDest);

		EnableStop(true);
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

	if (pKeyboard->GetTrigger(DIK_G))
	{
		CItemRepair *pRepair = CItemRepair::Create();
		pRepair->SetPosition(D3DXVECTOR3(40.0f, 0.0f, 300.0f));
	}
}

//=====================================================
// 描画処理
//=====================================================
void CGame::Draw(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	char *apString[STATE::STATE_MAX] =
	{
		"NONE",
		"NORMAL",
		"ESCAPE",
		"RESULT",
		"END",
	};

	pDebugProc->Print("\nゲームの状態[%s]\n", apString[m_state]);
}