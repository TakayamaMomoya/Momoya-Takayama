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
#include "effect3D.h"
#include "sound.h"
#include "frame.h"
#include "particle.h"
#include "stateEnemyBoss.h"
#include "beam.h"
#include "slow.h"
#include "fade.h"
#include "camera.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int INITIAL_SCORE = 30000;	// 初期スコア
const float MOVE_FACT = 0.04f;	// 移動係数
const float LINE_END = 5.0f;	// 移動終了のしきい値
const int DAMAGE_FRAME = 10;	// ダメージ状態の時間
const float SPEED_BACK = 1.8f;	// 後退の移動速度
const float SPEED_DODGE = 2.1f;	// 後退時の横移動速度
const float DAMAGE_SLASH = 10.0f;	// 斬撃ダメージ
}

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

		if (m_pEnemyBoss != nullptr)
		{
			m_pEnemyBoss->Load("data\\MOTION\\robot01.txt");

			m_pEnemyBoss->Init();
		}
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

		//pSound->Play(CSound::LABEL_BGM_BOSS);
	}

	// フレーム演出の生成
	CFrame::Create(100, 240, 70);

	// 継承クラスの初期化
	CEnemy::Init();

	// 初期の体力設定
	SetLife(Boss::INITIAL_LIFE);

	// 状態設定
	ChangeState(new CStateBossApper);

	FollowCollision();

	SetPosition(D3DXVECTOR3(0.0f, 1500.0f, 0.0f));

	SetDistLock(50000.0f);

	EnableStamp(false);

	if (m_info.pClsnAttack == nullptr)
	{// 球の当たり判定生成
		m_info.pClsnAttack = CCollisionSphere::Create(CCollision::TAG_NONE, CCollision::TYPE_SPHERE, this);

		if (m_info.pClsnAttack != nullptr)
		{// 情報の設定
			m_info.pClsnAttack->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_info.pClsnAttack->SetRadius(0.0f);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyBoss::Uninit(void)
{
	m_pEnemyBoss = nullptr;

	if (m_info.pState != nullptr)
	{
		delete m_info.pState;
		m_info.pState = nullptr;
	}

	if (m_info.pClsnAttack != nullptr)
	{
		m_info.pClsnAttack->Uninit();
		m_info.pClsnAttack = nullptr;
	}

	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyBoss::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();

	if (m_info.pState != nullptr)
	{// 状態ごとの行動
		m_info.pState->Attack(this);
		m_info.pState->Move(this);
	}

	// 当たり判定管理
	ManageCollision();

	D3DXVECTOR3 pos = GetPosition();

	universal::LimitPosInSq(23000.0f, 23000.0f,&pos);

	SetPosition(pos);
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
// プレイヤーを狙う処理
//=====================================================
void CEnemyBoss::AimPlayer(float fSpeed, bool bPridict)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// 目標向きの取得
		D3DXVECTOR3 pos = GetMtxPos(15);

		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		D3DXVECTOR3 posPridiction;

		if (bPridict)
		{
			posPridiction = universal::LinePridiction(pos, fSpeed, posPlayer, movePlayer);
		}
		else
		{
			posPridiction = posPlayer;
		}

		D3DXVECTOR3 vecDiff = posPridiction - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);
		rotDest.x -= D3DX_PI * 0.5f;
		rotDest.y -= D3DX_PI;

		// 向きの補正
		D3DXVECTOR3 rot = GetRotation();

		universal::FactingRot(&rot.x, rotDest.x, 0.15f);
		universal::FactingRot(&rot.y, rotDest.y, 0.15f);

		SetRotation(rot);
	}
}

//=====================================================
// 後退の処理
//=====================================================
void CEnemyBoss::Back(void)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 pos = GetPosition();

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// 差分ベクトルを移動量に足す
	D3DXVECTOR3 vecDiff = pos - posPlayer;

	D3DXVec3Normalize(&vecDiff,&vecDiff);

	vecDiff *= SPEED_BACK;
	vecDiff.y = 0.0f;

	move += vecDiff;

	// 横方向にも移動量を足す
	float fAngle = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI * 0.5f;

	universal::LimitRot(&fAngle);

	move += {sinf(fAngle + D3DX_PI * 0.5f) * SPEED_DODGE, 0.0f, cosf(fAngle + D3DX_PI * 0.5f) * SPEED_DODGE};
	move += {sinf(fAngle) * SPEED_DODGE, 0.0f, cosf(fAngle) * SPEED_DODGE};

	SetMove(move);
}

