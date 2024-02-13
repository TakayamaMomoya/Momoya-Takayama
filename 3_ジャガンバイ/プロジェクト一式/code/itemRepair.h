//*****************************************************
//
// 修理アイテムの処理[ItemRepair.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ITEMREPIR_H_
#define _ITEMREPIR_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gimmick.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;
class CFan3D;
class CBillboard;
class CAnim3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CItemRepair : public CGimmick
{
public:
	CItemRepair(int nPriority = 5);	// コンストラクタ
	~CItemRepair();	// デストラクタ

	static CItemRepair *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Detatch(void);

private:
	void Load(void);
	void CreateGuide(void);
	void Interact(CObject* pObj);
	void CollisionRocket(void);
	void CheckPlayerAlive(void);
	void FollowPlayerHand(void);

	bool m_bInRocket;
	bool m_bSound;
	float m_fCntRepair;
	float m_fRepairCounter;
	CPlayer *m_pPlayer;
	CFan3D *m_pGauge;
	CBillboard *m_pInteract;
	CAnim3D *m_pGuide;
};

#endif

