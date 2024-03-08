//*****************************************************
//
// ブロックの処理[block.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "block.h"
#include "blockManager.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "collision.h"
#include "debrisSpawner.h"
#include <stdio.h>

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
	const float SPEED_MOVE(1.0f);	// 移動速度
	const char* MAP_FILE = "data\\MAP\\map01.bin";	// マップのファイルパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CBlock::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CBlock::CBlock(int nPriority)
{
	m_pCollisionCube = nullptr;
	m_fLife = 0.0f;
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_nIdx = -1;

	// 先頭、最後尾アドレス取得
	CBlockManager *pManager = CBlockManager::GetInstance();
	CBlock *pHead = nullptr;
	CBlock *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

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

	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CBlock::~CBlock()
{
	// 先頭、最後尾アドレス取得
	CBlockManager *pManager = CBlockManager::GetInstance();
	CBlock *pHead = nullptr;
	CBlock *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

	m_nNumAll--;

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

	m_nNumAll--;
}

//=====================================================
// 生成処理
//=====================================================
CBlock *CBlock::Create(int nIdxModel)
{
	CBlock *pBlock = nullptr;

	if (pBlock == nullptr)
	{// インスタンス生成
		pBlock = new CBlock;

		// 初期化処理
		pBlock->Init();

		// 種類ごとのモデル読込
		pBlock->BindModel(nIdxModel);

		if (pBlock->m_pCollisionCube == nullptr)
		{// 当たり判定生成
			pBlock->m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_BLOCK, pBlock);

			if (pBlock->m_pCollisionCube != nullptr)
			{
				pBlock->m_pCollisionCube->SetPosition(pBlock->GetPosition());

				pBlock->m_pCollisionCube->SetVtx(pBlock->GetVtxMax(), pBlock->GetVtxMin());
			}
		}
	}

	return pBlock;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBlock::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 継承クラスの初期化
	CObjectX::Init();

	// タイプの設定
	SetType(TYPE_BLOCK);

	m_fLife = 300.0f;

	// 影の有効化
	EnableShadow(true);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBlock::Uninit(void)
{
	// 当たり判定削除
	DeleteCollision();

	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 当たり判定の削除
//=====================================================
void CBlock::DeleteCollision(void)
{
	if (m_pCollisionCube != nullptr)
	{// 当たり判定の消去
		m_pCollisionCube->Uninit();

		m_pCollisionCube = nullptr;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CBlock::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();
}

//=====================================================
// 位置設定
//=====================================================
void CBlock::SetPosition(D3DXVECTOR3 pos)
{
	if (m_pCollisionCube != nullptr)
	{
		m_pCollisionCube->SetPosition(pos);
	}

	CObjectX::SetPosition(pos);
}

//=====================================================
// 描画処理
//=====================================================
void CBlock::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//=====================================================
// ヒット処理
//=====================================================
void CBlock::Hit(float fDamage)
{

}

//=====================================================
// 頂点を入れ替える処理
//=====================================================
void CBlock::SetRotation(D3DXVECTOR3 rot)
{
	if (rot.y != 0)
	{// 角度によって最大頂点、最小頂点を変える処理
		SwapVtx();
	}

	CObjectX::SetRotation(rot);
}

//=====================================================
// 頂点を入れ替える処理
//=====================================================
void CBlock::SwapVtx(void)
{
	D3DXVECTOR3 vtxMax = GetVtxMax();
	D3DXVECTOR3 vtxMin = GetVtxMin();
	D3DXVECTOR3 vtxTemp = vtxMin;
	
	vtxMin = { -vtxMax.z,vtxMin.y,-vtxMax.x };
	vtxMax = { -vtxTemp.z,vtxMax.y,-vtxTemp.x };

	SetVtxMax(vtxMax);
	SetVtxMin(vtxMin);

	if (m_pCollisionCube != nullptr)
	{
		m_pCollisionCube->SetVtx(vtxMax, vtxMin);
	}
}