//*****************************************************
//
// 体力表示の処理[life.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "life.h"
#include "texture.h"
#include "fan2D.h"
#include "player.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR3 GAUGE_POS = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };	// ゲージの位置
const char* TEXTURE_PATH = "data\\TEXTURE\\UI\\boost00.png";	// ゲージのパス
const float RADIUS_GAUGE = 315.0f;	// ゲージの半径
const float INITIAL_ROT = D3DX_PI * 1.2f;
const float ANGLE_MAX = D3DX_PI * 0.3f;
}

//=====================================================
// コンストラクタ
//=====================================================
CLife::CLife(int nPriority) : CObject(nPriority)
{
	m_pObjectGauge = nullptr;
	m_pObjectFrame = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CLife::~CLife()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CLife::Init(void)
{
	if (m_pObjectFrame == nullptr)
	{// フレームの生成
		m_pObjectFrame = CFan2D::Create();

		if (m_pObjectFrame != nullptr)
		{
			m_pObjectFrame->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectFrame->SetAngleMax(ANGLE_MAX);
			m_pObjectFrame->SetRotation(INITIAL_ROT);
			m_pObjectFrame->SetRadius(RADIUS_GAUGE);
			m_pObjectFrame->SetVtx();

			D3DXCOLOR col = universal::ConvertRGB(255, 255, 255, 60);

			m_pObjectFrame->SetCol(col);

			int nIdx = CTexture::GetInstance()->Regist(TEXTURE_PATH);
			m_pObjectFrame->SetIdxTexture(nIdx);
		}
	}

	if (m_pObjectGauge == nullptr)
	{// ゲージの生成
		m_pObjectGauge = CFan2D::Create();

		if (m_pObjectGauge != nullptr)
		{
			m_pObjectGauge->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectGauge->SetAngleMax(ANGLE_MAX);
			m_pObjectGauge->SetRotation(INITIAL_ROT);
			m_pObjectGauge->SetRadius(RADIUS_GAUGE);
			m_pObjectGauge->SetVtx();

			D3DXCOLOR col = universal::ConvertRGB(0, 240, 156, 255);

			m_pObjectGauge->SetCol(col);

			int nIdx = CTexture::GetInstance()->Regist(TEXTURE_PATH);
			m_pObjectGauge->SetIdxTexture(nIdx);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CLife::Uninit(void)
{
	if (m_pObjectGauge != nullptr)
	{
		m_pObjectGauge->Uninit();
		m_pObjectGauge = nullptr;
	}

	if (m_pObjectFrame != nullptr)
	{
		m_pObjectFrame->Uninit();
		m_pObjectFrame = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CLife::Update(void)
{
	// 変数宣言
	float fLife = 0.0f;
	float fRate;
	D3DXVECTOR3 pos;

	// プレイヤーの取得
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	fLife = pPlayer->GetLife();
	CPlayer::SParam param = pPlayer->GetParam();

	// 割合を算出
	fRate = fLife / param.fInitialLife;

	if (m_pObjectGauge != nullptr)
	{// ゲージの設定
		// サイズ設定
		m_pObjectGauge->SetRateAngle(fRate);
		m_pObjectGauge->SetVtx();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CLife::Draw(void)
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

		if (pLife != nullptr)
		{
			pLife->Init();
		}
	}

	return pLife;
}