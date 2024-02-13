//*****************************************************
//
// 2D円の処理[fan2D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FAN2D_H_
#define _FAN2D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CFan2D	: public CObject
{
public:
	CFan2D(int nPriority = 7);
	~CFan2D();	//	デストラクタ

	static CFan2D *Create(int nPriority = 3,int nNumVtx = 16);	// 生成処理
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void SetPosition(D3DXVECTOR3 pos);	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	void SetRot(float fRot);// 向き設定処理
	float GetRot(void) { return m_fRot; }	// 向き取得処理
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetCol(D3DXCOLOR col);
	void SetVtx(void);
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	int GetIdxTexture(void) { return m_nIdxTexture; }
	void SetMaxAngle(float fAngle) { m_fAngleMax = fAngle; }
	float GetMaxAngle(void) { return m_fAngleMax; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXCOLOR m_col;	// 色
	float m_fRot;	// 向き
	float m_fRadius;	// 半径
	float m_fAngleMax;	// 最大の角度
	int m_nNumVtx;	// 外周の頂点数
	int m_nIdxTexture;	// テクスチャの番号
};

#endif