//*****************************************************
//
// アイテムの処理[item.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "gimmick.h"
#include "gimmickManager.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "playerManager.h"
#include "weapon.h"
#include "player.h"
#include "billboard.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SIZE_INTERACT = 30.0f;	// インタラクト表示のサイズ
}

//=====================================================
// コンストラクタ
//=====================================================
CGimmick::CGimmick(int nPriority) : CObjectX(nPriority)
{
	m_pCollisionSphere = nullptr;
	m_pInteract = nullptr;
	m_bEnable = true;

	// 先頭、最後尾アドレス取得
	CGimmickManager *pManager = CGimmickManager::GetInstance();
	CGimmick *pHead = nullptr;
	CGimmick *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}
	else
	{
		return;
	}

	// 値のクリア
	m_pPrev = nullptr;
	m_pNext = nullptr;

	if (pHead == nullptr)
	{// 先頭と最後尾アドレスの代入
		pManager->SetHead(this);
		pManager->SetTail(this);

		return;
	}

	// 前のアドレスに最後尾のアドレスを代入する
	m_pPrev = pTail;

	// 最後尾のアドレスを自分にする
	pManager->SetTail(this);

	if (m_pPrev != nullptr)
	{
		// 前のオブジェクトの次のアドレスを自分にする
		m_pPrev->m_pNext = this;
	}
}

//=====================================================
// デストラクタ
//=====================================================
CGimmick::~CGimmick()
{
	// 先頭、最後尾アドレス取得
	CGimmickManager *pManager = CGimmickManager::GetInstance();
	CGimmick *pHead = nullptr;
	CGimmick *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

	if (pTail != this && pHead != this)
	{// 真ん中のアドレスの破棄
		if (m_pPrev != nullptr)
		{
			// 前のアドレスから次のアドレスをつなぐ
			m_pPrev->m_pNext = m_pNext;
		}

		if (m_pNext != nullptr)
		{
			// 次のアドレスから前のアドレスをつなぐ
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	if (pHead == this)
	{// 先頭アドレスの破棄
		//if (m_pNext != nullptr)
		{// 先頭アドレスを次のアドレスに引き継ぐ
			pManager->SetHead(m_pNext);

			if (m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
			}
		}
	}

	if (pTail == this)
	{// 最後尾アドレスの破棄
		if (m_pPrev != nullptr)
		{// 最後尾アドレスを前のアドレスに引き継ぐ
			pManager->SetTail(m_pPrev);

			m_pPrev->m_pNext = nullptr;
		}
	}
}

//=====================================================
// 生成処理
//=====================================================
CGimmick *CGimmick::Create(void)
{
	CGimmick *pItem = nullptr;

	if (pItem == nullptr)
	{
		pItem = new CGimmick;

		if (pItem != nullptr)
		{
			// 初期化
			pItem->Init();
		}
	}

	return pItem;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGimmick::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	if (m_pCollisionSphere == nullptr)
	{// 当たり判定の生成
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ITEM, CCollision::TYPE_SPHERE, this);

		if (m_pCollisionSphere != nullptr)
		{
			m_pCollisionSphere->SetRadius(50.0f);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGimmick::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{
		m_pCollisionSphere->Uninit();
		m_pCollisionSphere = nullptr;
	}

	if (m_pInteract != nullptr)
	{
		m_pInteract->Uninit();
		m_pInteract = nullptr;
	}

	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CGimmick::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();

	if (m_pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionSphere->SetPosition(pos);

		// プレイヤーとの当たり判定
		if (m_pCollisionSphere->OnEnter(CCollision::TAG_PLAYER))
		{
			if (m_pInteract == nullptr && m_bEnable == true)
			{// インタラクト表示生成
				D3DXVECTOR3 pos = GetPosition();

				pos.y += 50.0f;

				m_pInteract = CBillboard::Create(pos, SIZE_INTERACT, SIZE_INTERACT);

				if (m_pInteract != nullptr)
				{
					int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\interact.png");
					m_pInteract->SetIdxTexture(nIdx);
					m_pInteract->SetZTest(true);
				}
			}
			// 当たったオブジェクトの取得
			CObject *pObj = m_pCollisionSphere->GetOther();

			// アイテム入手処理
			Interact(pObj);
		}
		else
		{
			if (m_pInteract != nullptr)
			{// インタラクト表示削除
				m_pInteract->Uninit();
				m_pInteract = nullptr;
			}
		}

		if (m_pCollisionSphere != nullptr)
		{
			if (m_pCollisionSphere->IsTriggerExit(CCollision::TAG_PLAYER))
			{
				// 当たったオブジェクトの取得
				CObject *pObj = m_pCollisionSphere->GetOther();

				Exit(pObj);
			}
		}
	}

	if (m_pInteract != nullptr && m_bEnable == false)
	{// インタラクト削除
		m_pInteract->Uninit();
		m_pInteract = nullptr;
	}
}

//=====================================================
// アイテム入手時の処理
//=====================================================
void CGimmick::Interact(CObject *pObj)
{

}

//=====================================================
// 描画処理
//=====================================================
void CGimmick::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}