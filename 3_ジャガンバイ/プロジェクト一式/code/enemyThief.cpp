//*****************************************************
//
// 泥棒敵の処理[enemyThief.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemyThief.h"
#include "enemyManager.h"
#include <stdio.h>
#include "playerManager.h"
#include "game.h"
#include "block.h"
#include "universal.h"
#include "rocket.h"
#include "motion.h"
#include "itemRepair.h"
#include "telop.h"
#include "object3D.h"
#include "texture.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace 
{
const float TIME_CAMERAEVENT = 4.0f;	// カメラのイベント継続時間
const float MOVE_SPEED = 1.7f;			// 移動速度

const float LIMID_RANGE_LEFT = -460.0f;
const float LIMID_RANGE_RIGHT = 460.0f;
const float LIMID_RANGE_UP = 460.0f;
const float LIMID_RANGE_DOWN = -460.0f;
const char* CAUTION_PATH = "data\\TEXTURE\\UI\\steal.png";	// 警告表示のテクスチャパス
const D3DXVECTOR3 POS_CAUTION = { 0.0f,150.0f,100.0f };	// 警告の位置
}

//=====================================================
// コンストラクタ
//=====================================================
CEnemyThief::CEnemyThief()
{
	m_bTakeRepair = false;
	m_state = STATE_NONE;
	m_pCaution = nullptr;
	m_bSound = false;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyThief::~CEnemyThief()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyThief::Init(void)
{
	// 継承クラスの初期化
	CEnemy::Init();

	// 体力を個別設定
	SetLife(50.0f);

	// 歩きモーションに設定
	CMotion *pBody = GetBody();

	if (pBody != nullptr)
	{
		pBody->SetMotion(MOTION_WALK);
	}

	// 追跡状態に設定
	m_state = STATE_CHASE;

	// 移動速度の設定
	SetMoveSpeed(MOVE_SPEED);

	// カメライベントの設定
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		D3DXVECTOR3 posOwn = GetPosition();
		D3DXVECTOR3 posV = posOwn;
		posV.x += 100.0f;
		posV.y += 150.0f;
		posV.z -= 150.0f;
		posOwn.y += 60.0f;

		pGame->SetEventCamera(TIME_CAMERAEVENT, posOwn, posV);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyThief::Uninit(void)
{
	if (m_pCaution != nullptr)
	{
		m_pCaution->Uninit();
		m_pCaution = nullptr;
	}

	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 死亡時の処理
//=====================================================
void CEnemyThief::Death(void)
{
	int nMotion = GetMotion();

	if (nMotion != MOTION_DEATH)
	{// 死亡モーションの設定
		SetMotion(MOTION_DEATH);

		if (m_bTakeRepair)
		{// 修理アイテムのドロップ
			CItemRepair *pRepair = CItemRepair::Create();

			if (pRepair != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();

				pRepair->SetPosition(pos);
			}
		}
	}

	CEnemy::Death();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyThief::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();

	CBlock *pBlock = GetTouchBlock();

	if (pBlock != nullptr)
	{
		// 生存チェック
		CBlock **ppBlock = CBlock::GetBlock();

		for (int i = 0; i < NUM_OBJECT; i++)
		{
			if (ppBlock[i] == pBlock)
			{
				pBlock->Hit(5.0f);
			}
		}
	}

	CEnemy::STATE state = GetState();

	if (state != CEnemy::STATE::STATE_DEATH)
	{
		// 更新処理の分岐
		SwitchUpdate();
	}

	if (m_pCaution != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		pos += POS_CAUTION;

		m_pCaution->SetPosition(pos);
	}
}

//=====================================================
// 更新処理の分岐
//=====================================================
void CEnemyThief::SwitchUpdate(void)
{
	switch (m_state)
	{
	case CEnemyThief::STATE_CHASE:	// ロケットの追跡
		// ロケットとの当たり判定
		CollisionRocket();

		break;
	case CEnemyThief::STATE_ESCAPE:	// 逃走状態
		// 逃げる処理
		Escape();
		break;
	default:
		break;
	}
}

//=====================================================
// 目標を追跡する処理
//=====================================================
void CEnemyThief::ChaseTarget(void)
{
	// ゲーム状態によって追跡させない
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL || m_state != STATE_CHASE)
		{
			return;
		}
	}

	// 目標位置の取得
	D3DXVECTOR3 posTarget = { 0.0f,0.0f,0.0f };

	CRocket *pRocket = CRocket::GetInstance();

	if (pRocket != nullptr)
	{
		posTarget = pRocket->GetPosition();
	}

	// 移動量の設定
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posTarget - pos;
	D3DXVECTOR3 move = GetMove();

	D3DXVec3Normalize(&vecDiff, &vecDiff);

	float fSpeed = GetSpeed();

	vecDiff *= fSpeed;

	move += vecDiff;

	SetMove(move);

	// 向きを目標方向に補正
	float fAngleDist = atan2f(vecDiff.x, vecDiff.z);
	D3DXVECTOR3 rot = GetRot();

	fAngleDist += D3DX_PI;

	universal::FactingRot(&rot.y, fAngleDist, 0.1f);

	SetRot(rot);
}

//=====================================================
// ロケットとの当たり判定
//=====================================================
void CEnemyThief::CollisionRocket(void)
{
	CRocket *pRocket = CRocket::GetInstance();

	if (pRocket == nullptr)
	{
		return;
	}

	// 差分距離を取得
	D3DXVECTOR3 posRocket = pRocket->GetPosition();
	D3DXVECTOR3 posOwn = GetPosition();
	D3DXVECTOR3 vecDiff = posOwn - posRocket;

	float fLegnthDiff = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);
	float fLegnth = pRocket->GetRadius();

	if (fLegnthDiff < fLegnth)
	{// 進行状況を戻し、逃走状態に移行
		pRocket->AddProgress(-1);

		m_state = STATE_ESCAPE;

		m_bTakeRepair = true;

		// カメライベントの設定
		CGame *pGame = CGame::GetInstance();

		if (pGame != nullptr)
		{
			D3DXVECTOR3 posV = posOwn;
			posV.x += 100.0f;
			posV.y += 150.0f;
			posOwn.y += 60.0f;

			pGame->SetEventCamera(TIME_CAMERAEVENT, posOwn, posV);
		}

		// テロップの生成
		CTelop::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_STAMP);
			m_bSound = true;
		}

		if (m_pCaution == nullptr)
		{// 警告の生成
			D3DXVECTOR3 pos = GetPosition();

			pos += POS_CAUTION;

			m_pCaution = CObject3D::Create(pos);

			if (m_pCaution != nullptr)
			{
				int nIdx = CTexture::GetInstance()->Regist(CAUTION_PATH);
				m_pCaution->SetIdxTexture(nIdx);

				m_pCaution->SetSize(100.0f, 50.0f);
				m_pCaution->EnableBillboard(true);
				m_pCaution->EnableZtest(true);
				m_pCaution->EnableLighting(false);
				m_pCaution->SetVtx();
			}
		}
	}
}

