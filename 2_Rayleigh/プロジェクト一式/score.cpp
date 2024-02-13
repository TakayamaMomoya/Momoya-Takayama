//*****************************************************
//
// スコアの処理[score.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "score.h"
#include "timer.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"
#include "number.h"
#include "UI.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE	(6)	// 桁数
#define SCORE_SPEED	(71)	// スコアの変わるスピード
#define RANK_BONUS	(0.05f)	// 1ランクにおけるボーナス倍率
#define RANK_RATE	(0.0004f)	// ランクの上がる倍率
#define MAX_TIME	(180)	// タイムボーナスが付与される最低時間
#define TIME_BONUS	(500)	// 一秒あたりのボーナス

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CScore *CScore::m_pScore = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CScore::CScore()
{
	m_pObjNumber = nullptr;
	m_nScore = 0;
	m_nSocreDest = 0;
	m_nIdxTexture = -1;
}

//=====================================================
// デストラクタ
//=====================================================
CScore::~CScore()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CScore::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CScore::Uninit(void)
{
	if (m_pObjNumber != nullptr)
	{
		m_pObjNumber = nullptr;
	}

	m_pScore = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CScore::Update(void)
{
	//スコア値上昇演出==============================
	if (m_nScore >= m_nSocreDest)
	{
		m_nScore = m_nSocreDest;
	}
	else
	{
		m_nScore += 50;
	}

	if (m_pObjNumber != nullptr)
	{
		m_pObjNumber->SetValue(m_nScore, NUM_PLACE);
	}
}

//=====================================================
// スコア加算
//=====================================================
int CScore::AddScore(int nValue)
{
	m_nSocreDest += nValue;

	return nValue;
}

//=====================================================
// 生成処理
//=====================================================
CScore *CScore::Create(void)
{
	if (m_pScore == nullptr)
	{
		m_pScore = new CScore;

		if (m_pScore->m_pObjNumber == nullptr)
		{
			m_pScore->m_pObjNumber = CNumber::Create(NUM_PLACE, m_pScore->m_nScore);
			m_pScore->m_pObjNumber->SetPosition(D3DXVECTOR3(1100.0f, 80.0f, 0.0f));
			m_pScore->m_pObjNumber->SetSizeAll(15.0f,32.5f);

			CUI *pObject2D = CUI::Create();

			if (pObject2D != nullptr)
			{
				pObject2D->SetPosition(D3DXVECTOR3(1030.0f, 80.0f, 0.0f));
				pObject2D->SetSize(70.0f, 35.0f);

				// テクスチャ番号取得
				int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\scoreBack.png");

				pObject2D->SetIdxTexture(nIdx);
				pObject2D->SetVtx();
			}
		}
	}

	return m_pScore;
}

//=====================================================
// リザルトスコア設定
//=====================================================
void CScore::SetResultScore(void)
{
	CTimer *pTimer = CTimer::GetInstance();
	int nTime = 0;
	int nScore = GetScore();

	if (pTimer != nullptr)
	{// 時間の取得
		nTime = pTimer->GetSecond();
	}

	int nBonus = 0;

	// ボーナスの計算
	nBonus = (MAX_TIME - nTime) * TIME_BONUS;

	// ボーナスを加算
	nScore += nBonus;

	CManager::SetScore(nScore);
}