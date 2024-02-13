//*****************************************************
//
// 背景の処理[bg.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BG_H_
#define _BG_H_

#include "object2D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CBg : public CObject2D
{
public:
	CBg(int nPriority = 1);	// コンストラクタ
	~CBg();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBg *Create(void);	// 生成処理
	static HRESULT Load(void);	// 読込
	static void Unload(void);
	void SetScroll(float fSpeedV,float fSpeedU);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	D3DXVECTOR2 m_texLeftUp;	// テクスチャ右上座標
	D3DXVECTOR2 m_texRightDown;	// テクスチャ左下座標
	float m_fSpeedU;	// 横スクロール速度
	float m_fSpeedV;	// 縦スクロール速度
};

#endif
