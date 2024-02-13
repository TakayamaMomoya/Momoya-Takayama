//*****************************************************
//
// ゴールの処理[goal.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "goal.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "fade.h"
#include "goalTimer.h"
#include "playerManager.h"
#include "player.h"
#include "result.h"
#include "rocket.h"
#include "game.h"
#include "sound.h"
#include "object3D.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* INFO_PATH = "data\\TEXT\\goal.txt";	// ゴール情報のテキスト
const char* TEX_PATH = "data\\TEXTURE\\UI\\escape.png";	// 脱出表示テクスチャのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CGoal *CGoal::m_pGoal = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CGoal::CGoal(int nPriority) : CObjectX(nPriority)
{
	m_bFinish = false;
	m_fRadius = 0.0f;
	m_pCollisionGoal = nullptr;
	m_pArea = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CGoal::~CGoal()
{

}

//=====================================================
// 生成処理
//=====================================================
CGoal *CGoal::Create()
{
	if (m_pGoal == nullptr)
	{
		m_pGoal = new CGoal;

		if (m_pGoal != nullptr)
		{
			// 初期化
			m_pGoal->Init();
		}
	}

	return m_pGoal;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGoal::Init(void)
{
	// 継承クラスの初期化
	//CObjectX::Init();

	if (m_pCollisionGoal == nullptr)
	{// 球の当たり判定生成
		m_pCollisionGoal = CCollisionSphere::Create(CCollision::TAG_GOAL, CCollision::TYPE_SPHERE, this);

		if (m_pCollisionGoal != nullptr)
		{// 情報の設定
			m_pCollisionGoal->SetPosition(GetPosition());
			m_pCollisionGoal->SetRadius(5);
		}
	}

	// 情報読み込み
	Load();

	if (m_pArea == nullptr)
	{// 範囲表示の生成
		D3DXVECTOR3 pos = GetPosition();
		pos.y += 1.0f;

		m_pArea = CObject3D::Create(pos);

		if (m_pArea != nullptr)
		{
			int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
			m_pArea->SetIdxTexture(nIdx);

			m_pArea->SetSize(m_fRadius, m_fRadius);
			m_pArea->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// 読み込み処理
//=====================================================
void CGoal::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntParam = 0;

	// ファイルから読み込む
	FILE* pFile = fopen(INFO_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "GOALSET") == 0)
			{// パラメーター読込開始
				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// 基底パラメーター読み込み
					ApplyInfo(pFile, &cTemp[0]);

					if (strcmp(cTemp, "END_GOALSET") == 0)
					{// パラメーター読込終了
						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 読み込みの終了
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		assert(("ゴール情報読み込みに失敗", false));
	}
}

//=====================================================
// 情報反映
//=====================================================
void CGoal::ApplyInfo(FILE* pFile, char* pTemp)
{
	if (strcmp(pTemp, "POS") == 0)
	{// 位置
		(void)fscanf(pFile, "%s", pTemp);

		D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

		for (int i = 0;i < 3;i++)
		{
			(void)fscanf(pFile, "%f", &pos[i]);
		}

		SetPosition(pos);

		if (m_pCollisionGoal != nullptr)
		{// 当たり判定の位置設定
			m_pCollisionGoal->SetPosition(pos);
		}
	}

	if (strcmp(pTemp, "MODEL") == 0)
	{// モデル読み込み
		//(void)fscanf(pFile, "%s", pTemp);

		//(void)fscanf(pFile, "%s", pTemp);

		//// モデルの読込
		//int nIdx = CModel::Load(pTemp);
		//SetIdxModel(nIdx);
		//BindModel(nIdx);
	}

	if (strcmp(pTemp, "RADIUS") == 0)
	{// 判定の大きさ
		float fRadius;

		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &fRadius);

		m_fRadius = fRadius;

		if (m_pCollisionGoal != nullptr)
		{// 当たり判定の位置設定
			m_pCollisionGoal->SetRadius(fRadius);
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CGoal::Uninit(void)
{
	if (m_pCollisionGoal != nullptr)
	{
		m_pCollisionGoal->Uninit();
		m_pCollisionGoal = nullptr;
	}

	if (m_pArea != nullptr)
	{
		m_pArea->Uninit();
		m_pArea = nullptr;
	}

	m_pGoal = nullptr;

	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CGoal::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();

	// プレイヤーの検出
	DetectPlayer();

	if (m_pCollisionGoal != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionGoal->SetPosition(pos);
	}

	if (m_pArea != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		pos.y += 1.0f;

		m_pArea->SetPosition(pos);
		m_pArea->SetVtx();
	}
}

//=====================================================
// プレイヤーの検出
//=====================================================
void CGoal::DetectPlayer(void)
{
	CGoalTimer *pGoalTimer = CGoalTimer::GetInstance();

	if (pGoalTimer != nullptr)
	{
		return;
	}

	if (m_pCollisionGoal != nullptr)
	{
		if (m_pCollisionGoal->OnEnter(CCollision::TAG_PLAYER))
		{
			// ゴールタイマーの生成
			CGoalTimer::Create();
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CGoal::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//=====================================================
// 制限時間の終了
//=====================================================
void CGoal::DeadLine(void)
{
	m_bFinish = true;

	// ロケットを飛ばす
	CRocket *pRocket = CRocket::GetInstance();

	if (pRocket != nullptr)
	{
		pRocket->SetState(CRocket::STATE::STATE_ESCAPE);
	}

	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_SE_LUNCH);
	}

	// ゲームを脱出状態にする
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		pGame->SetState(CGame::STATE::STATE_ESCAPE);
	}
}

//=====================================================
// リザルトに移る処理
//=====================================================
void CGoal::SetResult(void)
{
	// ゲームをリザルト状態にする
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		pGame->SetState(CGame::STATE::STATE_RESULT);
	}

	// リザルトの生成
	CResult *pResult = CResult::Create();

	if (pResult == nullptr)
	{
		return;
	}

	// 範囲内のプレイヤー検出
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{
		for (int i = 0;i < NUM_PLAYER;i++)
		{
			CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

			if (pPlayer != nullptr)
			{
				if (pResult != nullptr)
				{// リザルトにプレイヤー情報を渡す
					// 座標の差分から距離を計算
					D3DXVECTOR3 pos = GetPosition();
					D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
					posPlayer.y = 0.0f;
					D3DXVECTOR3 vecDiff = posPlayer - pos;
					float fDiff = D3DXVec3Length(&vecDiff);
					
					if (fDiff < m_fRadius)
					{
						pResult->SetSurvived(pPlayer);
					}
				}
			}
		}
	}
}