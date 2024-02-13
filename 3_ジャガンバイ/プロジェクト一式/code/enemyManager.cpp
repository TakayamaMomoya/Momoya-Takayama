//*****************************************************
//
// 敵マネージャー[enemyManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "enemyThief.h"
#include "playerManager.h"
#include "manager.h"
#include "camera.h"
#include "universal.h"
#include <stdio.h>
#include "game.h"
#include "rocket.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int RAND_SPAWN = 400;	// スポーン範囲

const float LIMID_RANGE_LEFT = -460.0f;
const float LIMID_RANGE_RIGHT = 460.0f;
const float LIMID_RANGE_UP = 460.0f;
const float LIMID_RANGE_DOWN = -460.0f;
const int NUM_SPAWNANGLE = 3;	// スポーン角度の数
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CEnemyManager *CEnemyManager::m_pEnemyManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CEnemyManager::CEnemyManager()
{
	m_fTimerSpawn = 0;
	m_nMinTimeSpawnThief = 0;
	m_nMaxTimeSpawnThief = 0;
	m_fTimerThief = 0.0f;
	m_fTimeSpawnThief = 0.0f;
	m_fDistSpawn = 0.0f;
	m_fRateProgress = 0.0f;
	m_fScaleSpawnTime = 0.0f;
	ZeroMemory(&m_afTime[0], sizeof(float) * NUM_PLAYER);
	m_nMaxEnemy = 0;

	m_pHead = nullptr;
	m_pTail = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyManager::~CEnemyManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CEnemyManager *CEnemyManager::Create(void)
{
	if (m_pEnemyManager == nullptr)
	{// インスタンス生成
		m_pEnemyManager = new CEnemyManager;

		// 初期化処理
		m_pEnemyManager->Init();
	}

	return m_pEnemyManager;
}

//=====================================================
// 敵の生成処理
//=====================================================
CEnemy *CEnemyManager::CreateEnemy(D3DXVECTOR3 pos, CEnemy::TYPE type)
{
	char *apPath[CEnemy::TYPE_MAX] = 
	{
		nullptr,
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy01.txt",
	};

	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// インスタンス生成
		switch (type)
		{
		case CEnemy::TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			break;
		case CEnemy::TYPE_THIEF:

			pEnemy = new CEnemyThief;

			break;
		default:
			break;
		}

		if (pEnemy != nullptr)
		{
			// 位置設定
			pEnemy->SetPosition(pos);

			// モーション読込
			pEnemy->Load(apPath[type]);

			// 初期化処理
			pEnemy->Init();
		}
	}

	return pEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyManager::Init(void)
{
	// 読込処理
	Load();

	// 最初の出現時間設定
	m_fTimeSpawnThief = (float)universal::RandRange(m_nMaxTimeSpawnThief, m_nMinTimeSpawnThief);

	// スケールのリセット
	m_fScaleSpawnTime = 1.0f;

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CEnemyManager::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntTime = 0;

	// ファイルから読み込む
	FILE *pFile = fopen("data\\TEXT\\enemy.txt", "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "TIME_SPAWN_THIEF") == 0)
			{// 泥棒出現時間
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_nMinTimeSpawnThief);
				(void)fscanf(pFile, "%d", &m_nMaxTimeSpawnThief);
			}

			if (strcmp(cTemp, "TIME_SPAWN") == 0)
			{// 出現時間
				(void)fscanf(pFile, "%s", &cTemp[0]);
				
				(void)fscanf(pFile, "%f", &m_afTime[nCntTime]);

				nCntTime++;
			}

			if (strcmp(cTemp, "DIST_SPAWN") == 0)
			{// 出現距離
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_fDistSpawn);
			}

			if (strcmp(cTemp, "PROGRESS_TIME") == 0)
			{// 進行度合いの割合
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_fRateProgress);
			}

			if (strcmp(cTemp, "MAX_ENEMY") == 0)
			{// 敵の最大数
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_nMaxEnemy);
			}

			if (strcmp(cTemp, "NUM_SPAWN_ANGLE") == 0)
			{// 出現角度読込
				int nCntAngle = 0;

				// 出現角度数読込
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_nNumSpawnAngle);

				if (m_pAngleSpawn == nullptr)
				{// 判定情報の生成
					m_pAngleSpawn = new float[m_nNumSpawnAngle];

					for (int i = 0; i < m_nNumSpawnAngle; i++)
					{// 情報のクリア
						m_pAngleSpawn[i] = 0.0f;
					}
				}
				else
				{
					break;
				}

				if (m_pAngleSpawn == nullptr)
				{// 生成できなかった場合
					assert(("敵出現角度数読込に失敗", false));
				}

				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "ANGLE") == 0)
					{// 角度読込
						float fAngle;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &fAngle);

						m_pAngleSpawn[nCntAngle] = D3DXToRadian(fAngle);

						nCntAngle++;
					}

					if (m_nNumSpawnAngle <= nCntAngle)
					{
						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		assert(("敵情報読み込みに失敗", false));
	}
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyManager::Uninit(void)
{
	m_pEnemyManager = nullptr;

	if (m_pAngleSpawn != nullptr)
	{
		delete[] m_pAngleSpawn;
		m_pAngleSpawn = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyManager::Update(void)
{
	// ゲーム状態によってスポーンさせない
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL)
		{
			return;
		}
	}

	// プレイヤー数の取得
	int nIdx = 1;

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{
		nIdx = pPlayerManager->GetNumPlayer() - 1;
	}

	// 時間の加算
	float fTick = CManager::GetTick();

	m_fTimerSpawn += fTick;

	// スポーン時間の設定
	float fTime = m_afTime[nIdx];

	fTime *= m_fScaleSpawnTime;

	if (m_fTimerSpawn >= fTime)
	{// 通常敵のスポーン
		SpawnNormal();
	}

	// 泥棒敵のスポーン
	CRocket *pRocket = CRocket::GetInstance();

	if (pRocket != nullptr)
	{// 既にいる場合は通らない
		int nProgress = pRocket->GetProgress();

		if (nProgress > 0)
		{// 一度でも修理されている場合のみスポーン
			SpawnThief();
		}
	}
}

