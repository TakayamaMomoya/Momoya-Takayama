//*****************************************************
//
// 3Dポリゴンの処理[object3D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CObject3D : public CObject
{
public:
	CObject3D(int nPriority = 3);	// コンストラクタ
	~CObject3D();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });
	float GetWidth(void) { return m_width; }	// サイズ取得
	float GetHeight(void) { return m_heigth; }	// サイズ取得
	void SetSize(float width, float height);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(D3DXVECTOR3 rot = { D3DX_PI * 0.5f,0.0f,0.0f }) { m_rot = rot; }
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	int GetIdxTexture(void) { return m_nIdxTexture; }
	void SetColor(D3DXCOLOR col);
	void SetTex(D3DXVECTOR2 texLeftUp, D3DXVECTOR2 texRightDown);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	void EnableAdd(bool bAdd) { m_bAdd = bAdd; }

private:
	void DrawNormal(void);
	void DrawBillboard(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_rot;							//向き
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	D3DXCOLOR m_col;	// 色
	float m_width;	// 幅
	float m_heigth;	// 高さ
	int m_nIdxTexture;	// テクスチャの番号
	bool m_bBillboard;	// ビルボードにするかどうか
	bool m_bZTest;	// Zテストを無効化するかどうか
	bool m_bAdd;	// 加算合成するかどうか
};

#endif