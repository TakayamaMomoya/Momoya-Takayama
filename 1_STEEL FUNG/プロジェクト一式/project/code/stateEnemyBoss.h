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
class CAnim3D;

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
// 行動選択
//=====================================================
class CStateBossSelect : public CStateBoss
{// 行動選択
	void Init(CEnemyBoss *pBoss) override;

	virtual void Close(int nRand, CEnemyBoss *pBoss) = 0;
	virtual void Middle(int nRand, CEnemyBoss *pBoss) = 0;
	virtual void Far(int nRand, CEnemyBoss *pBoss) = 0;
};

class CStateBossSelect1st : public CStateBossSelect
{// 第一形態の行動選択
	void Close(int nRand, CEnemyBoss *pBoss) override;
	void Middle(int nRand, CEnemyBoss *pBoss) override;
	void Far(int nRand, CEnemyBoss *pBoss) override;
};

class CStateBossSelect2nd : public CStateBossSelect
{// 第二形態の行動選択
	void Close(int nRand, CEnemyBoss *pBoss) override;
	void Middle(int nRand, CEnemyBoss *pBoss) override;
	void Far(int nRand, CEnemyBoss *pBoss) override;
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

class CStateBossStep1st : public CStateBoss
{// ステップの逃げ
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	D3DXVECTOR3 m_posDestMid;
	D3DXVECTOR3 m_posDest;
	bool m_bMid;	// 中間のステップを終えたか
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

class CStateBossJump : public CStateBoss
{// 飛び上がり
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	D3DXVECTOR3 m_posDest;
};

class CStateBossBeamAir : public CStateBoss
{// 空中ビーム
	~CStateBossBeamAir();
	void Init(CEnemyBoss *pBoss) override;
	void DisideRotDest(CEnemyBoss *pBoss);
	void Attack(CEnemyBoss *pBoss) override;
	void Rotation(CEnemyBoss *pBoss);
	void Radiation(CEnemyBoss *pBoss);

	CAnim3D *m_pAnim;
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_posAim;
	D3DXVECTOR3 m_moveAim;
	float m_fTimer;
};

//=====================================================
// 撃破
//=====================================================
class CStateBossDeath : public CStateBoss
{// 死亡する
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

class CStateBossAfterDeath : public CStateBoss
{// 死亡中
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

#endif