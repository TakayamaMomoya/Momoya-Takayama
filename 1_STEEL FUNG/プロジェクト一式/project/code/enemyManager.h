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
// 前方宣言
//*****************************************************
class CUI;

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
	CEnemy *GetHead(void) { return m_pHead; }
	CEnemy *GetTail(void) { return m_pTail; }
	void SetHead(CEnemy *pEnemy) { m_pHead = pEnemy; }
	void SetTail(CEnemy *pEnemy) { m_pTail = pEnemy; }
	static CEnemyManager *GetInstance(void) { return m_pEnemyManager; }

private:
	struct SInfoEnemy
	{// 敵の配置情報
		D3DXVECTOR3 pos;	// 位置
		int nType;	// 種類
	};
	struct SInfoEnemyGroup
	{// 敵集団の情報
		int nNumEnemy;	// 敵の数
		SInfoEnemy *pInfoEnemy;	// 敵の情報
	};

	void Load(void);

	SInfoEnemyGroup *m_pInfoGroup;	// 敵集団の情報
	CEnemy *m_pEnemyLockon;	// ロックオンしてる敵
	bool m_bLockTarget;	// ターゲットをロックしているかどうか
	float m_fTimer;	// スポーンタイマー
	CUI *m_pCursor;	// ロックオンカーソル
	CEnemy *m_pHead;	// 先頭のアドレス
	CEnemy *m_pTail;	// 最後尾のアドレス

	static CEnemyManager *m_pEnemyManager;	// 自身のポインタ
};

#endif
