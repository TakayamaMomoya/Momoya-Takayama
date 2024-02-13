//*****************************************************
//
// 2Dオブジェクトの処理[object2D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CObject2D	: public CObject
{
public:
	CObject2D(int nPriority = 3);
	~CObject2D();	//	デストラクタ

	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
	static CObject2D *Create(int nPriority = 3,float fCenterHeight = 0.5f);	// 生成処理

	void SetSize(float width, float height);
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_width, m_heigth); }
	void SetAnim(int nAnim, int nNumAnim, int nNumV);
	void SetTex(D3DXVECTOR2 texLeftUp, D3DXVECTOR2 texRightDown);
	void SetPosition(D3DXVECTOR3 pos);	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void SetMove(D3DXVECTOR3 move);	// 移動量設定処理
	void AddMove(D3DXVECTOR3 move);	// 移動量加算処理
	void DicMove(float fDicrease);	// 移動量減衰処理
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 移動量取得処理
	void SetRot(float fRot);// 向き設定処理
	float GetRot(void) { return m_fRot; }	// 向き取得処理
	void LimitPos(void);	// 移動制限
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetCol(D3DXCOLOR col);
	float GetWidth(void) { return m_width; }	// サイズ取得
	float GetHeight(void) { return m_heigth; }	// サイズ取得
	void SetVtx(void);
	void SetCenterHeight(float fHeight);
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	int GetIdxTexture(void) { return m_nIdxTexture; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXCOLOR m_col;	// 色
	float m_fRot;	// 向き
	float m_width;	// 幅
	float m_heigth;	// 高さ
	float m_fCenterHeight;	// 中心の高さ
	int m_nIdxTexture;	// テクスチャの番号
};

#endif