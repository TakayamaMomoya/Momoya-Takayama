//*****************************************************
//
// オブジェクトの処理[object.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "camera.h"
#include "manager.h"
#include "collision.h"
#include "renderer.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CObject *CObject::m_apTop[NUM_PRIORITY] = {};	// 先頭のオブジェクトのポインタ
CObject *CObject::m_apCur[NUM_PRIORITY] = {};	// 最後尾のオブジェクトのポインタ
CObject *CObject::m_apNotStop[NUM_OBJECT] = {};	// 停止中も動くオブジェクトの配列
int CObject::m_nNumAll = 0;	// 総数

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CObject::CObject(int nPriority)
{
	m_nNumAll++;

	// 値のクリア
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_bDeath = false;
	m_bWire = false;
	m_bZtest = false;
	m_bNotStop = false;
	m_bLighting = true;
	m_bAdd = false;
	m_type = TYPE::TYPE_NONE;
	m_nID = -1;

	m_nPriority = nPriority;

	if (m_apTop[nPriority] == nullptr)
	{// 先頭と最後尾アドレスの代入
		m_apTop[nPriority] = this;
		m_apCur[nPriority] = this;

		return;
	}

	// 前のアドレスに最後尾のアドレスを代入する
	m_pPrev = m_apCur[nPriority];

	// 最後尾のアドレスを自分にする
	m_apCur[nPriority] = this;

	if (m_pPrev != nullptr)
	{
		// 前のオブジェクトの次のアドレスを自分にする
		m_pPrev->m_pNext = this;
	}
}

//=====================================================
// デストラクタ
//=====================================================
CObject::~CObject()
{
	m_nNumAll--;
}

//=====================================================
// 個別リリース処理
//=====================================================
void CObject::Release(void)
{
	// 死亡フラグを立てる
	m_bDeath = true;

	for (int i = 0; i < NUM_OBJECT; i++)
	{// 停止しないオブジェクトの削除
		if (m_apNotStop[i] == this)
		{
			m_apNotStop[i] = nullptr;

			break;
		}
	}
}

//=====================================================
// 破棄処理
//=====================================================
void CObject::Delete(void)
{
	if (m_apCur[m_nPriority] != this && m_apTop[m_nPriority] != this)
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

	if (m_apTop[m_nPriority] == this)
	{// 先頭アドレスの破棄
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = nullptr;
		}

		// 先頭アドレスを次のアドレスに引き継ぐ
		m_apTop[m_nPriority] = m_pNext;
	}
	
	if (m_apCur[m_nPriority] == this)
	{// 最後尾アドレスの破棄
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = nullptr;
		}

		// 最後尾アドレスを前のアドレスに引き継ぐ
		m_apCur[m_nPriority] = m_pPrev;
	}

	// 自身の破棄
	delete this;
}

// 静的メンバ関数======================================

//=====================================================
// 全リリース処理
//=====================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			// 終了処理
			pObject->Uninit();

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{
				// 削除
				pObject->Delete();
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}
}

//=====================================================
// 全更新処理
//=====================================================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{

			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			// 更新処理
			pObject->Update();

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{
				// 終了処理
				pObject->Delete();
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}
}

//=====================================================
// 止まらないオブジェクトの更新処理
//=====================================================
void CObject::UpdateNotStop(void)
{
	// 更新処理
	for (int i = 0; i < NUM_OBJECT; i++)
	{
		if (m_apNotStop[i] != nullptr)
		{
			m_apNotStop[i]->Update();
		}
	}

	// 死亡フラグの立っているオブジェクトの削除
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{
				// 終了処理
				pObject->Delete();
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}
}

//=====================================================
// 全描画処理
//=====================================================
void CObject::DrawAll(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{// カメラの設定
		pCamera->SetCamera();
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			if (pObject->m_bWire)
			{// ワイヤーフレームの設定
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}

			if (pObject->m_bZtest)
			{// Zテストの設定
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}

			if (pObject->m_bLighting == false)
			{
				// ライティングを無効化
				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			}

			if (pObject->m_bAdd)
			{
				// 加算合成かどうか
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// 描画処理
			pObject->Draw();

			if (pObject->m_bWire)
			{
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}

			if (pObject->m_bZtest)
			{// Zテストの設定
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}

			if (pObject->m_bLighting == false)
			{
				// ライティングを有効化
				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			}

			if (pObject->m_bAdd)
			{
				// 加算合成を戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject* pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{
				// 削除
				pObject->Delete();
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}
}

//=====================================================
// 停止しないオブジェクトに設定する
//=====================================================
void CObject::EnableNotStop(bool bNotStop)
{
	m_bNotStop = bNotStop;

	if (bNotStop)
	{// 止めないオブジェクトに設定

		for (int i = 0; i < NUM_OBJECT; i++)
		{
			if (m_apNotStop[i] == nullptr)
			{
				m_apNotStop[i] = this;

				break;
			}
		}
	}
	else
	{// 止めないオブジェクトから削除
		for (int i = 0; i < NUM_OBJECT; i++)
		{
			if (m_apNotStop[i] == this)
			{
				m_apNotStop[i] = nullptr;

				break;
			}
		}
	}
}

//=====================================================
//タイプ設定処理
//=====================================================
void CObject::SetType(TYPE type)
{
	m_type = type;
}