//*****************************************************
//
// セーブデータ管理[saveDataManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "saveDataManager.h"
#include "checkPointManager.h"
#include "player.h"
#include "game.h"
#include <stdio.h>

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* SAVE_PATH = "data\\TEXT\\save.txt";	// セーブデータのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CSaveDataManager *CSaveDataManager::m_pSaveDataManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CSaveDataManager::CSaveDataManager()
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CSaveDataManager::~CSaveDataManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CSaveDataManager *CSaveDataManager::Create(void)
{
	if (m_pSaveDataManager == nullptr)
	{
		m_pSaveDataManager = new CSaveDataManager;

		if (m_pSaveDataManager != nullptr)
		{
			m_pSaveDataManager->Init();
		}
	}

	return m_pSaveDataManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSaveDataManager::Init(void)
{
	Load();

	return S_OK;
}

//=====================================================
// 読込
//=====================================================
void CSaveDataManager::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntPos = 0;

	// ファイルから読み込む
	FILE *pFile = fopen(SAVE_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "PROGRESS") == 0)
			{// 進行状況
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.nProgress);
			}

			if (strcmp(cTemp, "LIFE_PLAYER") == 0)
			{// 体力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_info.fLife);
			}

			if (strcmp(cTemp, "INITIALLIFE_PLAYER") == 0)
			{// 初期体力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_info.fInitialLife);
			}

			if (strcmp(cTemp, "ADDREWARD") == 0)
			{// 加算報酬
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.nAddReward);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CSaveDataManager::Uninit(void)
{
	m_pSaveDataManager = nullptr;

	delete this;
}

//=====================================================
// 保存
//=====================================================
void CSaveDataManager::Save(void)
{
	CCheckPointManager *pCheck = CCheckPointManager::GetInstance();
	CPlayer *pPlayer = CPlayer::GetInstance();
	CGame *pGame = CGame::GetInstance();

	if (pCheck == nullptr || pGame == nullptr)
		assert(("保存失敗！",false));

	float fIntialLife = 0;
	float fLife = 0;

	if (pPlayer != nullptr)
	{
		fIntialLife = pPlayer->GetParam().fInitialLife;
		fLife = pPlayer->GetLife();
	}

	int nProgress = pCheck->GetProgress();

	int nAddReward = pGame->GetAddReward();

	FILE *pFile = nullptr;

	pFile = fopen(SAVE_PATH, "w");

	if (pFile != nullptr)
	{
		fprintf(pFile, "PROGRESS = %d\n", nProgress);

		fprintf(pFile, "LIFE_PLAYER = %.2f\n", fLife);

		fprintf(pFile, "INITIALLIFE_PLAYER = %.2f\n", fIntialLife);

		fprintf(pFile, "ADDREWARD = %d\n", nAddReward);

		fprintf(pFile, "END_SCRIPT");

		fclose(pFile);
	}
}