//*****************************************************
//
// 通常敵の処理[enemyNormal.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYNORMAL_H_
#define _ENEMYNORMAL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyNormal : public CEnemy
{
public:
	CEnemyNormal();	// コンストラクタ
	~CEnemyNormal();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void HitField(void) override;

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機モーション
		MOTION_WALK,	// 歩きモーション
		MOTION_ATTACK,	// 攻撃モーション
		MOTION_DEATH,	// 死亡モーション
		MOTION_MAX
	};

	virtual void Wait(void);
	virtual void Chase(void);
	virtual void Attack(void);
	void KeepDistance(void);

	float m_fTimeFire;	// 射射撃タイマー
};

#endif
