//*****************************************************
//
// ボス敵状態[stateEnemyBoss.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "stateEnemyBoss.h"
#include "enemyBoss.h"
#include "enemyDrone.h"
#include "manager.h"
#include "missile.h"
#include "bullet.h"
#include "player.h"
#include "animEffect3D.h"
#include "fade.h"
#include "explosionspawner.h"
#include "particle.h"
#include "anim3D.h"
#include "camera.h"
#include "meshfield.h"
#include "blockManager.h"
#include "orbit.h"
#include "beamBlade.h"
#include "effect3D.h"
#include "beam.h"
#include "game.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_MISSILE = 0.5f;	// ミサイル発射の時間
const int NUM_MISSILE = 10;	// ミサイルの発射数
const float TIME_DRONE = 1.0f;	// ドローン発射の時間
const int NUM_DRONE = 3;	// ドローンの発射数
const int RANGE_HEIGHT_DRONE = 500;	// ドローンの高さの幅
const float MOVE_DRONE = 50.0f;	// ドローンの射出時の移動量
const float TIME_MG = 0.15f;	// マシンガン発射の時間
const int NUM_MG = 30;	// マシンガンの発射数
const int NUM_BEAMSMALL = 4;	// 小ビームの発射数
const float SPEED_BULLET = 200.0f;	// マシンガン弾の速度
const int ACCURACY_MG = 10;	// マシンガンの精度
const float GRAVITY = 0.4f;	// 重力
const float SPEED_SLASH = 0.05f;	// 斬撃時の突撃スピード
const float SPEED_STEP = 0.02f;	// ステップのスピード
const float RANGE_SLASH = 1000.0f;	// 斬撃に移るまでの距離
const D3DXVECTOR3 POS_CENTER = { 5170.0f,-6245.0f,3791.0f };	// ステージの中心座標
const float RADIUS_BLADE = 70.0f;	// ブレードの半径
const float SPEED_EXPAND_BLADE = 0.01f;	// ブレードの膨らむ速度
const float DIST_CLOSE = 3000.0f;	// 近距離判定の距離
const float DIST_FAR = 7000.0f;	// 遠距離判定の距離
}

CStateBoss::CStateBoss()
{

}

CStateBoss::~CStateBoss()
{

}

//=====================================================
// 出現状態
//=====================================================
void CStateBossApper::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_APPER);
}

void CStateBossApper::Move(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// モーション終了で次の状態へ移る
		pBoss->ChangeState(new CStateBossAttackMachinegun);
	}
}

//=====================================================
// ビーム攻撃
//=====================================================
void CStateBossAttackBeam::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_SHOT);
}

void CStateBossAttackBeam::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->AimPlayer();

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// モーション終了で次の状態へ移る
		pBoss->ChangeState(new CStateBossLaunchDrone);
	}
}

//=====================================================
// ミサイル攻撃
//=====================================================
void CStateBossAttackMissile::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nCnt = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossAttackMissile::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	if (pBoss->AttackTimer(TIME_MISSILE))
	{// ミサイルの発射
		CMissile *pMissile = CMissile::Create(pBoss->GetMtxPos(1));

		if (pMissile != nullptr)
		{
			D3DXVECTOR3 rot = pBoss->GetRotation();


			if (m_nCnt % 2 == 0)
			{
				rot.x = -D3DX_PI * 0.3f;
				rot.y = D3DX_PI * 0.5f;
			}
			else
			{
				rot.x = -D3DX_PI * 0.3f;
				rot.y = -D3DX_PI * 0.3f;
			}

			pMissile->SetRotation(rot);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MISSILE)
		{// 一定数撃ったら次の行動へ
			pBoss->ChangeState(new CStateBossAttackMachinegun);
		}
	}

	// 後退処理
	pBoss->AimPlayer();
}

