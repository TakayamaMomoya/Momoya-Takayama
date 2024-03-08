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
#include "manager.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "enemyBomb.h"
#include "enemyDrone.h"
#include "enemyTutorial.h"
#include "enemyBoss.h"
#include "checkPointManager.h"
#include "checkPointBehavior.h"
#include "inputkeyboard.h"
#include "effect3D.h"
#include "player.h"
#include "UI.h"
#include "fan2D.h"
#include "texture.h"
#include "caution.h"
#include "debugproc.h"
#include <stdio.h>

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* FILE_PATH = "data\\TEXT\\checkPoint.txt";	// ファイルのパス
const float TIME_SPAWN = 5.0f;	// 敵のスポーン
const float SIZE_CURSOR = 60.0f;	// カーソルサイズ
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\lockon01.png";	// カーソルのテクスチャ
const char* MANUAL_PATH = "data\\TEXTURE\\UI\\isLock00.png";	// マニュアルエイムのパス
const char* LOCK_PATH = "data\\TEXTURE\\UI\\isLock01.png";	// アシストエイムのパス
const D3DXVECTOR3 GAUGE_POS = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };	// ゲージの位置
const char* TEXTURE_PATH = "data\\TEXTURE\\UI\\boost00.png";	// ゲージのパス
const float RADIUS_GAUGE = 330.0f;	// ゲージの半径
const float INITIAL_ROT = -D3DX_PI * 0.15f;
const float ANGLE_MAX = D3DX_PI * 0.3f;
const float TIME_CHANGE = 1.0f;	// ロック切り替え時間
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
	m_pInfoGroup = nullptr;
	m_pEnemyLockon = nullptr;
	m_bLockTarget = false;
	m_pCursor = nullptr;
	m_pIsLock = nullptr;
	m_pObjectFrame = nullptr;
	m_pObjectGauge = nullptr;
	m_bEndSpawn = false;
	m_fTimerSpawn = 0.0f;
	m_nCntSpawn = 0;
	m_fTimerChange = TIME_CHANGE;
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
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// インスタンス生成
		switch (type)
		{
		case CEnemy::TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			break;
		case CEnemy::TYPE_BOMB:

			pEnemy = new CEnemyBomb;

			break;
		case CEnemy::TYPE_DRONE:

			pEnemy = new CEnemyDrone;

			break;
		case CEnemy::TYPE_TUTORIAL:

			pEnemy = new CEnemyTutorial;

			break;
		default:
			break;
		}

		if (pEnemy != nullptr)
		{
			// 位置設定
			pEnemy->SetPosition(pos);

			// モーション読込
			pEnemy->Load((char*)PATH_BODY_ENEMY[type]);

			// 初期化処理
			pEnemy->Init();

			pEnemy->SetType(type);
		}
	}

	return pEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyManager::Init(void)
{
	// チェックポイントの情報取得
	CCheckPointManager *pCheckPointManager = CCheckPointManager::GetInstance();

	if (pCheckPointManager != nullptr)
	{
		int nNumCheckPoint = pCheckPointManager->GetNumCheckPoint();

		if (m_pInfoGroup == nullptr)
		{
			m_pInfoGroup = new SInfoEnemyGroup[nNumCheckPoint];

			if (m_pInfoGroup != nullptr)
			{
				ZeroMemory(m_pInfoGroup, sizeof(SInfoEnemyGroup) * nNumCheckPoint);
			}
		}

		// 読込処理
		Load();
	}

	if (m_pCursor == nullptr)
	{// カーソル生成
		m_pCursor = CUI::Create();

		if (m_pCursor != nullptr)
		{
			m_pCursor->SetSize(SIZE_CURSOR, SIZE_CURSOR);
			m_pCursor->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
			m_pCursor->SetIdxTexture(nIdx);
			m_pCursor->SetVtx();
		}
	}

	if (m_pIsLock == nullptr)
	{// ロックしてるかの表示生成
		m_pIsLock = CUI::Create();

		if (m_pIsLock != nullptr)
		{
			m_pIsLock->SetSize(70.0f, 35.0f);
			m_pIsLock->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(MANUAL_PATH);
			m_pIsLock->SetIdxTexture(nIdx);
			m_pIsLock->SetVtx();
		}
	}

	m_bEndSpawn = true;

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CEnemyManager::Load(void)
{
	if (m_pInfoGroup == nullptr)
	{
		return;
	}

	// 変数宣言
	char cTemp[256];
	int nCntPos = 0;

	// ファイルから読み込む
	FILE *pFile = fopen(FILE_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "CHECKPOINTSET") == 0)
			{// パラメーター読込開始
				int nNumEnemy = 0;

				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "NUM_ENEMY") == 0)
					{// 敵の数取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_pInfoGroup[nCntPos].nNumEnemy);

						if (m_pInfoGroup[nCntPos].nNumEnemy > 0)
						{
							m_pInfoGroup[nCntPos].pInfoEnemy = new SInfoEnemy[m_pInfoGroup[nCntPos].nNumEnemy];
						}

					}

					if (strcmp(cTemp, "ENEMYSET") == 0)
					{// 敵情報
						SInfoEnemy *Info = &m_pInfoGroup[nCntPos].pInfoEnemy[nNumEnemy];

						while (true)
						{// 敵の情報読込
							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "POS") == 0)
							{// 位置
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%f", &Info->pos.x);
								(void)fscanf(pFile, "%f", &Info->pos.y);
								(void)fscanf(pFile, "%f", &Info->pos.z);
							}

							if (strcmp(cTemp, "POSDEST") == 0)
							{// 初期目標位置
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%f", &Info->posDestInitial.x);
								(void)fscanf(pFile, "%f", &Info->posDestInitial.y);
								(void)fscanf(pFile, "%f", &Info->posDestInitial.z);
							}

							if (strcmp(cTemp, "TYPE") == 0)
							{// 種類
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &Info->nType);
							}

							if (strcmp(cTemp, "DELAY") == 0)
							{// ディレイ
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%f", &Info->fDelaySpawn);
							}

							if (strcmp(cTemp, "END_ENEMYSET") == 0)
							{// パラメーター読込終了
								nNumEnemy++;

								break;
							}
						}
					}

					if (strcmp(cTemp, "END_CHECKPOINTSET") == 0)
					{// パラメーター読込終了
						nCntPos++;

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
// 終了処理
//=====================================================
void CEnemyManager::Uninit(void)
{
	m_pEnemyManager = nullptr;

	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

	if (m_pIsLock != nullptr)
	{
		m_pIsLock->Uninit();
		m_pIsLock = nullptr;
	}

	if (m_pInfoGroup != nullptr)
	{
		delete[] m_pInfoGroup;
		m_pInfoGroup = nullptr;
	}

	DeleteGauge();

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyManager::Update(void)
{
	if (m_fTimerChange < TIME_CHANGE)
	{
		m_fTimerChange += CManager::GetDeltaTime();
	}

	// ロックオンゲージの制御
	if (m_pEnemyLockon != nullptr)
	{
		CreateGauge();

		ControlGauge();
	}
	else
	{
		DeleteGauge();
	}

	if (m_pIsLock != nullptr)
	{
		CPlayer *pPlayer = CPlayer::GetInstance();

		if (pPlayer != nullptr)
		{
			bool bLock = pPlayer->IsLock();

			if (bLock)
			{
				int nIdx = CTexture::GetInstance()->Regist(LOCK_PATH);
				m_pIsLock->SetIdxTexture(nIdx);
			}
			else
			{
				int nIdx = CTexture::GetInstance()->Regist(MANUAL_PATH);
				m_pIsLock->SetIdxTexture(nIdx);
			}
		}
	}

#ifdef _DEBUG

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_C))
		{
			CreateEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(0.0f, 500.0f, 500.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(-1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(-1200.0f, 400.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(1500.0f, 1000.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
		}
	}

	/*if(m_pEnemyLockon != nullptr)
		CEffect3D::Create(m_pEnemyLockon->GetPosition(), 30.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));*/
#endif
}

//=====================================================
// ロックオン処理
//=====================================================
CEnemy *CEnemyManager::Lockon(CEnemy *pEnemyExclusive)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return m_pEnemyLockon;
	}

	bool bLock = IsLockTarget();

	bool bInAny = false;
	float fDistMax = FLT_MAX;
	D3DXVECTOR3 posCenter = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
	D3DXVECTOR3 posScreen = posCenter;

	std::list<CEnemy*> listEnemy = GetListRanking();

	for (auto pEnemy : listEnemy)
	{
		CEnemy::STATE state = pEnemy->GetState();

		pEnemy->EnableLock(false);

		if (state != CEnemy::STATE::STATE_DEATH && pEnemyExclusive != pEnemy)
		{
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(0);
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXMATRIX mtx = *pEnemy->GetMatrix();

			D3DXVECTOR3 vecDiffPlayer = pos - posPlayer;
			float fDistPlayer = D3DXVec3Length(&vecDiffPlayer);
			D3DXVECTOR3 posScreenTemp;

			// ロックオンする敵の決定
			if (universal::IsInScreen(pos, mtx, &posScreenTemp))
			{
				float fDist = pEnemy->GetDistLock();

				if (fDist > fDistPlayer)
				{// 距離内に敵がいたら
					bInAny = true;

					pEnemy->EnableLock(true);
					pEnemy->SetPositionCursor(posScreenTemp);

					if (m_fTimerChange > TIME_CHANGE || (bLock == false || (m_pEnemyLockon == nullptr && bLock == true)))
					{// 自動でロックオン対象が切り替わるかどうか
						D3DXVECTOR3 vecDiff = posScreenTemp - posCenter;

						// 画面中心からの距離を計算
						float fDist = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

						if (fDist < fDistMax)
						{
							posScreen = posScreenTemp;

							m_pEnemyLockon = pEnemy;

							fDistMax = fDist;
						}
					}
				}
				else if (m_pEnemyLockon == pEnemy)
				{
					if (SwitchTarget(1, 1) == nullptr &&
						SwitchTarget(-1, -1) == nullptr)
					{
						m_pEnemyLockon = nullptr;
					}
				}
			}
		}
	}

	D3DXVECTOR3 posDestCursor = posCenter;

	if (bInAny == false)
	{
		if (bLock == false)
		{
			m_pEnemyLockon = nullptr;
		}
	}
	else
	{
		posDestCursor = posScreen;
	}

	if (m_pEnemyLockon != nullptr)
	{
		D3DXVECTOR3 pos = m_pEnemyLockon->GetMtxPos(0);
		D3DXMATRIX mtx = *m_pEnemyLockon->GetMatrix();

		universal::IsInScreen(pos, mtx, &posDestCursor);
	}

	if (m_pCursor != nullptr)
	{// カーソルの位置補正
		D3DXVECTOR3 posCursor = m_pCursor->GetPosition();
		D3DXVECTOR3 vecDiffCursor = posDestCursor - posCursor;

		posCursor += vecDiffCursor * 0.1f;

		m_pCursor->SetPosition(posCursor);
		m_pCursor->SetVtx();
	}

	return m_pEnemyLockon;
}

//=====================================================
// 敵体力ゲージの生成
//=====================================================
void CEnemyManager::CreateGauge(void)
{
	if (m_pObjectFrame == nullptr)
	{// フレームの生成
		m_pObjectFrame = CFan2D::Create();

		if (m_pObjectFrame != nullptr)
		{
			m_pObjectFrame->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectFrame->SetAngleMax(ANGLE_MAX);
			m_pObjectFrame->SetRotation(INITIAL_ROT);
			m_pObjectFrame->SetRadius(RADIUS_GAUGE);
			m_pObjectFrame->SetVtx();

			D3DXCOLOR col = universal::ConvertRGB(255, 255, 255, 60);

			m_pObjectFrame->SetCol(col);

			int nIdx = CTexture::GetInstance()->Regist(TEXTURE_PATH);
			m_pObjectFrame->SetIdxTexture(nIdx);
		}
	}

	if (m_pObjectGauge == nullptr)
	{// ブーストゲージの生成
		m_pObjectGauge = CFan2D::Create();

		if (m_pObjectGauge != nullptr)
		{
			m_pObjectGauge->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectGauge->SetAngleMax(ANGLE_MAX);
			m_pObjectGauge->SetRotation(INITIAL_ROT);
			m_pObjectGauge->SetRadius(RADIUS_GAUGE);
			m_pObjectGauge->SetVtx();

			D3DXCOLOR col = universal::ConvertRGB(255, 60, 38, 255);

			m_pObjectGauge->SetCol(col);

			int nIdx = CTexture::GetInstance()->Regist(TEXTURE_PATH);
			m_pObjectGauge->SetIdxTexture(nIdx);
		}
	}
}

//=====================================================
// 敵体力ゲージの破棄
//=====================================================
void CEnemyManager::DeleteGauge(void)
{
	if (m_pObjectFrame != nullptr)
	{
		m_pObjectFrame->Uninit();
		m_pObjectFrame = nullptr;
	}

	if (m_pObjectGauge != nullptr)
	{
		m_pObjectGauge->Uninit();
		m_pObjectGauge = nullptr;
	}
}

//=====================================================
// 敵体力ゲージの制御
//=====================================================
void CEnemyManager::ControlGauge(void)
{
	if (m_pEnemyLockon == nullptr)
		return;

	float fLifeInitial = m_pEnemyLockon->GetLifeInitial();
	float fLife = m_pEnemyLockon->GetLife();

	float fRate = fLife / fLifeInitial;

	if (m_pObjectGauge != nullptr)
	{// ゲージの設定
		// 向きの設定
		float fRot = INITIAL_ROT + ANGLE_MAX * (1.0f - fRate);

		m_pObjectGauge->SetRotation(fRot);

		// サイズ設定
		m_pObjectGauge->SetRateAngle(fRate);
		m_pObjectGauge->SetVtx();
	}
}

//=====================================================
// ターゲットの切り替え
//=====================================================
CEnemy *CEnemyManager::SwitchTarget(int nAxisX, int nAxisY, CEnemy *pEnemyExclusive)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (m_pEnemyLockon == nullptr)
	{
		return nullptr;
	}

	CEnemy *pEnemyLock = nullptr;
	float fLengthDiff = FLT_MAX;

	// 現在ロックしている敵のスクリーン座標
	D3DXMATRIX mtx = *m_pEnemyLockon->GetMatrix();
	D3DXVECTOR3 posScreenLockEnemy;
	D3DXVECTOR3 posLockEnemy = m_pEnemyLockon->GetMtxPos(0);
	universal::IsInScreen(posLockEnemy, mtx, &posScreenLockEnemy);

	std::list<CEnemy*> listEnemy = GetListRanking();

	for (auto pEnemy : listEnemy)
	{
		CEnemy::STATE state = pEnemy->GetState();

		if (state != CEnemy::STATE::STATE_DEATH && m_pEnemyLockon != pEnemy && pEnemyExclusive != pEnemy)
		{
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(0);

			D3DXVECTOR3 vecDiff = pos - pPlayer->GetPosition();
			float fDistLockon = D3DXVec3Length(&vecDiff);

			D3DXVECTOR3 posScreenTemp;

			// ロックオンする敵の決定
			if (universal::IsInScreen(pos, mtx, &posScreenTemp))
			{
				float fDist = pEnemy->GetDistLock();

				// 距離制限
				if (fDist > fDistLockon)
				{
					bool bOk = false;

					// 今ロックしてる敵との位置を比べる
					if (nAxisX > 0)
					{
						if (posScreenTemp.x > posScreenLockEnemy.x)
						{// 今のより右にいたら
							bOk = true;
						}
					}
					if (nAxisX < 0)
					{
						if (posScreenTemp.x < posScreenLockEnemy.x)
						{// 今のより左にいたら
							bOk = true;
						}
					}
					if (nAxisY < 0)
					{
						if (posScreenTemp.y > posScreenLockEnemy.y)
						{// 今のより下にいたら
							bOk = true;
						}
					}
					if (nAxisY > 0)
					{
						if (posScreenTemp.y < posScreenLockEnemy.y)
						{// 今のより上にいたら
							bOk = true;
						}
					}

					if (bOk)
					{
						// 差分距離を取得
						D3DXVECTOR3 vecDIffScreen = posScreenTemp - posScreenLockEnemy;

						float fLengthScreen = D3DXVec3Length(&vecDIffScreen);

						if (fLengthScreen < fLengthDiff)
						{// 最大距離より短かったら次のロックオンにする
							pEnemyLock = pEnemy;
							fLengthDiff = fLengthScreen;
						}
					}
				}
			}
		}
	}

	if (pEnemyLock != nullptr)
	{
		m_pEnemyLockon = pEnemyLock;

		m_fTimerChange = 0.0f;
	}

	return pEnemyLock;
}

