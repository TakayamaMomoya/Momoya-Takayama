//*****************************************************
//
// ミサイルのビヘイビア[missileBehavior.xpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "missileBehavior.h"
#include "missile.h"
#include "player.h"
#include "manager.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float DIST_STARTCHASE = 500.0f;	// 待機までの距離
const float TIME_START_CHASE = 1.0f;	// 追跡開始までの時間
const float SPEED_CHASE = 100.0f;	// 追跡スピード
const int RANGE_RANDOM = 500;	// ランダム範囲
}

//=====================================================
// 基底クラス
//=====================================================
CMissileBehavior::CMissileBehavior()
{// コンストラクタ
	m_bEndChase = false;
}

CMissileBehavior::~CMissileBehavior()
{// デストラクタ

}

//=====================================================
// 直線動きのミサイル
//=====================================================
CMissileLine::CMissileLine()
{// コンストラクタ
	
}

CMissileLine::~CMissileLine()
{// デストラクタ

}

void CMissileLine::Init(CMissile *pMissile)
{// 初期化

}

void CMissileLine::Uninit(CMissile *pMissile)
{// 終了

}

void CMissileLine::Update(CMissile *pMissile)
{// 更新
	bool bEndChase = IsEndChase();

	if (bEndChase)
		return;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// 目標向きの取得
		D3DXVECTOR3 pos = pMissile->GetMtxPos(0);
		D3DXVECTOR3 move = pMissile->GetMove();

		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();

		float fSpeedMax = pMissile->GetSpeedMax();

		posPlayer = universal::LinePridiction(pos, fSpeedMax, posPlayer, movePlayer * 0.1f);

		float fSpeed = D3DXVec3Length(&move);

		D3DXVECTOR3 vecDiff = posPlayer - pos;
		float fLengthDiff = D3DXVec3Length(&vecDiff);

		if (fLengthDiff < 1000.0f)
		{
			bEndChase = true;

			EnableChase(bEndChase);
		}

		float fDeltaTime = CManager::GetDeltaTime();

		float fTime = pMissile->GetTimerHit();

		fTime -= fDeltaTime;

		pMissile->SetTimerHit(fTime);

		float fSpeedChase = pMissile->GetSpeedChase();

		if (fTime > 0.0f)
		{
			D3DXVECTOR3 acceleration = 2.0f * (vecDiff - move);

			if (D3DXVec3Length(&acceleration) > fSpeedChase)
			{
				D3DXVec3Normalize(&acceleration, &acceleration);

				acceleration *= fSpeedChase;
			}

			// 移動量を正面に足す
			move += acceleration;

			if (D3DXVec3Length(&move) > fSpeedMax)
			{
				D3DXVec3Normalize(&move, &move);

				move *= fSpeedMax;
			}

			pMissile->SetMove(move);
		}
	}
}

//=====================================================
// 直角に曲がるミサイル
//=====================================================
CMissileRightAngle::CMissileRightAngle()
{// コンストラクタ
	m_state = STATE_MID;
	m_posMid = { 0.0f,0.0f,0.0f };
	m_fTimerStartChase = 0.0f;
}

CMissileRightAngle::~CMissileRightAngle()
{// デストラクタ

}

void CMissileRightAngle::Init(CMissile *pMissile)
{// 初期化
	m_state = STATE_MID;

	DecidePosMid(pMissile);
}

void CMissileRightAngle::DecidePosMid(CMissile *pMissile)
{// 中間地点の設定
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 pos = pMissile->GetPosition();
	D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	m_posMid = posPlayer;

	m_posMid +=
	{
		(float)universal::RandRange(RANGE_RANDOM, -RANGE_RANDOM),
		(float)universal::RandRange(RANGE_RANDOM, RANGE_RANDOM / 2),
		(float)universal::RandRange(RANGE_RANDOM, -RANGE_RANDOM),
	};

	float fLength = D3DXVec3Length(&vecDiff) * 0.8f;

	universal::LimitDistSphereInSide(fLength, &m_posMid, pos);
}

void CMissileRightAngle::Uninit(CMissile *pMissile)
{// 終了

}

void CMissileRightAngle::Update(CMissile *pMissile)
{// 更新
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posDest = m_posMid;
	D3DXVECTOR3 posMissile = pMissile->GetPosition();
	D3DXVECTOR3 move = pMissile->GetMove();
	D3DXVECTOR3 vecDiff = posDest - posMissile;

	switch (m_state)
	{// 目標地点の設定
	case CMissileRightAngle::STATE_MID:	// 中間地点に向かう
	{
		float fSpeed = pMissile->GetSpeedMax();
		universal::VecConvertLength(&vecDiff, fSpeed);

		move += vecDiff;

		if (universal::DistCmp(posMissile, posDest, DIST_STARTCHASE, nullptr))
		{// 追跡開始まで移行する猶予
			move *= 0.0f;

			pMissile->SetMove(move);

			m_state = STATE_WAIT;
		}
		
		pMissile->SetMove(move);
	}
		break;
	case CMissileRightAngle::STATE_WAIT:	// 待機状態
	{
		posDest = pPlayer->GetMtxPos(0);

		vecDiff = posDest - posMissile;

		universal::VecConvertLength(&vecDiff, 0.01f);

		move += vecDiff;

		pMissile->SetMove(move);

		float fDeltaTime = CManager::GetDeltaTime();

		m_fTimerStartChase += fDeltaTime;

		if (m_fTimerStartChase >= TIME_START_CHASE)
		{
			D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

			vecDiff = posPlayer - posMissile;

			m_state = STATE_CHASE;

			universal::VecConvertLength(&vecDiff, SPEED_CHASE);

			move += vecDiff;

			pMissile->SetMove(move);

			Sound::Play(CSound::LABEL_SE_SHOT03);
		}
	}
	break;
	case CMissileRightAngle::STATE_CHASE:	// プレイヤー追跡
	{
		posDest = pPlayer->GetMtxPos(0);

		// 微ホーミングさせる
		universal::Horming(posMissile, posDest, 3.0f, &move);
		
		pMissile->SetMove(move);
	}
		break;
	default:
		break;
	}

	
}