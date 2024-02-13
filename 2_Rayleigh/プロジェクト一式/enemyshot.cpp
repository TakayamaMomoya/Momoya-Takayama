//*****************************************************
//
// 動く敵の処理[enemyshot.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemyshot.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "universal.h"
#include "debugproc.h"
#include "game.h"
#include "bullet.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define INITIAL_LIFE	(5)	// 初期体力
#define INITIAL_SCORE	(500)	// 初期スコア
#define TIME_SHOT	(240)	// 射撃までのカウンター
#define ROLL_FACT	(0.1f)	// 回転係数
#define BULLET_SPEED	(2.0f)	// 弾の速度
#define BULLET_SIZE	(2.5f)	// 弾の大きさ
#define GRAVITY	(0.3f)	// 重力

//=====================================================
// コンストラクタ
//=====================================================
CEnemyShot::CEnemyShot()
{

}

//=====================================================
// デストラクタ
//=====================================================
CEnemyShot::~CEnemyShot()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyShot::Init(void)
{
	// 継承クラスの初期化
	CEnemyNormal::Init();

	// 初期の体力設定
	SetLife(INITIAL_LIFE);

	// スコア設定
	SetScore(INITIAL_SCORE);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyShot::Uninit(void)
{
	// 継承クラスの終了
	CEnemyNormal::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyShot::Update(void)
{
	// 位置を保存
	SetPositionOld(GetPosition());

	// 継承クラスの更新
	CEnemyNormal::Update();
	
	// 攻撃の管理
	ManageAttack();

	// 目標を向く処理
	RotDest();

	// 重力
	D3DXVECTOR3 move = GetMove();

	move.y -= GRAVITY;

	SetMove(move);
}

//=====================================================
// 攻撃管理
//=====================================================
void CEnemyShot::ManageAttack(void)
{
	int nMotion = GetMotion();
	int nFrame = GetFrame();
	int nKey = GetKey();

	if (GetAttackCounter() >= TIME_SHOT && nMotion != MOTION_SHOT)
	{// 射撃モーション
		SetMotion(MOTION_SHOT);
	}

	if (nMotion == MOTION_SHOT && 
		nFrame == 0 &&
		nKey == 2)
	{// 指定のタイミングで弾を撃つ
		D3DXVECTOR3 pos = GetPosition();
		float fRot = GetRot().y;
		D3DXVECTOR3 move;

		pos.y += 10.0f;

		move =
		{
			sinf(fRot) * BULLET_SPEED,
			0.0f,
			cosf(fRot) * BULLET_SPEED
		};

		// 目標位置取得
		CPlayer *pPlayer = CPlayer::GetInstance();

		if (pPlayer != nullptr)
		{
			D3DXVECTOR3 posTarget = pPlayer->GetPosition();

			D3DXVECTOR3 vecDiff = posTarget - pos;

			float fLength = D3DXVec3Length(&vecDiff);

			if (fLength > 250.0f)
			{
				return;
			}
		}

		CBullet::Create(pos, -move, 500, CBullet::TYPE_ENEMY,false,BULLET_SIZE, 2.0f,D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));

		SetAttackCounter(0);
	}

	if (nMotion == MOTION_SHOT &&
		IsFinish())
	{// 射撃モーションの終了
		SetMotion(MOTION_NEUTRAL);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyShot::Draw(void)
{
	// 継承クラスの描画
	CEnemyNormal::Draw();
}