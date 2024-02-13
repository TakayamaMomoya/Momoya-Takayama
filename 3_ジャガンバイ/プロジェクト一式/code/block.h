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
		TYPE_DESK,		// 机
		TYPE_CONCRETE,	// コンクリート壁
		TYPE_WOOD,		// 木壁
		TYPE_DOOR,		// ドア
		TYPE_SOIL,		// 土
		TYPE_SHELF,		// 棚
		TYPE_HIGHCONCRETE,	// 高いコンクリート壁
		TYPE_IRONWALL,	// 鉄の壁
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

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);
	int GetID(void) { return m_nID; }
	static HRESULT Load(char *pPath);	// 読込
	static void Save(void);	// 保存
	static void Delete(int nIdx);	// 部分削除処理
	static void DeleteAll(void);	// 全削除処理
	static int GetNumAll(void) { return m_nNumAll; }
	static CBlock **GetBlock(void) { return &m_apBlock[0]; }
	static float CheckShadow(D3DXVECTOR3 pos);
	static void LoadModel(void);
	static void DeleteIdx(void);
	static int *GetIndex(void) { return m_pIdxObject; }

private:
	void SwapVtx(void);

	TYPE m_type;
	static int *m_pIdxObject;	// モデルのタイプ番号のポインタ
	static CBlock *m_apBlock[NUM_OBJECT];	// ブロックの配列
	static int m_nNumAll;	// 総数
	CCollisionCube *m_pCollisionCube;	// 立方体の当たり判定
	float m_fLife;	// 体力
	int m_nID;	// ID
};

#endif
