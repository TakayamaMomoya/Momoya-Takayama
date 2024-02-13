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
	void ChaseTarget(void);

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機モーション
		MOTION_WALK,	// 歩きモーション
		MOTION_ATTACK,	// 攻撃モーション
		MOTION_DEATH,	// 死亡モーション
		MOTION_MAX
	};
};

#endif
