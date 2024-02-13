//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "character.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CArrow;
class CBlock;

//*****************************************************
// マクロ定義
//*****************************************************
#define LOCKON_RANGE	(4000.0f)	// ロックオン距離
#define AIM_LOCKON_RANGE	(LOCKON_RANGE * 2.0f)	// エイム時のロックオン距離

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemy : public CCharacter
{
public:
	enum TYPE
	{// 敵の種類
		TYPE_NONE = 0,	// 何でもない
		TYPE_NORMAL,	// 通常敵
		TYPE_THIEF,		// 泥棒敵
		TYPE_MAX
	};

	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX
	};

	enum MOVESTATE
	{// 移動状態
		MOVESTATE_NONE = 0,	// 何でもない状態
		MOVESTATE_INTRUSION,	// 侵入しようとしてる状態
		MOVESTATE_CHASE,	// 追跡状態
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
	void SetLife(float fLife);
	float GetLife(void) { return m_fLife; }
	void Hit(float fDamage);
	void SetSpherePosition(D3DXVECTOR3 pos);
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }
	CCollisionSphere *GetClsnSphere(void) { return m_pCollisionSphere; }
	CEnemy *GetNext(void) { return m_pNext; }
	int GetCntState(void) { return m_nTimerState; }
	void SetCntState(int nCnt) { m_nTimerState = nCnt; }
	void DeleteCollision(void);
	float GetSpeed(void) { return m_fMoveSpeed; }
	void SetMoveSpeed(float fSpeed) { m_fMoveSpeed = fSpeed; }
	MOVESTATE GetMoveState(void) { return m_moveState; }
	void SetPosDest(D3DXVECTOR3 pos) { m_posDest = pos; }

protected:
	CArrow *GetShadow(void) { return m_pShadow; }
	void ManageScore(void);
	CBlock *GetTouchBlock(void) { return m_pBlock; }
	virtual void ChaseTarget(void);
	virtual void Death(void);
	virtual void TransferChase(void);

private:
	void ManageState(void);
	void ManageMoveState(void);
	void ManageCollision(void);
	bool IsInArea(void);

	static int m_nNumAll;	// 総数
	float m_fLife;	// 体力
	float m_fMoveSpeed;	// 移動速度
	int m_nTimerState;	// 状態遷移カウンター
	CCollisionSphere *m_pCollisionSphere;	// 球の当たり判定
	CCollisionCube *m_pCollisionCube;	// 立方体の当たり判定
	CArrow *m_pShadow;	// 影のポインタ
	CBlock *m_pBlock;	// ブロックのポインタ
	STATE m_state;	// 状態
	MOVESTATE m_moveState;	// 移動状態
	D3DXVECTOR3 m_posDest;	// 目標位置

	CEnemy *m_pPrev;	// 前のアドレス
	CEnemy *m_pNext;	// 次のアドレス
};

#endif
