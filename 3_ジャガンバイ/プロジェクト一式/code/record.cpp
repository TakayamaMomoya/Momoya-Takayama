//*****************************************************
//
// 戦績処理[record.cpp]
// Author:酒井南勝
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "record.h"
#include "debugproc.h"
#include "manager.h"
#include "scene.h"
#include "game.h"

#include "playerManager.h"
#include "enemyManager.h"
#include "enemy.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CRecord* CRecord::m_pRecord = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CRecord::CRecord()
{
	ZeroMemory(&m_aInfo[0], sizeof(m_aInfo));
	m_nNumSuvived = 0;
	m_fGameTime = 0.0f;
	m_nTimeRank = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CRecord::~CRecord()
{

}

//=====================================================
// 生成処理
//=====================================================
CRecord* CRecord::Create(void)
{
	if (m_pRecord == nullptr)
	{// インスタンス生成
		m_pRecord = new CRecord;

		// 初期化処理
		m_pRecord->Init();
	}

	return m_pRecord;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRecord::Init(void)
{
	// プレイヤー数の設定処理
	SetNumPlayer();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRecord::Uninit(void)
{
	m_pRecord = nullptr;

	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CRecord::Update(void)
{
	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetState() == CGame::STATE_NORMAL &&
			CGame::GetInstance()->GetStop() == false)
		{
			// プレイ時間を加算
			m_fGameTime += CManager::GetTick();
		}
	}

	// デバック処理
	Debug();
}

//=====================================================
// 敵の破壊数を加算処理
//=====================================================
void CRecord::AddDestroy(int nIdx)
{
	// プレイヤー番号が最大数を少ない
	if (m_nNumSuvived > nIdx)
	{
		// 破壊数を加算
		m_aInfo[nIdx].nDestroy++;
	}
}

//=====================================================
// 敵の死亡判定処理
//=====================================================
void CRecord::CheckDeathEnemy(CObject* pObj,int nIdx)
{
	CEnemyManager* pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr || pObj == nullptr)
	{
		return;
	}

	CEnemy* pEnemy = pEnemyManager->GetHead();

	while (pEnemy != nullptr)
	{
		CEnemy* pEnemyNext = pEnemy->GetNext();

		if ((CObject*)pEnemy == pObj)
		{
			// 敵の有無を判定
			if (pEnemy->GetState() == CEnemy::STATE_DEATH)
			{
				CRecord* pRecord = CRecord::GetInstance();

				if (pRecord != nullptr)
				{
					// 現在のプレイヤーの破壊数を加算
					pRecord->AddDestroy(nIdx);
				}
			}
		}

		pEnemy = pEnemyNext;
	}
}

//=====================================================
// 当たったすべての敵の死亡判定処理
//=====================================================
void CRecord::CheckDeathEnemyAll(CCollision** ppCollsionMissile,D3DXVECTOR3 posMissile,float fRadiusMissile, int nIdx)
{
	bool bHit = false;

	CCollision** ppCollision = ppCollsionMissile;

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == CCollision::TYPE_SPHERE)
			{
				if (CCollision::TAG_ENEMY == CCollision::TAG_NONE)
				{

				}
				else if (ppCollision[nCnt]->GetTag() != CCollision::TAG_ENEMY)
				{
					continue;
				}

				// 差分取得
				D3DXVECTOR3 vecDiff = ppCollision[nCnt]->GetPosition() - posMissile;

				float fLength = D3DXVec3Length(&vecDiff);

				if (fLength < ppCollision[nCnt]->GetRadius() + fRadiusMissile)
				{
					CRecord* pRecord = CRecord::GetInstance();

					if (pRecord != nullptr)
					{
						// 現在のプレイヤーの破壊数を加算
						pRecord->AddDestroy(nIdx);
					}
				}
			}
		}
	}
}

//=====================================================
// 味方を殴った数の加算処理
//=====================================================
void CRecord::AddMedman(int nIdx)
{
	// プレイヤー番号が最大数を少ない
	if (m_nNumSuvived > nIdx)
	{
		// 味方を殴った数を加算
		m_aInfo[nIdx].nMadman++;
	}
}

//=====================================================
// デバッグ表示
//=====================================================
void CRecord::AllSort(void)
{
	// 敵の破壊数のソート
	for (int nCutRank = 0; nCutRank < NUM_PLAYER; nCutRank++)
	{
		// 置き換え番号
		for (int nCutScoreRank = 0; nCutScoreRank < NUM_PLAYER; nCutScoreRank++)
		{
			// 置き換え番号より数値が低い
			if (m_aInfo[nCutRank].nDestroy < m_aInfo[nCutScoreRank].nDestroy)
			{
				// 順位を下げる
				m_aInfo[nCutRank].nDestroyRank++;
			}
			// 破壊数が同数 and 置き換え番号の方が大きい場合
			else if (m_aInfo[nCutRank].nDestroy == m_aInfo[nCutScoreRank].nDestroy &&
					 nCutRank > nCutScoreRank)
			{
				// 置き換え番号より数値が低い場合順位を下げる
				m_aInfo[nCutRank].nDestroyRank++;
			}
		}
	}

	// 味方を殴った数のソート
	for (int nCutRank = 0; nCutRank < NUM_PLAYER; nCutRank++)
	{
		// 置き換え番号
		for (int nCutScoreRank = 0; nCutScoreRank < NUM_PLAYER; nCutScoreRank++)
		{
			// 置き換え番号より数値が低い
			if (m_aInfo[nCutRank].nMadman < m_aInfo[nCutScoreRank].nMadman)
			{
				// 順位を下げる
				m_aInfo[nCutRank].nMadmanRank++;
			}
			// 破壊数が同数 and 置き換え番号の方が大きい場合
			else if (m_aInfo[nCutRank].nMadman == m_aInfo[nCutScoreRank].nMadman &&
					 nCutRank > nCutScoreRank)
			{
				// 置き換え番号より数値が低い場合順位を下げる
				m_aInfo[nCutRank].nMadmanRank++;
			}
		}
	}
}

//=====================================================
// デバッグ表示
//=====================================================
void CRecord::Debug(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\n敵倒したP1[%d]", m_aInfo[0].nDestroy);
	pDebugProc->Print("\n時間[%f]", m_fGameTime);
}

//=====================================================
// プレイヤーの数を設定処理
//=====================================================
void CRecord::SetNumPlayer(void)
{
	CPlayerManager* pPlayerMagazine = CPlayerManager::GetInstance();

	if (pPlayerMagazine != nullptr)
	{
		// 最大生存者数を設定
		m_nNumSuvived = pPlayerMagazine->GetNumPlayer();
	}
}