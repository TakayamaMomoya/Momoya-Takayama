//*****************************************************
//
// ビームブレードの処理[BoostEffect.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "boostEffect.h"
#include "meshcylinder.h"
#include "object3D.h"
#include "texture.h"
#include "anim3D.h"
#include "animEffect3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int MESH_V = 6;	// メッシュの縦の分割数
}

//=====================================================
// コンストラクタ
//=====================================================
CBoostEffect::CBoostEffect(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CBoostEffect::~CBoostEffect()
{

}

//=====================================================
// 生成処理
//=====================================================
CBoostEffect *CBoostEffect::Create(void)
{
	CBoostEffect *pBoostEffect = nullptr;

	if (pBoostEffect == nullptr)
	{
		pBoostEffect = new CBoostEffect;

		if (pBoostEffect != nullptr)
		{
			// 初期化
			pBoostEffect->Init();
		}
	}

	return pBoostEffect;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBoostEffect::Init(void)
{
	m_info.fRadius = 100.0f;
	m_info.fHeight = 400.0f;
	m_info.col = { 1.0f,1.0f,1.0f,1.0f };

	if (m_info.pBoost == nullptr)
	{
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if(pAnim3D != nullptr)
			m_info.pBoost = pAnim3D->CreateEffect(D3DXVECTOR3(0.0f,0.0f,0.0f),CAnimEffect3D::TYPE::TYPE_BOOST);

		if (m_info.pBoost != nullptr)
		{
			m_info.pBoost->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
			m_info.pBoost->SetRotation(D3DXVECTOR3(-1.57f, 0.0f, 0.0f));
			m_info.pBoost->SetSize(m_info.fRadius, m_info.fHeight);
			m_info.pBoost->SetPosition(D3DXVECTOR3(0.0f, m_info.fHeight, 0.0f));
			m_info.pBoost->EnableAdd(true);
			m_info.pBoost->EnableZtest(false);
			m_info.pBoost->SetAlphaTest(100);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBoostEffect::Uninit(void)
{
	if (m_info.pBoost != nullptr)
	{
		m_info.pBoost->Uninit();
		m_info.pBoost = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBoostEffect::Update(void)
{
	// 見た目の追従
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	if (m_info.pBoost != nullptr)
	{
		pos +=
		{
			sinf(rot.x) * sinf(rot.y) * m_info.fHeight,
				cosf(rot.x) * m_info.fHeight,
				sinf(rot.x) * cosf(rot.y) * m_info.fHeight
		};

		rot.x -= 1.57f;

		m_info.pBoost->SetPosition(pos);
		m_info.pBoost->SetRotation(rot);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBoostEffect::Draw(void)
{

}

// 半径設定
void CBoostEffect::SetRadius(float fRadius)
{
	m_info.fRadius = fRadius;

	if (m_info.pBoost != nullptr)
	{
		float fHeight = m_info.pBoost->GetHeight();

		m_info.pBoost->SetSize(fRadius, fHeight);
		m_info.pBoost->SetVtx();
	}
}

// 高さ設定
void CBoostEffect::SetHeight(float fHeight)
{
	m_info.fHeight = fHeight;

	if (m_info.pBoost != nullptr)
	{
		float fWidth = m_info.pBoost->GetWidth();

		m_info.pBoost->SetSize(fWidth, fHeight);
	}
}

// 色設定
void CBoostEffect::SetColor(D3DXCOLOR col)
{
	m_info.col = col;
	
	if (m_info.pBoost != nullptr)
	{
		m_info.pBoost->SetColor(col);
	}
}

// 位置設定
void CBoostEffect::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;

	D3DXVECTOR3 rot = GetRotation();

	if (m_info.pBoost != nullptr)
	{
		pos +=
		{
			sinf(rot.x) * sinf(rot.y) * m_info.fHeight,
				cosf(rot.x) * m_info.fHeight,
				sinf(rot.x) * cosf(rot.y) * m_info.fHeight
		};

		rot.x -= 1.57f;

		m_info.pBoost->SetPosition(pos);
		m_info.pBoost->SetRotation(rot);
	}
}

// Zテストをするかどうか
void CBoostEffect::EnableZtestBoost(bool bZtest)
{
	if (m_info.pBoost != nullptr)
	{
		m_info.pBoost->EnableZtest(bZtest);
	}
}