//*****************************************************
//
// マウス入力処理[inputmouse.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "input.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_BUTTON_MAX		(8)				//マウスボタンの数

//*****************************************************
// クラスの定義
//*****************************************************
class CInputMouse : public CInput
{
public:
	//マウスボタン
	typedef enum
	{
		BUTTON_LMB = 0,					//左クリック
		BUTTON_RMB,						//右クリック
		BUTTON_WHEEL,						//ウィールクリック
		BUTTON_SIDE01,						//サイドボタン１
		BUTTON_SIDE02,						//サイドボタン２
		BUTTON_MAX
	}BUTTON;

	CInputMouse();
	~CInputMouse();

	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	int GetRepeat(int nKey);
	LONG GetMoveIX(void);
	LONG GetMoveIY(void);
	LONG GetMoveIZ(void);
	static CInputMouse *GetInstance(void) { return m_pMouse; }

private:
	DIMOUSESTATE2 m_mouseState;					//マウスの情報構造体
	BYTE m_aKeyState[NUM_BUTTON_MAX];					// プレス情報
	BYTE m_aKeyStateTrigger[NUM_BUTTON_MAX];			// トリガー情報
	BYTE m_aKeyStateRelease[NUM_BUTTON_MAX];			// リリース情報
	BYTE m_aKeyStateRepeat[NUM_BUTTON_MAX];			// リピート情報
	int m_aCntRepeat[NUM_BUTTON_MAX];	// リピートカウンター
	static CInputMouse *m_pMouse;	// 自身のポインタ
};

#endif