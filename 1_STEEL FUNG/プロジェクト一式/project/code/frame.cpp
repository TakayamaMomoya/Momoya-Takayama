//*****************************************************
//
// Frameの処理[Frame.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "frame.h"
#include "object2D.h"
#include "UIManager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define FRAME_HEIGHT	(50.0f)	// フレームの高さ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CFrame::CFrame(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CFrame::~CFrame()
{

}

//=====================================================
// 生成処理
//=====================================================
CFrame *CFrame::Create(int nTimeIn, int nTimeWait, int nTimeOut)
{
	CFrame *pFrame = nullptr;

	if (pFrame == nullptr)
	{
		// インスタンス生成
		pFrame = new CFrame;

		if (pFrame != nullptr)
		{
			// 引数受け取り
			pFrame->m_info.nTimeIn = nTimeIn;
			pFrame->m_info.nTimeWait = nTimeWait;
			pFrame->m_info.nTimeOut = nTimeOut;

			// 初期化処理
			pFrame->Init();
		}
	}

	return pFrame;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CFrame::Init(void)
{
	// UIを非表示にする
	CUIManager *pUIManager = CUIManager::GetInstance();

	m_info.state = STATE_IN;

	if (pUIManager != nullptr)
	{
		pUIManager->EnableDisp(false);
	}

	// ポリゴンの生成
	if (m_info.pUpper == nullptr)
	{
		m_info.pUpper = CObject2D::Create(7);

		if (m_info.pUpper != nullptr)
		{
			m_info.pUpper->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f));
			m_info.pUpper->SetSize(SCREEN_WIDTH * 0.5f, 0.0f);
			m_info.pUpper->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			m_info.pUpper->SetVtx();
		}
	}

	if (m_info.pUnder == nullptr)
	{
		m_info.pUnder = CObject2D::Create(7);

		if (m_info.pUnder != nullptr)
		{
			m_info.pUnder->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT, 0.0f));
			m_info.pUnder->SetSize(SCREEN_WIDTH * 0.5f, 0.0f);
			m_info.pUnder->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			m_info.pUnder->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFrame::Uninit(void)
{
	// UIを表示する
	CUIManager *pUIManager = CUIManager::GetInstance();

	if (pUIManager != nullptr)
	{
		pUIManager->EnableDisp(true);
	}

	// ポリゴンの破棄
	if (m_info.pUnder != nullptr)
	{
		m_info.pUnder->Uninit();
		m_info.pUnder = nullptr;
	}

	if (m_info.pUpper != nullptr)
	{
		m_info.pUpper->Uninit();
		m_info.pUpper = nullptr;
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CFrame::Update(void)
{
	// 状態の管理
	ManageState();

	if (m_info.state != STATE_WAIT)
	{
		// 上側の管理
		ManageUpper();

		// 下側の管理
		ManageUnder();
	}
}

//=====================================================
// 状態の管理
//=====================================================
void CFrame::ManageState(void)
{
	switch (m_info.state)
	{
	case STATE_IN:

		m_info.nCntFade++;

		if (m_info.nTimeIn <= m_info.nCntFade)
		{
			m_info.state = STATE_WAIT;

			m_info.nCntFade = 0;
		}

		break;
	case STATE_WAIT:

		m_info.nCntFade++;

		if (m_info.nTimeWait <= m_info.nCntFade)
		{
			m_info.state = STATE_OUT;

			m_info.nCntFade = m_info.nTimeOut;
		}

		break; 
	case STATE_OUT:

		m_info.nCntFade--;

		if (0 >= m_info.nCntFade)
		{// 終了処理
			Uninit();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// 上側の管理
//=====================================================
void CFrame::ManageUpper(void)
{
	if (m_info.pUpper == nullptr)
	{
		return;
	}

	int nTime = 0;

	switch (m_info.state)
	{
	case STATE_IN:

		nTime = m_info.nTimeIn;

		break;
	case STATE_OUT:

		nTime = m_info.nTimeOut;

		break;
	default:
		break;
	}

	float fRate = (float)m_info.nCntFade / (float)nTime;

	float fHeight = fRate * FRAME_HEIGHT;

	// 位置設定
	D3DXVECTOR3 pos = { SCREEN_WIDTH * 0.5f,fHeight,0.0f };

	// サイズ設定
	m_info.pUpper->SetPosition(pos);
	m_info.pUpper->SetSize(SCREEN_WIDTH * 0.5f, fHeight);
	m_info.pUpper->SetVtx();
}

//=====================================================
// 下側の管理
//=====================================================
void CFrame::ManageUnder(void)
{
	if (m_info.pUnder == nullptr)
	{
		return;
	}

	int nTime = 0;

	switch (m_info.state)
	{
	case STATE_IN:

		nTime = m_info.nTimeIn;

		break;
	case STATE_OUT:

		nTime = m_info.nTimeOut;

		break;
	default:
		break;
	}

	float fRate = (float)m_info.nCntFade / (float)nTime;

	float fHeight = fRate * FRAME_HEIGHT;

	// 位置設定
	D3DXVECTOR3 pos = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT - fHeight,0.0f };

	// サイズ設定
	m_info.pUnder->SetPosition(pos);
	m_info.pUnder->SetSize(SCREEN_WIDTH * 0.5f, fHeight);
	m_info.pUnder->SetVtx();
}

//=====================================================
// 描画
//=====================================================
void CFrame::Draw(void)
{

}