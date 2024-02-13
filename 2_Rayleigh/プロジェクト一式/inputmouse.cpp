//*****************************************************
//
// マウス入力処理[inputmouse.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "inputmouse.h"
#include "manager.h"
#include "debugproc.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CInputMouse *CInputMouse::m_pMouse = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CInputMouse::CInputMouse()
{
	// 値のクリア
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));
	ZeroMemory(&m_aKeyState[0], sizeof(m_aKeyState));
	ZeroMemory(&m_aKeyStateTrigger[0], sizeof(m_aKeyStateTrigger));
	ZeroMemory(&m_aKeyStateRelease[0], sizeof(m_aKeyStateRelease));
	ZeroMemory(&m_aKeyStateRepeat[0], sizeof(m_aKeyStateRepeat));
}

//=====================================================
// デストラクタ
//=====================================================
CInputMouse::~CInputMouse()
{

}

//=====================================================
// 生成処理
//=====================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouse;

		if (m_pMouse != nullptr)
		{
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	return m_pMouse;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 基本クラスの初期化
	CInput::Init(hInstance, hWnd);

	//入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//データフォーマットの生成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_REL;						//相対位置モード（REL）、（ABS）絶対位置

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	//マウスのアクセス権獲得
	m_pDevice->Acquire();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CInputMouse::Uninit(void)
{
	// 基本クラスの終了処理
	CInput::Uninit();
}

//===================================================================
//マウス更新処理
//===================================================================
void CInputMouse::Update(void)
{
	//変数宣言
	DIMOUSESTATE2 aMouseState;						//マウス入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &aMouseState)))
	{
		for (nCntKey = 0; nCntKey < NUM_BUTTON_MAX; nCntKey++)
		{
			if (m_mouseState.rgbButtons[nCntKey] == 0x80)
			{// リピートのカウンター
				m_aCntRepeat[nCntKey]++;
			}
			else if (m_mouseState.rgbButtons[nCntKey] == 0 && aMouseState.rgbButtons[nCntKey] == 0)
			{
				m_aCntRepeat[nCntKey] = 0;
			}

			//トリガー情報
			m_aKeyStateTrigger[nCntKey] = (m_mouseState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey])
				& aMouseState.rgbButtons[nCntKey];

			// リリース情報
			m_aKeyStateRelease[nCntKey] = (m_mouseState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]) 
				& m_mouseState.rgbButtons[nCntKey];
		}

		m_mouseState = aMouseState;
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//=====================================================
//移動量取得
//=====================================================
LONG CInputMouse::GetMoveIX(void)
{
	return m_mouseState.lX;
}

//=====================================================
//移動量取得
//=====================================================
LONG CInputMouse::GetMoveIY(void)
{
	return m_mouseState.lY;
}

//=====================================================
//移動量取得
//=====================================================
LONG CInputMouse::GetMoveIZ(void)
{
	return m_mouseState.lZ / LONG_MAX;
}

//=====================================================
//プレス情報取得
//=====================================================
bool CInputMouse::GetPress(int nKey)
{
	//三項演算子
	return(m_mouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================================
//トリガー情報取得
//=====================================================
bool CInputMouse::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=====================================================
//リリース情報取得
//=====================================================
bool CInputMouse::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=====================================================
//リピート情報取得
//=====================================================
int CInputMouse::GetRepeat(int nKey)
{
	return m_aCntRepeat[nKey];
}