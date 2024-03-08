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
#include "collision.h"
#include "debugproc.h"
#include "particle.h"
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "motion.h"
#include "universal.h"
#include "animEffect3D.h"
#include "anim3D.h"
#include "sound.h"
#include "particle.h"
#include "debrisSpawner.h"
#include "texture.h"
#include "UI.h"
#include "slow.h"
#include "meshfield.h"
#include "effect3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float INITIAL_LIFE = 5.0f;	// 初期体力
const float INITIAL_SPEED = 1.0f;	// 初期速度
const float LINE_CHASE = 470;	// 追跡状態に移行するエリア
const float LINE_ARRIVAL = 20.0f;	// 目標位置に到達した判定のしきい値
const int DAMAGE_FRAME = 10;	// ダメージ状態のフレーム数
const int TIME_DEATH = 60;	// 死亡までの時間
const float DAMAGE_THROWN = 10.0f;	// 投げでのダメージ
const float SIZE_CURSOR = 30.0f;	// カーソルサイズ
const float INITIAL_DIST_LOCK = 5000.0f;	// カーソルサイズ
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\lockon00.png";	// カーソルのテクスチャ
const float INITIAL_DIST_MOVESTATE[CEnemy::MOVESTATE_MAX] =
{// 移動状態が切り替わる距離
	0.0f,
	3000.0f,
	5000.0f,
	7000.0f
};
}

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

	ZeroMemory(&m_info, sizeof(SInfo));

	// リストに追加
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->AddToList(this);
	}
}

//=====================================================
// デストラクタ
//=====================================================
CEnemy::~CEnemy()
{
	m_nNumAll--;

	// リストから削除
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->RemoveFromList(this);
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
		case TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			pEnemy->Load("data\\Character\\robot00.txt");

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
	// 継承クラスの初期化
	CMotion::Init();

	// タイプの設定
	CObject::SetType(TYPE_ENEMY);

	// 当たり判定生成
	CreateCollision();

	// パラメーター初期設定
	SetLife(INITIAL_LIFE, true);
	SetMoveSpeed(INITIAL_SPEED);
	m_info.fDistLock = INITIAL_DIST_LOCK;
	m_info.bStamp = true;
	m_info.fFactMove = 0.98f;

	// 通常状態にする
	m_info.state = STATE_NORMAL;
	m_info.moveState = MOVESTATE_WAIT;;

	SetPositionOld(GetPosition());

	// 影の有効可
	//EnableShadow(true);
	SetPosShadow(D3DXVECTOR3(0.0f, 0.5f, 0.0f));

	for (int i = 0; i < MOVESTATE_MAX; i++)
	{
		m_info.aDistMoveState[i] = INITIAL_DIST_MOVESTATE[i];
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemy::Uninit(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
		pEnemyManager->CheckDeathLockon(this);

	if (m_info.pCursor != nullptr)
	{
		m_info.pCursor->Uninit();
		m_info.pCursor = nullptr;
	}

	// 当たり判定削除
	DeleteCollision();

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

	// 状態管理
	ManageState();

	// 移動状態管理
	ManageMoveState();

	// 移動量を反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();

	SetPositionOld(pos);

	CSlow *pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		// 位置の更新
		pos += move * fScale;
	}
	else
	{
		// 位置の更新
		pos += move;
	}

	SetPosition(pos);

	// 当たり判定の管理
	ManageCollision();

	// 移動量の減衰
	move *= m_info.fFactMove;
	SetMove(move);
}

//=====================================================
// 当たり判定の管理
//=====================================================
void CEnemy::ManageCollision(void)
{
	if (m_info.pCollisionSphere != nullptr)
	{// 球の当たり判定の管理
		D3DXVECTOR3 posWaist = GetMtxPos(0);

		D3DXVECTOR3 pos = GetPosition();

		m_info.pCollisionSphere->SetPosition(pos);

		if (m_info.state != STATE_THROWN)
		{
			m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_PLAYER);
			m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_ENEMY);

			SetPosition(pos);
		}

		m_info.pCollisionSphere->SetPositionOld(m_info.pCollisionSphere->GetPosition());

		m_info.pCollisionSphere->SetPosition(posWaist);
	}

	if (m_info.pCollisionCube != nullptr)
	{// 立方体の当たり判定の管理
		// 当たり判定の位置設定
		m_info.pCollisionCube->SetPositionOld(m_info.pCollisionCube->GetPosition());
		m_info.pCollisionCube->SetPosition(GetPosition());

		D3DXVECTOR3 move = GetMove();

		// 押し出しの当たり判定
		m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);
		SetMove(move);
	}

	// メッシュフィールドとの当たり判定
	CMeshField *pMesh = CMeshField::GetInstance();

	if (pMesh != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 move = GetMove();

		float fHeight = pMesh->GetHeight(pos, &move);

		if (pos.y <= fHeight)
		{
			CDebugProc::GetInstance()->Print("\nメッシュに当たってるよ[%f]",fHeight);

			move.y = 0.0f;

			pos.y = fHeight;
			SetPosition(pos);
			SetMove(move);

			HitField();
		}
	}
}

