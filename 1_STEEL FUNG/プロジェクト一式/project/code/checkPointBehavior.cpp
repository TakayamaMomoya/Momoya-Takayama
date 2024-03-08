//*****************************************************
//
// チェックポイントのBehavior[checkpointBehavior.h]
// Author:高山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "checkPointBehavior.h"
#include "checkPointManager.h"
#include "UI.h"
#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "enemyManager.h"
#include "limit.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SIZE_CURSOR = 20.0f;	// カーソルサイズ
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\checkPoint00.png";	// カーソルのテクスチャ
const float DIST_PROGRESS = 1000.0f;	// 進行する距離
const float LIMIT_DIST = 200.0f;	// 画面中心から離れる制限距離
}

//=====================================================
// コンストラクタ
//=====================================================
CCheckPointBehavior::CCheckPointBehavior()
{

}

//=====================================================
// デストラクタ
//=====================================================
CCheckPointBehavior::~CCheckPointBehavior()
{

}

//=====================================================
// 移動パート
//=====================================================
CCheckPointMove::CCheckPointMove()
{// コンストラクタ
	m_pCursor = nullptr;
}

CCheckPointMove::~CCheckPointMove()
{// デストラクタ

}

void CCheckPointMove::Init(CCheckPointManager *pCheckPoint)
{// 初期化
	if (m_pCursor == nullptr)
	{// カーソル生成
		m_pCursor = CUI::Create();

		if (m_pCursor != nullptr)
		{
			m_pCursor->SetSize(SIZE_CURSOR * 3, SIZE_CURSOR * 3);

			m_pCursor->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
			m_pCursor->SetIdxTexture(nIdx);
			m_pCursor->SetVtx();
		}
	}
}

void CCheckPointMove::Uninit(CCheckPointManager *pCheckPoint)
{// 終了
	Object::DeleteObject((CObject**)&m_pCursor);
}

void CCheckPointMove::Update(CCheckPointManager *pCheckPoint)
{// 更新
	int nProgress = pCheckPoint->GetProgress();

	D3DXVECTOR3 posNext = pCheckPoint->GetCheckPosition(nProgress + 1);

	// カーソルの表示
	if (m_pCursor != nullptr)
	{
		// 大きさの変更
		float fHeight = m_pCursor->GetHeight();
		float fWidth = m_pCursor->GetWidth();

		fWidth += (SIZE_CURSOR - fWidth) * 0.1f;
		fHeight += (SIZE_CURSOR - fHeight) * 0.1f;

		m_pCursor->SetSize(fWidth, fHeight);

		D3DXMATRIX mtx;
		D3DXVECTOR3 posScreen;

		bool bInScreen = universal::IsInScreen(posNext, mtx, &posScreen);

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

		m_pCursor->SetPosition(posScreen);
		m_pCursor->SetVtx();
	}

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// 到着判定
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		if (posPlayer.x >= posNext.x)
		{// 進行状況の加算、戦闘へ移行
			pCheckPoint->ChangeBehavior(new CCheckPointBattle);
		}
	}
}

//=====================================================
// 戦闘状態
//=====================================================
CCheckPointBattle::CCheckPointBattle()
{// コンストラクタ
	ZeroMemory(&m_apLimit[0], sizeof(m_apLimit));
}

CCheckPointBattle::~CCheckPointBattle()
{// デストラクタ

}

void CCheckPointBattle::Init(CCheckPointManager *pCheckPoint)
{// 初期化
	int nProgress = pCheckPoint->GetProgress();

	D3DXVECTOR3 pos = pCheckPoint->GetCheckPosition(nProgress + 1);

	D3DXVECTOR3 aPos[CheckPointBehavior::NUM_LIMIT] =
	{
		{6000.0f,0.0f,0.0f},
		{-6000.0f,0.0f,0.0f},
	};
	D3DXVECTOR3 aRot[CheckPointBehavior::NUM_LIMIT] =
	{
		{0.0f,D3DX_PI * 0.5f,0.0f},
		{0.0f,-D3DX_PI * 0.5f,0.0f},
	};

	for (int i = 0; i < CheckPointBehavior::NUM_LIMIT; i++)
	{
		m_apLimit[i] = CLimit::Create();

		if (m_apLimit[i] != nullptr)
		{
			m_apLimit[i]->SetPosition(pos + aPos[i]);
			m_apLimit[i]->SetRotation(aRot[i]);
		}
	}
}

void CCheckPointBattle::Uninit(CCheckPointManager *pCheckPoint)
{// 終了
	Object::DeleteObject((CObject**)&m_apLimit[0], CheckPointBehavior::NUM_LIMIT);
}

void CCheckPointBattle::Update(CCheckPointManager *pCheckPoint)
{// 更新
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();
	int nNumEnemy = CEnemy::GetNumAll();
	int nProgress = pCheckPoint->GetProgress();

	if (pEnemyManager != nullptr)
	{
		bool bEndSpawn = pEnemyManager->IsEndSpawn();

		pEnemyManager->SpawnGroup(nProgress + 1);

		if (nNumEnemy == 0 && bEndSpawn)
		{
			pEnemyManager->EnableEndSpawn(false);
			pCheckPoint->AddProgress();
			pCheckPoint->ChangeBehavior(new CCheckPointMove);
		}
	}
}