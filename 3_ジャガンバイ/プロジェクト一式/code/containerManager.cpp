//*****************************************************
//
// コンテナマネージャー[containerManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "containerManager.h"
#include "container.h"
#include "manager.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CContainerManager *CContainerManager::m_pContainerManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CContainerManager::CContainerManager()
{
	ZeroMemory(&m_apInfoSave[0], sizeof(m_apInfoSave));
}

//=====================================================
// デストラクタ
//=====================================================
CContainerManager::~CContainerManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CContainerManager *CContainerManager::Create(void)
{
	if (m_pContainerManager == nullptr)
	{// インスタンス生成
		m_pContainerManager = new CContainerManager;

		// 初期化処理
		m_pContainerManager->Init();
	}

	return m_pContainerManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CContainerManager::Init(void)
{
	// 読込処理
	Load();

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CContainerManager::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntAttack = 0;

	// ファイルから読み込む
	FILE *pFile = fopen("data\\TEXT\\container.txt", "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "TIME_SPAWN") == 0)
			{// リスポーン時間
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_fTimeSpawm);
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
void CContainerManager::Uninit(void)
{
	for (int i = 0; i < containermanager::MAX_SAVE; i++)
	{
		DeleteSaveInfo(i);
	}

	m_pContainerManager = nullptr;

	Release();
}

//=====================================================
// 個別の削除
//=====================================================
void CContainerManager::DeleteSaveInfo(int nIdx)
{
	if (m_apInfoSave[nIdx] != nullptr)
	{
		delete m_apInfoSave[nIdx];
		m_apInfoSave[nIdx] = nullptr;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CContainerManager::Update(void)
{
	for (int i = 0; i < containermanager::MAX_SAVE; i++)
	{
		if (m_apInfoSave[i] != nullptr)
		{
			float fTick = CManager::GetTick();

			m_apInfoSave[i]->fTimeSpawn -= fTick;

			if (m_apInfoSave[i]->fTimeSpawn <= 0.0f)
			{// コンテナをスポーンさせる
				CContainer *pContainer = CContainer::Create();

				if (pContainer != nullptr)
				{
					pContainer->SetPosition(m_apInfoSave[i]->pos);
				}

				// 情報の削除
				DeleteSaveInfo(i);
			}
		}
	}
}

//=====================================================
// 位置の保存
//=====================================================
void CContainerManager::SavePosition(D3DXVECTOR3 pos)
{
	for (int i = 0; i < containermanager::MAX_SAVE; i++)
	{
		if (m_apInfoSave[i] == nullptr)
		{
			m_apInfoSave[i] = new SInfoSave;

			m_apInfoSave[i]->pos = pos;

			m_apInfoSave[i]->fTimeSpawn = m_fTimeSpawm;

			break;
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CContainerManager::Draw(void)
{

}