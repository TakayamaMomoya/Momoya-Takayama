//=========================================================
//
// 修理状況処理 [repairStatus.cpp]
// Author = 阿部翔大郎
//
//=========================================================

//===============================================
// インクルード
//===============================================
#include "repairStatus.h"
#include "rocket.h"
#include "texture.h"
#include "game.h"

//===============================================
// 定数定義
//===============================================
namespace
{
	// 修理状況のテクスチャのパス
	const char* TEX_PATH = "data\\TEXTURE\\UI\\repair_status.png";

	// 修理状況のフレームテクスチャのパス
	const char* FRAME_PATH = "data\\TEXTURE\\UI\\repair_status_frame.png";

	// 高さの位置
	const float POS_Y = 300.0f;

	// 修理したときの変動量
	const float INCREASED_VALUE = 25.0f;
}

//===============================================
// 静的メンバ変数宣言
//===============================================
CRepairStatus* CRepairStatus::m_pRepairStatus = nullptr;	// 自身のポインタ

//===============================================
// コンストラクタ
//===============================================
CRepairStatus::CRepairStatus(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_pStatus = nullptr;
	m_pFrame = nullptr;
}

//===============================================
// デストラクタ
//===============================================
CRepairStatus::~CRepairStatus()
{
	
}

//===============================================
// 生成処理
//===============================================
CRepairStatus *CRepairStatus::Create(int nPriority)
{
	if (m_pRepairStatus == nullptr)
	{
		// 修理状況の生成
		m_pRepairStatus = new CRepairStatus();

		if (m_pRepairStatus != nullptr)
		{
			// 初期化処理
			m_pRepairStatus->Init();
		}
		else if (m_pRepairStatus == nullptr)
		{
			return nullptr;
		}
	}

	return m_pRepairStatus;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CRepairStatus::Init()
{
	m_pStatus = nullptr;
	m_pFrame = nullptr;

	// インスタンスを取得
	CRocket* pRocket = pRocket->GetInstance();

	if (m_pRepairStatus != nullptr)
	{
		if (pRocket != nullptr)
		{
			D3DXVECTOR3 posRocket = pRocket->GetPosition();

			if (m_pStatus == nullptr)
			{// ゲージの生成
				m_pStatus = CObject3D::Create({ posRocket.x - INCREASED_VALUE * 3, POS_Y, posRocket.z - 50.0f });

				if (m_pStatus != nullptr)
				{// 初期情報の設定
					m_pStatus->SetSize(0.0f, 50.0f);
					m_pStatus->SetPosition({ posRocket.x - INCREASED_VALUE * 3, POS_Y, posRocket.z - 50.0f });
					int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
					m_pStatus->SetIdxTexture(nIdx);
					m_pStatus->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
					m_pStatus->EnableBillboard(true);
					m_pStatus->SetVtx();
				}
			}

			if (m_pFrame == nullptr)
			{// 枠の生成
				m_pFrame = CObject3D::Create({ posRocket.x, 250.0f, posRocket.z - 50.0f });
				
				if (m_pFrame != nullptr)
				{// 枠の情報設定
					m_pFrame->SetSize(3 * INCREASED_VALUE + INCREASED_VALUE * 0.1f, 20.0f + INCREASED_VALUE * 0.1f);
					m_pFrame->SetPosition({ posRocket.x, POS_Y, posRocket.z - 50.0f });
					int nIdx = CTexture::GetInstance()->Regist(FRAME_PATH);
					m_pFrame->SetIdxTexture(nIdx);
					m_pFrame->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
					m_pFrame->EnableBillboard(true);
					m_pFrame->SetVtx();
				}
			}
		}
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRepairStatus::Uninit(void)
{
	if (m_pStatus != nullptr)
	{
		m_pStatus->Uninit();
		m_pStatus = nullptr;
	}

	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}

	m_pRepairStatus = nullptr;
}

//===============================================
// 更新処理
//===============================================
void CRepairStatus::Update(void)
{
	// インスタンスを取得
	CRocket* pRocket = pRocket->GetInstance();
	CGame* pGame = pGame->GetInstance();

	if (pRocket != nullptr)
	{// 修理状況によってゲージの状態を変える
		int nProgress = pRocket->GetProgress();
		D3DXVECTOR3 posRocket = pRocket->GetPosition();

		if (m_pStatus != nullptr)
		{
			m_pStatus->SetSize(nProgress * INCREASED_VALUE, 20.0f);
			m_pStatus->SetPosition({ posRocket.x - INCREASED_VALUE * 3 + nProgress * INCREASED_VALUE, POS_Y, posRocket.z - 50.0f });
		}
	}

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state == CGame::STATE_ESCAPE)
		{// リザルトで非表示にする
			Uninit();
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CRepairStatus::Draw(void)
{
	
}