//=====================================================
// 子機の射出
//=====================================================
void CStateBossLaunchDrone::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nCnt = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossLaunchDrone::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	if (pBoss->AttackTimer(TIME_DRONE))
	{// 射出する
		CEnemyDrone *pDrone = new CEnemyDrone;

		if (pDrone != nullptr)
		{
			pDrone->Init();

			D3DXVECTOR3 pos = pBoss->GetPosition();

			pDrone->SetPosition(pos);

			pos.y += universal::RandRange(0, -RANGE_HEIGHT_DRONE);

			// 射出してからの位置設定
			pDrone->SetPositionDest(pos);

			// 移動量を足す
			D3DXVECTOR3 move = { 0.0f,MOVE_DRONE,0.0f };
			pDrone->SetMove(move);
		}

		m_nCnt++;

		if (m_nCnt > NUM_DRONE)
		{// 一定数撃ったら次の行動へ
			pBoss->ChangeState(new CStateBossAttackMissile);
		}
	}

	// 後退処理
	pBoss->Back();
	pBoss->AimPlayer();
}

//=====================================================
// マシンガン
//=====================================================
void CStateBossAttackMachinegun::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nCnt = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossAttackMachinegun::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	if (pBoss->AttackTimer(TIME_MG))
	{// 射出する
		D3DXVECTOR3 posMazzle = pBoss->GetMtxPos(5);
		D3DXVECTOR3 moveBullet;
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// 偏差の計算
		D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);

		posPrediction.x += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.y += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.z += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);

		D3DXVECTOR3 vecDiffBullet = posMazzle - posPrediction;
		D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

		moveBullet = vecDiffBullet * SPEED_BULLET;

		// 弾の発射
		CBullet::Create(posMazzle, -moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f, 0.01f);

		// アニメエフェクト生成
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if (pAnim3D != nullptr)
		{
			pAnim3D->CreateEffect(posMazzle, CAnimEffect3D::TYPE::TYPE_MUZZLEFLUSH);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MG)
		{// 一定数撃ったら次の行動へ
			pBoss->ChangeState(new CStateBossAttackBeam);
		}
	}

	// 後退処理
	pBoss->Back();
	pBoss->AimPlayer();
}

//=====================================================
// 第二形態へ移行する
//=====================================================
void CStateBossTrans::Init(CEnemyBoss *pBoss)
{
	m_fTimeTrans = 0.0f;

	CheckPointer(pBoss);

}

void CStateBossTrans::Move(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_fTimeTrans += CManager::GetDeltaTime();

	CFade *pFade = CFade::GetInstance();

	int nMotion = pBoss->GetMotion();
	bool bFinish = pBoss->IsFinish();

	if (pFade != nullptr)
	{
		CFade::FADE state = pFade->GetState();

		if (state == CFade::FADE::FADE_NONE)
		{
			if (m_fTimeTrans > 6.0f)
			{// フェードで第二形態に移る
				Evolve(pBoss);
			}
		}

		if (state == CFade::FADE::FADE_OUT)
		{
			if (nMotion != CEnemyBoss::MOTION::MOTION_DEATH)
			{// ムービーにいく
				TransMovie(pBoss);
			}

			if (m_fTimeTrans > 6.0f)
			{// 地形の変動
				// ブロック削除
				CBlockManager *pBlockManager = CBlockManager::GetInstance();

				if (pBlockManager != nullptr)
					pBlockManager->DeleteAll();

				// メッシュフィールド変形
				CMeshField *pMesh = CMeshField::GetInstance();

				if (pMesh != nullptr)
					pMesh->Load("data\\MAP\\field01.bin");

				pBoss->ChangeState(new CStateBossBeforeTrans);
			}
		}
	}
}