//=====================================================
// 腕からビームブレードを出す処理
//=====================================================
void CEnemyBoss::BeamBlade(void)
{
	D3DXMATRIX mtxArm = *GetParts(IDX_HAND_L)->pParts->GetMatrix();
	D3DXMATRIX mtx;

	D3DXVECTOR3 vecBlade = universal::VecToOffset(mtxArm, D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	D3DXVECTOR3 rot = universal::VecToRot(vecBlade);
	m_info.posBlade =
	{
		mtxArm._41,
		mtxArm._42,
		mtxArm._43
	};

	rot.x *= -1;

	CParticle::Create(m_info.posBlade, CParticle::TYPE::TYPE_BEAM_BLADE,rot,&m_info.posBlade);
}

//=====================================================
// イベントの管理
//=====================================================
void CEnemyBoss::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = *GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };
	float fRadius = pEventInfo->fRadius;

	if (nMotion == MOTION_SHOT)
	{
		// ビームの生成
		CBeam *pBeam = CBeam::Create();

		D3DXMATRIX mtxBeam;

		universal::SetOffSet(&mtxBeam, mtxParent, D3DXVECTOR3(0.0f, -1.0f, 1.0f));

		if (pBeam != nullptr)
		{
			D3DXVECTOR3 posMazzle = { mtxBeam._41,mtxBeam._42 ,mtxBeam._43 };
			D3DXVECTOR3 vecMazzle = posMazzle - pos;

			D3DXVECTOR3 rot = GetRotation();

			pBeam->SetCol(D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f));
			pBeam->SetMtx(mtxBeam);
			pBeam->SetRotation(rot);
			pBeam->SetPosition(posMazzle);
			pBeam->SetAnimSize(1000.0f, 5000.0f);
			pBeam->SetShrink(5.0f);
			pBeam->SetExpand(80.0f);
			pBeam->SetExtend(100.0f);
		}
	}

	if (nMotion == MOTION_SLASH)
	{// 斬撃
		if (m_info.pClsnAttack != nullptr)
		{// 情報の設定
			m_info.pClsnAttack->SetPosition(pos);
			m_info.pClsnAttack->SetRadius(fRadius);

			if (m_info.pClsnAttack->OnEnter(CCollision::TAG_PLAYER))
			{
				CObject *pObj = m_info.pClsnAttack->GetOther();

				if (pObj != nullptr)
				{
					pObj->Hit(DAMAGE_SLASH);
				}
			}
		}
	}
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
		//CEffect3D::Create(pos, pCollision->GetRadius(), 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif

		pCollision->SetPositionOld(pCollision->GetPosition());
		pCollision->SetPosition(pos);
		pCollision->SetRadius(Boss::RADIUS_COLLISION);
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

	//SetPosition(pos);

	return bEnd;
}

//=====================================================
// 状態の切り替え
//=====================================================
void CEnemyBoss::ChangeState(CStateBoss *pNext)
{
	if (pNext == nullptr)
		assert(("ボスの状態がnullptrで渡されました",false));

	if (m_info.pState != nullptr)
	{
		delete m_info.pState;
		m_info.pState = nullptr;
	}

	m_info.pState = pNext;
	
	if (m_info.pState != nullptr)
	{
		m_info.pState->Init(this);
	}
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
		{// ヒットサウンド
			//pSound->Play(CSound::LABEL_SE_HIT_BOSS);
		}

		if (fLife <= 0.0f)
		{// 死亡状態

			fLife = 0.0f;

			// スコア管理
			ManageScore();

			// 当たり判定削除
			DeleteCollision();

			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
				pSlow->SetSlowTime(0.5f,0.1f);

			if (m_info.bTrans == false)
			{
				ChangeState(new CStateBossTrans);

				CFade *pFade = CFade::GetInstance();

				if (pFade != nullptr)
				{
					pFade->SetFade(CScene::MODE_GAME, false);
				}
			}
			else
			{
				ChangeState(new CStateBossDeath);

				if (pSlow != nullptr)
					pSlow->SetSlowTime(3.0f, 0.1f);

				CCamera *pCamera = CManager::GetCamera();

				if (pCamera != nullptr)
					pCamera->SetQuake(1.5f, 1.5f, 160);
			}
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