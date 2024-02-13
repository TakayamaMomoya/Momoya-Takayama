//*****************************************************
//
// コンテナマネージャー[containerManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CONTAINERMANAGER_H_
#define _CONTAINERMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "Container.h"
#include <stdio.h>

//*****************************************************
// 定数定義
//*****************************************************
namespace containermanager
{
const int MAX_SAVE = 24;	// 保存できるコンテナ位置の数
}

//*****************************************************
// クラスの定義
//*****************************************************
class CContainerManager : public CObject
{
public:
	CContainerManager();	// コンストラクタ
	~CContainerManager();	// デストラクタ

	static CContainerManager*Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CContainerManager*GetInstance(void) { return m_pContainerManager; }
	void SavePosition(D3DXVECTOR3 pos);

private:
	struct SInfoSave
	{// 保存する情報
		float fTimeSpawn;	// スポーン時間
		D3DXVECTOR3 pos; // 位置
	};

	void Load(void);
	void DeleteSaveInfo(int nIdx);

	SInfoSave *m_apInfoSave[containermanager::MAX_SAVE];	// 保存情報のポインタ
	float m_fTimeSpawm;	// スポーン頻度
	static CContainerManager *m_pContainerManager;	// 自身のポインタ
};

#endif
