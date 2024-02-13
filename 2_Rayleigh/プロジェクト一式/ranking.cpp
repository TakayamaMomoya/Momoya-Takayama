//*****************************************************
//
// ランキングの処理[Ranking.cpp]
//Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "ranking.h"
#include "fade.h"
#include <stdio.h>
#include "object.h"
#include "manager.h"
#include "texture.h"
#include "inputManager.h"
#include "skybox.h"
#include "universal.h"
#include "sound.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE					(6)										// スコアの桁数
#define TIMER_TRANS					(360)									// 遷移時間
#define RANKING_WIDTH			(80)	// 横幅
#define RANKING_HEIGHT			(80)	// 高さ
#define FLASH_SPEED					(10)	// 点滅スピード
#define RANKING_BIN_FILE			"data\\BIN\\ranking"								// ランキングファイル名
#define RANKING_TEX_FILE			"data\\TEXTURE\\UI\\Number000.png"		// ランキングテクスチャ名
#define BG_TEX_FILE					"data\\TEXTURE\\BG\\ResultBg000.png"	// ランキング背景テクスチャ名
#define RANKING_PATH	"data\\TEXTURE\\UI\\ranking.png"	// 項目テクスチャ名
#define ROTATION_SPEED	(0.001f)	// 回転速度

//=====================================================
// コンストラクタ
//=====================================================
CRanking::CRanking()
{
	ZeroMemory(&m_apNumber[0],sizeof(m_apNumber));
	m_nCntState = 0;
	m_nRankUpdate = -1;
	m_nScore = 0;
	m_nTimerTrans = 0;
	ZeroMemory(&m_aScore[0],sizeof(int) * NUM_RANK);
	m_state = STATE_NORMAL;
	m_pSkybox = nullptr;
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
	// 項目の生成
	CObject2D *pObject2D = CObject2D::Create();
	pObject2D->SetSize(300.0f,90.0f);
	pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 100.0f, 0.0f));

	int nIdx = CTexture::GetInstance()->Regist(RANKING_PATH);
	pObject2D->SetIdxTexture(nIdx);
	pObject2D->SetVtx();

	// 数字の生成
	for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(NUM_PLACE, 0);
	}

	// 初期設定
	Reset();
	Sort();

	// スコアの取得
	int nScore = CManager::GetScore();

	// 取得したスコアでランキング設定
	Set(nScore);

	// 保存
	Save();

	// スカイボックス
	m_pSkybox = CSkybox::Create();

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_BGM000);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRanking::Uninit(void)
{
	if (m_pSkybox != nullptr)
	{
		m_pSkybox->Uninit();
		m_pSkybox = nullptr;
	}

	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CRanking::Update(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CFade *pFade = CFade::GetInstance();
	CUniversal *pUniversal = CUniversal::GetInstance();

	// シーンの更新
	CScene::Update();

	// カウントアップ
	m_nCntState++;

	if (m_nRankUpdate != -1)
	{// ランキングが更新された
		// 該当のランクを点滅==================================
		
		if (m_nCntState >= FLASH_SPEED)
		{// カウンタが一定の値に達したら
			switch (m_state)
			{
			case STATE_NORMAL:
				m_state = STATE_DIFF;
				break;
			case STATE_DIFF:
				m_state = STATE_NORMAL;
				break;
			}

			m_nCntState = 0;
		}

		// 変数宣言
		D3DXCOLOR colRanking = {0.0f,0.0f,0.0f,0.0f};

		switch (m_state)
		{// 状態による分岐
		case STATE_NORMAL:
			colRanking = { 1.0f,1.0f,1.0f,1.0f};
			break;
		case STATE_DIFF:
			colRanking = { 0.5f,1.0f,0.5f,1.0f };
			break;
		}

		if (m_apNumber[m_nRankUpdate] != nullptr)
		{
			m_apNumber[m_nRankUpdate]->SetColor(colRanking);
		}
	}

	// 画面遷移==========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
	{//ENTERキーが押されたら
		//タイトルに移行
		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_TITLE);
		}
	}

	// スカイボックスの回転
	if (m_pSkybox != nullptr)
	{
		D3DXVECTOR3 rot = m_pSkybox->GetRot();

		rot.y += ROTATION_SPEED;

		pUniversal->LimitRot(&rot.y);

		m_pSkybox->SetRot(rot);
	}

	// 遷移タイマー進行
	m_nTimerTrans++;
}

