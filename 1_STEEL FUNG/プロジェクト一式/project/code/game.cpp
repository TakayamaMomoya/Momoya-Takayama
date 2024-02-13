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
#include "texture.h"
#include "skybox.h"
#include "enemyManager.h"
#include "block.h"
#include "renderer.h"
#include "animEffect3D.h"
#include "pause.h"
#include "player.h"
#include "slow.h"
#include "checkPointManager.h"
#include "blockManager.h"
#include "meshfield.h"
#include "enemyBoss.h"
#include "cameraBehavior.h"
#include "particle.h"
#include "beamBlade.h"
#include "boostEffect.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define TRANS_TIME	(300)	// 終了までの余韻のフレーム数

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
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGame::Init(void)
{
	m_pGame = this;

	m_state = STATE_NORMAL;
	m_bStop = false;

	// UIマネージャーの追加
	CUIManager::Create();

	// スカイボックスの生成
	CSkybox::Create();

	// ブロック管理の生成
	CBlockManager::Create();

	// チェックポイント管理の生成
	CCheckPointManager::Create();

	// 敵マネージャーの生成
	CEnemyManager *pEnemyManager = CEnemyManager::Create();

	// ３Dアニメーション管理の生成
	CAnimEffect3D::Create();

	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		//pSound->Play(pSound->LABEL_BGM_GAME);
	}

	// フォグをかける
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
	}

	// プレイヤーの生成
	CPlayer::Create();

	// スロー管理の生成
	CSlow::Create();

	// メッシュフィールド生成
	CMeshField::Create();

	//CEnemyBoss::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGame::Uninit(void)
{
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
	CSound* pSound = CSound::GetInstance();

	if (m_bStop == false)
	{
		// シーンの更新
		CScene::Update();

		//CParticle::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), CParticle::TYPE::TYPE_BEAM_BLADE);

		// カーソルを中心に固定
		SetCursorPos((int)(SCREEN_WIDTH * 0.5f), (int)(SCREEN_HEIGHT * 0.5f));

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

	pCamera->Update();
	pCamera->Quake();

	pCamera->MoveDist(0.3f);
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

		if (m_bStop)
		{
			Camera::ChangeBehavior(new CMoveControl);
		}
		else
		{
			Camera::ChangeBehavior(new CFollowPlayer);
		}
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
		"END",
	};

	pDebugProc->Print("\nゲームの状態[%s]\n", apString[m_state]);
}