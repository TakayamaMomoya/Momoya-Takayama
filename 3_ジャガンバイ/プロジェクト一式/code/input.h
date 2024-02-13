//*****************************************************
//
// 入力処理[input.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "Xinput.h"

//*****************************************************
// ライブラリ
//*****************************************************
#pragma comment(lib,"xinput.lib")

//*****************************************************
// クラスの定義
//*****************************************************
class CInput
{
public:
	CInput();	// コンストラクタ
	virtual ~CInput();	// デストラクタ

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;	// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;	// 各デバイスへのポインタ
};

#endif