void CStateBossTrans::TransMovie(CEnemyBoss *pBoss)
{// ムービーに移行
	D3DXVECTOR3 pos = { 7000.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,D3DX_PI * 0.5f,0.0f };

	// ボス位置を設定
	pBoss->SetPosition(pos);
	pBoss->SetRotation(rot);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// プレイヤー位置を設定
		D3DXVECTOR3 posPlayer = { 5000.0f,0.0f,0.0f };
		D3DXVECTOR3 rotPlayer = { 0.0f,0.0f,0.0f };

		pPlayer->SetPosition(posPlayer);
		pPlayer->SetRotation(rotPlayer);
	}

	// 爆発エフェクト
	pos.y += 400.0f;
	CExplSpawner::Create(pos, 300.0f, 360);
	CParticle::Create(pos, CParticle::TYPE::TYPE_TURN_EXPLOSION);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_DEATH);
}

void CStateBossTrans::Evolve(CEnemyBoss *pBoss)
{// 第二形態に進化
	// 大爆発を起こす
	CAnimEffect3D *pAnim = CAnimEffect3D::GetInstance();

	if (pAnim != nullptr)
	{
		D3DXVECTOR3 pos = pBoss->GetMtxPos(0);

		CAnim3D *pExplosion;

		pExplosion = pAnim->CreateEffect(pos, CAnimEffect3D::TYPE_EXPLOSION);

		if (pExplosion != nullptr)
		{
			pExplosion->SetSize(2000.0f, 2000.0f);
		}
	}

	// カメラを揺らす
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetQuake(0.2f, 0.2f, 120);
	}

	// 白いフェードをかける
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		pFade->SetFade(CScene::MODE_GAME, false);
	}
}

//=====================================================
// 第二形態に移ったときの処理
//=====================================================
void CStateBossBeforeTrans::Init(CEnemyBoss *pBoss)
{
	pBoss->Load("data\\MOTION\\robot00.txt");

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_PRE_SLASH);

	pBoss->EnableTrans(true);
}

void CStateBossBeforeTrans::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 move = pBoss->GetMove();

	move.y -= GRAVITY;

	pBoss->SetMove(move);

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// 行動に遷移
		pBoss->ChangeState(new CStateBossSlash);

		// 当たり判定再生成
		pBoss->CreateCollision(Boss::RADIUS_COLLISION);

		// ライフリセット
		pBoss->SetLife(Boss::INITIAL_LIFE);
	}
}

