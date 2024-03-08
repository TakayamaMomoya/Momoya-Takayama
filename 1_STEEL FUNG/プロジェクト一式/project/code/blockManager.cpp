//*****************************************************
//
// ブロック管理[blockManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "BlockManager.h"
#include <stdio.h>

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const char* FILE_PATH = "data\\TEXT\\map.txt";	// マップ情報
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CBlockManager *CBlockManager::m_pBlockManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CBlockManager::CBlockManager()
{
	m_pInfoBlock = nullptr;
	m_nNumInfoBlock = 0;
	m_pHead = nullptr;
	m_pTail = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CBlockManager::~CBlockManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CBlockManager *CBlockManager::Create(void)
{
	if (m_pBlockManager == nullptr)
	{// インスタンス生成
		m_pBlockManager = new CBlockManager;

		// 初期化処理
		m_pBlockManager->Init();
	}

	return m_pBlockManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBlockManager::Init(void)
{
	Load();

	return S_OK;
}

//=====================================================
// 読み込み処理
//=====================================================
void CBlockManager::Load(void)
{
	//変数宣言
	char cTemp[256];
	int nCntBlock= 0;

	//ファイルから読み込む
	FILE *pFile = fopen(FILE_PATH, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		while (true)
		{
			//文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "NUM_BLOCK") == 0)
			{
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_nNumInfoBlock);

				if (m_pInfoBlock == nullptr)
				{// ブロック情報の確保
					m_pInfoBlock = new SInfoBlock[m_nNumInfoBlock];

					ZeroMemory(m_pInfoBlock, sizeof(SInfoBlock) * m_nNumInfoBlock);
				}
			}

			if (strcmp(cTemp, "INFOBLOCK") == 0)
			{
				while (true)
				{
					//文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "TAG") == 0)
					{
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%s", &m_pInfoBlock[nCntBlock].aTag[0]);
					}

					if (strcmp(cTemp, "MODEL") == 0)
					{
						(void)fscanf(pFile, "%s", &cTemp[0]);
						(void)fscanf(pFile, "%s", &cTemp[0]);

						m_pInfoBlock[nCntBlock].nIdxModel = CModel::Load(&cTemp[0]);
					}

					if (strcmp(cTemp, "SNAG") == 0)
					{
						int nBool;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &nBool);

						if (nBool == 1)
						{
							m_pInfoBlock[nCntBlock].bSnag = true;
						}
						else
						{
							m_pInfoBlock[nCntBlock].bSnag = false;
						}
					}

					if (strcmp(cTemp, "END_INFOBLOCK") == 0)
					{
						nCntBlock++;

						break;
					}
				}
			}

			// ロード情報の読み込み
			LoadMap(pFile, &cTemp[0]);

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		fclose(pFile);
	}
}

//=====================================================
// マップ配置の読み込み
//=====================================================
void CBlockManager::LoadMap(FILE *pFile, char *pTemp)
{
	if (strcmp(pTemp, "SETBLOCK") == 0)
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int nIdx;

		while (true)
		{
			//文字読み込み
			(void)fscanf(pFile, "%s", pTemp);

			if (strcmp(pTemp, "IDX") == 0)
			{// インデックス
				(void)fscanf(pFile, "%s", pTemp);

				(void)fscanf(pFile, "%d", &nIdx);
			}

			if (strcmp(pTemp, "POS") == 0)
			{// 位置
				(void)fscanf(pFile, "%s", pTemp);

				(void)fscanf(pFile, "%f", &pos.x);
				(void)fscanf(pFile, "%f", &pos.y);
				(void)fscanf(pFile, "%f", &pos.z);
			}

			if (strcmp(pTemp, "ROT") == 0)
			{// 向き
				(void)fscanf(pFile, "%s", pTemp);

				(void)fscanf(pFile, "%f", &rot.x);
				(void)fscanf(pFile, "%f", &rot.y);
				(void)fscanf(pFile, "%f", &rot.z);
			}

			if (strcmp(pTemp, "END_SETBLOCK") == 0)
			{
				// ブロック生成
				CBlock *pBlock = CBlock::Create(m_pInfoBlock[nIdx].nIdxModel);

				if (pBlock != nullptr)
				{
					pBlock->SetPosition(pos);
					pBlock->SetRotation(rot);
					pBlock->SetIdx(nIdx);

					if (m_pInfoBlock[nIdx].bSnag == false)
					{
						pBlock->DeleteCollision();
						pBlock->EnableShadow(false);
					}
				}

				break;
			}
		}
	}
}

//=====================================================
// マップ情報保存
//=====================================================
void CBlockManager::Save(char *pPath)
{
	FILE *pFile = nullptr;

	pFile = fopen(pPath, "w");

	if (pFile != nullptr)
	{
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# マップのブロック配置情報[map.txt]\n");
		fprintf(pFile, "# Author：Momoya Takayama\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "SCRIPT\n\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# モデル数\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "NUM_BLOCK = %d\n\n", m_nNumInfoBlock);

		for (int i = 0;i < m_nNumInfoBlock;i++)
		{
			char *pPathBlock = CModel::GetPath(m_pInfoBlock[i].nIdxModel);

			fprintf(pFile, "#%d番目のブロック\n", i);
			fprintf(pFile, "INFOBLOCK\n");
			fprintf(pFile, " TAG = %s\n", &m_pInfoBlock[i].aTag[0]);
			fprintf(pFile, " MODEL = %s\n", pPathBlock);
			fprintf(pFile, " SNAG = %d\n", m_pInfoBlock[i].bSnag);
			fprintf(pFile, "END_INFOBLOCK\n\n");
		}

		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# 配置情報\n");
		fprintf(pFile, "#====================================================================\n");

		int nCntBlock = 0;

		CBlock *pBlock = GetHead();

		while (pBlock != nullptr)
		{
			CBlock *pBlockNext = pBlock->GetNext();

			if (pBlock != nullptr)
			{
				D3DXVECTOR3 pos = pBlock->GetPosition();
				D3DXVECTOR3 rot = pBlock->GetRotation();
				int nIdx = pBlock->GetIdx();

				fprintf(pFile, "SETBLOCK\n");

				fprintf(pFile, " IDX = %d\n", nIdx);
				fprintf(pFile, " POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
				fprintf(pFile, " ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);

				fprintf(pFile, "END_SETBLOCK\n\n");

			}

			pBlock = pBlockNext;
		}


		fprintf(pFile, "END_SCRIPT\n");

		fclose(pFile);
	}
}

//=====================================================
// 全削除処理
//=====================================================
void CBlockManager::DeleteAll(void)
{
	CBlock *pBlock = GetHead();

	while (pBlock != nullptr)
	{
		CBlock *pBlockNext = pBlock->GetNext();

		if (pBlock != nullptr)
		{
			pBlock->Uninit();
		}

		pBlock = pBlockNext;
	}
}

//=====================================================
// 終了処理
//=====================================================
void CBlockManager::Uninit(void)
{
	m_pBlockManager = nullptr;

	if (m_pInfoBlock != nullptr)
	{
		delete[] m_pInfoBlock;
		m_pInfoBlock = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBlockManager::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CBlockManager::Draw(void)
{
#ifdef _DEBUG

#endif
}