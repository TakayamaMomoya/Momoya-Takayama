//*****************************************************
//
// 敵マネージャー[enemyManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyManager : public CObject
{
public:
	CEnemyManager();	// コンストラクタ
	~CEnemyManager();	// デストラクタ

	CEnemy *CreateEnemy(D3DXVECTOR3 pos, CEnemy::TYPE type);
	static CEnemyManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEnemy *GetHead(void) { return m_pHead; }
	CEnemy *GetTail(void) { return m_pTail; }
	void SetHead(CEnemy *pEnemy) { m_pHead = pEnemy; }
	void SetTail(CEnemy *pEnemy) { m_pTail = pEnemy; }
	void ProgressTimeSpawn(bool bAdd);
	void SetTimeScale(float fScale) { m_fScaleSpawnTime = fScale; }
	static CEnemyManager *GetInstance(void) { return m_pEnemyManager; }

private:
	void Load(void);
	void SpawnNormal(void);
	void SpawnThief(void);

	CEnemy *m_pHead;	// 先頭のアドレス
	CEnemy *m_pTail;	// 最後尾のアドレス
	float m_fTimerSpawn;	// 通常敵スポーンカウンター
	float m_fTimerThief;	// 泥棒敵スポーンタイマー
	float m_fTimeSpawnThief;	// 泥棒敵がスポーンするまでの時間
	int m_nMinTimeSpawnThief;	// 泥棒敵のスポーンタイマーの最小
	int m_nMaxTimeSpawnThief;	// 泥棒敵のスポーンタイマーの最大
	float m_afTime[NUM_PLAYER];	// 敵が出現する頻度
	float m_fDistSpawn;	// スポーン距離
	float *m_pAngleSpawn;	// スポーン角度のポインタ
	int m_nNumSpawnAngle;	// スポーン角度の数
	float m_fRateProgress;	// 進行によって出現頻度を減らす割合
	int m_nMaxEnemy;	// 最大の敵数
	float m_fScaleSpawnTime;	// タイムのスケール

	static CEnemyManager *m_pEnemyManager;	// 自身のポインタ
};

#endif
