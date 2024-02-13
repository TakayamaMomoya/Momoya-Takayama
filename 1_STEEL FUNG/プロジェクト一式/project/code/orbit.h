//******************************************
//
//  軌跡の生成(orbit.h)
//  Author : 髙山　桃也
//
//******************************************

#ifndef _ORBIT_H_
#define _ORBIT_H_

//******************************************
// インクルード
//******************************************
#include "main.h"
#include "object.h"

//******************************************
// マクロ定義
//******************************************
#define MAX_ORBIT	(64)	//軌跡の最大数
#define MAX_EDGE	(500)	//辺の最大数
#define NUM_OFFSET	(2)	//オフセットの数

//******************************************
// クラス定義
//******************************************
class COrbit : public CObject
{
public:
	COrbit(int nPriority = 6);	// コンストラクタ
	~COrbit();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPositionOffset(D3DXMATRIX mtxWorld, int nIdxOrbit);
	static COrbit *Create(D3DXMATRIX mtxWorld, D3DXVECTOR3 posOffset1, D3DXVECTOR3 posOffset2, D3DXCOLOR col, int nNumEdge);
	bool IsEnd(void) { return m_bEnd; }
	void SetEnd(bool bEnd) { m_bEnd = bEnd; }

private:
	void UpdatePolygon(void);

	int m_nIdxTexture;	// テクスチャ番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_posOffset[NUM_OFFSET];	// オフセット位置
	D3DXVECTOR3 m_aPosPoint[MAX_EDGE][NUM_OFFSET];	// 頂点座標保存用
	D3DXCOLOR m_col;	// 色
	D3DXCOLOR m_aColPoint[MAX_EDGE][NUM_OFFSET];	// 頂点カラー保存用
	D3DXMATRIX m_aMtxOffset[NUM_OFFSET];	// オフセットのマトリックス
	int m_nNumEdge;	//辺の数
	int m_nID;	// 番号
	bool m_bEnd;	// 終了してるかどうか
};

#endif