//*****************************************************
//
// ボス敵状態[stateEnemyBoss.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _STATEENEMYBOSS_H_
#define _STATEENEMYBOSS_H_

#include <assert.h>
#include "main.h"

class CEnemyBoss;
class COrbit;
class CBeamBlade;

//*****************************************************
// クラスの定義
//*****************************************************
class CStateBoss
{
public:
	CStateBoss();
	virtual ~CStateBoss();

	virtual void Init(CEnemyBoss *pBoss) {}
	virtual void Attack(CEnemyBoss *pBoss) {}
	virtual void Move(CEnemyBoss *pBoss) {}

protected:
	void CheckPointer(CEnemyBoss *pBoss) {
		if(pBoss == nullptr)assert(("ボス情報の参照ができません", false));
	};

private:
};

//=====================================================
// 第一形態
//=====================================================
class CStateBossApper : public CStateBoss
{// 出現状態
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

class CStateBossAttackBeam : public CStateBoss
{// ビーム攻撃
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;
};

class CStateBossAttackMissile : public CStateBoss
{// ミサイル攻撃
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // 射出カウンター
};

class CStateBossLaunchDrone : public CStateBoss
{// 子機の射出
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // 射出カウンター
};

class CStateBossAttackMachinegun : public CStateBoss
{// マシンガン
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // 射出カウンター
};

class CStateBossTrans : public CStateBoss
{// 第二形態へ移行する
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	void TransMovie(CEnemyBoss *pBoss);
	void Evolve(CEnemyBoss *pBoss);

	float m_fTimeTrans;	// 遷移タイマー
};

//=====================================================
// 第ニ形態
//=====================================================
class CStateBossBeforeTrans : public CStateBoss
{// 第二形態への移行時
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

class CStateBossSelect : public CStateBoss
{// 行動選択
	void Init(CEnemyBoss *pBoss) override;

	void Close(int nRand, CEnemyBoss *pBoss);
	void Middle(int nRand, CEnemyBoss *pBoss);
	void Far(int nRand, CEnemyBoss *pBoss);
};

class CStateBossSlash : public CStateBoss
{// 斬撃
	~CStateBossSlash();
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	COrbit *m_pOrbit;
	CBeamBlade *m_pBlade;
};

class CStateBossBeamSmall : public CStateBoss
{// 小ビーム
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt;
};

class CStateBossStep : public CStateBoss
{// ステップの逃げ
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	D3DXVECTOR3 m_posDestMid;
	D3DXVECTOR3 m_posDest;
	bool m_bMid;	// 中間のステップを終えたか
};

//=====================================================
// 撃破
//=====================================================
class CStateBossDeath : public CStateBoss
{// 死亡中
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};
#endif