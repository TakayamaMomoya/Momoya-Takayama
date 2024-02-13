//*****************************************************
//
// ランキングの処理[Rank.cpp]
// Author:酒井南勝
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************

#include "rank.h"
#include "object.h"

#include <stdio.h>

#include "record.h"

#include "object2D.h"
#include "number.h"

#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const char* RANK_TEX[RANK::NUM_MAX] =
	{// テクスチャのパス[種類]
		"data\\TEXTURE\\UI\\1st.png",
		"data\\TEXTURE\\UI\\2nd.png",
		"data\\TEXTURE\\UI\\3rd.png",
	};
}
//=====================================================
// コンストラクタ
//=====================================================
CRank::CRank()
{
	ZeroMemory(&m_infoVisualUi,sizeof(m_infoVisualUi));

	m_typeGenre = (CRecord::GENRE_TYPE)0;
	m_pFileName = nullptr;

	ZeroMemory(&m_aRankScore, sizeof(m_aRankScore));
	ZeroMemory(&m_aRankFace, sizeof(m_aRankFace));
	ZeroMemory(&m_aUpdateIdx, sizeof(m_aUpdateIdx));
	ZeroMemory(&m_aIsUpdate, sizeof(m_aIsUpdate));
}

//=====================================================
// デストラクタ
//=====================================================
CRank::~CRank()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRank::Init(void)
{
	// 初期設定
	Reset();
	Sort();

	// 取得したスコアでランキング設定
	Set();

	// 保存
	Save();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRank::Uninit(void)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apRank[nCount] != nullptr)
		{
			m_infoVisualUi.apRank[nCount]->Uninit();
			m_infoVisualUi.apRank[nCount] = nullptr;
		}

		if (m_infoVisualUi.apNum[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->Uninit();
			m_infoVisualUi.apNum[nCount] = nullptr;
		}
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CRank::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CRank::Draw(void)
{

}

//=====================================================
// 生成処理
//=====================================================
CRank* CRank::Create(CRecord::GENRE_TYPE typeGenre,const char* pFileName)
{
	CRank* pRank = new CRank;

	if (pRank != nullptr)
	{
		// 戦績種類を設定
		pRank->m_typeGenre = typeGenre;

		// ランキングファイル名を代入
		pRank->m_pFileName = (char*)pFileName;

		// ランクの初期化
		if (pRank->Init() != S_OK)
		{
			return nullptr;
		}
	}

	return pRank;
}

//====================================================
// UIの設定処理
//=====================================================
void CRank::SetUi(void)
{
	// 数字の生成
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		m_infoVisualUi.apNum[nCount] = CNumber::Create(4, 0);
	}

	// 順位の生成
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apRank[nCount] == nullptr)
		{
			m_infoVisualUi.apRank[nCount] = CObject2D::Create();

			if (m_infoVisualUi.apNum[nCount] != nullptr)
			{
				CTexture* pTexture = CTexture::GetInstance();

				if (pTexture != nullptr)
				{
					int nIdx = pTexture->Regist(RANK_TEX[nCount]);
					m_infoVisualUi.apRank[nCount]->SetIdxTexture(nIdx);
					m_infoVisualUi.apRank[nCount]->SetVtx();
				}
			}
		}
	}
}

//====================================================
// UIのリセット処理
//=====================================================
void CRank::ReSetUi(void)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apRank[nCount] != nullptr)
		{
			m_infoVisualUi.apRank[nCount]->Uninit();
			m_infoVisualUi.apRank[nCount] = nullptr;
		}

		if (m_infoVisualUi.apNum[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->Uninit();
			m_infoVisualUi.apNum[nCount] = nullptr;
		}
	}
}

//=====================================================
// 設定処理
//=====================================================
void CRank::Set(void)
{
	// 変数宣言
	bool bNewRank = false;
	int aNewScore[NUM_PLAYER] = {};

	// 戦績の取得
	CRecord* pRecord = CRecord::GetInstance();

	// ソート
	Sort();

	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (pRecord != nullptr)
		{
			switch (m_typeGenre)
			{
			case CRecord::GENRE_TYPE_DESTROY:

				aNewScore[nCount] = pRecord->GetDestroy(nCount);

				break;

			case CRecord::GENRE_TYPE_MADMAN:

				aNewScore[nCount] = pRecord->GetMadman(nCount);

				break;
			case CRecord::GENRE_TYPE_ENGINEER:

				aNewScore[nCount] = pRecord->GetEngineer();

				break;
			}
		}
	}

	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (m_typeGenre == CRecord::GENRE_TYPE_ENGINEER)
		{
			if (aNewScore[nCount] < m_aRankScore[RANK::NUM_MAX - 1])
			{// 最小値を越したら代入
				m_aRankScore[RANK::NUM_MAX - 1] = aNewScore[nCount];

				// 再ソート
				Sort();

				bNewRank = true;

				break;
			}
		}
		else
		{
			if (aNewScore[nCount] > m_aRankScore[RANK::NUM_MAX - 1])
			{// 最小値を越したら代入
				m_aRankScore[RANK::NUM_MAX - 1] = aNewScore[nCount];

				// 再ソート
				Sort();

				bNewRank = true;
			}
		}
	}

	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (bNewRank == true)
		{
			for (int nCnt = 0; nCnt < RANK::NUM_MAX; nCnt++)
			{// 足した値と合致する記録を探す
				if (aNewScore[nCount] == m_aRankScore[nCnt] &&
					m_aRankScore[nCnt] != 0)
				{// ニューレコード番号を記録
					m_aUpdateIdx[nCount] = nCnt;
					m_aIsUpdate[nCount] = true;
				}
			}
		}
	}

	// 保存処理
