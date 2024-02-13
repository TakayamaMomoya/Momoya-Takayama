//*****************************************************
//
// ミサイルの処理[enemymissile.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "objectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class COrbit;

//*****************************************************
// クラスの定義
//*****************************************************
class CMissile : public CObjectX
{// プレイヤーに向かう敵
public:
	CMissile();	// コンストラクタ
	~CMissile();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMissile *Create(D3DXVECTOR3 pos);
	void Hit(float fDamage) { Death(); }

private:
	void ChasePlayer(void);	// 追跡処理
	void Tilt(void);	// 傾く処理
	void Death(void);	// 死亡処理

	int m_nDeathTimer;	// 死亡カウンター
	CCollisionSphere *m_pCollisionSphere;	// 球の当たり判定
	COrbit *m_pOrbit;	// 軌跡のポインタ
};

#endif
