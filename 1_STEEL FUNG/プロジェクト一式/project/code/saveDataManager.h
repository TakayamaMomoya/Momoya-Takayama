//*****************************************************
//
//セーブデータ管理[saveDataManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SAVEDATAMANAGER_H_
#define _SAVEDATAMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSaveDataManager : public CObject
{
public:
	CSaveDataManager();	// コンストラクタ
	~CSaveDataManager();	// デストラクタ

	static CSaveDataManager *Create(void);
	static CSaveDataManager *GetInstance(void) { return m_pSaveDataManager; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Load(void);
	void Save(void);

	int m_nProgress;	// 現在の進行状況

	static CSaveDataManager *m_pSaveDataManager;	// 自身のポインタ
};

#endif