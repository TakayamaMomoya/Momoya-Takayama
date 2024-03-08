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
		BUTTON_SKIP,	// スキップボタン
		BUTTON_JUMP,	// ジャンプ
		BUTTON_SHOT,	// 射撃
		BUTTON_MELEE,	// 近接攻撃
		BUTTON_GRAB,	// 掴み
		BUTTON_DODGE,	// 回避
		BUTTON_LOCK,	// ターゲットロック
		BUTTON_PAUSE,	// ポーズ
		BUTTON_AXIS_UP,	// 上方向キー
		BUTTON_AXIS_DOWN,	// 下方向キー
		BUTTON_AXIS_RIGHT,	// 右方向キー
		BUTTON_AXIS_LEFT,	// 左方向キー
		BUTTON_TRIGGER_UP,	// 上方向弾き
		BUTTON_TRIGGER_DOWN,	// 下方向弾き
		BUTTON_TRIGGER_RIGHT,	// 右方向弾き
		BUTTON_TRIGGER_LEFT,	// 左方向弾き
		BUTTON_MAX
	};
	struct SAxis
	{// 方向のまとめ
		D3DXVECTOR3 axisMove;	// 移動方向
		D3DXVECTOR3 axisCamera;	// カメラを動かす方向
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
	SAxis GetAxis(void) { return m_axis; }

private:
	struct SInfo
	{// 情報の構造体
		bool abTrigger[BUTTON_MAX];	// トリガー情報
		bool abPress[BUTTON_MAX];	// プレス情報
	};

	static CInputManager *m_pInputManager;	// 自身のポインタ
	SInfo m_info;	// 情報
	SAxis m_axis;	// 方向の情報
};

#endif
