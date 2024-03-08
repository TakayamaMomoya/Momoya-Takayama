//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CBlock;
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemy : public CMotion
{
public:
	enum TYPE
	{// 敵の種類
		TYPE_NONE = 0,	// 何でもない
		TYPE_NORMAL,	// 通常敵
		TYPE_BOMB,	// 爆弾敵
		TYPE_DRONE,	// ドローン敵
		TYPE_TUTORIAL,	// チュートリアル敵
		TYPE_BOSS,		// ボス敵
		TYPE_MAX
	};
	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_THROWN,	// 投げられた状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX
	};
	enum MOVESTATE
	{// 移動状態
		MOVESTATE_NONE = 0,	// 何でもない状態
		MOVESTATE_WAIT,	// 待機状態
		MOVESTATE_CHASE,	// 追跡状態
		MOVESTATE_ATTACK,	// 攻撃状態
		MOVESTATE_MAX
	};

	CEnemy();	// コンストラクタ
	~CEnemy();	// デストラクタ

	static CEnemy *Create(D3DXVECTOR3 pos,TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }
	void SetLife(float fLife,bool bInit = false);
	float GetLife(void) { return m_info.fLife; }
	float GetLifeInitial(void) { return m_info.fLifeInitial; }
	void Hit(float fDamage);
	void SetSpherePosition(D3DXVECTOR3 pos);
	STATE GetState(void) { return m_info.state; }
	void SetState(STATE state) { m_info.state = state; }
	CCollisionSphere *GetClsnSphere(void) { return m_info.pCollisionSphere; }
	int GetCntState(void) { return m_info.nTimerState; }
	void SetCntState(int nCnt) { m_info.nTimerState = nCnt; }
	float GetSpeed(void) { return m_info.fMoveSpeed; }
	void SetMoveSpeed(float fSpeed) { m_info.fMoveSpeed = fSpeed; }
	void SetMoveState(MOVESTATE state) { m_info.moveState = state; }
	MOVESTATE GetMoveState(void) { return m_info.moveState; }
	void SetPosDest(D3DXVECTOR3 pos) { m_info.posDest = pos; }
	void CreateCollision(float fRadius = 90.0f);
	void DeleteCollision(void);
	void EnableLock(bool bLock);
	void SetPositionCursor(D3DXVECTOR3 pos);
	void SetDistMoveState(float fValue, MOVESTATE moveState) { m_info.aDistMoveState[moveState] = fValue; }
	bool AttackTimer(float fTime);
	void SetDistLock(float fDist) { m_info.fDistLock = fDist; }
	float GetDistLock(void) { return m_info.fDistLock; }
	bool IsStamp(void) { return m_info.bStamp; }
	void EnableStamp(bool bStamp) { m_info.bStamp = bStamp; }
	void SetMovefact(float fFact) { m_info.fFactMove = fFact; }
	void SetType(TYPE type) { m_info.type = type; }
	TYPE GetType(void) { return m_info.type; }

protected:
	void ManageScore(void);
	virtual void Wait(void);
	virtual void Chase(void);
	virtual void Attack(void);
	virtual void Death(void);
	virtual void TransferChase(void);
	virtual void HitField(void);
	bool MoveToDest(D3DXVECTOR3 posDest,float fSpeed);

private:
	struct SInfo
	{
		float fLife;	// 体力
		float fLifeInitial;	// 初期体力
		float fMoveSpeed;	// 移動速度
		int nTimerState;	// 状態遷移カウンター
		float fCntAttack;	// 攻撃タイマー
		float fRadius;	// 判定半径
		CCollisionSphere *pCollisionSphere;	// 球の当たり判定
		CCollisionCube *pCollisionCube;	// 立方体の当たり判定
		STATE state;	// 状態
		MOVESTATE moveState;	// 移動状態
		D3DXVECTOR3 posDest;	// 目標位置
		CUI *pCursor;	// 捕捉可能カーソル
		float fDistLock;	// ロック可能距離
		float aDistMoveState[MOVESTATE_MAX];	// 移動状態がかわる距離
		bool bStamp;	// 踏めるかどうか
		float fFactMove;	// 移動減衰係数
		TYPE type;	// 種類
	};
	void ManageState(void);
	void ManageMoveState(void);
	void ManageCollision(void);
	bool IsInArea(void);
	void CollisionThrown(void);

	static int m_nNumAll;	// 総数
	SInfo m_info;	// 情報
};

#endif