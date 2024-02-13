//*****************************************************
//
// リザルトの処理[result.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "result.h"
#include "manager.h"
#include "object2D.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "player.h"
#include "number.h"
#include "inputjoypad.h"
#include "playerManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float RESULT_WIDTH = 918.0f * 0.4f;	// 結果の幅
	const float RESULT_HEIGHT = 178.0f * 0.4f;	// 結果の高さ
	const char* RESULT_PATH = "data\\TEXTURE\\UI\\result.png";	// 結果のパス

	const float CAPTION_WIDTH = 100.0f;	// キャプションの幅
	const float CAPTION_HEIGHT = 35.0f;	// キャプションの高さ
	const char* CAPTION_PATH = "data\\TEXTURE\\UI\\caption.png";	// キャプションのパス

	const int NUM_PLACE = 1;	// 桁数
	const float ICON_WIDTH = 40.0f;	// 数字の幅
	const float ICON_HEIGHT = 40.0f;	// 数字の高さ

	const char* ICON_PATH[NUM_PLAYER] =
	{// プレイヤーアイコンのパス
		"data\\TEXTURE\\UI\\player01.png",
		"data\\TEXTURE\\UI\\player02.png",
		"data\\TEXTURE\\UI\\player03.png",
		"data\\TEXTURE\\UI\\player04.png"
	};
}

//====================================================
// コンストラクタ
//====================================================
CResult::CResult()
{
	m_state = STATE_NONE;
	m_pBg = nullptr;
	m_p2DResult = nullptr;
	ZeroMemory(&m_aInfoSurvived[0], sizeof(m_aInfoSurvived));
	m_nNumSuvived = 0;
}

//====================================================
// デストラクタ
//====================================================
CResult::~CResult()
{

}

//====================================================
// 生成処理
//====================================================
CResult *CResult::Create(bool bWin)
{
	CResult *pResult = nullptr;

	pResult = new CResult;

	if (pResult != nullptr)
	{
		pResult->Init();

		pResult->Create2D(bWin);
	}

	return pResult;
}

//====================================================
// ２Dオブジェクトの生成
//====================================================
void CResult::Create2D(bool bWin)
{
	int nIdxTexture;

	char *pPathCaption[2] =
	{
		"data\\TEXTURE\\UI\\result.png",
		"data\\TEXTURE\\UI\\result.png",
	};

	// 背景の生成
	m_pBg = CObject2D::Create(7);

	if (m_pBg != nullptr)
	{
		m_pBg->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f));

		m_pBg->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f));

		m_pBg->SetVtx();
	}

	// 見出しの生成
	m_p2DResult = CObject2D::Create(7);

	if (m_p2DResult != nullptr)
	{
		m_p2DResult->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.4f, 0.0f));

		m_p2DResult->SetSize(RESULT_WIDTH, RESULT_HEIGHT);

		m_p2DResult->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		nIdxTexture = CTexture::GetInstance()->Regist(pPathCaption[bWin]);

		m_p2DResult->SetIdxTexture(nIdxTexture);

		m_p2DResult->SetVtx();
	}
}

//====================================================
// 初期化処理
//====================================================
HRESULT CResult::Init(void)
{
	m_state = STATE_WAIT;

	// ゲームをリザルト状態にする
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		pGame->SetState(CGame::STATE_RESULT);
	}

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CResult::Uninit(void)
{
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	if (m_p2DResult != nullptr)
	{
		m_p2DResult->Uninit();
		m_p2DResult = nullptr;
	}

	ZeroMemory(&m_aInfoSurvived[0], sizeof(m_aInfoSurvived));

	// プレイヤーマネージャーの終了
	CPlayerManager *pPlayerManger = CPlayerManager::GetInstance();

	if (pPlayerManger != nullptr)
	{
		pPlayerManger->Uninit();
	}

	Release();
}

//====================================================
// 更新処理
//====================================================
void CResult::Update(void)
{
	// 操作処理
	Input();
}

//====================================================
// 操作処理
//====================================================
void CResult::Input(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
	{
		return;
	}

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, i))
		{
			// ゲームを終了状態にする
			CGame *pGame = CGame::GetInstance();

			if (pGame != nullptr)
			{
				pGame->SetState(CGame::STATE::STATE_END);
			}
		}
	}
}

//====================================================
// 描画処理
//====================================================
void CResult::Draw(void)
{

}

//====================================================
// 生存者の設定
//====================================================
void CResult::SetSurvived(CPlayer *pPlayer)
{
	if (m_nNumSuvived == NUM_PLAYER)
	{// 最大に達していた場合のエラー
		return;
	}

	if (m_aInfoSurvived[m_nNumSuvived].pSuvived == nullptr)
	{
		// 情報の保存
		m_aInfoSurvived[m_nNumSuvived].pSuvived = pPlayer;

		// 情報の表示
		DispSuvived(&m_aInfoSurvived[m_nNumSuvived]);

		m_nNumSuvived++;
	}
	else
	{
		assert(("リザルトでプレイヤー情報の受け取りに失敗", false));
	}
}

//====================================================
// 生存者の表示
//====================================================
void CResult::DispSuvived(SInfoSuvived *pInfo)
{
	int nIdx = m_nNumSuvived;

	if (pInfo->pSuvived != nullptr)
	{
		int nID = pInfo->pSuvived->GetID();

		if (pInfo->pIcon == nullptr)
		{// アイコンの生成
			pInfo->pIcon = CObject2D::Create(7);

			if (pInfo->pIcon != nullptr)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.6f + ICON_HEIGHT * nIdx * 2, 0.0f);

				pInfo->pIcon->SetPosition(pos);
				pInfo->pIcon->SetSize(ICON_WIDTH, ICON_HEIGHT);

				int nIdx = CTexture::GetInstance()->Regist(ICON_PATH[nID]);
				pInfo->pIcon->SetIdxTexture(nIdx);
				
				pInfo->pIcon->SetVtx();
			}
		}
		
		if (pInfo->pCaption == nullptr)
		{// キャプションの生成
			pInfo->pCaption = CObject2D::Create(7);

			if (pInfo->pCaption != nullptr)
			{
				pInfo->pCaption->SetSize(CAPTION_WIDTH, CAPTION_HEIGHT);
				pInfo->pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.35f + ICON_WIDTH + CAPTION_WIDTH, SCREEN_HEIGHT * 0.6f + ICON_HEIGHT * nIdx * 2, 0.0f));

				int nIdx = CTexture::GetInstance()->Regist(CAPTION_PATH);
				pInfo->pCaption->SetIdxTexture(nIdx);
				pInfo->pCaption->SetVtx();
			}
		}
	}
}