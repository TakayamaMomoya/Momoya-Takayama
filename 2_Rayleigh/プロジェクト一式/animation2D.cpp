//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "animation2D.h"
#include "manager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define ANIM_TIME				(2)											//アニメーションを行う頻度
#define PLAYER_ANIM				(10)										//プレイヤーのアニメーション枚数

//=====================================================
// コンストラクタ
//=====================================================
CAnim2D::CAnim2D(int nPriority) : CObject2D(nPriority)
{
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nNumAnim = 0;
	m_nTimerAnim = 0;
	m_nNumV = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CAnim2D::~CAnim2D()
{

}

//=====================================================
// 生成処理
//=====================================================
CAnim2D *CAnim2D::Create(int nPtnAnim, int nNumAnim, int nTimeAnim, int nNumV)
{
	CAnim2D *pAnim2D = nullptr;

	if (pAnim2D != nullptr)
	{
		// 初期化処理
		pAnim2D->Init();

		// アニメーション情報設定
		SetInfo(nPtnAnim, nNumAnim, nTimeAnim, nNumV);
	}

	return pAnim2D;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CAnim2D::Init(void)
{
	// 継承クラスの初期化
	CObject2D::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CAnim2D::Uninit(void)
{
	// 継承クラスの終了
	CObject2D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CAnim2D::Update(void)
{
	// カウンターを加算
	m_nCounterAnim++;

	if (m_nCounterAnim % m_nTimerAnim == 0)
	{// 一定時間ごとにアニメーションを行う
		m_nCounterAnim = 0;
		m_nPatternAnim++;

		// アニメーション数を超えたら0枚に戻す
		m_nPatternAnim = m_nPatternAnim % m_nNumAnim;
	}

	// 継承クラスの更新
	CObject2D::Update();

	SetAnim(m_nPatternAnim,m_nNumAnim,m_nNumV);

	SetVtx();
}

//=====================================================
// 描画処理
//=====================================================
void CAnim2D::Draw(void)
{
	// 継承クラスの描画
	CObject2D::Draw();
}

//=====================================================
// 設定処理
//=====================================================
void CAnim2D::SetInfo(int nPtnAnim, int nNumAnim, int nTimeAnim, int nNumV)
{
	m_nPatternAnim = nPtnAnim;
	m_nNumAnim = nNumAnim;
	m_nTimerAnim = nTimeAnim;
	m_nNumV = nNumV;
}