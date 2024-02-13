//*****************************************************
//
// ボス敵の処理[enemyboss.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemyboss.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "universal.h"
#include "debugproc.h"
#include "game.h"
#include "bullet.h"
#include "missile.h"
#include "effect3D.h"
#include "frame.h"
#include "shadow.h"
#include "score.h"
#include "sound.h"
#include "particle.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define INITIAL_LIFE	(60.0f)	// 初期体力
#define INITIAL_SCORE	(30000)	// 初期スコア
#define TIME_SHOT	(240)	// 射撃までのカウンター
#define ROLL_FACT	(0.1f)	// 回転係数
#define BULLET_SPEED	(2.0f)	// 弾の速度
#define BULLET_SIZE	(2.5f)	// 弾の大きさ
#define GRAVITY	(0.3f)	// 重力
#define TIME_MISSILE	(20)	// ミサイル発射頻度
#define MISSILE_UP	(3.0f)	// ミサイルの初速
#define NUM_MISSILE	(3)	// ミサイルの発射数
#define MOVE_FACT	(0.04f)	// 移動係数
#define LINE_END	(5.0f)	// 移動終了のしきい値
#define MID_POINT	(2740.0f)	// 真ん中の値
#define WIDTH_STAGE	(160.0f)	// ステージの幅
#define DAMAGE_FRAME	(10)	// ダメージ状態の時間
#define FLOAT_HEIGTH	(180.0f)	// 高さ
#define SHOT_TIME	(3)	// 射撃の頻度
#define SHOT_HEIGHT	(30.0f)	// 射撃時の高度

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CEnemyBoss *CEnemyBoss::m_pEnemyBoss = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CEnemyBoss::CEnemyBoss()
{
	// 自身の情報をクリア
	ZeroMemory(&m_info,sizeof(Sinfo));
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyBoss::~CEnemyBoss()
{

}

//=====================================================
// 生成処理
//=====================================================
CEnemyBoss *CEnemyBoss::Create(void)
{
	if (m_pEnemyBoss == nullptr)
	{
		m_pEnemyBoss = new CEnemyBoss;
	}

	return m_pEnemyBoss;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyBoss::Init(void)
{
	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Stop();

		pSound->Play(CSound::LABEL_BGM_BOSS);
	}

	// フレーム演出の生成
	CFrame::Create(100, 240, 70);

	// 継承クラスの初期化
	CEnemy::Init();

	// 初期の体力設定
	SetLife(INITIAL_LIFE);

	// スコア設定
	SetScore(INITIAL_SCORE);

	// モーション設定
	SetMotion(MOTION_APPER);
	m_info.state = STATE_APPER;

	// 出現地点合わせ
	SetMatrix();
	CMotion::Update();
	CMotion::Update();

	FollowCollision();

	// 影の設定
	CShadow *pShadow = GetShadow();

	if (pShadow != nullptr)
	{
		pShadow->SetSize(40.0f, 40.0f);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyBoss::Uninit(void)
{
	CParticle::Create(GetMtxPos(0), CParticle::TYPE_EXPLOSION);

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_SE_EXPLOSION);
	}

	m_pEnemyBoss = nullptr;

	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyBoss::Update(void)
{
	// 位置を保存
	SetPositionOld(GetPosition());

	// 継承クラスの更新
	CEnemy::Update();

	// 状態管理
	ManageState();

	// 条件ごとの更新
	UpdateState();
	
	// 当たり判定管理
	ManageCollision();

	// 攻撃の管理
	ManageAttack();
}

//=====================================================
// 状態管理
//=====================================================
void CEnemyBoss::ManageState(void)
{
	CEnemy::STATE state = CEnemy::GetState();

	int nTimer = GetCntState();

	switch (state)
	{
	case CEnemy::STATE_NORMAL:
		break;
	case CEnemy::STATE_DAMAGE:

		nTimer++;

		SetAllCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		if (nTimer > DAMAGE_FRAME)
		{// 通常状態に戻る
			nTimer = 0;
			state = CEnemy::STATE_NORMAL;

			ResetAllCol();
		}
		break;
	case CEnemy::STATE_DEATH:
		if (m_info.nCntState >= 180)
		{
			Death();
		}
		else
		{
			m_info.nCntState++;
		}

		break;
	default:
		break;
	}

	CEnemy::SetState(state);

	SetCntState(nTimer);
}

//=====================================================
// 状態ごとの更新分岐
//=====================================================
void CEnemyBoss::UpdateState(void)
{
	switch (m_info.state)
	{
	case STATE_APPER:
		UpdateApper();
		break;
	case STATE_BATTLE:
		UpdateAttackState();
		break;
	default:
		break;
	}
}

//=====================================================
// 出現状態の更新
//=====================================================
void CEnemyBoss::UpdateApper(void)
{
	bool bFinish = IsFinish();

	if (bFinish)
	{// 状態切り替え
		SwitchState();

		m_info.state = STATE_BATTLE;
		SwitchState();
	}
}

//=====================================================
// 戦闘状態の更新
//=====================================================
void CEnemyBoss::UpdateAttackState(void)
{
	switch (m_info.attackState)
	{
	case ATTACK_MISSILE:
		UpdateMissile();
		break;
	case ATTACK_DASH:
		UpdateDash();
		break;
	case ATTACK_SHOT_UNDER:
		UpdateShotUnder();
		break;
	default:
		break;
	}
}

//=====================================================
// ミサイル攻撃の更新
//=====================================================
void CEnemyBoss::UpdateMissile(void)
{
	D3DXVECTOR3 pos = GetPosition();

	// プレイヤーの方を向く処理
	RotDest();

	if (pos.x < MID_POINT)
	{// 左にいた場合
		m_info.posDest = { MID_POINT - WIDTH_STAGE, FLOAT_HEIGTH, 0.0f };
	}
	else
	{
		m_info.posDest = { MID_POINT + WIDTH_STAGE, FLOAT_HEIGTH, 0.0f };
	}


	// 目標位置追従
	bool bEnd = FollowDest();

	if (bEnd)
	{
		// ミサイル攻撃
		m_info.nCntAttack++;

		if (TIME_MISSILE <= m_info.nCntAttack)
		{// ミサイル発射
			D3DXVECTOR3 posMissile = GetMtxPos(IDX_SHOULDER_L);

			CMissile *pMissile = CMissile::Create(posMissile);

			if (pMissile != nullptr)
			{
				pMissile->SetMove(D3DXVECTOR3(0.0f, MISSILE_UP, 0.0f));
			}

			CSound *pSound = CSound::GetInstance();

			if (pSound != nullptr)
			{
				pSound->Play(CSound::LABEL_SE_MISSILE);
			}

			m_info.nCntAttack = 0;

			// 攻撃回数加算
			m_info.nNumAttack++;

			if (m_info.nNumAttack >= NUM_MISSILE)
			{
				SwitchState();
			}
		}
	}
}

//=====================================================
// 突進攻撃の更新
//=====================================================
void CEnemyBoss::UpdateDash(void)
{
	bool bFinish = IsFinish();

	if (bFinish)
	{// 状態切り替え
		// 目標位置追従
		bool bEnd = FollowDest();

		if (bEnd)
		{
			SwitchState();
		}
	}
	else
	{
		RotDest();
	}
}

//=====================================================
// 下から射撃攻撃の更新
//=====================================================
void CEnemyBoss::UpdateShotUnder(void)
{
	// 汎用処理取得
	CUniversal *pUniversal = CUniversal::GetInstance();

	RotDest();

	FollowDest();

	bool bFinish = IsFinish();
	int nKey = GetKey();

	if (nKey == 3)
	{
		m_info.nCntAttack++;

		if (m_info.nCntAttack >= SHOT_TIME)
		{
			D3DXMATRIX mtxWeapon = *GetParts(IDX_WEAPON)->m_pParts->GetMatrix();
			D3DXMATRIX mtxMazzle;
			D3DXMATRIX mtxMazzleVec;

			// オフセットの位置設定
			pUniversal->SetOffSet(&mtxMazzle, mtxWeapon, D3DXVECTOR3(5.0f, -30.0f, 0.0f));
			pUniversal->SetOffSet(&mtxMazzleVec, mtxMazzle,D3DXVECTOR3(0.0f,-1.0f,0.0f));

			// 差分のベクトルを取得
			D3DXVECTOR3 posMazzle =
			{
				mtxMazzle._41,
				mtxMazzle._42,
				mtxMazzle._43,
			};
			D3DXVECTOR3 posBullet =
			{
				mtxMazzleVec._41,
				mtxMazzleVec._42,
				mtxMazzleVec._43,
			};

			D3DXVECTOR3 vecBullet = posBullet - posMazzle;

			D3DXVec3Normalize(&vecBullet, &vecBullet);

			vecBullet *= BULLET_SPEED;

			CBullet::Create(posMazzle, vecBullet, 400, CBullet::TYPE_ENEMY, false, 10.0f, 5.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

			m_info.nCntAttack = 0;

			CSound *pSound = CSound::GetInstance();

			if (pSound != nullptr)
			{
				pSound->Play(CSound::LABEL_SE_BULLET);
			}
		}
	}

	if (bFinish)
	{// 状態切り替え
		// 目標位置追従
		SwitchState();
	}
}

//=====================================================
// 当たり判定管理
//=====================================================
void CEnemyBoss::ManageCollision(void)
{
	// 当たり判定追従
	FollowCollision();
}

//=====================================================
// 当たり判定の追従
//=====================================================
void CEnemyBoss::FollowCollision(void)
{
	CCollisionSphere *pCollision = GetClsnSphere();

	if (pCollision != nullptr)
	{
		D3DXVECTOR3 pos = GetMtxPos(IDX_WAIST);

		pos.y -= 25.0f;

#ifdef _DEBUG
		//dCEffect3D::Create(pos, pCollision->GetRadius(), 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif

		pCollision->SetPositionOld(pCollision->GetPosition());
		pCollision->SetPosition(pos);
		pCollision->SetRadius(35.0f);
	}
}

//=====================================================
// 目標位置への移動
//=====================================================
bool CEnemyBoss::FollowDest(void)
{
	bool bEnd = false;

	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 vecDiff = m_info.posDest - pos;

	// 差分距離の取得
	float fLength = D3DXVec3Length(&vecDiff);

	if (LINE_END > fLength)
	{
		bEnd = true;
	}

	vecDiff *= MOVE_FACT;

	pos += vecDiff;

	SetPosition(pos);

	return bEnd;
}

//=====================================================
// 状態の切り替え
//=====================================================
void CEnemyBoss::SwitchState(void)
{
	m_info.nNumAttack = 0;

	ATTACKSTATE state;

	// 状態を振る
	state = (ATTACKSTATE)(rand() % (ATTACK_MAX));

	while (m_info.attackState == state)
	{
		state = (ATTACKSTATE)(rand() % (ATTACK_MAX));
	}

	m_info.attackState = state;

	switch (m_info.attackState)
	{
	case ATTACK_MISSILE:

		SetMotion(MOTION_MISSILE);

		break;
	case ATTACK_DASH:
	{
		SetMotion(MOTION_DASH);

		D3DXVECTOR3 pos = GetPosition();

		if (pos.x < MID_POINT)
		{// 左にいた場合
			m_info.posDest = { MID_POINT + WIDTH_STAGE, FLOAT_HEIGTH, 0.0f };
		}
		else
		{
			m_info.posDest = { MID_POINT - WIDTH_STAGE, FLOAT_HEIGTH, 0.0f };
		}
	}
		break;
	case ATTACK_SHOT_UNDER:
	{
		int nRand = rand() % 2;

		if (nRand == 1)
		{
			SetMotion(MOTION_SHOT_UPPER);
		}
		else
		{
			SetMotion(MOTION_SHOT_UNDER);
		}

		D3DXVECTOR3 pos = GetPosition();

		if (pos.x < MID_POINT)
		{// 左にいた場合
			m_info.posDest = { MID_POINT - WIDTH_STAGE, FLOAT_HEIGTH + SHOT_HEIGHT, 0.0f };
		}
		else
		{
			m_info.posDest = { MID_POINT + WIDTH_STAGE, FLOAT_HEIGTH + SHOT_HEIGHT, 0.0f };
		}
	}
		break;
	default:
		break;
	}
}

//=====================================================
// 攻撃管理
//=====================================================
void CEnemyBoss::ManageAttack(void)
{

}

//=====================================================
// ヒット処理
//=====================================================
void CEnemyBoss::Hit(float fDamage)
{
	CEnemy::STATE state = CEnemy::GetState();

	if (state == CEnemy::STATE_NORMAL)
	{
		float fLife = CEnemy::GetLife();

		fLife -= fDamage;

		CSound *pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_HIT_BOSS);
		}

		if (fLife <= 0.0f)
		{// 死亡状態
			SetMotion(MOTION_DEATH);

			CParticle::Create(GetMtxPos(0), CParticle::TYPE_FIRE);

			fLife = 0.0f;

			CGame::SetState(CGame::STATE_END);

			m_info.state = STATE_NONE;
			state = CEnemy::STATE_DEATH;

			// スコア管理
			ManageScore();

			// スコア設定
			CScore *pScore = CScore::GetInstance();

			if (pScore != nullptr)
			{
				pScore->SetResultScore();
			}

			// 当たり判定削除
			DeleteCollision();
		}
		else
		{
			state = CEnemy::STATE_DAMAGE;
		}

		CEnemy::SetLife(fLife);
	}

	CEnemy::SetState(state);
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyBoss::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\nボス位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	CDebugProc::GetInstance()->Print("\nボス体力：[%f]", GetLife());
	CDebugProc::GetInstance()->Print("\nボス状態：[%d]",CEnemy::GetState());
	CDebugProc::GetInstance()->Print("\nボスモーション：[%d]", GetMotion());
	CDebugProc::GetInstance()->Print("\nキー：[%d]", GetKey());
	CDebugProc::GetInstance()->Print("\nフレーム：[%d]", GetFrame());
	CDebugProc::GetInstance()->Print("\nIsFinish：[%d]", IsFinish());
#endif
}