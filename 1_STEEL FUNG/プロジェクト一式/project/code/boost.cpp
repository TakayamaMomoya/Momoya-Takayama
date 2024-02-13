//*****************************************************
//
// ブーストの処理[boost.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "boost.h"
#include "texture.h"
#include "fan2D.h"
#include "player.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR3 GAUGE_POS = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };	// ゲージの位置
const char* TEXTURE_PATH = "data\\TEXTURE\\UI\\boost00.png";	// テクスチャパス
const float RADIUS_GAUGE = 100.0f;	// ゲージの半径
const float INITIAL_ROT = D3DX_PI * 0.35f;
const float ANGLE_MAX = D3DX_PI * 0.3f;
}

//=====================================================
// コンストラクタ
//=====================================================
CBoost::CBoost(int nPriority) : CObject(nPriority)
{
	m_pObjectGauge = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CBoost::~CBoost()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBoost::Init(void)
{
	if (m_pObjectGauge == nullptr)
	{// ブーストゲージの生成
		m_pObjectGauge = CFan2D::Create();

		if (m_pObjectGauge != nullptr)
		{
			m_pObjectGauge->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectGauge->SetAngleMax(ANGLE_MAX);
			m_pObjectGauge->SetRotation(INITIAL_ROT);
			m_pObjectGauge->SetRadius(RADIUS_GAUGE);
			m_pObjectGauge->SetVtx();

			int nIdx = CTexture::GetInstance()->Regist(TEXTURE_PATH);
			m_pObjectGauge->SetIdxTexture(nIdx);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBoost::Uninit(void)
{
	if (m_pObjectGauge != nullptr)
	{// ゲージの破棄
		m_pObjectGauge->Uninit();
		m_pObjectGauge = nullptr;
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBoost::Update(void)
{
	// 変数宣言
	float fBoost = 0.0f;
	float fRate;
	D3DXVECTOR3 pos;

	// プレイヤーの取得
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	// プレイヤー情報の取得
	fBoost = pPlayer->GetBoost();
	CPlayer::SParam param = pPlayer->GetParam();

	// ブーストの割合を算出
	fRate = fBoost / param.fInitialBoost;

	if (m_pObjectGauge != nullptr)
	{// ゲージの設定
		// 向きの設定
		float fRot = INITIAL_ROT + ANGLE_MAX * (1.0f - fRate);

		m_pObjectGauge->SetRotation(fRot);

		// サイズ設定
		m_pObjectGauge->SetRateAngle(fRate);
		m_pObjectGauge->SetVtx();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBoost::Draw(void)
{

}

//=====================================================
// 生成処理
//=====================================================
CBoost *CBoost::Create(void)
{
	CBoost *pBoost = nullptr;

	if (pBoost == nullptr)
	{
		pBoost = new CBoost;

		if (pBoost != nullptr)
		{
			pBoost->Init();
		}
	}

	return pBoost;
}