//=====================================================
// 描画処理
//=====================================================
void CRanking::Draw(void)
{

}

//=====================================================
// 設定処理
//=====================================================
void CRanking::Set(int nScore)
{
	// 変数宣言
	D3DXVECTOR3 pos;
	char *apPath[NUM_RANK] = 
	{
		"data\\TEXTURE\\UI\\1st.png",
		"data\\TEXTURE\\UI\\2nd.png",
		"data\\TEXTURE\\UI\\3rd.png",
		"data\\TEXTURE\\UI\\4th.png",
		"data\\TEXTURE\\UI\\5th.png",
	};

	// ソート
	Sort();

	if (nScore > m_aScore[NUM_RANK - 1])
	{// 最小値を越したら代入
		m_aScore[NUM_RANK - 1] = nScore;

		// 再ソート
		Sort();

		for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
		{// 足した値と合致する記録を探す
			if (nScore == m_aScore[nCnt])
			{// ニューレコード番号を記録
				m_nRankUpdate = nCnt;
			}
		}
	}

	if (m_aScore != nullptr)
	{
		// 数字の設定
		for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
		{
			if (m_apNumber[nCnt] != nullptr)
			{
				// 値の設定
				m_apNumber[nCnt]->SetValue(m_aScore[nCnt], NUM_PLACE);

				// 位置の設定
				pos = D3DXVECTOR3{ 450.0f,250.0f + RANKING_HEIGHT * nCnt,0.0f };

				m_apNumber[nCnt]->SetPosition(pos);

				pos = m_apNumber[nCnt]->GetPosition();

				m_apNumber[nCnt]->SetSizeAll(RANKING_WIDTH * 0.5f, RANKING_HEIGHT * 0.5f);

				// 順位の生成
				CObject2D *pObject2D = CObject2D::Create();
				pObject2D->SetSize(60.0f, 40.0f);
				pos.x -= 100 + RANKING_WIDTH * 0.5f;

				pObject2D->SetPosition(pos);

				int nIdx = CTexture::GetInstance()->Regist(apPath[nCnt]);
				pObject2D->SetIdxTexture(nIdx);
				pObject2D->SetVtx();
			}
		}
	}

	// 保存処理
#ifndef _DEBUG
	Save();
#endif
}

//=====================================================
// ランキングソート
//=====================================================
void CRanking::Sort(void)
{
	for (int nCntRanking = 0; nCntRanking < NUM_RANK - 1; nCntRanking++)
	{//ランキングをソート
	 //左端の値を最大値とする
		int nTop = nCntRanking;

		for (int nCount2 = nCntRanking + 1; nCount2 < NUM_RANK; nCount2++)
		{//左の値と対象の値を比較
			if (m_aScore[nTop] < m_aScore[nCount2])
			{//もし比較した数字が小さかったら
				nTop = nCount2;
			}
		}

		//要素の入れ替え
		int nTemp = m_aScore[nCntRanking];
		m_aScore[nCntRanking] = m_aScore[nTop];
		m_aScore[nTop] = nTemp;
	}
}

//=====================================================
// ランキング情報リセット
//=====================================================
void CRanking::Reset(void)
{
	//外部ファイル読み込み
	Load();

#if 1
	//ランキング初期設定
	m_aScore[0] = 32;
	m_aScore[1] = 31;
	m_aScore[2] = 30;
	m_aScore[3] = 29;
	m_aScore[4] = 28;
#endif
}

//=====================================================
// ランキング情報保存
//=====================================================
void CRanking::Save(void)
{
	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(RANKING_BIN_FILE, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	    //バイナリファイルに書き込む
		fwrite(&m_aScore[0], sizeof(int), NUM_RANK, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合

	}
}

//=====================================================
//ランキング情報読み込み
//=====================================================
void CRanking::Load(void)
{
	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(RANKING_BIN_FILE, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	    //バイナリファイルから読み込む
		fread(&m_aScore[0], sizeof(int), NUM_RANK, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
	}
}