//*****************************************************
//
// 警告表示の処理[caution.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "caution.h"
#include "UI.h"
#include "texture.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SIZE_ICON = 30.0f;	// アイコンのサイズ
const float LIMIT_DIST = 200.0f;	// 画面中心から離れる制限距離
const float INITIAL_LIFE = 5.0f;	// 初期寿命
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CCaution::CCaution(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CCaution::~CCaution()
{

}

//=====================================================
// 生成処理
//=====================================================
CCaution *CCaution::Create(D3DXVECTOR3 pos)
{
	CCaution *pCaution = nullptr;

	if (pCaution == nullptr)
	{
		// インスタンス生成
		pCaution = new CCaution;

		if (pCaution != nullptr)
		{
			pCaution->m_info.pos = pos;

			// 初期化処理
			pCaution->Init();
		}
	}

	return pCaution;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CCaution::Init(void)
{
	m_info.fLife = INITIAL_LIFE;

	// アイコンの生成
	m_info.pIcon = CUI::Create();

	if (m_info.pIcon != nullptr)
	{
		m_info.pIcon->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_info.pIcon->SetSize(SIZE_ICON * 3, SIZE_ICON * 3);
		
		int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\caution00.png");
		m_info.pIcon->SetIdxTexture(nIdx);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CCaution::Uninit(void)
{
	if (m_info.pIcon != nullptr)
	{
		m_info.pIcon->Uninit();
		m_info.pIcon = nullptr;
	}

	if (m_info.pArrow != nullptr)
	{
		m_info.pArrow->Uninit();
		m_info.pArrow = nullptr;
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CCaution::Update(void)
{
	// アイコンの更新
	UpdateIcon();

	float fDeltaTime = CManager::GetDeltaTime();

	m_info.fLife -= fDeltaTime;

	if (m_info.fLife < 0.0f)
	{
		Uninit();

		return;
	}
}

//=====================================================
// アイコンの更新
//=====================================================
void CCaution::UpdateIcon(void)
{
	if (m_info.pIcon == nullptr)
		return;

	// 大きさの変更
	float fHeight = m_info.pIcon->GetHeight();
	float fWidth = m_info.pIcon->GetWidth();

	fWidth += (SIZE_ICON - fWidth) * 0.1f;
	fHeight += (SIZE_ICON - fHeight) * 0.1f;

	m_info.pIcon->SetSize(fWidth, fHeight);

	D3DXMATRIX mtx;
	D3DXVECTOR3 posScreen;

	// 画面内座標取得
	bool bInScreen = universal::IsInScreen(m_info.pos, mtx, &posScreen);

	CDebugProc::GetInstance()->Print("\n画面座標[%f,%f,%f]", posScreen.x, posScreen.y, posScreen.z);

	D3DXVECTOR3 vecDiff = SCRN_MID - posScreen;
	float fDiff = D3DXVec3Length(&vecDiff);

	if (!bInScreen || fDiff > LIMIT_DIST || posScreen.z >= 1)
	{
		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= LIMIT_DIST;

		if (posScreen.z >= 1)
		{
			posScreen = SCRN_MID + vecDiff;
		}
		else
		{
			posScreen = SCRN_MID - vecDiff;
		}
	}

	// 位置の設定
	m_info.pIcon->SetPosition(posScreen);
	m_info.pIcon->SetVtx();
}

//=====================================================
// 描画
//=====================================================
void CCaution::Draw(void)
{

}