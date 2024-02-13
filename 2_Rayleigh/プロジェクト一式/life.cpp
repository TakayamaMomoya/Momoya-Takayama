//*****************************************************
//
// ライフの処理[life.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "life.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "player.h"
#include "game.h"
#include "UI.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE	(6)	// 桁数
#define FRAME_WIDTH	(100.0f)	// フレームの横幅
#define FRAME_HEIGHT	(50.0f)	// フレームの縦の長さ
#define FRAME_POS	(D3DXVECTOR3(110.0f,70.0f,0.0f))	// フレームの初期位置
#define FRAME_PATH	"data\\TEXTURE\\UI\\lifeFrame.png"	// フレームのパス
#define PULSE_PATH	"data\\TEXTURE\\UI\\life.png"	// 波形のパス
#define DANGER_RATE	(0.4f)	// 危険時の割合
#define CAUTION_RATE	(0.6f)	// 注意時の割合
#define IN_TIME	(50)	// フェードイン時間
#define COL_FINE	(D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))	// 平穏時の色
#define COL_CAUTION	(D3DXCOLOR(1.0f,0.9f,0.0f,1.0f))	// 注意時の色
#define COL_DANGER	(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))	// 危険時の色

//=====================================================
// コンストラクタ
//=====================================================
CLife::CLife(int nPriority) : CObject(nPriority)
{
	m_pFrame = nullptr;
	m_pPulse = nullptr;
	m_pulseState = PULSESTATE_IN;
	m_nCntPulse = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CLife::~CLife()
{

}

//=====================================================
// 生成処理
//=====================================================
CLife *CLife::Create(void)
{
	CLife *pLife = nullptr;

	if (pLife == nullptr)
	{
		pLife = new CLife;

		pLife->Init();
	}

	return pLife;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CLife::Init(void)
{
	if (m_pFrame == nullptr)
	{// フレームの生成
		m_pFrame = CUI::Create();

		if (m_pFrame != nullptr)
		{
			m_pFrame->SetPosition(D3DXVECTOR3(FRAME_POS.x, FRAME_POS.y, 0.0f));
			m_pFrame->SetSize(FRAME_WIDTH, FRAME_HEIGHT);
			int nIdx = CTexture::GetInstance()->Regist(FRAME_PATH);
			m_pFrame->SetIdxTexture(nIdx);
			m_pFrame->SetCol(COL_FINE);
			m_pFrame->SetVtx();
		}
	}

	if (m_pPulse == nullptr)
	{// 波形の生成
		m_pPulse = CUI::Create();

		if (m_pPulse != nullptr)
		{
			m_pPulse->SetPosition(D3DXVECTOR3(FRAME_POS.x, FRAME_POS.y, 0.0f));
			m_pPulse->SetSize(FRAME_WIDTH, FRAME_HEIGHT);
			int nIdx = CTexture::GetInstance()->Regist(PULSE_PATH);
			m_pPulse->SetIdxTexture(nIdx);
			m_pPulse->SetCol(COL_FINE);
			m_pPulse->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CLife::Uninit(void)
{
	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}

	if (m_pPulse != nullptr)
	{
		m_pPulse->Uninit();
		m_pPulse = nullptr;
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CLife::Update(void)
{
	UpdatePulse();
}

//=====================================================
// 波形の更新
//=====================================================
void CLife::UpdatePulse(void)
{
	// プレイヤーの取得
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (m_pPulse == nullptr || pPlayer == nullptr)
	{
		return;
	}

	// 体力と初期体力の取得
	int nLife = pPlayer->GetLife();
	int nInitialLife = pPlayer->GetinitialLife();

	// 割合の算出
	float fRate = (float)nLife / (float)nInitialLife;

	D3DXCOLOR colPulse = m_pPulse->GetCol();
	D3DXCOLOR colDiff;
	D3DXCOLOR colDist;

	if (fRate <= DANGER_RATE)
	{
		colDist = COL_DANGER;
	}
	else if (fRate <= CAUTION_RATE)
	{
		colDist = COL_CAUTION;
	}
	else
	{
		colDist = COL_FINE;
	}

	colDiff.r = colDist.r - colPulse.r * 0.1f;
	colDiff.g = colDist.g - colPulse.g * 0.1f;
	colDiff.b = colDist.b - colPulse.b * 0.1f;

	colPulse.r += colDiff.r;
	colPulse.g += colDiff.g;
	colPulse.b += colDiff.b;

	m_pPulse->SetCol(colPulse);
	m_pFrame->SetCol(colPulse);

	// 波形のフェード
	FadePulse();
}

//=====================================================
// 波形のフェード
//=====================================================
void CLife::FadePulse(void)
{
	if (m_pPulse == nullptr)
	{
		return;
	}

	m_nCntPulse++;

	float fRate = (float)m_nCntPulse / (float)IN_TIME;

	switch (m_pulseState)
	{
	case PULSESTATE_IN:
	{
		float fWidth = fRate * FRAME_WIDTH;

		// 位置設定
		D3DXVECTOR3 pos = { FRAME_POS.x + fWidth - FRAME_WIDTH,FRAME_POS.y,0.0f };

		// サイズ設定
		m_pPulse->SetPosition(pos);
		m_pPulse->SetSize(fWidth, FRAME_HEIGHT);

		// テクスチャ座標設定
		D3DXVECTOR2 rightUp = { 0.0f,0.0f };
		D3DXVECTOR2 leftDown = { fRate,1.0f };
		m_pPulse->SetTex(rightUp, leftDown);

		m_pPulse->SetVtx();

		if (m_nCntPulse >= IN_TIME)
		{
			m_nCntPulse = 0;
		}
	}
		break;
	case PULSESTATE_OUT:
		break;
	default:
		break;
	}
}