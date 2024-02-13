//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemy.h"
#include "manager.h"
#include "collision.h"
#include "debugproc.h"
#include "particle.h"
#include <stdio.h>
#include "player.h"
#include "playerManager.h"
#include "game.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "motion.h"
#include "universal.h"
#include "animEffect3D.h"
#include "sound.h"
#include "particle.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float INITIAL_LIFE = 10.0f;	// 初期体力
	const float INITIAL_SPEED = 1.0f;	// 初期速度
	const float LINE_CHASE = 470;	// 追跡状態に移行するエリア
	const int DAMAGE_FRAME = 10;	// ダメージ状態のフレーム数
	const int TIME_DEATH = 60;	// 死亡までの時間
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CEnemy::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CEnemy::CEnemy()
{
	// 総数カウントアップ
	m_nNumAll++;

	// 先頭、最後尾アドレス取得
	CEnemyManager *pManager = CEnemyManager::GetInstance();
	CEnemy *pHead = nullptr;
	CEnemy *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

	m_fLife = 0;
	m_fMoveSpeed = 0.0f;
	m_nTimerState = 0;
	m_pCollisionSphere = nullptr;
	m_pCollisionCube = nullptr;
	m_pShadow = nullptr;
	m_state = STATE_NORMAL;
	m_moveState = MOVESTATE_NONE;
	m_posDest = { 0.0f,0.0f,0.0f };

	// 値のクリア
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_pBlock = nullptr;

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
CEnemy::~CEnemy()
{
	// 先頭、最後尾アドレス取得
	CEnemyManager *pManager = CEnemyManager::GetInstance();
	CEnemy *pHead = nullptr;
	CEnemy *pTail = nullptr;

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
}

//=====================================================
// 生成処理
//=====================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// インスタンス生成
		switch (type)
		{
		case TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			pEnemy->Load("data\\Character\\robot00.txt");

			break;
		default:
			break;
		}

		// 位置設定
		pEnemy->SetPosition(pos);

		// 初期化処理
		pEnemy->Init();
	}

	return pEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemy::Init(void)
{
	// 継承クラスの初期化
	CCharacter::Init();

	// タイプの設定
	SetType(TYPE_ENEMY);

	if (m_pCollisionSphere == nullptr)
	{// 球の当たり判定生成
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMY, CCollision::TYPE_SPHERE,this);

		if (m_pCollisionSphere != nullptr)
		{// 情報の設定
			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(50.0f);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// 立方体の当たり判定
		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_ENEMY, this);

		D3DXVECTOR3 vtxMax = { 20,20,20 };
		D3DXVECTOR3 vtxMin = { -20,0.0f,-20 };

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// パラメーター初期設定
	m_fLife = INITIAL_LIFE;
	SetMoveSpeed(INITIAL_SPEED);

	// 通常状態にする
	m_state = STATE_NORMAL;
	m_moveState = MOVESTATE_INTRUSION;

	SetPositionOld(GetPosition());

	// 影の有効可
	CMotion *pBody = GetBody();

	if (pBody != nullptr)
	{
		pBody->EnableShadow(true);
		pBody->SetPosShadow(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemy::Uninit(void)
{
	// 当たり判定削除
	DeleteCollision();

	// 継承クラスの終了
	CCharacter::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemy::Update(void)
{
	// 継承クラスの更新
	CCharacter::Update();

	// 目標の追跡
	ChaseTarget();

	// 状態管理
	ManageState();

	// 移動状態の管理
	ManageMoveState();

	// 当たり判定の管理
	ManageCollision();

	// 移動量を反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();

	SetPositionOld(pos);

	pos += move;
	SetPosition(pos);

	// 移動量の減衰
	move *= 0.1f;
	SetMove(move);
}

//=====================================================
// 当たり判定の管理
//=====================================================
void CEnemy::ManageCollision(void)
{
	if (m_pCollisionSphere != nullptr)
	{// 球の当たり判定の管理
		CMotion* pBody = GetBody();

		if (pBody != nullptr)
		{
			D3DXVECTOR3 posWaist = pBody->GetMtxPos(0);

			D3DXVECTOR3 pos = GetPosition();

			m_pCollisionSphere->SetPosition(pos);

			m_pCollisionSphere->PushCollision(&pos, CCollision::TAG_PLAYER);
			m_pCollisionSphere->PushCollision(&pos, CCollision::TAG_ENEMY);

			SetPosition(pos);

			m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());

			m_pCollisionSphere->SetPosition(posWaist);
		}
	}

	if (m_pCollisionCube != nullptr)
	{// 立方体の当たり判定の管理
		// 当たり判定の位置設定
		m_pCollisionCube->SetPositionOld(m_pCollisionCube->GetPosition());
		m_pCollisionCube->SetPosition(GetPosition());

		D3DXVECTOR3 move = GetMove();

		// 押し出しの当たり判定
		m_pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move, (CObject**)&m_pBlock);
		SetMove(move);
	}
}

//=====================================================
// 状態管理
//=====================================================
void CEnemy::ManageState(void)
{
	D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	switch (m_state)
	{
	case STATE_NORMAL:
		break;
	case STATE_DAMAGE:
		if (m_nTimerState > DAMAGE_FRAME)
		{// 通常状態に戻る
			m_nTimerState = 0;
			m_state = STATE_NORMAL;

			GetBody()->ResetAllCol();
		}
		else
		{// カウントアップ
			m_nTimerState++;
		}

		break;
	case STATE_DEATH:

		m_nTimerState++;

		// 透明になりながら消える
		col = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

		col.a = 1.0f - (float)((float)(m_nTimerState) / (TIME_DEATH));

		GetBody()->SetAllCol(col);

		if (m_nTimerState >= TIME_DEATH)
		{// 死亡
			Uninit();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// 移動状態管理
//=====================================================
void CEnemy::ManageMoveState(void)
{
	switch (m_moveState)
	{
	case MOVESTATE_INTRUSION:
		// 侵入しようとしている状態
		if (IsInArea())
		{// エリア内に入ったら、追跡へ移行
			TransferChase();
		}

		break;
	case MOVESTATE_CHASE:
		// 対象を追跡している状態
		

		break;
	}
}

//=====================================================
// 追跡に移行する
//=====================================================
void CEnemy::TransferChase(void)
{
	m_moveState = MOVESTATE_CHASE;
}

//=====================================================
// エリアに入った判定
//=====================================================
bool CEnemy::IsInArea(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (pos.x <= LINE_CHASE && pos.x >= -LINE_CHASE && pos.z <= LINE_CHASE && pos.z >= -LINE_CHASE)
	{
		return true;
	}

	return false;
}

//=====================================================
// 目標の追跡
//=====================================================
void CEnemy::ChaseTarget(void)
{
	// 状態によって追跡させない
	CGame *pGame = CGame::GetInstance();
	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_DEATH)
	{
		return;
	}

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL)
		{
			return;
		}
	}

	// 移動量の設定
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = m_posDest - pos;
	D3DXVECTOR3 move = GetMove();

	D3DXVec3Normalize(&vecDiff, &vecDiff);

	vecDiff *= m_fMoveSpeed;

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
// 体力設定
//=====================================================
void CEnemy::SetLife(float fLife)
{
	m_fLife = fLife;

	if (m_fLife < 0)
	{
		m_fLife = 0;
	}
}

//=====================================================
// ヒット処理
//=====================================================
void CEnemy::Hit(float fDamage)
{
	if (m_state != STATE_DEATH)
	{
		m_fLife -= fDamage;

		// ダメージエフェクトの生成
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_HIT);
		}

		if (pAnim3D != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();

			pAnim3D->CreateEffect(pos,CAnimEffect3D::TYPE::TYPE_BLOOD);
		}

		if (m_fLife <= 0.0f)
		{// 死亡状態
			m_fLife = 0.0f;

			m_state = STATE_DEATH;

			// スコア管理
			ManageScore();

			if (pAnim3D != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();

				pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_BLOOD1);
			}

			// トマト汁
			CParticle::Create(GetPosition(), CParticle::TYPE::TYPE_TOMATO_JUICE);

			// 当たり判定削除
			DeleteCollision();

			// 死亡処理
			Death();
		}
		else
		{
			m_state = STATE_DAMAGE;

			// ヒット色
			D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			GetBody()->SetAllCol(col);
		}
	}
}

//=====================================================
// 死亡処理
//=====================================================
void CEnemy::Death(void)
{
	m_state = STATE_DEATH;
}

//=====================================================
// 当たり判定削除
//=====================================================
void CEnemy::DeleteCollision(void)
{
	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の消去
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pCollisionCube != nullptr)
	{// 当たり判定の消去
		m_pCollisionCube->Uninit();

		m_pCollisionCube = nullptr;
	}
}

//=====================================================
// スコア管理
//=====================================================
void CEnemy::ManageScore(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// 継承クラスの描画
	CCharacter::Draw();

#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n敵の位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	//CDebugProc::GetInstance()->Print("\n敵の移動量：[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
#endif
}

//=====================================================
// 球の当たり判定の位置設定
//=====================================================
void CEnemy::SetSpherePosition(D3DXVECTOR3 pos)
{
	if (m_pCollisionSphere != nullptr)
	{// 位置設定
		m_pCollisionSphere->SetPosition(pos);
	}
}