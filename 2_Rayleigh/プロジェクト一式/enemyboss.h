//*****************************************************
//
// ボス敵の処理[enemyboss.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYBOSS_H_
#define _ENEMYBOSS_H_

#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyBoss : public CEnemy
{// 撃ってくる敵
public:
	CEnemyBoss();	// コンストラクタ
	~CEnemyBoss();	// デストラクタ

	static CEnemyBoss *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemyBoss *GetInstance(void) { return m_pEnemyBoss; }
	void Hit(float fDamage);

private:
	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何もしていない状態
		STATE_APPER,	// 出現状態
		STATE_BATTLE,	// 戦闘状態
		STATE_MAX
	};
	enum ATTACKSTATE
	{
		ATTACK_MISSILE = 0,	// ミサイル攻撃
		ATTACK_DASH,	// 突進攻撃
		ATTACK_SHOT_UNDER,	// 下から射撃攻撃
		ATTACK_MAX
	};
	enum MOTION
	{// モーション
		MOTION_APPER = 0,	// 出現モーション
		MOTION_MISSILE,	// ミサイルモーション
		MOTION_DASH,	// 突進モーション
		MOTION_SHOT_UNDER,	// 射撃モーション
		MOTION_SHOT_UPPER,	// 反転射撃モーション
		MOTION_DEATH,	// 死亡モーション
		MOTION_MAX
	};
	enum IDXPARTS
	{// パーツの番号
		IDX_WAIST = 0,	// 腰
		IDX_BODY,	// 胴
		IDX_HEAD,	// 頭
		IDX_SHOULDER_L,	// 左肩
		IDX_ARM_L,	// 左腕
		IDX_HAND_L,	// 左手
		IDX_SHOULDER_R,	// 右肩
		IDX_ARM_R,	// 右手
		IDX_HAND_R,	// 右手
		IDX_LEG_L,	// 左腿
		IDX_FOOT_L,	// 左脛
		IDX_TOE_L,	// 左足先
		IDX_LEG_R,	// 右腿
		IDX_FOOT_R,	// 右脛
		IDX_TOE_R,	// 右足先
		IDX_WEAPON,	// 武器
		IDX_MAX
	};
	struct Sinfo
	{// 自身の情報
		STATE state;	// 状態
		ATTACKSTATE attackState;	// 攻撃状態
		D3DXVECTOR3 posDest;	// 目標位置
		int nCntAttack;	// 攻撃カウンター
		int nNumAttack;	// 攻撃した回数
		int nCntState;	// 状態遷移カウンター
	};

	void ManageState(void);
	void UpdateState(void);
	// 状態ごとの更新======
	void UpdateApper(void);
	void UpdateAttackState(void);
	//=====================
	// 攻撃状態ごとの更新==
	void UpdateMissile(void);
	void UpdateDash(void);
	void UpdateShotUnder(void);
	//=====================
	void ManageAttack(void);
	void ManageCollision(void);
	void FollowCollision(void);
	bool FollowDest(void);
	void SwitchState(void);

	static CEnemyBoss *m_pEnemyBoss;	// 自身のポインタ
	Sinfo m_info;	// 自身の情報
};

#endif