//=====================================================
// 通常敵のスポーン
//=====================================================
void CEnemyManager::SpawnNormal(void)
{
	if (m_pAngleSpawn == nullptr)
	{// 角度のエラー
		return;
	}

	int nNumEnemy = CEnemy::GetNumAll();

	if (nNumEnemy >= m_nMaxEnemy)
	{// 最大数制限
		return;
	}

	int nIdxAngle = universal::RandRange(m_nNumSpawnAngle, 0);

	D3DXVECTOR3 posCenter = { 0.0f,0.0f,0.0f };

	// 出現する座標を設定
	posCenter.x = sinf(m_pAngleSpawn[nIdxAngle]) * m_fDistSpawn;
	posCenter.z = cosf(m_pAngleSpawn[nIdxAngle]) * m_fDistSpawn;

	// 敵スポーン
	CreateEnemy(posCenter, CEnemy::TYPE::TYPE_NORMAL);

	m_fTimerSpawn = 0.0f;
}

//=====================================================
// 泥棒のスポーン
//=====================================================
void CEnemyManager::SpawnThief(void)
{
	CRocket* pRocket = CRocket::GetInstance();
	int nProgress = 0;
	int nNumEnemy = CEnemy::GetNumAll();

	if (nNumEnemy >= m_nMaxEnemy)
	{// 最大数制限
		return;
	}

	if (pRocket != nullptr)
	{// ロケットの修理状況を取得
		nProgress = pRocket->GetProgress();
	}

	float fTick = CManager::GetTick();
	m_fTimerThief += fTick;

	if (m_fTimerThief > m_fTimeSpawnThief && nProgress < 3)
	{
		D3DXVECTOR3 posCenter = { 0.0f,0.0f,0.0f };

		int nIdxAngle = universal::RandRange(m_nNumSpawnAngle, 0);

		// 出現する座標を設定
		posCenter.x = sinf(m_pAngleSpawn[nIdxAngle]) * m_fDistSpawn;
		posCenter.z = cosf(m_pAngleSpawn[nIdxAngle]) * m_fDistSpawn;

		// 敵スポーン
		CreateEnemy(posCenter, CEnemy::TYPE::TYPE_THIEF);

		m_fTimerThief = 0.0f;

		// 次に出現する時間を設定
		m_fTimeSpawnThief = (float)universal::RandRange(m_nMaxTimeSpawnThief,m_nMinTimeSpawnThief);
	}
}

//=====================================================
// スポーン時間の進行
//=====================================================
void CEnemyManager::ProgressTimeSpawn(bool bAdd)
{
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if (bAdd)
		{
			m_afTime[i] += m_fRateProgress * m_afTime[i];
		}
		else
		{
			m_afTime[i] *= m_fRateProgress;
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyManager::Draw(void)
{
#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n敵の位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
#endif
}