//=====================================================
// 行動選択
//=====================================================
void CStateBossSelect::Init(CEnemyBoss *pBoss)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// プレイヤーとの距離によって行動が変化
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 pos = pBoss->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	float fDest = D3DXVec3Length(&vecDiff);

	int nRand = universal::RandRange(100, 0);

	if (fDest < DIST_CLOSE)
	{// 近距離
		Close(nRand,pBoss);
#ifdef _DEBUG
		CEffect3D::Create(pBoss->GetMtxPos(0), 600.0f,60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
	}
	else if (fDest > DIST_FAR)
	{// 遠距離
		Far(nRand, pBoss);
#ifdef _DEBUG
		CEffect3D::Create(pBoss->GetMtxPos(0), 1000.0f, 60, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
#endif
	}
	else
	{// 中距離
		Middle(nRand, pBoss);
#ifdef _DEBUG
		CEffect3D::Create(pBoss->GetMtxPos(0), 600.0f, 60, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
#endif
	}
}

void CStateBossSelect::Close(int nRand, CEnemyBoss *pBoss)
{// 近距離
	if (nRand < 30)
	{
		pBoss->ChangeState(new CStateBossSlash);
	}
	else
	{// ステップ回避
		pBoss->ChangeState(new CStateBossStep);
	}
}

void CStateBossSelect::Middle(int nRand, CEnemyBoss *pBoss)
{// 中距離
	if (nRand < 40)
	{
		pBoss->ChangeState(new CStateBossBeamSmall);
	}
	else if (nRand < 80)
	{
		pBoss->ChangeState(new CStateBossStep);
	}
	else
	{
		pBoss->ChangeState(new CStateBossSlash);
	}
}

void CStateBossSelect::Far(int nRand, CEnemyBoss *pBoss)
{// 遠距離
	if (nRand < 60)
	{
		pBoss->ChangeState(new CStateBossBeamSmall);
	}
	else
	{
		pBoss->ChangeState(new CStateBossSlash);
	}
}

//=====================================================
// 斬撃モーション
//=====================================================
CStateBossSlash::~CStateBossSlash()
{
	if (m_pBlade != nullptr)
	{
		m_pBlade->Uninit();
		m_pBlade = nullptr;
	}

	if (m_pOrbit != nullptr)
	{
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}
}

void CStateBossSlash::Init(CEnemyBoss *pBoss)
{
	m_pOrbit = nullptr;
	m_pBlade = nullptr;

	m_pBlade = CBeamBlade::Create();

	if (m_pBlade != nullptr)
	{
		m_pBlade->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_pBlade->SetRadius(0.0f);
	}

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_PRE_SLASH);
}

void CStateBossSlash::Move(CEnemyBoss *pBoss)
{
	int nMotion = pBoss->GetMotion();
	bool bFinish = pBoss->IsFinish();

	if (m_pBlade != nullptr)
	{// 斬撃の更新
		// 位置・向きの追従
		D3DXMATRIX mtxArm = *pBoss->GetParts(CEnemyBoss::IDX_HAND_L)->pParts->GetMatrix();
		D3DXMATRIX mtx;

		D3DXVECTOR3 vecBlade = universal::VecToOffset(mtxArm, D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		D3DXVECTOR3 rotBlade = universal::VecToRot(vecBlade);
		D3DXVECTOR3 posBlade =
		{
			mtxArm._41,
			mtxArm._42,
			mtxArm._43
		};

		rotBlade.x *= -1;

		m_pBlade->SetPosition(posBlade);
		m_pBlade->SetRotation(rotBlade);

		CParticle::Create(posBlade, CParticle::TYPE_BLADE_PARTICLE, rotBlade);

		// 大きさの更新
		float fRadius = m_pBlade->GetRadius();

		fRadius += (RADIUS_BLADE - fRadius) * SPEED_EXPAND_BLADE;

		m_pBlade->SetRadius(fRadius);
	}

	if (nMotion == CEnemyBoss::MOTION::MOTION_PRE_SLASH)
	{
		pBoss->AimPlayer(0.0f, false);

		if (bFinish)
		{
			// 目的地を設定
			CPlayer *pPlayer = CPlayer::GetInstance();

			if (pPlayer == nullptr)
				return;

			// プレイヤーを追尾
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXVECTOR3 pos = pBoss->GetPosition();

			universal::MoveToDest(&pos, posPlayer, SPEED_SLASH);

			pBoss->SetPosition(pos);

			if (universal::DistCmp(pos, posPlayer, RANGE_SLASH, nullptr))
			{
				pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_SLASH);

				if (m_pOrbit == nullptr)
				{// 軌跡の生成
					D3DXMATRIX mtx = *pBoss->GetParts(CEnemyBoss::IDX_HAND_L)->pParts->GetMatrix();

					D3DXVECTOR3 offset = { 0.0f,-1000.0f,0.0f };

					m_pOrbit = COrbit::Create(mtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), offset, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20);
				}
			}
		}

		// ビームパーティクルの発生
		pBoss->BeamBlade();
	}
	else if (nMotion == CEnemyBoss::MOTION::MOTION_SLASH)
	{
		if (m_pOrbit != nullptr)
		{// 軌跡の更新
			D3DXMATRIX mtx = *pBoss->GetParts(CEnemyBoss::IDX_HAND_L)->pParts->GetMatrix();

			m_pOrbit->SetPositionOffset(mtx, 0);
		}

		if (bFinish)
		{// 斬撃の終了
			if (m_pOrbit != nullptr)
			{
				m_pOrbit->SetEnd(true);
				m_pOrbit = nullptr;
			}

			if (m_pBlade != nullptr)
			{
				m_pBlade->Uninit();
				m_pBlade = nullptr;
			}

			pBoss->ChangeState(new CStateBossSelect);
		}
	}
}

