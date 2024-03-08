//*****************************************************
//
// 通常敵の処理[EnemyBomb.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "EnemyBomb.h"
#include "player.h"
#include "effect3D.h"
#include "debugproc.h"
#include "bullet.h"
#include "manager.h"
#include "slow.h"
#include "animEffect3D.h"
#include "anim3D.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float RADIUS_EXPLOSION = 1300.0f;	// 爆発範囲
}

//=====================================================
// コンストラクタ
//=====================================================
CEnemyBomb::CEnemyBomb()
{

}

//=====================================================
// デストラクタ
//=====================================================
CEnemyBomb::~CEnemyBomb()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyBomb::Init(void)
{
	// 継承クラスの初期化
	CEnemy::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyBomb::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 地面にぶつかったときの処理
//=====================================================
void CEnemyBomb::HitField(void)
{
	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_THROWN)
	{
		// 死亡
		Hit(20.0f);

		// 移動量を減衰
		D3DXVECTOR3 move = GetMove();

		move *= 0.1f;

		SetMove(move);
	}

	CEnemy::HitField();
}

//=====================================================
// 死亡時の処理
//=====================================================
void CEnemyBomb::Death(void)
{
	CCollisionSphere *pCollision = GetClsnSphere();

	if (pCollision != nullptr)
	{// 周囲の敵にダメージを与える
		Sound::Play(CSound::LABEL_SE_EXPLOSION01);

		pCollision->SetRadius(RADIUS_EXPLOSION);
		pCollision->DamageAll(CCollision::TAG_ENEMY, 15.0f);

		// アニメーションエフェクト生成
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if (pAnim3D != nullptr)
		{
			D3DXVECTOR3 pos = GetMtxPos(0);

			CAnim3D *pAnim = pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);

			if (pAnim != nullptr)
			{
				pAnim->SetSize(RADIUS_EXPLOSION, RADIUS_EXPLOSION);
			}
		}
	}

	CEnemy::Death();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyBomb::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyBomb::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}