//=====================================================
// ターゲットのロック
//=====================================================
void CEnemyManager::EnableLockTarget(bool bLock)
{
	m_bLockTarget = bLock;

}

//=====================================================
// 集団のスポーン
//=====================================================
void CEnemyManager::SpawnGroup(int nIdx)
{
	float fDeltaTime = CManager::GetDeltaTime();
	float fTimeOld = m_fTimerSpawn;

	m_fTimerSpawn += fDeltaTime;

	if (m_pInfoGroup != nullptr)
	{
		if (m_pInfoGroup[nIdx].pInfoEnemy == nullptr || m_bEndSpawn)
		{
			m_nCntSpawn = 0;
			m_fTimerSpawn = 0;
			m_bEndSpawn = true;

			return;
		}

		for (int i = 0; i < m_pInfoGroup[nIdx].nNumEnemy; i++)
		{
			float fDelay = m_pInfoGroup[nIdx].pInfoEnemy[i].fDelaySpawn;

			if (fTimeOld <= fDelay && m_fTimerSpawn > fDelay)
			{// 敵のスポーン
				D3DXVECTOR3 pos = m_pInfoGroup[nIdx].pInfoEnemy[i].pos;
				int nType = m_pInfoGroup[nIdx].pInfoEnemy[i].nType;

				CEnemy *pEnemy = CreateEnemy(pos, (CEnemy::TYPE)nType);

				if (pEnemy != nullptr)
				{
					pEnemy->SetPosDest(m_pInfoGroup[nIdx].pInfoEnemy[i].posDestInitial);
				}

				CCaution::Create(m_pInfoGroup[nIdx].pInfoEnemy[i].pos);

				m_nCntSpawn++;

				if (m_nCntSpawn >= m_pInfoGroup[nIdx].nNumEnemy)
				{// 戦闘パートの終了
					m_nCntSpawn = 0;
					m_fTimerSpawn = 0;
					m_bEndSpawn = true;

					return;
				}
			}
		}
	}
}

