//*****************************************************
//
// 3D円の処理[fan2D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FAN3D_H_
#define _FAN3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CFan3D	: public CObject
{
public:
	CFan3D(int nPriority = 6);
	~CFan3D();	//	デストラクタ

	static CFan3D *Create(int nPriority = 3,int nNumVtx = 16);	// 生成処理
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
	void DrawNormal(void);	// 通常描画
	void DrawBillboard(void);	// ビルボード描画

	void SetPosition(D3DXVECTOR3 pos);	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }// 向き設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }	// 向き取得処理
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetCol(D3DXCOLOR col);
	void SetVtx(void);
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	int GetIdxTexture(void) { return m_nIdxTexture; }
	void SetMaxAngle(float fAngle) { m_fAngleMax = fAngle; }
	float GetMaxAngle(void) { return m_fAngleMax; }
	void EnableBillboard(bool bEnable) { m_bBillboard = bEnable; }
	void SetAngleMax(float fAngle) { m_fAngleMax = fAngle; }
	float GetAngleMax(void) { return m_fAngleMax; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	float GetRadius(void) { return m_fRadius; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_rot;	// 向き
	D3DXCOLOR m_col;	// 色
	D3DXMATRIX m_mtxWorld;	// マトリックス
	float m_fRadius;	// 半径
	float m_fAngleMax;	// 最大の角度
	int m_nNumVtx;	// 外周の頂点数
	int m_nIdxTexture;	// テクスチャの番号
	bool m_bBillboard;	// ビルボードかどうか
};

#endif