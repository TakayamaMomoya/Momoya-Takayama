//*****************************************************
//
// ゴールタイマーの処理[goalGoalTimer.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "goalTimer.h"
#include "manager.h"
#include "number.h"
#include "game.h"
#include "debugproc.h"
#include "goal.h"
#include "UI.h"
#include "texture.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int NUM_PLACE = 2;	// 桁数
	const float INITIAL_TIME = 10.0f;	// 初期の時間
	const float SIZE_POINT = 10.0f;	// 小数点のサイズ
	const float WIDTH_NUMBER = 20.0f;	// 数字の幅
	const float HEIGHT_NUMBER = 50.0f;	// 数字の高さ
	const D3DXVECTOR3 POS_SECOND = { 620.0f, 80.0f, 0.0f };	// 秒数の位置
	const D3DXVECTOR3 POS_DECIMAL = { 720.0f, 80.0f, 0.0f };	// 秒数の位置
	const D3DXVECTOR3 POS_POINT = { 690.0f, 120.0f, 0.0f };	// 秒数の位置
	const char* POINT_TEX = "data\\TEXTURE\\UI\\point.png";	// 小数点のテクスチャパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CGoalTimer *CGoalTimer::m_pGoalTimer = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CGoalTimer::CGoalTimer()
{
	m_pObjSecond = nullptr;
	m_pObjDecimal = nullptr;
	m_pPoint = nullptr;
	m_bStop = false;
	m_bSound = false;
	m_fSecond = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CGoalTimer::~CGoalTimer()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGoalTimer::Init(void)
{
	m_fSecond = INITIAL_TIME;
	
	if (m_pObjSecond == nullptr)
	{// 秒表示の生成
		m_pObjSecond = CNumber::Create(NUM_PLACE, 0);

		if (m_pObjSecond != nullptr)
		{
			m_pObjSecond->SetPosition(POS_SECOND);
			m_pObjSecond->SetSizeAll(WIDTH_NUMBER, HEIGHT_NUMBER);
		}
	}

	if (m_pObjDecimal == nullptr)
	{// 小数点以下表示の生成
		m_pObjDecimal = CNumber::Create(NUM_PLACE, 0);

		if (m_pObjDecimal != nullptr)
		{
			m_pObjDecimal->SetPosition(POS_DECIMAL);
			m_pObjDecimal->SetSizeAll(WIDTH_NUMBER, HEIGHT_NUMBER);
		}
	}

	if (m_pPoint == nullptr)
	{
		m_pPoint = CUI::Create();

		if (m_pPoint != nullptr)
		{
			int nIdx = CTexture::GetInstance()->Regist(POINT_TEX);
			m_pPoint->SetIdxTexture(nIdx);

			m_pPoint->SetPosition(POS_POINT);
			m_pPoint->SetSize(SIZE_POINT, SIZE_POINT);
			m_pPoint->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGoalTimer::Uninit(void)
{
	if (m_pObjSecond != nullptr)
	{
		m_pObjSecond->Uninit();
		m_pObjSecond = nullptr;
	}

	if (m_pObjDecimal != nullptr)
	{
		m_pObjDecimal->Uninit();
		m_pObjDecimal = nullptr;
	}

	if (m_pPoint != nullptr)
	{
		m_pPoint->Uninit();
		m_pPoint = nullptr;
	}

	m_pGoalTimer = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CGoalTimer::Update(void)
{
	if (CGame::GetState() == CGame::STATE_END || 
		m_bStop)
	{
		return;
	}

	// 秒数を減らす
	float fTick = CManager::GetTick();

	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr && m_bSound == false)
	{
		pSound->Play(pSound->LABEL_SE_COUNT_DOWN);
		m_bSound = true;
	}
	
	m_fSecond -= fTick;

	if (m_fSecond < 0.0f)
	{// タイムアップの処理
		m_fSecond = 0.0f;

		CGoal *pGoal = CGoal::GetInstance();

		if (pGoal != nullptr)
		{
			if (pGoal->IsFinish() == false)
			{
				pGoal->DeadLine();
			}
		}
	}

	int nDecimal = (int)(m_fSecond * 100.0f) % 100;

	if (m_pObjSecond != nullptr)
	{// 秒表示の制御
		m_pObjSecond->SetValue((int)m_fSecond, NUM_PLACE);
	}

	if (m_pObjDecimal != nullptr)
	{// 小数点以下表示の制御
		m_pObjDecimal->SetValue(nDecimal, NUM_PLACE);
	}
}

//=====================================================
// 時間加算
//=====================================================
void CGoalTimer::AddGoalTimer(int nValue)
{
	m_fSecond += nValue;
}

//=====================================================
// 生成処理
//=====================================================
CGoalTimer *CGoalTimer::Create(void)
{
	if (m_pGoalTimer == nullptr)
	{
		m_pGoalTimer = new CGoalTimer;

		m_pGoalTimer->Init();
	}

	return m_pGoalTimer;
}

//=====================================================
// 描画処理
//=====================================================
void CGoalTimer::Draw(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\nゴール残り時間[%f]", m_fSecond);
}