//=====================================================
// 逃げる処理
//=====================================================
void CEnemyThief::Escape(void)
{
	// ゲーム状態によって動かさない
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL)
		{
			return;
		}
	}

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posTarget = { 0.0f,0.0f,0.0f };

	// 最遠距離の宣言
	float fLengthMax = FLT_MAX;

	for (int i = 0; i < NUM_PLAYER; i++)
	{// 最も近いプレイヤーを参照
		CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

			// 距離の比較
			bool bNear = universal::DistCmp(pos, posPlayer, fLengthMax, &fLengthMax);

			if (bNear)
			{
				posTarget = posPlayer;
			}
		}
	}

	// 移動量の設定
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posTarget - pos;
	D3DXVECTOR3 move = GetMove();

	D3DXVec3Normalize(&vecDiff, &vecDiff);

	float fSpeed = GetSpeed();

	vecDiff *= fSpeed;

	move -= vecDiff;

	SetMove(move);

	// 向きを目標方向に補正
	float fAngleDist = atan2f(vecDiff.x, vecDiff.z);
	D3DXVECTOR3 rot = GetRot();

	universal::FactingRot(&rot.y, fAngleDist, 0.1f);

	SetRot(rot);

	LimidPostion();
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyThief::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}

//=====================================================
// 移動範囲
//=====================================================
void CEnemyThief::LimidPostion(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (pos.x <= LIMID_RANGE_LEFT)
	{
		pos.x = LIMID_RANGE_LEFT;
	}
	else if (pos.x >= LIMID_RANGE_RIGHT)
	{
		pos.x = LIMID_RANGE_RIGHT;
	}
	if (pos.z >= LIMID_RANGE_UP)
	{
		pos.z = LIMID_RANGE_UP;
	}
	else if (pos.z <= LIMID_RANGE_DOWN)
	{
		pos.z = LIMID_RANGE_DOWN;
	}
	SetPosition(pos);
}