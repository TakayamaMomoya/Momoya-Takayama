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
		TYPE_FLOOR,	// 床
		TYPE_CHAIR,	// 椅子
		TYPE_DESK,	// 机
		TYPE_DOCUMENT,	// 書類
		TYPE_FUN,	// 換気扇
		TYPE_LOCKER,	// ロッカー
		TYPE_WINDOW,	// 窓
		TYPE_FLOORBIG,	// でか床
		TYPE_WINDOWBIG,	// でか窓
		TYPE_CASE00,	// 窓ふき１
		TYPE_CASE01,	// 窓ふき柵
		TYPE_CASE02,	// 窓ふき柵
		TYPE_FLOOR4,	// 床×４
		TYPE_ROOFTOPBIG,	// 屋上床
		TYPE_ROOFTOP4,	// 屋上床×４
		TYPE_ROOFTOP1,	// 屋上床×１
		TYPE_HELIPORT,	// ヘリポート
		TYPE_FRONTWINDOW,	// 手前の窓
		TYPE_SIDEWINDOW,	// 横の窓
		TYPE_WALL,	// 壁
		TYPE_WALL2,	// 壁2
		TYPE_WALL3,	// 壁3
		TYPE_SIDEWINDOWBIG,	// でか横窓
		TYPE_FRONTWINDOW4,	// 手前窓4
		TYPE_LONGDESK,	// 長机
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
	//void Hit(float fDamage);

private:
	TYPE m_type;
	static int *m_pIdxObject;	// モデルのタイプ番号のポインタ
	static CBlock *m_apBlock[NUM_OBJECT];	// ブロックの配列
	static int m_nNumAll;	// 総数
	CCollisionCube *m_pCollisionCube;	// 立方体の当たり判定
	int m_nLife;	// 体力
};

#endif