//=====================================================
// 小ビーム攻撃
//=====================================================
void CStateBossBeamSmall::Init(CEnemyBoss *pBoss)
{
	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_BEAMSMALL);

	m_nCnt = 0;
}

void CStateBossBeamSmall::Attack(CEnemyBoss *pBoss)
{
	pBoss->AimPlayer(0.0f, false);

	bool bShot = pBoss->AttackTimer(0.5f);

	if (bShot)
	{
		// ビームの生成
		CBeam *pBeam = CBeam::Create();

		D3DXMATRIX mtxParent = *pBoss->GetParts(CEnemyBoss::IDX_HAND_R)->pParts->GetMatrix();
		D3DXMATRIX mtxBeam;

		universal::SetOffSet(&mtxBeam, mtxParent, D3DXVECTOR3(0.0f, -1.0f, 1.0f));

		if (pBeam != nullptr)
		{
			D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };
			D3DXVECTOR3 posMazzle = { mtxBeam._41,mtxBeam._42 ,mtxBeam._43 };
			D3DXVECTOR3 vecMazzle = posMazzle - pos;

			D3DXVECTOR3 rot = pBoss->GetRotation();

			pBeam->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			pBeam->SetMtx(mtxBeam);
			pBeam->SetRotation(rot);
			pBeam->SetPosition(posMazzle);
			pBeam->SetAnimSize(200.0f, 10000.0f);
			pBeam->SetShrink(2.5f);
			pBeam->SetExpand(160.0f);
			pBeam->SetExtend(100.0f);
		}

		m_nCnt++;

		if (m_nCnt > NUM_BEAMSMALL)
		{
			pBoss->ChangeState(new CStateBossSelect);
		}
	}
}

//=====================================================
// ステップ回避
//=====================================================
void CStateBossStep::Init(CEnemyBoss *pBoss)
{
	// 目的地を設定
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// 目的地を設定
	m_posDest = universal::RelativeInversPos(posPlayer,POS_CENTER,1.0f);
	m_posDestMid = universal::RelativeInversPos(posPlayer, POS_CENTER, 0.0f);

	m_bMid = false;
}

void CStateBossStep::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 pos = pBoss->GetPosition();
	D3DXVECTOR3 posDest = {};

	// 残像を出す
	pBoss->SetAfterImage(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f), 20);

	if (m_bMid)
	{// 中間地点への移動
		posDest = m_posDest;

		universal::MoveToDest(&pos, posDest, SPEED_STEP);

		if (universal::DistCmpFlat(pos, posDest, RANGE_SLASH, nullptr))
		{
			pBoss->ChangeState(new CStateBossSelect);
		}
	}
	else
	{// 最終目標への移動
		posDest = m_posDestMid;

		universal::MoveToDest(&pos, posDest, SPEED_STEP);

		if (universal::DistCmpFlat(pos, posDest, RANGE_SLASH, nullptr))
		{
			m_bMid = true;
		}
	}

	pBoss->SetPosition(pos);

	pBoss->AimPlayer(0.0f, false);
}

//=====================================================
// 死亡時
//=====================================================
void CStateBossDeath::Init(CEnemyBoss *pBoss)
{
	CGame::SetState(CGame::STATE::STATE_END);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_DEATH);
}

void CStateBossDeath::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 posParticle = pBoss->GetMtxPos(CEnemyBoss::IDX_BODY);

	CParticle::Create(posParticle, CParticle::TYPE_SMOKE_DEATH);

	D3DXVECTOR3 move = pBoss->GetMove();

	move.y -= GRAVITY;

	pBoss->SetMove(move);
}