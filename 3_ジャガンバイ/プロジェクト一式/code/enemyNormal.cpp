//*****************************************************
//
// 通常敵の処理[enemyNormal.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemyNormal.h"
#include "manager.h"
#include "collision.h"
#include "debugproc.h"
#include "particle.h"
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "enemyManager.h"
#include "debrisSpawner.h"
#include "block.h"
#include "motion.h"
#include "playerManager.h"

//=====================================================
// コンストラクタ
//=====================================================
CEnemyNormal::CEnemyNormal()
{

}

//=====================================================
// デストラクタ
//=====================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyNormal::Init(void)
{
	// 継承クラスの初期化
	CEnemy::Init();

	// 歩きモーションに設定
	CMotion *pBody = GetBody();

	if (pBody != nullptr)
	{
		pBody->SetMotion(MOTION_WALK);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyNormal::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyNormal::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();

	CBlock *pBlock = GetTouchBlock();

	if (pBlock != nullptr)
	{
		// 生存チェック
		CBlock **ppBlock = CBlock::GetBlock();

		for (int i = 0; i < NUM_OBJECT; i++)
		{
			if (ppBlock[i] == pBlock)
			{
				pBlock->Hit(5.0f);
			}
		}
	}

	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_DEATH)
	{// 死亡モーション
		int nMotion = GetMotion();

		if (nMotion != MOTION_DEATH)
		{
			SetMotion(MOTION_DEATH);
		}
	}
	else
	{

	}
}

//=====================================================
// 目標の追跡
//=====================================================
void CEnemyNormal::ChaseTarget(void)
{
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr)
	{
		return;
	}

	MOVESTATE moveState = GetMoveState();

	if (moveState == MOVESTATE_CHASE)
	{
		D3DXVECTOR3 posTarget = { 0.0f,0.0f,0.0f };

		// 最遠距離の宣言
		float fLengthMax = FLT_MAX;

		for (int i = 0; i < NUM_PLAYER; i++)
		{// 最も近いプレイヤーを参照
			CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

			if (pPlayer != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();
				D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

				// 距離の比較
				bool bNear = universal::DistCmp(pos, posPlayer, fLengthMax, &fLengthMax);

				if (bNear)
				{
					SetPosDest(posPlayer);
				}
			}
		}
	}

	CEnemy::ChaseTarget();
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyNormal::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}