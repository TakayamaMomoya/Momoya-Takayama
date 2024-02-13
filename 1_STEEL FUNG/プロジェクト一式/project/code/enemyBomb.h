//*****************************************************
//
// 爆弾敵の処理[enemyBomb.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYBOMB_H_
#define _ENEMYBOMB_H_

//*****************************************************
// インクルード
//*****************************************************
#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyBomb : public CEnemy
{
public:
	CEnemyBomb();	// コンストラクタ
	~CEnemyBomb();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void HitField(void) override;
	void Death(void);

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機モーション
		MOTION_MAX
	};
};

#endif
