//*****************************************************
//
//セーブデータ管理[saveDataManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SAVEDATAMANAGER_H_
#define _SAVEDATAMANAGER_H_

#include "main.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSaveDataManager
{
public:
	struct SInfo
	{
		int nProgress;	// 進行状況
		float fInitialLife;	// 初期体力
		float fLife;	// 体力
		int nAddReward;	// 加算報酬
	};

	CSaveDataManager();	// コンストラクタ
	~CSaveDataManager();	// デストラクタ

	static CSaveDataManager *Create(void);
	static CSaveDataManager *GetInstance(void) { return m_pSaveDataManager; }
	HRESULT Init(void);
	void Uninit(void);
	int GetProgress(void) { return m_info.nProgress; }
	SInfo *GetInfo(void) { return &m_info; }
	void Save(void);
	void Load(void);

private:
	SInfo m_info;

	static CSaveDataManager *m_pSaveDataManager;	// 自身のポインタ
};

#endif