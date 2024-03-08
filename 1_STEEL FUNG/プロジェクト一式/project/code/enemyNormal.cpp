//*****************************************************
//
// 通常敵の処理[enemyNormal.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "enemyNormal.h"
#include "player.h"
#include "effect3D.h"
#include "debugproc.h"
#include "bullet.h"
#include "manager.h"
#include "slow.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float DIST_FIRE = 500.0f;	// 発射するまでの視線とプレイヤーの差分距離
const float SPEED_BULLET = 150.0f;	// 弾の速度
const float TIME_FIRE = 0.5f;	// 発射間隔
const float DIST_KEEP = 3000.0f;	// 戦闘時に保つ距離
const float SPEED_MOVE = 0.4f;	// 移動速度
const int ACCURACY = 500;	// 精度
}

//=====================================================
// コンストラクタ
//=====================================================
CEnemyNormal::CEnemyNormal()
{
	m_fTimeFire = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyNormal::Init(void)
{
	// 継承クラスの初期化
	CEnemy::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyNormal::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyNormal::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();

	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_DEATH)
	{// 死亡モーション
		int nMotion = GetMotion();

		if (nMotion != MOTION_DEATH)
		{
			SetMotion(MOTION_DEATH);
		}
	}
}

//=====================================================
// 待機
//=====================================================
void CEnemyNormal::Wait(void)
{
	CEnemy::Wait();
}

//=====================================================
// 目標の追跡
//=====================================================
void CEnemyNormal::Chase(void)
{
	CEnemy::Chase();
}

//=====================================================
// 攻撃
//=====================================================
void CEnemyNormal::Attack(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();
	D3DXVECTOR3 rot = GetRotation();

	// 距離を保つ
	KeepDistance();

	if (pPlayer != nullptr)
	{
		// プレイヤーの方を向く
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// 差分角度の取得
		D3DXVECTOR3 vecDiff = posPlayer - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);

		universal::FactingRot(&rot.x, -rotDest.x + D3DX_PI * 0.5f, 0.1f);
		universal::FactingRot(&rot.y, rotDest.y, 0.1f);

		SetRotation(rot);

		// プレイヤーに向かって弾を撃つ
		float fDist = D3DXVec3Length(&vecDiff);

		D3DXMATRIX mtxEye;
		D3DXMATRIX mtx = *GetMatrix();

		universal::SetOffSet(&mtxEye, mtx, D3DXVECTOR3(0.0f, 0.0f, fDist));

		D3DXVECTOR3 vecEye = { mtxEye._41,mtxEye._42 ,mtxEye._43 };

		D3DXVECTOR3 vecDiffEye = posPlayer - vecEye;

		float fDistEyeToPlayer = D3DXVec3Length(&vecDiffEye);

		// 狙いが近かったら発射
		if (fDistEyeToPlayer < DIST_FIRE)
		{
			float fDeltaTime = CManager::GetDeltaTime();

			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{// タイムスケールをかける
				float fScale = pSlow->GetScale();

				fDeltaTime *= fScale;
			}

			// タイマー加算
			m_fTimeFire += fDeltaTime;

			if (m_fTimeFire > TIME_FIRE)
			{// 弾の発射
				Sound::Play(CSound::LABEL_SE_SHOT01);

				D3DXVECTOR3 posMazzle = GetMtxPos(0);
				D3DXVECTOR3 moveBullet;
				D3DXVECTOR3 movePlayer = pPlayer->GetMove();

				D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);

				posPrediction.x += (float)universal::RandRange(ACCURACY, -ACCURACY);
				posPrediction.y += (float)universal::RandRange(ACCURACY, -ACCURACY);
				posPrediction.z += (float)universal::RandRange(ACCURACY, -ACCURACY);

				D3DXVECTOR3 vecDiffBullet = pos - posPrediction;
				D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

				moveBullet = vecDiffBullet * SPEED_BULLET;

				CBullet::Create(posMazzle, -moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f, 0.25f);

				m_fTimeFire = 0;
			}
		}
	}

	CEnemy::Attack();
}

//=====================================================
// 距離を保つ処理
//=====================================================
void CEnemyNormal::KeepDistance(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 move = GetMove();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// 差分角度の取得
		D3DXVECTOR3 vecDiff = pos - posPlayer;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= DIST_KEEP;

		D3DXVECTOR3 posDest = posPlayer + vecDiff;

		posDest.y = pos.y;

		// 移動量を補正する
		vecDiff = (posDest - pos);

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= SPEED_MOVE;

		move += vecDiff;

		SetMove(move);

#ifdef _DEBUG
		CEffect3D::Create(posDest, 20.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.01f));
#endif

	}
}

//=====================================================
// 地面にぶつかったときの処理
//=====================================================
void CEnemyNormal::HitField(void)
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
// 描画処理
//=====================================================
void CEnemyNormal::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}