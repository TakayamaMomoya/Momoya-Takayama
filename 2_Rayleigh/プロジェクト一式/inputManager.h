//*****************************************************
//
// 入力マネージャー[inputManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CInputManager
{
public:
	enum BUTTON
	{
		BUTTON_ENTER = 0,	// エンターボタン
		BUTTON_BACK,	// 戻るボタン
		BUTTON_JUMP,	// ジャンプ
		BUTTON_ATTACK,	// 攻撃
		BUTTON_PARRY,	// パリィ
		BUTTON_MOVE_RIGHT,	// 右移動
		BUTTON_MOVE_LEFT,	// 左移動
		BUTTON_DASH,	// ダッシュ
		BUTTON_PAUSE,	// ポーズ
		BUTTON_AXIS_UP,	// 上方向キー
		BUTTON_AXIS_DOWN,	// 下方向キー
		BUTTON_AXIS_RIGHT,	// 右方向キー
		BUTTON_AXIS_LEFT,	// 左方向キー
		BUTTON_MAX
	};

	CInputManager();	// コンストラクタ
	~CInputManager();	// デストラクタ

	static CInputManager *Create(HINSTANCE hInstance, HWND hWnd);
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	static CInputManager *GetInstance(void) { return m_pInputManager; }
	bool GetTrigger(BUTTON button) { return m_info.abTrigger[button]; }
	bool GetPress(BUTTON button) { return m_info.abPress[button]; }

private:
	struct SInfo
	{// 情報の構造体
		bool abTrigger[BUTTON_MAX];	// トリガー情報
		bool abPress[BUTTON_MAX];	// プレス情報
	};

	static CInputManager *m_pInputManager;	// 自身のポインタ
	SInfo m_info;	// 情報
};

#endif
