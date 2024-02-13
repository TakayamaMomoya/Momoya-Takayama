//*****************************************************
//
// エディターの処理[edit.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "edit.h"
#include "objectX.h"
#include "inputkeyboard.h"
#include "manager.h"
#include "debugproc.h"
#include "block.h"
#include "enemy.h"
#include "enemyManager.h"
#include <assert.h>

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(0.5f)	// 移動速度
#define SPEED_ROTATION	(0.01f)	// 回転

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CEdit *CEdit::m_pEdit = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CEdit::CEdit()
{
	m_pObjectCursor = nullptr;
	m_nIdxObject = 0;
	m_type = CBlock::TYPE_FLOOR;
}

//=====================================================
// デストラクタ
//=====================================================
CEdit::~CEdit()
{

}

//=====================================================
// 生成処理
//=====================================================
CEdit *CEdit::Create(void)
{
	if (m_pEdit == nullptr)
	{// インスタンス生成
		m_pEdit = new CEdit;

		// 初期化処理
		m_pEdit->Init();
	}

	return m_pEdit;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEdit::Init(void)
{
	// 番号取得
	int *pIdx = CBlock::GetIndex();

	m_pObjectCursor = nullptr;

	m_pObjectCursor = CObjectX::Create();

	// タイプの初期設定
	m_type = CBlock::TYPE_FLOOR;

	// モデル番号の設定
	m_pObjectCursor->SetIdxModel(pIdx[m_type]);
	m_pObjectCursor->BindModel(pIdx[m_type]);

	m_pObjectCursor->SetEmissiveCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEdit::Uninit(void)
{
	if (m_pObjectCursor != nullptr)
	{
		m_pObjectCursor->Uninit();
		m_pObjectCursor = nullptr;
	}

	m_pEdit = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CEdit::Update(void)
{
	// 番号取得
	int *pIdx = CBlock::GetIndex();

	// 情報取得
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	// 変数宣言
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	float fSpeed = SPEED_MOVE;
	CBlock **pBlock = CBlock::GetBlock();

	if (m_pObjectCursor != nullptr && pKeyboard != nullptr)
	{
		if (pKeyboard->GetPress(DIK_RSHIFT))
		{
			fSpeed *= 5.0f;
		}

		if (pKeyboard->GetPress(DIK_J))
		{
			pos.x -= fSpeed;
		}
		if (pKeyboard->GetPress(DIK_L))
		{
			pos.x += fSpeed;
		}
		if (pKeyboard->GetPress(DIK_I))
		{
			pos.z += fSpeed;
		}
		if (pKeyboard->GetPress(DIK_K))
		{
			pos.z -= fSpeed;
		}
		if (pKeyboard->GetPress(DIK_U))
		{
			pos.y += fSpeed;
		}
		if (pKeyboard->GetPress(DIK_O))
		{
			pos.y -= fSpeed;
		}

		// 回転
		if (pKeyboard->GetPress(DIK_N))
		{
			rot.y += SPEED_ROTATION;
		}
		if (pKeyboard->GetPress(DIK_M))
		{
			rot.y -= SPEED_ROTATION;
		}

		if (pKeyboard->GetTrigger(DIK_B))
		{
			m_pObjectCursor->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// ブロックの生成
			CBlock::Create(m_pObjectCursor->GetPosition(), m_pObjectCursor->GetRot(),m_type);
		}

		if (pKeyboard->GetTrigger(DIK_BACKSPACE))
		{// 敵の生成
			CEnemyManager *pEnemyManager = nullptr;

			pEnemyManager = CEnemyManager::GetInstance();

			if (pEnemyManager != nullptr)
			{
				pEnemyManager->CreateEnemy(m_pObjectCursor->GetPosition(), CEnemy::TYPE_DRONE);
			}
		}

		if (pKeyboard->GetTrigger(DIK_0) && CBlock::GetNumAll() != 0)
		{// オブジェクト選択処理
			if (pBlock[m_nIdxObject] != nullptr)
			{
				pBlock[m_nIdxObject]->ResetColor();
			}

			m_nIdxObject = (m_nIdxObject + 1) % CBlock::GetNumAll();

			if (pBlock[m_nIdxObject] != nullptr)
			{
				pBlock[m_nIdxObject]->SetEmissiveCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		if (pKeyboard->GetTrigger(DIK_MINUS) && CBlock::GetNumAll() != 0)
		{// オブジェクト選択処理
			if (pBlock[m_nIdxObject] != nullptr)
			{
				pBlock[m_nIdxObject]->ResetColor();
			}
			
			m_nIdxObject = (m_nIdxObject + CBlock::GetNumAll() - 1) % CBlock::GetNumAll();

			if (pBlock[m_nIdxObject] != nullptr)
			{
				pBlock[m_nIdxObject]->SetEmissiveCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		if (pKeyboard->GetTrigger(DIK_6))
		{
			m_type = (CBlock::TYPE)((m_type + CBlock::TYPE_MAX - 1) % CBlock::TYPE_MAX);

			// モデル番号の設定
			m_pObjectCursor->SetIdxModel(pIdx[m_type]);
			m_pObjectCursor->BindModel(pIdx[m_type]);
		}

		if (pKeyboard->GetTrigger(DIK_7))
		{// ブロックタイプ切り替え
			m_type = (CBlock::TYPE)((m_type + 1) % CBlock::TYPE_MAX);

			// モデル番号の設定
			m_pObjectCursor->SetIdxModel(pIdx[m_type]);
			m_pObjectCursor->BindModel(pIdx[m_type]);
		}

		if(pKeyboard->GetTrigger(DIK_9))
		{// 指定のブロックを削除
			CBlock::Delete(m_nIdxObject);
		}

		if (pKeyboard->GetTrigger(DIK_8))
		{// ブロックの保存
			CBlock::Save();
		}

		if (m_pObjectCursor != nullptr)
		{// カーソルのトランスフォーム
			m_pObjectCursor->SetPosition(m_pObjectCursor->GetPosition() + pos);
			m_pObjectCursor->SetRot(m_pObjectCursor->GetRot() + rot);
		}

		CDebugProc::GetInstance()->Print("\n//=======================\n");
		CDebugProc::GetInstance()->Print("// エディター\n");
		CDebugProc::GetInstance()->Print("//=======================\n");
		CDebugProc::GetInstance()->Print("エディターの位置：[%f,%f,%f]\n", m_pObjectCursor->GetPosition().x, m_pObjectCursor->GetPosition().y, m_pObjectCursor->GetPosition().z);
		CDebugProc::GetInstance()->Print("エディターの向き：[%f,%f,%f]\n", m_pObjectCursor->GetRot().x, m_pObjectCursor->GetRot().y, m_pObjectCursor->GetRot().z);
		CDebugProc::GetInstance()->Print("移動[IJKL]\n");
		CDebugProc::GetInstance()->Print("上下移動[UO]\n");
		CDebugProc::GetInstance()->Print("設置[ENTER]\n");
		CDebugProc::GetInstance()->Print("保存[8]\n");
		CDebugProc::GetInstance()->Print("選択中のブロック：[%d]:[0]\n", m_nIdxObject);
		CDebugProc::GetInstance()->Print("選択ブロック削除[9]\n");
		CDebugProc::GetInstance()->Print("設置するタイプ：[%d]:[7]\n", m_type);
		CDebugProc::GetInstance()->Print("//=======================\n");
	}
}