//=====================================================
// 死亡した敵がロックオンしてるものと同じかどうか
//=====================================================
void CEnemyManager::CheckDeathLockon(CEnemy *pEnemy)
{
	if (pEnemy == m_pEnemyLockon)
	{
		m_pEnemyLockon = nullptr;

		if (SwitchTarget(1, 1) == nullptr &&
			SwitchTarget(-1, -1) == nullptr)
		{
			m_pEnemyLockon = nullptr;
		}
	}
}

//=====================================================
// 全敵を削除する
//=====================================================
void CEnemyManager::DeleteAll(void)
{
	std::list<CEnemy*> listEnemy = GetListRanking();

	for (auto pEnemy : listEnemy)
	{
		pEnemy->Uninit();
	}

	m_pEnemyLockon = nullptr;
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyManager::Draw(void)
{
#ifdef _DEBUG

	if (m_pEnemyLockon != nullptr)
	{
		CDebugProc::GetInstance()->Print("\nロックオンしてる敵いるよ");
		CDebugProc::GetInstance()->Print("\nチェンジタイマー[%f]",m_fTimerChange);
	}
#endif
}

//=====================================================
// リストに追加する処理
//=====================================================
void CEnemyManager::AddToList(CEnemy *pEnemy)
{
	m_list.push_back(pEnemy);
}

//=====================================================
// リストから削除する処理
//=====================================================
void CEnemyManager::RemoveFromList(CEnemy *pEnemy)
{
	m_list.remove(pEnemy);
}