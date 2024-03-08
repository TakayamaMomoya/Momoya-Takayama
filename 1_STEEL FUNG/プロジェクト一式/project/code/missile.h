//*****************************************************
//
// ミサイルの処理[enemymissile.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "enemy.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMissileBehavior;
class COrbit;
class CBoostEffect;

//*****************************************************
// クラスの定義
//*****************************************************
class CMissile : public CEnemy
{
public:
	enum TYPE
	{
		TYPE_LINE = 0,	// 直線誘導
		TYPE_RIGHTANGLE,	// 直角誘導
		TYPE_MAX
	};
	CMissile();	// コンストラクタ
	~CMissile();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMissile *Create(D3DXVECTOR3 pos,TYPE type = TYPE::TYPE_LINE);
	void Hit(float fDamage) { Death(); }
	void HitField(void) override;
	void SetChaseSpeed(float fSpeed) { m_fChaseSpeed = fSpeed; }
	float GetTimerHit(void) { return m_fTimerHit; }
	void SetTimerHit(float fTimer) { m_fTimerHit = fTimer; }
	float GetSpeedChase(void) { return m_fChaseSpeed; }
	void SetSpeedChase(float fSpeed) { m_fChaseSpeed = fSpeed; }
	float GetSpeedMax(void) { return m_fSpeedMax; }
	void SetSpeedMax(float fSpeed) { m_fSpeedMax = fSpeed; }

private:
	void ChasePlayer(void);	// 追跡処理
	void Tilt(void);	// 傾く処理
	void Death(void);	// 死亡処理

	float m_fTimerHit;	// 着弾タイマー
	int m_nDeathTimer;	// 死亡カウンター
	bool m_bEndChase;	// 追尾終了フラグ
	float m_fChaseSpeed;	// 追尾性能
	float m_fSpeedMax;	// 最大速度
	CMissileBehavior *m_pBehavior;	// 行動のポインタ
	COrbit *m_pOrbit;	// 軌跡
	CBoostEffect *m_pFire;	// 噴射炎
};

#endif
