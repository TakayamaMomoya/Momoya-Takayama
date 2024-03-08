//*****************************************************
//
// ミサイルの処理[enemymissile.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "missile.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "universal.h"
#include "debugproc.h"
#include "particle.h"
#include "collision.h"
#include "orbit.h"
#include "anim3D.h"
#include "animEffect3D.h"
#include "missileBehavior.h"
#include "boostEffect.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float MAX_SPEED = 100.0f;	// 移動速度
const float MAX_ACCL = 8.0f;	// 最大加速度
const float CHASE_SPEED = 3.0f;	// 追跡速度
const int INITIAL_LIFE = 1;	// 初期体力
const int DEATH_TIME = 240;	// 自滅までの時間
const float WIDTH_ORBIT = 30.0f;	// 軌跡の幅
}

//=====================================================
// コンストラクタ
//=====================================================
CMissile::CMissile()
{
	m_fTimerHit = 0.0f;
	m_nDeathTimer = 0;
	m_bEndChase = false;
	m_fChaseSpeed = 0.0f;
	m_fSpeedMax = 0.0f;
	m_pBehavior = nullptr;
	m_pOrbit = nullptr;
	m_pFire = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CMissile::~CMissile()
{

}

//=====================================================
// 生成処理
//=====================================================
CMissile *CMissile::Create(D3DXVECTOR3 pos, TYPE type)
{
	CMissile *pMissile = nullptr;

	pMissile = new CMissile;

	if (pMissile != nullptr)
	{
		pMissile->SetPosition(pos);

		// 初期化処理
		pMissile->Init();

		switch (type)
		{
		case CMissile::TYPE_LINE:
			pMissile->m_pBehavior = new CMissileLine;

			break;
		case CMissile::TYPE_RIGHTANGLE:
			pMissile->m_pBehavior = new CMissileRightAngle;

			break;
		default:
			assert(("不正なミサイル生成してますよ", false));
			break;
		}

		if (pMissile->m_pBehavior != nullptr)
		{
			pMissile->m_pBehavior->Init(pMissile);
		}
	}

	return pMissile;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMissile::Init(void)
{
	Sound::Play(CSound::LABEL_SE_SHOT02);

	m_fChaseSpeed = MAX_ACCL;
	m_fSpeedMax = MAX_SPEED;

	Load("data\\MOTION\\motionMissileSmall.txt");

	// 継承クラスの初期化
	CEnemy::Init();

	// 当たり判定をミサイルに変更
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		pCollision->SetTag(CCollision::TAG::TAG_ROCKET);
	}

	m_fTimerHit = 5.0f;

	MultiplyMtx();

	if (m_pOrbit == nullptr)
	{
		D3DXMATRIX mtx = *GetMatrix();
		D3DXVECTOR3 offset = { WIDTH_ORBIT,0.0f,0.0f };
		D3DXCOLOR col = { 1.0f, 1.0f,1.0f,1.0f };

		m_pOrbit = COrbit::Create(mtx, offset, -offset, col, 20);
	}

	if (m_pFire == nullptr)
	{
		m_pFire = CBoostEffect::Create();

		if (m_pFire != nullptr)
		{
			D3DXCOLOR col = universal::ConvertRGB(255, 102, 0, 255);

			m_pFire->SetColor(col);
			m_pFire->EnableZtestBoost(true);
		}
	}

	return S_OK;
}

//=====================================================
// 死亡処理
//=====================================================
void CMissile::Death(void)
{
	Sound::Play(CSound::LABEL_SE_EXPLOSION00);

	D3DXVECTOR3 pos = GetPosition();

	// アニメエフェクト生成
	CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

	if (pAnim3D != nullptr)
	{
		pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);
	}

	// 自身の終了
	Uninit();
}

//=====================================================
// 終了処理
//=====================================================
void CMissile::Uninit(void)
{
	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Uninit(this);
		delete m_pBehavior;
		m_pBehavior = nullptr;
	}

	if (m_pOrbit != nullptr)
	{
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}

	if (m_pFire != nullptr)
	{
		m_pFire->Uninit();
		m_pFire = nullptr;
	}

	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CMissile::Update(void)
{
	// 位置保存
	SetPositionOld(GetPosition());

	// 追跡処理
	ChasePlayer();

	if (m_pOrbit != nullptr)
	{
		D3DXMATRIX mtx = *GetMatrix();

		m_pOrbit->SetPositionOffset(mtx, 0);
	}

	if (m_pFire != nullptr)
	{
		D3DXVECTOR3 move = GetMove();
		D3DXMATRIX mtxParent = *GetMatrix();
		D3DXMATRIX mtx;
		D3DXVECTOR3 offset = { 0.0f,-10.0f,0.0f };

		universal::SetOffSet(&mtx, mtxParent, offset);

		D3DXVECTOR3 posBoost = { mtx._41, mtx._42 ,mtx._43 };

		D3DXVECTOR3 rot = universal::VecToRot(-move);
		rot.x *= -1;
		rot.x += D3DX_PI;

		m_pFire->SetRotation(rot);
		m_pFire->SetPosition(posBoost);
	}

	m_nDeathTimer++;

	if (m_nDeathTimer > DEATH_TIME)
	{
		Death();
	}

	// 傾きの制御
	Tilt();

	// 当たり判定をミサイルに変更
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		float fRadius = pCollision->GetRadius();

		pCollision->SetRadius(fRadius * 1.4f);
		
		if (pCollision->OnEnter(CCollision::TAG_PLAYER))
		{
			CObject *pObj = pCollision->GetOther();

			if (pObj != nullptr)
			{
				// 当たったオブジェクトのヒット処理
				pObj->Hit(5.0f);

				Death();

				return;
			}
		}

		pCollision->SetRadius(fRadius);
	}

	// 継承クラスの更新
	CEnemy::Update();
}

//=====================================================
// 追跡処理
//=====================================================
void CMissile::ChasePlayer(void)
{
	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Update(this);
	}
}

//=====================================================
// 傾きの管理
//=====================================================
void CMissile::Tilt(void)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = GetMove();
	float fLength;

	// Y軸平面の角度を取得
	rot.y = atan2f(move.x, move.z);

	// Y軸平面の移動量を取得
	fLength = D3DXVec2Length(&D3DXVECTOR2(move.x,move.z));

	// 横平面の角度を取得
	rot.x = atan2f(fLength,move.y);

	// 回転の設定
	SetRotation(rot);
}

//=====================================================
// 描画処理
//=====================================================
void CMissile::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}

//=====================================================
// 地面に当たったときの処理
//=====================================================
void CMissile::HitField(void)
{
	Death();
}