//*****************************************************
//
// スローの処理[slow.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "slow.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CSlow *CSlow::m_pSlow = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CSlow::CSlow()
{
	m_pSlow = this;

	m_fScale = 0.0f;
	m_fTimeSlow = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CSlow::~CSlow()
{

}

//=====================================================
// 生成処理
//=====================================================
CSlow *CSlow::Create(void)
{
	if (m_pSlow == nullptr)
	{
		m_pSlow = new CSlow;

		if (m_pSlow != nullptr)
		{
			m_pSlow->Init();
		}
	}

	return m_pSlow;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSlow::Init(void)
{
	m_fScale = 1.0f;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSlow::Uninit(void)
{
	m_pSlow = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CSlow::Update(void)
{
	if (m_fTimeSlow > 0.0f)
	{
		float fDeltaTime = CManager::GetDeltaTime();

		m_fTimeSlow -= fDeltaTime;

		if (m_fTimeSlow <= 0.0f)
		{
			SetScale(1.0f);
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CSlow::Draw(void)
{
#ifdef _DEBUG
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc != nullptr)
	{
		pDebugProc->Print("\nスロー倍率[%f]",m_fScale);
	}
#endif
}

//=====================================================
// スケールの設定
//=====================================================
void CSlow::SetScale(float fScale)
{
	if (fScale >= 0.0f)
	{
		m_fScale = fScale;
	}
}

//=====================================================
// スロータイムの設定
//=====================================================
void CSlow::SetSlowTime(float fTime, float fScale)
{
	m_fTimeSlow = fTime;

	universal::LimitValue(&m_fTimeSlow, FLT_MAX, 0.0f);
	
	SetScale(fScale);
}

namespace Slow
{
float GetScale(void)
{
	float fScale = 1.0f;

	CSlow *pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		fScale = pSlow->GetScale();
	}

	return fScale;
}
}