#ifndef _DEBUG
	Save();
#endif
}

//=====================================================
// ランキング情報リセット
//=====================================================
void CRank::Reset(void)
{
	//外部ファイル読み込み
	Load();
}

//=====================================================
// ランキングソート
//=====================================================
void CRank::Sort(void)
{
	for (int nCntRanking = 0; nCntRanking < RANK::NUM_MAX - 1; nCntRanking++)
	{//ランキングをソート
		// 左端の値を最大値とする
		int nTop = nCntRanking;

		for (int nCount2 = nCntRanking + 1; nCount2 < RANK::NUM_MAX; nCount2++)
		{//左の値と対象の値を比較
			if (m_typeGenre == CRecord::GENRE_TYPE_ENGINEER)
			{
				if (m_aRankScore[nTop] > m_aRankScore[nCount2])
				{//もし比較した数字が小さかったら
					nTop = nCount2;
				}
			}
			else
			{
				if (m_aRankScore[nTop] < m_aRankScore[nCount2])
				{//もし比較した数字が小さかったら
					nTop = nCount2;
				}
			}
		}

		//要素の入れ替え
		int nTemp = m_aRankScore[nCntRanking];
		m_aRankScore[nCntRanking] = m_aRankScore[nTop];
		m_aRankScore[nTop] = nTemp;
	}
}

//=====================================================
// ランキング情報保存
//=====================================================
void CRank::Save(void)
{
#ifndef _DEBUG

	//ポインタ宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(m_pFileName, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//バイナリファイルに書き込む
		fwrite(&m_aRankScore[0], sizeof(int), RANK::NUM_MAX, pFile);
		fwrite(&m_aRankFace[0], sizeof(int), RANK::NUM_MAX, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		assert(("ランキング保存に失敗", false));
	}
#endif
}

//=====================================================
//ランキング情報読み込み
//=====================================================
void CRank::Load(void)
{
	//ポインタ宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(m_pFileName, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//バイナリファイルから読み込む
		fread(&m_aRankScore[0], sizeof(int), RANK::NUM_MAX, pFile);
		fread(&m_aRankFace[0], sizeof(int), RANK::NUM_MAX, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		assert(("ランキング読み込みに失敗", false));
	}
}

//=====================================================
// 位置情報設定処理
//=====================================================
void CRank::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 posWhile, D3DXVECTOR3 posWhileRank)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apNum[nCount] != nullptr &&
			m_infoVisualUi.apRank[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->SetPosition(pos + (posWhile * (float)nCount));
			m_infoVisualUi.apRank[nCount]->SetPosition((pos + (posWhile * (float)nCount)) + posWhileRank);

			m_infoVisualUi.apNum[nCount]->SetValue(m_aRankScore[nCount], 4);
			m_infoVisualUi.apRank[nCount]->SetVtx();
		}
	}
}

//=====================================================
// サイズ情報設定処理
//=====================================================
void CRank::SetSize(float width, float height, float widthRank, float heightRank)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apNum[nCount] != nullptr &&
			m_infoVisualUi.apRank[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->SetSizeAll(width, height);
			m_infoVisualUi.apRank[nCount]->SetSize(widthRank, heightRank);

			m_infoVisualUi.apNum[nCount]->SetValue(m_aRankScore[nCount], 4);
			m_infoVisualUi.apRank[nCount]->SetVtx();
		}
	}
}

//=====================================================
// 色情報設定処理
//=====================================================
void CRank::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apNum[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->SetColor(col);
		}
	}
}

//=====================================================
// テクスチャ情報設定処理
//=====================================================
void CRank::SetTex(char* fileNameTex)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apNum[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->SetTex(fileNameTex);
		}
	}
}

