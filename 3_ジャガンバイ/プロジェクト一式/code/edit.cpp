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
#include "inputmouse.h"
#include "manager.h"
#include "debugproc.h"
#include "block.h"
#include "enemy.h"
#include "enemyManager.h"
#include <assert.h>

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(6.0f)	// 移動速度
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
	m_type = CBlock::TYPE_DESK;
	m_mode = MODE_CREATE_BLOCK;
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
	m_type = CBlock::TYPE_DESK;

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
	CInputMouse* pMouse = CInputMouse::GetInstance();

	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	float fSpeed = SPEED_MOVE;
	CBlock **pBlock = CBlock::GetBlock();

	if (m_pObjectCursor != nullptr && pKeyboard != nullptr && pMouse != nullptr)
	{
		if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == false)
		{
			if (pKeyboard->GetPress(DIK_RSHIFT))
			{
				fSpeed *= 5.0f;
			}

			if (pKeyboard->GetTrigger(DIK_A))
			{
				pos.x -= fSpeed;
			}
			if (pKeyboard->GetTrigger(DIK_D))
			{
				pos.x += fSpeed;
			}
			if (pKeyboard->GetTrigger(DIK_W))
			{
				pos.z += fSpeed;
			}
			if (pKeyboard->GetTrigger(DIK_S))
			{
				pos.z -= fSpeed;
			}
			if (pKeyboard->GetTrigger(DIK_E))
			{
				pos.y += fSpeed;
			}
			if (pKeyboard->GetTrigger(DIK_Q))
			{
				pos.y -= fSpeed;
			}
		}

		// 回転
		if (pKeyboard->GetTrigger(DIK_Z))
		{
			rot.y = 0.0f;

			m_pObjectCursor->SetRot(rot);
		}
		if (pKeyboard->GetTrigger(DIK_C))
		{
			rot.y = D3DX_PI * 0.5f;

			m_pObjectCursor->SetRot(rot);
		}

		if (pKeyboard->GetTrigger(DIK_RETURN) || pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
		{// ブロックの生成
			CBlock::Create(m_pObjectCursor->GetPosition(), m_pObjectCursor->GetRot(),m_type);
		}

		// 削除ブロック選択
		int nIdxDelete = CheckDelete();

		if (pKeyboard->GetTrigger(DIK_BACKSPACE) && nIdxDelete != -1)
		{
			CBlock::Delete(nIdxDelete);
		}

		if (pKeyboard->GetTrigger(DIK_1))
		{
			m_type = (CBlock::TYPE)((m_type + CBlock::TYPE_MAX - 1) % CBlock::TYPE_MAX);

			// モデル番号の設定
			m_pObjectCursor->SetIdxModel(pIdx[m_type]);
			m_pObjectCursor->BindModel(pIdx[m_type]);
		}

		if (pKeyboard->GetTrigger(DIK_2) || pMouse->GetMoveIZ() > 0.0f)
		{// ブロックタイプ切り替え
			m_type = (CBlock::TYPE)((m_type + 1) % CBlock::TYPE_MAX);

			// モデル番号の設定
			m_pObjectCursor->SetIdxModel(pIdx[m_type]);
			m_pObjectCursor->BindModel(pIdx[m_type]);
		}

		if (pKeyboard->GetTrigger(DIK_F2))
		{// ブロックの保存
			CBlock::Save();
		}

		if (m_pObjectCursor != nullptr)
		{// カーソルのトランスフォーム
			m_pObjectCursor->SetPosition(m_pObjectCursor->GetPosition() + pos);
		}

		CDebugProc::GetInstance()->Print("\n//=======================\n");
		CDebugProc::GetInstance()->Print("// エディター\n");
		CDebugProc::GetInstance()->Print("//=======================\n");
		CDebugProc::GetInstance()->Print("エディターの位置：[%f,%f,%f]\n", m_pObjectCursor->GetPosition().x, m_pObjectCursor->GetPosition().y, m_pObjectCursor->GetPosition().z);
		CDebugProc::GetInstance()->Print("エディターの向き：[%f,%f,%f]\n", m_pObjectCursor->GetRot().x, m_pObjectCursor->GetRot().y, m_pObjectCursor->GetRot().z);
		CDebugProc::GetInstance()->Print("移動[WASD]\n");
		CDebugProc::GetInstance()->Print("上下移動[QE]\n");
		CDebugProc::GetInstance()->Print("回転[ZC]\n");
		CDebugProc::GetInstance()->Print("設置[ENTER]\n");
		CDebugProc::GetInstance()->Print("保存[F2]\n");
		CDebugProc::GetInstance()->Print("選択中のブロック：[%d]\n", m_nIdxObject);
		CDebugProc::GetInstance()->Print("選択ブロック削除[BACK SPACE]\n");
		CDebugProc::GetInstance()->Print("設置するタイプ：[%d]:[1,2]\n", m_type);
		CDebugProc::GetInstance()->Print("//=======================\n");
	}
}

//=====================================================
// 削除ブロックのチェック
//=====================================================
int CEdit::CheckDelete(void)
{
	CBlock** ppBlock = CBlock::GetBlock();
	int nNumBlock = CBlock::GetNumAll();

	if (ppBlock == nullptr)
	{
		return -1;
	}

	for (int i = 0; i < nNumBlock; i++)
	{
		CBlock* pBlock = ppBlock[i];

		if (pBlock != nullptr)
		{
			pBlock->ResetColor();

			D3DXVECTOR3 posBlock = pBlock->GetPosition();
			D3DXVECTOR3 pos = m_pObjectCursor->GetPosition();
			D3DXVECTOR3 vtxMax = pBlock->GetVtxMax();
			D3DXVECTOR3 vtxMin = pBlock->GetVtxMin();

			if (pos.x >= posBlock.x + vtxMin.x && 
				pos.x <= posBlock.x + vtxMax.x &&
				pos.z >= posBlock.z + vtxMin.z &&
				pos.z <= posBlock.z + vtxMax.z &&
				pos.y >= posBlock.y + vtxMin.y &&
				pos.y <= posBlock.y + vtxMax.y)
			{// ブロック内にいるとき
				pBlock->SetEmissiveCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

				return i;
			}
		}
	}

	return -1;
}