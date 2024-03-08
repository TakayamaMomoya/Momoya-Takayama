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
#include <iostream>
#include <list>

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;
class CFan2D;

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* PATH_BODY_ENEMY[CEnemy::TYPE::TYPE_MAX] =
{// モーションのパス
		nullptr,
		"data\\MOTION\\motionHeli.txt",
		"data\\MOTION\\motionBomb.txt",
		"data\\MOTION\\motionDrone01.txt",
		"data\\MOTION\\motionHeli.txt",
		"data\\MOTION\\motionArms00.txt",
};
}

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
	CEnemy *Lockon(CEnemy *pEnemyExclusive = nullptr);
	void EnableLockTarget(bool bLock);
	bool IsLockTarget(void) { return m_bLockTarget; }
	CEnemy *SwitchTarget(int nAxisX, int nAxisY, CEnemy *pEnemyExclusive = nullptr);
	void CheckDeathLockon(CEnemy *pEnemy);
	CEnemy *GetLockon(void) { return m_pEnemyLockon; }
	void DeleteAll(void);
	void SetEnemyLock(CEnemy *pEnemy) { m_pEnemyLockon = pEnemy; }
	void SpawnGroup(int nIdx);
	bool IsEndSpawn(void) { return m_bEndSpawn; }
	void EnableEndSpawn(bool bEnd) { m_bEndSpawn = bEnd; }
	std::list<CEnemy*> GetListRanking(void) { return m_list; }
	void AddToList(CEnemy *pEnemy);
	void RemoveFromList(CEnemy *pEnemy);
	static CEnemyManager *GetInstance(void) { return m_pEnemyManager; }

private:
	struct SInfoEnemy
	{// 敵の配置情報
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posDestInitial;	// 初期目標位置
		int nType;	// 種類
		float fDelaySpawn;	// スポーンディレイ
	};
	struct SInfoEnemyGroup
	{// 敵集団の情報
		int nNumEnemy;	// 敵の数
		SInfoEnemy *pInfoEnemy;	// 敵の情報
	};

	void Load(void);
	void CreateGauge(void);
	void DeleteGauge(void);
	void ControlGauge(void);

	std::list<CEnemy*> m_list;	// 一覧リスト
	SInfoEnemyGroup *m_pInfoGroup;	// 敵集団の情報
	CEnemy *m_pEnemyLockon;	// ロックオンしてる敵
	bool m_bLockTarget;	// ターゲットをロックしているかどうか
	CUI *m_pCursor;	// ロックオンカーソル
	CUI *m_pIsLock;	// ロックしてるかの表示
	CFan2D *m_pObjectGauge;	// ゲージのポインタ
	CFan2D *m_pObjectFrame;	// フレームのポインタ
	bool m_bEndSpawn;	// スポーン終了フラグ
	float m_fTimerSpawn;	// スポーンタイマー
	int m_nCntSpawn;	// スポーンカウンター
	float m_fTimerChange;	// ロック替えタイマー

	static CEnemyManager *m_pEnemyManager;	// 自身のポインタ
};

#endif
