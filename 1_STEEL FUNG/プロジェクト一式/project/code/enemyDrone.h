//*****************************************************
//
// 通常敵の処理[enemyNormal.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYDRONE_H_
#define _ENEMYDRONE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyDrone : public CEnemy
{
public:
	CEnemyDrone();	// コンストラクタ
	~CEnemyDrone();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPositionDest(D3DXVECTOR3 pos) { m_posDest = pos; }
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

	float m_fTimeFire;	// 射撃タイマー
	D3DXVECTOR3 m_posDest;	// 目標位置
	float m_fLengthDest;	// 目標位置の距離
	float m_fAngleDest;	// 目標位置の角度
};

#endif
