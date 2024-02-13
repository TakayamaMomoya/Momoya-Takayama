//*****************************************************
//
// チュートリアルマネージャー[tutorialManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "tutorialManager.h"
#include "manager.h"
#include "animation2D.h"
#include <stdio.h>

//*****************************************************
// マクロ定義
//*****************************************************
#define FILE_PATH	"data\\MAP\\enemies.txt"	// 配置データのパス

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CTutorialManager *CTutorialManager::m_pTutorialManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CTutorialManager::CTutorialManager()
{

}

//=====================================================
// デストラクタ
//=====================================================
CTutorialManager::~CTutorialManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CTutorialManager *CTutorialManager::Create(void)
{
	if (m_pTutorialManager == nullptr)
	{// インスタンス生成
		m_pTutorialManager = new CTutorialManager;

		// 初期化処理
		m_pTutorialManager->Init();
	}

	return m_pTutorialManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTutorialManager::Init(void)
{
	// 読込処理
	Load();

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CTutorialManager::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntAttack = 0;

	// ファイルから読み込む
	FILE *pFile = fopen(FILE_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			fscanf(pFile, "%s", &cTemp[0]);

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
void CTutorialManager::Uninit(void)
{
	m_pTutorialManager = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorialManager::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CTutorialManager::Draw(void)
{
#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n敵の位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
#endif
}