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
	bool IsFog(void) { return m_fogInfo.bEnable; }
	void EnableFog(bool bFog) { m_fogInfo.bEnable = bFog; }
	static CRenderer *GetInstance(void) { return m_pRenderer; }

private:
	struct SInfoFog
	{// フォグ情報
		float fStart;	// 開始距離
		float fEnd;	// 終了距離
		bool bEnable;	// 有効かどうか
		D3DXCOLOR col;	// 色
	};

	LPDIRECT3D9 m_pD3D;	// オブジェクトの生成
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// デバイス
	SInfoFog m_fogInfo;	// フォグの情報

	static CRenderer *m_pRenderer;	// 自身のポインタ
};

#endif