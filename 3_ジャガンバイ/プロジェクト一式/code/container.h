//*****************************************************
//
// コンテナの処理[container.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CONTAINER_H_
#define _CONTAINER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gimmick.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CItemWeapon;

//*****************************************************
// クラスの定義
//*****************************************************
class CContainer : public CGimmick
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_OPEN,	// 開いている状態
		STATE_END,	// 終了
		STATE_MAX
	};
public:
	CContainer(int nPriority = 3);	// コンストラクタ
	~CContainer();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	static CContainer *Create(void);

	STATE GetState(void) { return m_info.state; }

private:
	
	struct SInfo
	{// 情報
		CItemWeapon* pWeapon;	// 武器アイテムのポインタ
		CObjectX* pCap;	// 蓋のオブジェクト
		STATE state;	// 状態
		float fTimerDeath;	// 死亡までのタイマー
	};

	void Load(void);
	void UpdateOpen(void);
	void Interact(CObject* pObj);
	void Open(void);
	int WeaponRand(void);

	SInfo m_info;
};

#endif

