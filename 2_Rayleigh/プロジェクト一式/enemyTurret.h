//*****************************************************
//
// タレット[enemyTurret.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYTURRET_H_
#define _ENEMYTURRET_H_

#include "enemyNormal.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyTurret : public CEnemyNormal
{// 撃ってくる敵
public:
	CEnemyTurret();	// コンストラクタ
	~CEnemyTurret();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum MOTION
	{// モーション
		MOTION_NEUTRAL = 0,	// 待機モーション
		MOTION_SHOT,	// 射撃モーション
		MOTION_MAX
	};

	void ManageAttack(void);
};

#endif
