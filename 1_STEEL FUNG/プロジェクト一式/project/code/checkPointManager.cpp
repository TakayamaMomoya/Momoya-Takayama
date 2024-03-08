//*****************************************************
//
// チェックポイント管理の処理[checkPointmanager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "checkPointManager.h"
#include "checkPointBehavior.h"
#include "saveDataManager.h"
#include "UI.h"
#include "texture.h"
#include "player.h"
#include "fade.h"
#include "manager.h"
#include "enemyManager.h"
#include "enemyBoss.h"
#include "debugproc.h"
#include <stdio.h>
#include "cameraBehavior.h"
#include "camera.h"
#include "UIManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* FILE_PATH = "data\\TEXT\\checkPoint.txt";	// ファイルのパス
const float SIZE_CURSOR = 20.0f;	// カーソルサイズ
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\checkPoint00.png";	// カーソルのテクスチャ
const D3DXVECTOR3 BOSSBATTLE_POS_PLAYER = { 0.0f,0.0f,-2000.0f };	// ボス戦移行時のプレイヤー位置
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CCheckPointManager *CCheckPointManager::m_pCheckPointManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CCheckPointManager::CCheckPointManager()
{
	m_nProgress = 0;
	m_nNumCheckPoint = 0;
	m_pPosCheckPoint = nullptr;
	m_pBehavior = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CCheckPointManager::~CCheckPointManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CCheckPointManager *CCheckPointManager::Create(void)
{
	if (m_pCheckPointManager == nullptr)
	{
		m_pCheckPointManager = new CCheckPointManager;

		if (m_pCheckPointManager != nullptr)
		{
			m_pCheckPointManager->Init();
		}
	}

	return m_pCheckPointManager;
}

//=====================================================
// インスタンスの取得
//=====================================================
CCheckPointManager *CCheckPointManager::GetInstance(void)
{
	if (m_pCheckPointManager == nullptr)
	{
		//Create();
	}

	return m_pCheckPointManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CCheckPointManager::Init(void)
{
	// 読込
	Load();

	// セーブデータの取得
	CSaveDataManager *pSave = CSaveDataManager::GetInstance();

	if (pSave != nullptr)
	{
		m_nProgress = pSave->GetProgress() - 1;

		CPlayer *pPlayer = CPlayer::GetInstance();

		if (pPlayer != nullptr)
		{
			D3DXVECTOR3 pos = m_pPosCheckPoint[m_nProgress + 1];

			pPlayer->SetPosition(pos);
			
			if (m_nProgress == -1)
			{
				pPlayer->SetMotion(CPlayer::MOTION_APPER);

				Camera::ChangeBehavior(new CApperPlayer);

				CUIManager *pUIManager = CUIManager::GetInstance();

				if (pUIManager != nullptr)
				{
					pUIManager->EnableDisp(false);
				}
			}
		}
	}

	ChangeBehavior(new CCheckPointMove);
	
	return S_OK;
}

//=====================================================
// 読込
//=====================================================
void CCheckPointManager::Load(void)
{
	// チェックポイント情報読込
	LoadInfoCheckPoint();
}

//=====================================================
// チェックポイント情報読込
//=====================================================
void CCheckPointManager::LoadInfoCheckPoint(void)
{
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

								nCntPos++;
								break;
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
// 終了処理
//=====================================================
void CCheckPointManager::Uninit(void)
{
	m_pCheckPointManager = nullptr;

	if (m_pPosCheckPoint != nullptr)
	{
		delete[] m_pPosCheckPoint;
		m_pPosCheckPoint = nullptr;
	}

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Uninit(this);
		delete m_pBehavior;
		m_pBehavior = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CCheckPointManager::Update(void)
{
	if (m_nProgress >= m_nNumCheckPoint - 1)
	{
		CFade *pFade = CFade::GetInstance();

		if (pFade != nullptr)
		{
			CFade::FADE state = pFade->GetState();

			if (state == CFade::FADE::FADE_OUT)
			{// ボス敵へ移行
				TransBossBattle();

				if (m_pBehavior != nullptr)
				{
					m_pBehavior->Uninit(this);
					delete m_pBehavior;
					m_pBehavior = nullptr;
				}
			}
		}

		return;
	}

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Update(this);
	}
}

//=====================================================
// ボス戦へ移行
//=====================================================
void CCheckPointManager::TransBossBattle(void)
{
	// ボスの生成
	CEnemyBoss *pEnemyBoss = CEnemyBoss::GetInstance();

	if (pEnemyBoss == nullptr)
	{
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			pEnemyManager->DeleteAll();
		}

		pEnemyBoss = CEnemyBoss::Create();

		if (pEnemyBoss != nullptr)
		{
			pEnemyBoss->SetPosition(D3DXVECTOR3(22000.0f,0.0f,0.0f));
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CCheckPointManager::Draw(void)
{
#ifdef _DEBUG

	CDebugProc::GetInstance()->Print("\n進行状況[%d]", m_nProgress);

#endif
}

//=====================================================
// チェックポイント位置取得
//=====================================================
D3DXVECTOR3 CCheckPointManager::GetCheckPosition(int nProgress)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (m_pPosCheckPoint == nullptr)
		return pos;

	if (nProgress == -1)
	{
		pos = m_pPosCheckPoint[m_nProgress];
	}
	else
	{
		pos = m_pPosCheckPoint[nProgress];
	}

	return pos;
}

//=====================================================
// 進行度加算
//=====================================================
void CCheckPointManager::AddProgress(void)
{
	m_nProgress++;

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		if (m_nProgress > 0)
		{
			pEnemyManager->SpawnGroup(m_nProgress);
		}
	}

	if (m_nProgress >= m_nNumCheckPoint - 1)
	{// 最後のチェックポイントに到着
		CFade *pFade = CFade::GetInstance();

		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_RESULT, false);
		}
	}
}

//=====================================================
// ビヘイビア変更
//=====================================================
void CCheckPointManager::ChangeBehavior(CCheckPointBehavior *pBehavior)
{
	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Uninit(this);
		delete m_pBehavior;
	}

	m_pBehavior = pBehavior;

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Init(this);
	}
}

namespace CheckPoint
{
void SetProgress(int nProgress)
{
	CCheckPointManager *pCheck = CCheckPointManager::GetInstance();

	if (pCheck != nullptr)
	{
		pCheck->SetProgress(nProgress);
	}
}
}