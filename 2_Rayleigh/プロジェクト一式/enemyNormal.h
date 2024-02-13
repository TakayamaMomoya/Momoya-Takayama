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
	static void Load(void);	// 読込
	static int GetNumAll(void) { return m_nNumAll; }
	float GetLife(void) { return m_fLife; }
	void SetSpherePosition(D3DXVECTOR3 pos);
	void Death(void);
	CCollisionSphere *GetClsnSphere(void) { return m_pCollisionSphere; }
	CEnemyNormal *GetNext(void) { return m_pNext; }
	int GetScore(void) { return m_nScore; }
	void SetScore(int nScore) { m_nScore = nScore; }
	int GetAttackCounter(void) { return m_nCntAttack; }
	void SetAttackCounter(int nCounter) { m_nCntAttack = nCounter; }

private:
	void ManageState(void);
	void ManageCollision(void);

	static int m_nNumAll;	// 総数
	float m_fLife;	// 体力
	int m_nTimerState;	// 状態遷移カウンター
	CCollisionSphere *m_pCollisionSphere;	// 球の当たり判定
	CCollisionCube *m_pCollisionCube;	// 立方体の当たり判定
	int m_nScore;	// スコア値
	unsigned int m_nCntAttack;	// 攻撃カウンタ

	CEnemyNormal *m_pPrev;	// 前のアドレス
	CEnemyNormal *m_pNext;	// 次のアドレス
};

#endif