//=====================================================
// 地面に当たったときの処理
//=====================================================
void CEnemy::HitField(void)
{

}

//=====================================================
// 状態管理
//=====================================================
void CEnemy::ManageState(void)
{
	D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	switch (m_info.state)
	{
	case STATE_NORMAL:
		break;
	case STATE_THROWN:

		CollisionThrown();

		break;
	case STATE_DAMAGE:
		if (m_info.nTimerState > DAMAGE_FRAME)
		{// 通常状態に戻る
			m_info.nTimerState = 0;
			m_info.state = STATE_NORMAL;

			ResetAllCol();
		}
		else
		{// カウントアップ
			m_info.nTimerState++;
		}

		break;
	case STATE_DEATH:

		m_info.nTimerState++;

		// 透明になりながら消える
		col = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

		col.a = 1.0f - (float)((float)(m_info.nTimerState) / (TIME_DEATH));

		SetAllCol(col);

		if (m_info.nTimerState >= TIME_DEATH)
		{// 死亡
			Uninit();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// 移動状態管理
//=====================================================
void CEnemy::ManageMoveState(void)
{
	switch (m_info.moveState)
	{
	case MOVESTATE_WAIT:
		// 待機状態
		Wait();

		break;
	case MOVESTATE_CHASE:
		// 追跡
		Chase();

		break;
	case MOVESTATE_ATTACK:
		// 攻撃
		Attack();

		break;
	}
}

//=====================================================
// 追跡に移行する
//=====================================================
void CEnemy::TransferChase(void)
{
	m_info.moveState = MOVESTATE_CHASE;
}

//=====================================================
// エリアに入った判定
//=====================================================
bool CEnemy::IsInArea(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (pos.x <= LINE_CHASE && pos.x >= -LINE_CHASE && pos.z <= LINE_CHASE && pos.z >= -LINE_CHASE)
	{
		return true;
	}

	return false;
}

//=====================================================
// 投げられてる時の判定
//=====================================================
void CEnemy::CollisionThrown(void)
{
	if (m_info.pCollisionSphere != nullptr)
	{
		m_info.pCollisionSphere->SetRadius(200.0f);

		if (m_info.pCollisionSphere->OnEnter(CCollision::TAG_ROCKET))
		{// ミサイルの破壊
			m_info.pCollisionSphere->DamageAll(CCollision::TAG_ROCKET, DAMAGE_THROWN);
		}

		if (m_info.pCollisionSphere->OnEnter(CCollision::TAG_ENEMY))
		{
			// 当たった敵にダメージを与える
			m_info.pCollisionSphere->DamageAll(CCollision::TAG_ENEMY, DAMAGE_THROWN);

			D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

			SetMove(move);
		}

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(90.0f);
		}
	}
}

//=====================================================
// 待機
//=====================================================
void CEnemy::Wait(void)
{
	MoveToDest(m_info.posDest, 1.0f);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	float fDist = 0.0f;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	fDist = D3DXVec3Length(&vecDiff);

	float fRot = atan2f(vecDiff.x, vecDiff.z);

	D3DXVECTOR3 rot = GetRotation();

	universal::FactingRot(&rot.y, fRot, 0.1f);

	SetRotation(rot);

	if (m_info.aDistMoveState[MOVESTATE_CHASE] > fDist)
	{// 追跡に移行
		m_info.moveState = MOVESTATE_CHASE;
	}
}

//=====================================================
// 目標の追跡
//=====================================================
void CEnemy::Chase(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	float fDist = 0.0f;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	fDist = D3DXVec3Length(&vecDiff);

	if (m_info.aDistMoveState[MOVESTATE_ATTACK] > fDist)
	{// 攻撃に移行
		m_info.moveState = MOVESTATE_ATTACK;
	}
}

//=====================================================
// 目標に向かう
//=====================================================
bool CEnemy::MoveToDest(D3DXVECTOR3 posDest, float fSpeed)
{
	CEffect3D::Create(posDest, 20.0f, 1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	bool bArrival = false;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posDest - pos;

	// 移動終了の判定
	float fLength = D3DXVec3Length(&vecDiff);
	bArrival = fLength < LINE_ARRIVAL;

	// 移動量の加算
	D3DXVec3Normalize(&vecDiff, &vecDiff);

	vecDiff *= fSpeed;

	D3DXVECTOR3 move = GetMove();

	move += vecDiff;

	SetMove(move);

	return bArrival;
}

//=====================================================
// 攻撃のタイマー
//=====================================================
bool CEnemy::AttackTimer(float fTime)
{
	float fDeltaTime = CManager::GetDeltaTime();
	float fScaleTime = Slow::GetScale();

	m_info.fCntAttack += fDeltaTime * fScaleTime;

	if (m_info.fCntAttack > fTime)
	{
		m_info.fCntAttack = 0.0f;

		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================
// 攻撃
//=====================================================
void CEnemy::Attack(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	float fDist = 0.0f;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	fDist = D3DXVec3Length(&vecDiff);

	if (m_info.aDistMoveState[MOVESTATE_CHASE] < fDist)
	{// 待機に移行
		m_info.moveState = MOVESTATE_WAIT;
	}
}

//=====================================================
// 体力設定
//=====================================================
void CEnemy::SetLife(float fLife, bool bInit)
{
	if (bInit)
		m_info.fLifeInitial = fLife;

	m_info.fLife = fLife;

	if (m_info.fLife < 0)
	{
		m_info.fLife = 0;
	}
}

//=====================================================
// ヒット処理
//=====================================================
void CEnemy::Hit(float fDamage)
{
	if (m_info.state != STATE_DEATH)
	{
		m_info.fLife -= fDamage;

		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			//pSound->Play(pSound->LABEL_SE_HIT);
		}

		if (m_info.fLife <= 0.0f)
		{// 死亡状態
			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			// スコア管理
			ManageScore();

			// 死亡処理
			Death();

			// 当たり判定削除
			DeleteCollision();
		}
		else
		{
			m_info.state = STATE_DAMAGE;

			// ヒット色
			D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			SetAllCol(col);
		}
	}
}

//=====================================================
// 死亡処理
//=====================================================
void CEnemy::Death(void)
{
	Sound::Play(CSound::LABEL_SE_EXPLOSION00);

	D3DXVECTOR3 pos = GetMtxPos(0);

	// アニメーションエフェクト生成
	CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

	if (pAnim3D != nullptr)
	{
		CAnim3D *pAnim = pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);
	}

	// 破片生成
	CDebrisSpawner::Create(pos, CDebrisSpawner::TYPE::TYPE_DEATH);

	m_info.state = STATE_DEATH;

	Uninit();
}

//=====================================================
// スコア管理
//=====================================================
void CEnemy::ManageScore(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();

#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n敵の位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	//CDebugProc::GetInstance()->Print("\n敵の移動量：[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
#endif
}

//=====================================================
// 球の当たり判定の位置設定
//=====================================================
void CEnemy::SetSpherePosition(D3DXVECTOR3 pos)
{
	if (m_info.pCollisionSphere != nullptr)
	{// 位置設定
		m_info.pCollisionSphere->SetPosition(pos);
	}
}

//=====================================================
// 当たり判定生成
//=====================================================
void CEnemy::CreateCollision(float fRadius)
{
	m_info.fRadius = fRadius;

	if (m_info.pCollisionSphere == nullptr)
	{// 球の当たり判定生成
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMY, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{// 情報の設定
			m_info.pCollisionSphere->SetPosition(GetPosition());
			m_info.pCollisionSphere->SetRadius(fRadius);
		}
	}

	if (m_info.pCollisionCube == nullptr)
	{// 立方体の当たり判定
		m_info.pCollisionCube = CCollisionCube::Create(CCollision::TAG_ENEMY, this);

		D3DXVECTOR3 vtxMax = { 20,20,20 };
		D3DXVECTOR3 vtxMin = { -20,0.0f,-20 };

		if (m_info.pCollisionCube != nullptr)
		{
			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}
}

//=====================================================
// 当たり判定削除
//=====================================================
void CEnemy::DeleteCollision(void)
{
	if (m_info.pCollisionSphere != nullptr)
	{// 当たり判定の消去
		m_info.pCollisionSphere->Uninit();

		m_info.pCollisionSphere = nullptr;
	}

	if (m_info.pCollisionCube != nullptr)
	{// 当たり判定の消去
		m_info.pCollisionCube->Uninit();

		m_info.pCollisionCube = nullptr;
	}
}

//=====================================================
// ロックできるかどうかの設定
//=====================================================
void CEnemy::EnableLock(bool bLock)
{
	if (bLock)
	{
		if (m_info.pCursor == nullptr)
		{// カーソル生成
			m_info.pCursor = CUI::Create();

			if (m_info.pCursor != nullptr)
			{
				m_info.pCursor->SetSize(SIZE_CURSOR, SIZE_CURSOR);
				int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
				m_info.pCursor->SetIdxTexture(nIdx);
				m_info.pCursor->SetVtx();
			}
		}
	}
	else
	{
		if (m_info.pCursor != nullptr)
		{
			m_info.pCursor->Uninit();
			m_info.pCursor = nullptr;
		}
	}
}

//=====================================================
// カーソル位置設定
//=====================================================
void CEnemy::SetPositionCursor(D3DXVECTOR3 pos)
{
	if (m_info.pCursor != nullptr)
	{
		m_info.pCursor->SetPosition(pos);
		m_info.pCursor->SetVtx();
	}
}