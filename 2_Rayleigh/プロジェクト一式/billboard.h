//*****************************************************
//
// ビルボードの処理[billboard.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CBillboard : public CObject
{
public:
	CBillboard(int nPriority = 5);	// コンストラクタ
	~CBillboard();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBillboard *Create(D3DXVECTOR3 pos, float width, float height);
	float GetWidth(void) { return m_width; }	// サイズ取得
	float GetHeight(void) { return m_heigth; }	// サイズ取得
	void SetSize(float width, float height);
	void SetPosition(D3DXVECTOR3 pos);	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void SetTex(D3DXVECTOR2 texLeftUp, D3DXVECTOR2 texRightDown);
	void SetColor(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	int GetIdxTexture(void) { return m_nIdxTexture; }
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; }
	void SetMatrix(void);
	void SetZTest(bool bZTest) { m_bZTest = bZTest; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXCOLOR m_col;	// 色
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	float m_width;	// 幅
	float m_heigth;	// 高さ
	int m_nIdxTexture;	// テクスチャの番号
	bool m_bZTest;	// Zテストするかどうか
};

#endif