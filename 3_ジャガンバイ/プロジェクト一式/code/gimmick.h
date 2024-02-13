//*****************************************************
//
// アイテムの処理[item.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CBillboard;

//*****************************************************
// クラスの定義
//*****************************************************
class CGimmick : public CObjectX
{
public:
	CGimmick(int nPriority = 3);	// コンストラクタ
	~CGimmick();	// デストラクタ

	static CGimmick *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void Interact(CObject* pObj);
	CCollisionSphere *GetCollisionSphere(void) { return m_pCollisionSphere; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	CBillboard *GetGuide(void) { return m_pInteract; }
	CGimmick *GetNext(void) { return m_pNext; }

protected:
	virtual void Exit(CObject* pObj) {};

private:
	CCollisionSphere *m_pCollisionSphere;
	CBillboard *m_pInteract;
	bool m_bEnable;	// インタラクトできるかどうか

	CGimmick *m_pPrev;	// 前のアドレス
	CGimmick *m_pNext;	// 次のアドレス
};

#endif

