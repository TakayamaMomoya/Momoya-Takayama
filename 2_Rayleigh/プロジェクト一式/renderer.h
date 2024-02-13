//*****************************************************
//
// レンダラーの処理[renderer.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CRenderer
{
public:
	CRenderer();	// コンストラクタ
	~CRenderer();	// デストラクタ

	static CRenderer *Create(HWND hWnd, BOOL bWindow);
	HRESULT Init(HWND hWnd, BOOL bWindow);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
	void DrawFPS(void);	//FPS表示処理
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// デバイスの取得
	static bool IsFog(void) { return m_bFog; }
	static void EnableFog(bool bFog) { m_bFog = bFog; }
	static CRenderer *GetInstance(void) { return m_pRenderer; }

private:
	LPDIRECT3D9 m_pD3D;	// オブジェクトの生成
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// デバイス
	static bool m_bFog;	// フォグをかけるかどうか

	static CRenderer *m_pRenderer;	// 自身のポインタ
};

#endif