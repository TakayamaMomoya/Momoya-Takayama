//*****************************************************
//
// ランキングの処理[Ranking.cpp]
// Author:酒井南勝
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************

#include "ranking.h"
#include "main.h"

#include <stdio.h>

#include "manager.h"
#include "camera.h"
#include "record.h"

#include "object2D.h"
#include "number.h"

#include "rank.h"
#include "rank_acting.h"

#include "texture.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputmouse.h"
#include "sound.h"

#include "fade.h"

//*****************************************************
// マクロ定義
//*****************************************************

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	// 戦績文
	const D3DXVECTOR3 RECORD_TEXT_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.15f, 0.0f);	// 戦績文の位置
	const float RECORD_TEXT_SIZE = 250.0f;										// 戦績文の大きさ
	const float RECORD_TEXT_WIDTH = 1.0f * RECORD_TEXT_SIZE;					// 戦績文の幅
	const float RECORD_TEXT_HEIGHT = 0.2f * RECORD_TEXT_SIZE;					// 戦績文の高さ
	const char* RECORD_TEXT_TEX = "data\\TEXTURE\\UI\\renk_text_record.png";	// 戦績文のパス

	// 戦績種類アイコン
	const D3DXVECTOR3 GENRE_ICON_POS[CRecord::GENRE_TYPE_MAX] =
	{// 位置[種類]
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.55f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.55f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.55f, 0.0f),
	};
	const float GENRE_ICON_SIZE[CRecord::GENRE_TYPE_MAX] =
	{// 大きさ[種類]
		250.0f,
		250.0f,
		250.0f,
	};
	const float GENRE_ICON_WIDTH[CRecord::GENRE_TYPE_MAX] =
	{// 横幅[種類]
		0.5f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.5f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.5f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
	};
	const float GENRE_ICON_HEIGHT[CRecord::GENRE_TYPE_MAX] =
	{// 縦幅[種類]
		0.4f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.6f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.5f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
	};
	const char* GENRE_ICON_TEX[CRecord::GENRE_TYPE_MAX] =
	{// テクスチャのパス[種類]
		"data\\TEXTURE\\UI\\Icon_destroyer.png",
		"data\\TEXTURE\\UI\\Icon_madman.png",
		"data\\TEXTURE\\UI\\Icon_engineer.png",
	};

	// 戦績種類文
	const D3DXVECTOR3 GENRE_TEXT_POS[CRecord::GENRE_TYPE_MAX] =
	{// 位置[種類]
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.35f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.35f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.35f, 0.0f),
	};
	const float GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_MAX] =
	{// 大きさ[種類]
		175.0f,
		175.0f,
		175.0f,
	};
	const float GENRE_TEXT_WIDTH[CRecord::GENRE_TYPE_MAX] =
	{// 横幅[種類]
		0.8f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.8f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.8f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
	};
	const float GENRE_TEXT_HEIGHT[CRecord::GENRE_TYPE_MAX] =
	{// 縦幅[種類]
		0.3f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.3f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.3f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
	};
	const char* GENRE_TEXT_TEX[CRecord::GENRE_TYPE_MAX] =
	{// テクスチャのパス[種類]
		"data\\TEXTURE\\UI\\rank_text_destroyer.png",
		"data\\TEXTURE\\UI\\rank_text_madman.png",
		"data\\TEXTURE\\UI\\rank_text_speedy.png",
	};

	// 顔表示
	const D3DXVECTOR3 FACE_POS[NUM_PLAYER] =
	{// 位置[プレイヤー番号]
		D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.3f,  0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.425f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.550f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.675f,0.0f),
	};
	const float FACE_SIZE = 70.0f;
	const float FACE_WIDTH = 0.5f * FACE_SIZE;
	const float FACE_HEIGHT = 0.6f * FACE_SIZE;
	const char* FACE_FILE_NAME[NUM_PLAYER] =
	{// ファイル名 [プレイヤー番号]
		"data\\TEXTURE\\UI\\player01.png",
		"data\\TEXTURE\\UI\\player02.png",
		"data\\TEXTURE\\UI\\player03.png",
		"data\\TEXTURE\\UI\\player04.png",
	};

	// 数字
	const D3DXVECTOR3 NUMBER_POS[NUM_PLAYER] =
	{// 位置[プレイヤー番号]
		D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.3f,  0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.425f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.550f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.675f,0.0f),
	};
	const float NUMBER_SIZE = 25.0f;
	const float NUMBER_WIDTH = 1.0f * NUMBER_SIZE;
	const float NUMBER_HEIGHT = 1.0f * NUMBER_SIZE;
	const char* NUMBER_TEX = nullptr;

	const int GENRE_CHANGE_COUNT = 300;		// 種類を変えるまでの時間

	const char* RANKING_BIN_FILE[CRecord::GENRE_TYPE_MAX] =
	{// ランキングのファイル名
		"data\\BINARY\\ranking_destroyer",
		"data\\BINARY\\ranking_madman",
		"data\\BINARY\\ranking_engineer",
	};

	// ランキングの数字
	const D3DXVECTOR3 RANK_POS_NUM = D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.5f, 0.0f);
	const D3DXVECTOR3 RANK_POS_NUM_WIDTH = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.1f, 0.0f);
	const D3DXVECTOR3 RANK_POS_WIDTH = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);

	const float RANK_NUM_SIZE = 25.0f;
	const float RANK_NUM_WIDTH = 1.0f * RANK_NUM_SIZE;
	const float RANK_NUM_HEIGHT = 1.0f * RANK_NUM_SIZE;

	const float RANK_SIZE = 50.0f;
	const float RANK_WIDTH = 0.5f * RANK_SIZE;
	const float RANK_HEIGHT = 0.4f * RANK_SIZE;

	const float ALPHA_UPPER = 1.0f;			// α値の上限量
	const float ALPHA_LOWER = 0.25f;		// α値の下限量
	const float ALPHA_CHANGE = 0.1f;		// α値の変化量
}

