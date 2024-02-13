//*****************************************************
//
// 木箱の処理[box.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BOX_H_
#define _BOX_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CCollisionCube;

//*****************************************************
// クラスの定義
//*****************************************************
class CBox : public CObjectX
{
public:
	enum TYPE
	{
		TYPE_NONE = 0,	// 何でもない状態
		TYPE_REPAIR,	// 修理アイテムが出てくる
		TYPE_RANDOM,	// ランダムに物が出てくる
		TYPE_MAX
	};

	CBox(int nPriority = 3);	// コンストラクタ
	~CBox();	// デストラクタ

	static CBox *Create(TYPE type = TYPE_RANDOM);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);
	void SetType(TYPE type);
	CCollisionSphere *GetCollisionSphere(void) { return m_pCollisionSphere; }
	CCollisionCube *GetCollisionCube(void) { return m_pCollisionCube; }

private:
	CCollisionSphere *m_pCollisionSphere;	// 当たり判定
	CCollisionCube *m_pCollisionCube;	// 矩形の当たり判定
	TYPE m_type;	// 種類
};

#endif

