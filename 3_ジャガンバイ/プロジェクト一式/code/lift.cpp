//*****************************************************
//
// リフト処理[lift.cpp]
// Author:小笠原彪
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "lift.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "playerManager.h"
#include "player.h"
#include "fade.h"
#include "object2D.h"

#include "debugproc.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
	const float UP = 2.0f;
	const float RANGE(100.0f);
}

//=====================================================
//	静的メンバ変数宣言
//=====================================================
int CLift::m_nNumJoinPlayer = 0;
int CLift::m_nInCnt = 0;
bool CLift::m_abJoin[NUM_PLAYER] = {};

//=====================================================
// コンストラクタ
//=====================================================
CLift::CLift(int nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_nInCnt = 0;
	m_state = STATE_NONE;
}

//=====================================================
// デストラクタ
//=====================================================
CLift::~CLift()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CLift::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	int nIdx = CModel::Load("data\\MODEL\\select\\lift.x");
	// モデル読込
	BindModel(nIdx);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CLift::Uninit(void)
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CLift::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();

	// 情報取得
	CFade* pFade = CFade::GetInstance();
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CPlayer* pPlayer = nullptr;

	// 変数宣言
	int nInPlayer = 0;	// 範囲内プレイヤーカウント用

	// リフト位置の取得
	D3DXVECTOR3 pos = GetPosition();

	if (m_state == STATE_UP)
	{
		pos.y += UP;
	}

	SetPosition(pos);

	for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
	{
		// プレイヤーマネージャから情報を取得
		pPlayer = pPlayerManager->GetPlayer(nCnt);

		if (pPlayer == nullptr)
		{
			continue;
		}

		// リフト範囲の設定
		if (GetPosition().x + RANGE >= pPlayer->GetPosition().x &&
			GetPosition().x - RANGE <= pPlayer->GetPosition().x &&
			GetPosition().z + RANGE >= pPlayer->GetPosition().z &&
			GetPosition().z - RANGE <= pPlayer->GetPosition().z)
		{
			// プレイヤー位置の取得
			D3DXVECTOR3 playerPos = pPlayer->GetPosition();
			D3DXVECTOR3 playerRot = pPlayer->GetRot();

			// 移動量の設定
			D3DXVECTOR3 movePlayer = pPlayer->GetMove();
			movePlayer.y = 0.0f;
			pPlayer->SetMove(movePlayer);

			// 位置の設定
			pPlayer->SetPosition(D3DXVECTOR3(playerPos.x, pos.y, playerPos.z));
			pPlayer->SetRot(D3DXVECTOR3(playerRot.x, playerRot.y, playerRot.z));

			// 参加
			m_abJoin[nCnt] = true;	

			nInPlayer++;	// 範囲内プレイヤーのカウントアップ
		}
		else
		{
			// 不参加
			m_abJoin[nCnt] = false;
		}
	}

	// 範囲内プレイヤーのカウント
	m_nInCnt = nInPlayer;

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\n開始場所\n");
	CDebugProc::GetInstance()->Print("範囲内プレイヤー[%d]\n", nInPlayer);
#endif
}

//=====================================================
// 描画処理
//=====================================================
void CLift::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//=====================================================
// 生成処理
//=====================================================
CLift* CLift::Create(D3DXVECTOR3 pos,int nPriority)
{
	CLift* pLift = nullptr;

	if (pLift == nullptr)
	{// インスタンス生成
		pLift = new CLift(nPriority);

		if (pLift != nullptr)
		{
			pLift->SetPosition(pos);

			// 初期化処理
			pLift->Init();
		}
	}

	return pLift;
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CLift::Load(void)
{
	return S_OK;
}

//=====================================================
// 範囲内にいるプレイヤーのカウント
//=====================================================
int CLift::GetInPlayer(void)
{
	return m_nInCnt;
}

//=====================================================
// 範囲内に入った人数の判定
//=====================================================
bool CLift::GetIsIn(void)
{
	if (m_nNumJoinPlayer <= 0 && m_nInCnt <= 0)
	{
		return false;
	}
	else if (m_nNumJoinPlayer == m_nInCnt)
	{
		return true;
	}

	return false;
}