//=====================================================
// コンストラクタ
//=====================================================
CRanking::CRanking()
{
	ZeroMemory(&m_infoVisualUi, sizeof(m_infoVisualUi));
	ZeroMemory(&m_apRank[0], sizeof(m_apRank));
	m_pRankActing = nullptr;
	m_typeGenre = (CRecord::GENRE_TYPE)0;
	m_nGenreCnt = 0;

	for (int nCount = 0; nCount < CRecord::GENRE_TYPE_MAX; nCount++)
	{
		ZeroMemory(&m_aIsAlphaChange[nCount][0], sizeof(m_aIsAlphaChange[nCount]));
	}
}

//=====================================================
// デストラクタ
//=====================================================
CRanking::~CRanking()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRanking::Init(void)
{
	// 戦績のポインタを取得
	CRecord* pRecord = CRecord::GetInstance();

	if (pRecord != nullptr)
	{
		// 戦績のソート
		pRecord->AllSort();
	}

	// UIの初期化処理
	if (InitUi() != S_OK)
	{
		return E_FAIL;
	}
	
	// ランクの初期化処理
	if (InitRank() != S_OK)
	{
		return E_FAIL;
	}

	if (m_apRank[m_typeGenre] != nullptr)
	{
		// ランクの設定処理
		SetRank();
	}

	// ランキング演技の生成
	m_pRankActing = CRankActing::Create();
	
	if (m_pRankActing == nullptr)
	{
		return E_FAIL;
	}

	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_BGM_RANKING);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRanking::Uninit(void)
{
	// 戦績の終了処理
	CRecord* pRecord = CRecord::GetInstance();

	if (pRecord != nullptr)
	{
		pRecord->Uninit();
	}

	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CRanking::Update(void)
{
	// 入力情報取得
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

	CFade *pFade = CFade::GetInstance();

	// シーンの更新
	CScene::Update();

	// 戦績UIの更新
	UpdateUiRecord();

	// 演技の更新処理
	if (m_pRankActing != nullptr)
	{
		m_pRankActing->Update();
	}

	// 数値の更新処理
	UpdateNum();

	// 画面遷移==========================================
	if (pKeyboard->GetTrigger(DIK_RETURN) ||
		pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
	{//ENTERキーが押されたら
		//タイトルに移行
		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_TITLE);
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CRanking::Draw(void)
{

}

//=====================================================
// UIの初期化処理
//=====================================================
HRESULT CRanking::InitUi(void)
{
	// テクスチャのポインタを取得
	CTexture* pTexture = CTexture::GetInstance();

	// 戦績のポインタを取得
	CRecord* pRecord = CRecord::GetInstance();

	// レコードテキストの生成
	m_infoVisualUi.pRecordText = CObject2D::Create(7);

	if (m_infoVisualUi.pRecordText != nullptr)
	{
		m_infoVisualUi.pRecordText->SetPosition(RECORD_TEXT_POS);
		m_infoVisualUi.pRecordText->SetSize(RECORD_TEXT_WIDTH, RECORD_TEXT_HEIGHT);

		if (pTexture != nullptr)
		{
			int nIdx = pTexture->Regist(RECORD_TEXT_TEX);
			m_infoVisualUi.pRecordText->SetIdxTexture(nIdx);
		}

		m_infoVisualUi.pRecordText->SetVtx();
	}
	if (m_infoVisualUi.pRecordText == nullptr)
	{
		return E_FAIL;
	}

	// 種類アイコンの生成
	m_infoVisualUi.pGenreIcon = CObject2D::Create(7);

	if (m_infoVisualUi.pGenreIcon == nullptr)
	{
		return E_FAIL;
	}

	// 種類のテキストの生成
	m_infoVisualUi.pGenreText = CObject2D::Create(7);

	if (m_infoVisualUi.pGenreText == nullptr)
	{
		return E_FAIL;
	}

	if (pRecord != nullptr)
	{
		int nMaxPlayer = pRecord->GetNumSuvived();

		for (int nCount = 0; nCount < nMaxPlayer; nCount++)
		{
			// プレイヤー顔の生成
			m_infoVisualUi.apFace[nCount] = CObject2D::Create(7);

			if (m_infoVisualUi.apFace[nCount] == nullptr)
			{
				return E_FAIL;
			}

			// 数字の生成
			m_infoVisualUi.apNumber[nCount] = CNumber::Create(4, 0);

			if (m_infoVisualUi.apNumber[nCount] == nullptr)
			{
				return E_FAIL;
			}
		}
	}

	// 戦績種類の設定処理
	SetUiRecord();

	return S_OK;
}

//=====================================================
// UIの初期化処理
//=====================================================
HRESULT CRanking::InitRank(void)
{
	for (int nCount = 0; nCount < CRecord::GENRE_TYPE_MAX; nCount++)
	{
		if (m_apRank[nCount] == nullptr)
		{
			// ランクの生成
			m_apRank[nCount] = CRank::Create((CRecord::GENRE_TYPE)nCount,RANKING_BIN_FILE[nCount]);

			if (m_apRank[nCount] == nullptr)
			{
				return E_FAIL;
			}
		}
	}
	
	return S_OK;
}

//=====================================================
// 戦績の設定処理
//=====================================================
void CRanking::SetUiRecord(void)
{
	// テクスチャのポインタを取得
	CTexture* pTexture = CTexture::GetInstance();

	// 戦績のポインタを取得
	CRecord* pRecord = CRecord::GetInstance();

	// 戦績アイコンの設定処理
	if (m_infoVisualUi.pGenreIcon != nullptr)
	{
		m_infoVisualUi.pGenreIcon->SetPosition(GENRE_ICON_POS[m_typeGenre]);
		m_infoVisualUi.pGenreIcon->SetSize(GENRE_ICON_WIDTH[m_typeGenre], GENRE_ICON_HEIGHT[m_typeGenre]);

		if (pTexture != nullptr)
		{
			int nIdx = pTexture->Regist(GENRE_ICON_TEX[m_typeGenre]);
			m_infoVisualUi.pGenreIcon->SetIdxTexture(nIdx);
		}

		m_infoVisualUi.pGenreIcon->SetVtx();
	}

	// 戦績テキストの設定処理
	if (m_infoVisualUi.pGenreText != nullptr)
	{
		m_infoVisualUi.pGenreText->SetPosition(GENRE_TEXT_POS[m_typeGenre]);
		m_infoVisualUi.pGenreText->SetSize(GENRE_TEXT_WIDTH[m_typeGenre], GENRE_TEXT_HEIGHT[m_typeGenre]);

		if (pTexture != nullptr)
		{
			int nIdx = pTexture->Regist(GENRE_TEXT_TEX[m_typeGenre]);
			m_infoVisualUi.pGenreText->SetIdxTexture(nIdx);
		}

		m_infoVisualUi.pGenreText->SetVtx();
	}

	if (pRecord != nullptr)
	{
		int nMaxPlayer = pRecord->GetNumSuvived();

		for (int nCount = 0; nCount < nMaxPlayer; nCount++)
		{
			if (pRecord != nullptr)
			{
				int nRank = 0;		// 順位
				int nNumRank = 0;	// 順位での値

				// 種類ごとに戦績の順位を取得
				switch (m_typeGenre)
				{
				case CRecord::GENRE_TYPE_DESTROY:

					nRank = pRecord->GetDestroyRank(nCount);
					nNumRank = pRecord->GetDestroy(nCount);

					break;
				case CRecord::GENRE_TYPE_MADMAN:

					nRank = pRecord->GetMadmanRank(nCount);
					nNumRank = pRecord->GetMadman(nCount);

					break;
				case CRecord::GENRE_TYPE_ENGINEER:

					nRank = pRecord->GetEngineerRank();
					nNumRank = pRecord->GetEngineer();

					break;
				}

				// 顔UIの設定処理
				if (m_infoVisualUi.apFace[nCount] != nullptr)
				{
					// 顔UIを順位の位置に変更
					m_infoVisualUi.apFace[nCount]->SetPosition(FACE_POS[nRank]);
					m_infoVisualUi.apFace[nCount]->SetSize(FACE_WIDTH, FACE_HEIGHT);

					if (pTexture != nullptr)
					{
						int nIdx = pTexture->Regist(FACE_FILE_NAME[nCount]);
						m_infoVisualUi.apFace[nCount]->SetIdxTexture(nIdx);
					}

					m_infoVisualUi.apFace[nCount]->SetVtx();
				}

				// 数字の設定処理
				if (m_infoVisualUi.apNumber[nCount] != nullptr)
				{
					m_infoVisualUi.apNumber[nCount]->SetPosition(NUMBER_POS[nRank]);
					m_infoVisualUi.apNumber[nCount]->SetSizeAll(NUMBER_WIDTH, NUMBER_WIDTH);
					m_infoVisualUi.apNumber[nCount]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					m_infoVisualUi.apNumber[nCount]->SetValue(nNumRank, 4);

					if (nRank == 0)
					{
						m_infoVisualUi.apNumber[nCount]->SetTex("data\\TEXTURE\\UI\\number002.png");
					}
				}
			}
		}
	}
}

//=====================================================
// ランクの設定処理
//=====================================================
void CRanking::SetRank(void)
{
	if (m_apRank[m_typeGenre] != nullptr)
	{
		// ランクUIの生成
		m_apRank[m_typeGenre]->SetUi();

		// 設定処理
		m_apRank[m_typeGenre]->SetPos(RANK_POS_NUM, RANK_POS_NUM_WIDTH, RANK_POS_WIDTH);
		m_apRank[m_typeGenre]->SetSize(RANK_NUM_WIDTH, RANK_NUM_HEIGHT, RANK_WIDTH, RANK_HEIGHT);
	}
}

//=====================================================
// 戦績UIの更新
//=====================================================
void CRanking::UpdateUiRecord(void)
{
	// 種類カウントが設定値以下
	if (m_nGenreCnt <= GENRE_CHANGE_COUNT)
	{
		// カウントを増やす
		m_nGenreCnt++;
	}
	else
	{
		// カウントを初期化
		m_nGenreCnt = 0;

		if (m_apRank[m_typeGenre] != nullptr)
		{
			// ランクの数値をリセット
			m_apRank[m_typeGenre]->ReSetUi();
		}

		// 戦績種類を増やす
		m_typeGenre = (CRecord::GENRE_TYPE)(m_typeGenre + 1);

		// 戦績種類が最大値
		if (m_typeGenre >= CRecord::GENRE_TYPE_MAX)
		{
			// 戦績種類を初期化
			m_typeGenre = (CRecord::GENRE_TYPE)0;
		}

		// 戦績種類の設定
		SetUiRecord();

		// ランクの数字を表示
		SetRank();
	}
}

//=====================================================
// 数値の更新
//=====================================================
void CRanking::UpdateNum(void)
{
	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (m_apRank[m_typeGenre] != nullptr)
		{
			if (m_apRank[m_typeGenre]->GetIsUpdate(nCount))
			{
				if (m_infoVisualUi.apNumber[nCount] != nullptr &&
					m_apRank[m_typeGenre]->GetNum(m_apRank[m_typeGenre]->GetUpdateIdx(nCount)) != nullptr)
				{
					// 色の情報取得
					D3DXCOLOR col = m_infoVisualUi.apNumber[nCount]->GetColor();

					// α値の増減処理
					if (m_aIsAlphaChange[m_typeGenre][nCount] == false)
					{
						col.a -= ALPHA_CHANGE;
					}
					else if (m_aIsAlphaChange[m_typeGenre][nCount] == true)
					{
						col.a += ALPHA_CHANGE;
					}

					// α値の増減方向切り替え処理
					if (col.a <= ALPHA_LOWER)
					{
						m_aIsAlphaChange[m_typeGenre][nCount] = true;
					}
					else if (col.a >= ALPHA_UPPER)
					{
						m_aIsAlphaChange[m_typeGenre][nCount] = false;
					}

					m_infoVisualUi.apNumber[nCount]->SetColor(col);
					m_apRank[m_typeGenre]->GetNum(m_apRank[m_typeGenre]->GetUpdateIdx(nCount))->SetColor(col);
				}
			}
		}
	}
}