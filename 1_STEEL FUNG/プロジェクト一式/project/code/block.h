//*****************************************************
//
// ブロックの処理[block.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "ObjectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionCube;

//*****************************************************
// クラスの定義
//*****************************************************
class CBlock : public CObjectX
{
public:
	typedef enum
	{// 種類
		TYPE_WALL = 0,	// 壁
		TYPE_MAX
	}TYPE;

	typedef struct
	{// 保存するときの情報
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		TYPE type;
	}MemBlock;

	CBlock(int nPriority = 3);	// コンストラクタ
	~CBlock();	// デストラクタ

	static CBlock *Create(int nIdxModel);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);
	void SetRotation(D3DXVECTOR3 rot);
	int GetIdx(void) { return m_nIdx; }
	void SetIdx(int nIdx) { m_nIdx = nIdx; }
	CBlock *GetNext(void) { return m_pNext; }
	static int GetNumAll(void) { return m_nNumAll; }
	void SetPosition(D3DXVECTOR3 pos);
	void DeleteCollision(void);

private:
	void SwapVtx(void);

	static int m_nNumAll;	// 総数
	CCollisionCube *m_pCollisionCube;	// 立方体の当たり判定
	float m_fLife;	// 体力
	int m_nIdx;	// 種類のインデックス

	CBlock *m_pPrev;	// 前のアドレス
	CBlock *m_pNext;	// 次のアドレス
};

#endif
