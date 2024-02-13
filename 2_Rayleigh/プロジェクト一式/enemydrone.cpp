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
#include "enemydrone.h"
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
#define TIME_SHOT	(120)	// 射撃までのカウンター
#define BULLET_SPEED	(2.0f)	// 弾の速度
#define BULLET_SIZE	(1.0f)	// 弾の大きさ
#define GRAVITY	(0.3f)	// 重力

//=====================================================
// コンストラクタ
//=====================================================
CEnemyDrone::CEnemyDrone()
{

}

//=====================================================
// デストラクタ
//=====================================================
CEnemyDrone::~CEnemyDrone()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyDrone::Init(void)
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
void CEnemyDrone::Uninit(void)
{
	// 継承クラスの終了
	CEnemyNormal::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyDrone::Update(void)
{
	// 位置を保存
	SetPositionOld(GetPosition());

	// 継承クラスの更新
	CEnemyNormal::Update();
	
	// 攻撃の管理
	ManageAttack();

	// 目標を向く処理
	RotDest();
}

//=====================================================
// 攻撃管理
//=====================================================
void CEnemyDrone::ManageAttack(void)
{
	if (GetAttackCounter() >= TIME_SHOT)
	{// 攻撃
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posTarget = { 0.0f,0.0f,0.0f };
		float fRot = GetRot().y;
		D3DXVECTOR3 move;

		// 目標位置取得
		CPlayer *pPlayer = CPlayer::GetInstance();

		if (pPlayer != nullptr)
		{
			CMotion *pMotion = pPlayer->GetBody();

			posTarget = pMotion->GetMtxPos(0);

			move = posTarget - pos;

			float fLength = D3DXVec3Length(&move);

			if (fLength > 200.0f)
			{
				return;
			}

			D3DXVec3Normalize(&move, &move);

			move *= BULLET_SPEED;
		}

		CBullet::Create(pos, move, 500, CBullet::TYPE_ENEMY, false, BULLET_SIZE, 2.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		SetAttackCounter(0);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyDrone::Draw(void)
{
	// 継承クラスの描画
	CEnemyNormal::Draw();
}