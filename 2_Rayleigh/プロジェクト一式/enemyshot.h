//*****************************************************
//
// 撃ってくる敵の処理[enemyshot.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYSHOT_H_
#define _ENEMYSHOT_H_

#include "enemyNormal.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyShot : public CEnemyNormal
{// 撃ってくる敵
public:
	CEnemyShot();	// コンストラクタ
	~CEnemyShot();	// デストラクタ

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
