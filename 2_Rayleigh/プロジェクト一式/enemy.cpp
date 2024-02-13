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
#include "score.h"
#include "collision.h"
#include "debugproc.h"
#include "particle.h"
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "enemyshot.h"
#include "enemyManager.h"
#include "shadow.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(1.0f)	// 移動速度
#define RATE_RADIUS	(1.5f)	// 当たり判定の大きさの倍率
#define INITIAL_LIFE	(5.0f)	// 初期体力
#define DAMAGE_FRAME	(10)	// ダメージ状態の継続フレーム数
#define INITIAL_SCORE	(1000)	// 初期スコア
#define TIME_DEATH	(30)	// 死亡までのタイム
#define ROLL_FACT	(0.1f)	// 回転係数

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
	m_nScore = 0;
	m_nTimerState = 0;
	m_pCollisionSphere = nullptr;
	m_pCollisionCube = nullptr;
	m_pShadow = nullptr;
	m_state = STATE_NORMAL;

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

	if (m_state != STATE_DEATH)
	{
		m_nNumAll--;
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
	else if (pTail == this)
	{// 最後尾アドレスの破棄
		if (m_pPrev != nullptr)
		{// 最後尾アドレスを前のアドレスに引き継ぐ
			pManager->SetTail(m_pPrev);

			m_pPrev->m_pNext = nullptr;
		}
	}
	else
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
		case TYPE_SHOT:

			pEnemy = new CEnemyShot;

			pEnemy->CMotion::Load("data\\MOTION\\robot00.txt");

			pEnemy->SetMotion(0);

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
	// 最も大きいパーツの半径取得
	float fRadius = GetRadiusMax();

	// 継承クラスの初期化
	CMotion::Init();

	// タイプの設定
	SetType(TYPE_ENEMY);

	if (m_pCollisionSphere == nullptr)
	{// 球の当たり判定生成
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMY, CCollision::TYPE_SPHERE,this);

		if (m_pCollisionSphere != nullptr)
		{// 情報の設定
			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(fRadius);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// 立方体の当たり判定
		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_ENEMY, this);

		D3DXVECTOR3 vtxMax = { fRadius,fRadius,fRadius };
		D3DXVECTOR3 vtxMin = { -fRadius,-fRadius,-fRadius };

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// 体力の初期設定
	m_fLife = INITIAL_LIFE;

	// 通常状態にする
	m_state = STATE_NORMAL;

	m_nScore = INITIAL_SCORE;

	if (m_pShadow == nullptr)
	{// 影の生成
		m_pShadow = CShadow::Create(GetPosition(), 10.0f, 10.0f);
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

	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemy::Update(void)
{
	// 継承クラスの更新
	CMotion::Update();

	if (m_state != STATE_DEATH)
	{
		if (m_pShadow != nullptr)
		{// 影の追従
			D3DXVECTOR3 pos = GetMtxPos(0);

			m_pShadow->SetPosition(pos);
		}
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

		SetAllCol(col);

		if (m_nTimerState >= TIME_DEATH)
		{// 死亡
			Death();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// 目標方向を向く処理
//=====================================================
void CEnemy::RotDest(void)
{
	// 目標取得
	CPlayer *pPlayer;
	if (CPlayer::GetInstance() != nullptr)
	{
		pPlayer = CPlayer::GetInstance();
	}
	else
	{
		return;
	}

	D3DXVECTOR3 posTarget = pPlayer->GetPosition();
	D3DXVECTOR3 pos = GetPosition();
	float fRot = 0.0f;

	if (posTarget.x <= pos.x)
	{// 右側
		fRot = D3DX_PI * 0.5f;
	}
	else
	{// 左側
		fRot = -D3DX_PI * 0.5f;
	}

	// 変数宣言
	D3DXVECTOR3 vecDest;
	D3DXVECTOR3 rot = GetRot();

	float fRotDiff = fRot - rot.y;

	// 角度の修正
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= 6.28f;
	}
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += 6.28f;
	}

	// 角度補正
	SetRot(D3DXVECTOR3(rot.x, rot.y + fRotDiff * ROLL_FACT, rot.z));

	// 角度の修正
	rot = GetRot();

	if (GetRot().y > D3DX_PI)
	{
		// 角度補正
		SetRot(D3DXVECTOR3(GetRot().x, GetRot().y - 6.28f, GetRot().z));
	}
	if (GetRot().y < -D3DX_PI)
	{
		// 角度補正
		SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + 6.28f, GetRot().z));
	}
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
	if (m_state == STATE_NORMAL)
	{
		m_fLife -= fDamage;

		if (m_fLife <= 0.0f)
		{// 死亡状態
			m_fLife = 0.0f;

			m_state = STATE_DEATH;

			// スコア管理
			ManageScore();

			// 当たり判定削除
			DeleteCollision();
		}
		else
		{
			m_state = STATE_DAMAGE;
		}
	}
}

//=====================================================
// 死亡処理
//=====================================================
void CEnemy::Death(void)
{
	// 自身の終了
	Uninit();
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
	CScore *pScore = CScore::GetInstance();

	if (pScore != nullptr)
	{
		int nScore = GetScore();

		pScore->AddScore(nScore);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// マトリックスの設定
	SetMatrix();

	// 継承クラスの描画
	CMotion::Draw();

#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n敵の位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	//CDebugProc::GetInstance()->Print("\n敵の半径：[%f]", m_pCollisionSphere->GetRadius());
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