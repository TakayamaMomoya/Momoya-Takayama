//*****************************************************
//
// ドローン敵の処理[enemydrone.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYDRONE_H_
#define _ENEMYDRONE_H_

#include "enemyNormal.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyDrone : public CEnemyNormal
{
public:
	CEnemyDrone();	// コンストラクタ
	~CEnemyDrone();	// デストラクタ

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
