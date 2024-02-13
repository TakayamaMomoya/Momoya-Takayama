//*****************************************************
//
// 泥棒敵の処理[enemyThief.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYThief_H_
#define _ENEMYThief_H_

//*****************************************************
// インクルード
//*****************************************************
#include "enemy.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyThief : public CEnemy
{
public:
	CEnemyThief();	// コンストラクタ
	~CEnemyThief();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何もしていない状態
		STATE_CHASE,	// 追跡状態
		STATE_ESCAPE,	// 逃走状態
		STATE_MAX
	};
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機モーション
		MOTION_WALK,	// 歩きモーション
		MOTION_DEATH,	// 死亡モーション
		MOTION_DANCE,	// ダンスモーション
		MOTION_MAX
	};
	void SwitchUpdate(void);
	void ChaseTarget(void);
	void CollisionRocket(void);
	void Escape(void);
	void Death(void);
	void LimidPostion(void);

	bool m_bTakeRepair;
	STATE m_state;
	CObject3D *m_pCaution;
	bool m_bSound;
};

#endif
