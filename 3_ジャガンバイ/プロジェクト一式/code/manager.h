//*****************************************************
//
// マネージャー処理[manager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CRenderer;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;
class CFade;
class CObjectManager;

//*****************************************************
// クラスの定義
//*****************************************************
class CManager
{
public:
	CManager();	// コンストラクタ
	~CManager();	// デストラクタ

	HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);
	void Uninit(void);
	void Update();
	void Draw();
	static CCamera *GetCamera(void) { return m_pCamera; }
	static CLight *GetLight(void) { return m_pLight; }
	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void) { return m_mode; }
	static void SetScore(int nScore) { m_nScore = nScore; }
	static int GetScore(void) { return m_nScore; }
	static float GetTick(void) { return m_fTick; }
	static void SetTick(float fTick) { m_fTick = fTick; }

private:
	static CCamera *m_pCamera;	// カメラのポインタ
	static CLight *m_pLight;	// ライトのポインタ
	static CScene *m_pScene;	// 現在のシーン
	static CScene::MODE m_mode;	// 現在のモード
	static int m_nScore;	// スコア
	static float m_fTick;	// 前回のフレームから経過した秒数
};

#endif