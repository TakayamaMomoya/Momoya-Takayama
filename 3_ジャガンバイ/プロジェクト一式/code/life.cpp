//*****************************************************
//
// ライフ表示の処理[life.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "life.h"
#include "UI.h"

#include "texture.h"

#include "player.h"
#include "playerManager.h"

#include "UIManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float LIFE_CHANGE_RATE[CLife::LIFE_STATE_MAX]
	{
		1.00f,
		0.60f,
		0.25f,
		0.05f,
	};

	const D3DXCOLOR LIFE_COLOR[CLife::LIFE_STATE_MAX]
	{
		D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
		D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
	};
}
//=====================================================
// コンストラクタ
//=====================================================
CLife::CLife(int nPriority) : CObject(nPriority)
{
	m_pUILife = nullptr;
	m_pUILifeFrame = nullptr;
	ZeroMemory(&m_info, sizeof(m_info));
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
CLife *CLife::Create(int nIdx)
{
	CLife *pLife = new CLife;

	if (pLife != nullptr)
	{
		// プレイヤー番号を設定
		pLife->m_info.nIdxPlayer = nIdx;

		if (pLife->m_pUILifeFrame == nullptr)
		{
			// ライフ枠の生成処理
			pLife->m_pUILifeFrame = CUI::Create();
		}

		if (pLife->m_pUILife == nullptr)
		{
			// ライフの生成処理
			pLife->m_pUILife = CUI::Create();
		}

		// ライフの初期化
		pLife->Init();
	}

	return pLife;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CLife::Init(void)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		// 初期値設定処理（設定してなかったらこれ）
		m_pUILife->SetSize(50.0f, 100.0f);
		m_pUILife->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pUILifeFrame->SetSize(50.0f, 100.0f);
		m_pUILifeFrame->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CLife::Uninit(void)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		m_pUILife->Uninit();
		m_pUILifeFrame->Uninit();
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CLife::Update(void)
{
	// プレイヤーの体力
	SetLife();
}

//=====================================================
// 描画処理
//=====================================================
void CLife::Draw(void)
{

}

//=====================================================
// 位置設定処理
//=====================================================
void CLife::SetLife(void)
{
	// プレイヤー管理の取得
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CPlayer* pPlayer = nullptr;

	if (pPlayerManager != nullptr)
	{
		// プレイヤーの取得
		pPlayer = pPlayerManager->GetPlayer(m_info.nIdxPlayer);
	}

	if (pPlayerManager != nullptr )
	{
		if (pPlayer != nullptr)
		{
			// 現在の体力と前の体力が違う
			if (m_info.fLife != pPlayer->GetLife())
			{
				// 現在の体力を取得
				m_info.fLife = pPlayer->GetLife();

				// 色変更フラグ
				m_info.bColorChange = true;

				// 体力の初期値を取得
				float fMaxLife = pPlayerManager->GetPlayerParam().fInitialLife;

				// ゲージの消費量を計算
				float fLifeRatio = (m_info.fLife / fMaxLife);

				// サイズを変更
				m_pUILife->SetSize(
					LIFE::WIDTH_MAX * fLifeRatio,
					LIFE::HEIGHT_MAX * fLifeRatio);

				// 色を変更
				for (int nCount = 0; nCount < CLife::LIFE_STATE_MAX; nCount++)
				{
					// 現在の体力状況を判定
					if (m_info.fLife >= fMaxLife * LIFE_CHANGE_RATE[nCount])
					{
						// 色を設定
						m_info.colLife = LIFE_COLOR[nCount];
						m_pUILife->SetCol(LIFE_COLOR[nCount]);

						break;
					}
				}

				m_pUILife->SetVtx();
				m_pUILifeFrame->SetVtx();
			}
		}
		else
		{
			// 体力はない
			m_info.fLife = 0.0f;

			// 体力の初期値を取得
			float fMaxLife = pPlayerManager->GetPlayerParam().fInitialLife;

			// ゲージの消費量を計算
			float fLifeRatio = (m_info.fLife / fMaxLife);

			// サイズを変更
			m_pUILife->SetSize(
				LIFE::WIDTH_MAX * fLifeRatio,
				LIFE::HEIGHT_MAX * fLifeRatio);

			m_pUILife->SetVtx();
			m_pUILifeFrame->SetVtx();
		}
	}
}

//=====================================================
// 位置設定処理
//=====================================================
void CLife::SetPosition(D3DXVECTOR3 pos)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		m_info.posLife = pos;
		
		m_pUILife->SetPosition(pos);
		m_pUILifeFrame->SetPosition(pos);

		m_pUILife->SetVtx();
		m_pUILifeFrame->SetVtx();
	}
}

//=====================================================
// 大きさ設定処理
//=====================================================
void CLife::SetSize(float width, float height)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		m_info.fWidth = width;
		m_info.fHeight = height;

		m_pUILife->SetSize(width, height);
		m_pUILifeFrame->SetSize(width, height);

		m_pUILife->SetVtx();
		m_pUILifeFrame->SetVtx();
	}
}

//=====================================================
// 色設定処理
//=====================================================
void CLife::SetCol(D3DXCOLOR colLife, D3DXCOLOR colLifeFrame)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		m_info.colLife = colLife;
		m_info.colLifeFrame = colLifeFrame;

		m_pUILife->SetCol(colLife);
		m_pUILifeFrame->SetCol(colLifeFrame);

		m_pUILife->SetVtx();
		m_pUILifeFrame->SetVtx();
	}
}

//=====================================================
// テクスチャ設定処理
//=====================================================
void CLife::SetTexture(const char* pFileName)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		CTexture* pTexture = CTexture::GetInstance();

		if (pTexture != nullptr)
		{
			m_info.nIdxTexture = pTexture->Regist(pFileName);
		}

		m_pUILife->SetIdxTexture(m_info.nIdxTexture);
		m_pUILifeFrame->SetIdxTexture(m_info.nIdxTexture);

		m_pUILife->SetVtx();
		m_pUILifeFrame->SetVtx();
	}
}