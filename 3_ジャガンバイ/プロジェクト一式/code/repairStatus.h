//=========================================================
//
// 修理状況表示処理 [repairStatus.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _REPAIRSTATUS_H_     // このマクロ定義がされてなかったら
#define _REPAIRSTATUS_H_     // 2重インクルード防止のマクロ定義する

//===============================================
// インクルード
//===============================================
#include "main.h"
#include "object3D.h"

//===============================================
// 修理状況クラス
//===============================================
class CRepairStatus : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CRepairStatus(int nPriority = 3);		// デフォルトコンストラクタ
	~CRepairStatus();						// デストラクタ

	static CRepairStatus* Create(int nPriority = 3);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CRepairStatus* m_pRepairStatus;	// 自身のポインタ
	CObject3D* m_pStatus;					// ゲージのポインタ
	CObject3D* m_pFrame;					// 枠のポインタ
};

#endif