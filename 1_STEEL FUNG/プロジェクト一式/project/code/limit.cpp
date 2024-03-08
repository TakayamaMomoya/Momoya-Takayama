//*****************************************************
//
// 位置制限の処理[limit.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "limit.h"
#include "object3D.h"
#include "player.h"
#include "texture.h"
#include "enemyManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR2 SIZE_GUIDE = { 200.0f,100.0f };	// ガイド表示のサイズ
const float DIST_DISP = 2000.0f;	// 位置制限の表示
}

//=====================================================
// コンストラクタ
//=====================================================
CLimit::CLimit(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CLimit::~CLimit()
{

}

//=====================================================
// 生成処理
//=====================================================
CLimit *CLimit::Create(void)
{
	CLimit *pLimit = nullptr;

	if (pLimit == nullptr)
	{
		pLimit = new CLimit;

		if (pLimit != nullptr)
		{
			// 初期化
			pLimit->Init();
		}
	}

	return pLimit;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CLimit::Init(void)
{
	return S_OK;
}

//=====================================================
// ガイド表示生成
//=====================================================
void CLimit::CreateGuide(void)
{
	if (m_info.pGuide == nullptr)
	{
		m_info.pGuide = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (m_info.pGuide != nullptr)
		{
			D3DXVECTOR3 rot = m_info.rot;
			rot.x = D3DX_PI * -0.5f;

			m_info.pGuide->SetSize(SIZE_GUIDE.x, SIZE_GUIDE.y);
			m_info.pGuide->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
			m_info.pGuide->SetRotation(rot);
			m_info.pGuide->EnableCull(false);

			int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\limit00.png");
			m_info.pGuide->SetIdxTexture(nIdx);
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CLimit::Uninit(void)
{
	DeleteGuide();

	Release();
}

//=====================================================
// ガイド表示の削除
//=====================================================
void CLimit::DeleteGuide(void)
{
	if (m_info.pGuide != nullptr)
	{
		m_info.pGuide->Uninit();
		m_info.pGuide = nullptr;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CLimit::Update(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// プレイヤーの位置制限
		float fDiff;

		D3DXVECTOR3 posGuide = CheckLimit((CObject*)pPlayer, &fDiff);

		if (fDiff * fDiff < DIST_DISP * DIST_DISP)
		{
			CreateGuide();
		}
		else
		{
			DeleteGuide();
		}

		if (m_info.pGuide != nullptr)
		{
			SetColGuide(fDiff);

			m_info.pGuide->SetPosition(posGuide);
		}
	}

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		std::list<CEnemy*> listEnemy = pEnemyManager->GetListRanking();

		for (auto pEnemy : listEnemy)
		{
			CheckLimit((CObject*)pEnemy);
		}
	}
}

//=====================================================
// 位置制限の確認
//=====================================================
D3DXVECTOR3 CLimit::CheckLimit(CObject *pObj, float *pDiff)
{
	D3DXVECTOR3 posObject = pObj->GetPosition();
	D3DXVECTOR3 posGuide = pObj->GetPosition();
	D3DXVECTOR3 pos = m_info.pos;
	D3DXVECTOR3 rot = m_info.rot;
	float fDiff = 0.0f;

	if (rot.y < D3DX_PI * 0.6f && rot.y > D3DX_PI * 0.4f)
	{// 右向き
		posGuide.x = pos.x;
		fDiff = posObject.x - pos.x;

		if (posObject.x > pos.x)
		{
			posObject.x = pos.x;
		}
	}
	else if (rot.y > D3DX_PI * -0.6f && rot.y < D3DX_PI * -0.4f)
	{// 左向き
		posGuide.x = pos.x;
		fDiff = posObject.x - pos.x;

		if (posObject.x < pos.x)
		{
			posObject.x = pos.x;
		}
	}
	else if (rot.y > D3DX_PI * -0.1f && rot.y < D3DX_PI * 0.1f)
	{// 前
		posGuide.z = pos.z;
		fDiff = posObject.z - pos.z;

		if (posObject.z > pos.z)
		{
			posObject.z = pos.z;
		}
	}
	else
	{// 後ろ
		posGuide.z = pos.z;
		fDiff = posObject.z - pos.z;

		if (posObject.z < pos.z)
		{
			posObject.z = pos.z;
		}
	}

	pObj->SetPosition(posObject);

	if (pDiff != nullptr)
		*pDiff = fDiff;

	return posGuide;
}

//=====================================================
// ガイド表示の色設定
//=====================================================
void CLimit::SetColGuide(float fDiff)
{
	if (m_info.pGuide == nullptr)
		return;

	D3DXCOLOR col = m_info.pGuide->GetColor();

	float fRate = fDiff / DIST_DISP;

	if (fRate < 0.0f)
	{
		fRate *= -1;
	}

	col.a = 1.0f - fRate;

	m_info.pGuide->SetColor(col);
}

//=====================================================
// 描画処理
//=====================================================
void CLimit::Draw(void)
{

}