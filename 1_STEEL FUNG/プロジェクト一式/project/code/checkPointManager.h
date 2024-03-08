//*****************************************************
//
// チェックポイント管理の処理[checkPointmanager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CHECKPOINTMANAGER_H_
#define _CHECKPOINTMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;
class CLimit;
class CCheckPointBehavior;

//*****************************************************
// クラスの定義
//*****************************************************
class CCheckPointManager : public CObject
{
public:
	CCheckPointManager();	// コンストラクタ
	~CCheckPointManager();	// デストラクタ
	
	static CCheckPointManager *Create(void);
	static CCheckPointManager *GetInstance(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetNumCheckPoint(void) { return m_nNumCheckPoint; }
	int GetProgress(void) { return m_nProgress; }
	void SetProgress(int nProgress) { m_nProgress = nProgress; }
	D3DXVECTOR3 GetCheckPosition(int nProgress = -1);
	void AddProgress(void);
	void ChangeBehavior(CCheckPointBehavior *pBehavior);

private:
	void Load(void);
	void LoadInfoCheckPoint(void);
	void TransBossBattle(void);

	int m_nProgress;	// 現在の進行状況
	int m_nNumCheckPoint;	// チェックポイントの数
	D3DXVECTOR3 *m_pPosCheckPoint;	// チェックポイントの座標情報
	CCheckPointBehavior *m_pBehavior;	// ビヘイビア
	static CCheckPointManager *m_pCheckPointManager;	// 自身のポインタ
};

namespace CheckPoint
{
void SetProgress(int nProgress);
}

#endif