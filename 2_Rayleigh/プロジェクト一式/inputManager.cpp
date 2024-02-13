//*****************************************************
//
// 入力マネージャー[inputManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "inputManager.h"
#include "manager.h"
#include "inputjoypad.h"
#include "inputkeyboard.h"
#include "inputmouse.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CInputManager *CInputManager::m_pInputManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CInputManager::CInputManager()
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CInputManager::~CInputManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CInputManager *CInputManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInputManager == nullptr)
	{// インスタンス生成
		m_pInputManager = new CInputManager;

		// 初期化処理
		m_pInputManager->Init(hInstance, hWnd);
	}

	return m_pInputManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CInputManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ジョイパッド生成
	CInputJoypad::Create();

	// キーボード生成
	CInputKeyboard::Create(hInstance, hWnd);

	// マウス生成
	CInputMouse::Create(hInstance, hWnd);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CInputManager::Uninit(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	if (pJoypad != nullptr)
	{
		pJoypad->Uninit();
	}

	if (pKeyboard != nullptr)
	{
		pKeyboard->Uninit();
	}

	if (pMouse != nullptr)
	{
		pMouse->Uninit();
	}

	// 自身のポインタ破棄
	m_pInputManager = nullptr;
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CInputManager::Update(void)
{
	// 各入力デバイスの更新
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	if (pJoypad != nullptr)
	{
		pJoypad->Update();
	}

	if (pKeyboard != nullptr)
	{
		pKeyboard->Update();
	}

	if (pMouse != nullptr)
	{
		pMouse->Update();
	}

	// エンター
	m_info.abTrigger[BUTTON_ENTER] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0) ||
		pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
		pKeyboard->GetTrigger(DIK_RETURN)
	);

	// 戻るボタン
	m_info.abTrigger[BUTTON_BACK] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_B, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_BACK, 0) ||
		pMouse->GetTrigger(CInputMouse::BUTTON_RMB)
	);

	// ジャンプ
	m_info.abTrigger[BUTTON_JUMP] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0) ||
		pKeyboard->GetTrigger(DIK_SPACE)
	);

	// 右移動
	m_info.abPress[BUTTON_MOVE_RIGHT] = pKeyboard->GetPress(DIK_D) || pJoypad->GetJoyStickLX(0) > 0;

	// 左移動
	m_info.abPress[BUTTON_MOVE_LEFT] = pKeyboard->GetPress(DIK_A) || pJoypad->GetJoyStickLX(0) < 0;

	// 攻撃
	m_info.abTrigger[BUTTON_ATTACK] = 
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB, 0) ||
		pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
		pKeyboard->GetTrigger(DIK_RETURN)
	);

	// パリィ
	m_info.abTrigger[BUTTON_PARRY] = 
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LB, 0) ||
		pMouse->GetTrigger(CInputMouse::BUTTON_RMB) ||
		pKeyboard->GetTrigger(DIK_BACKSPACE)
	);

	// ダッシュ
	m_info.abTrigger[BUTTON_DASH] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_B, 0) ||
		pKeyboard->GetTrigger(DIK_LSHIFT)
	);

	// ポーズ
	m_info.abTrigger[BUTTON_PAUSE] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0) ||
		pKeyboard->GetTrigger(DIK_P)
	);

	// 上方向キー
	m_info.abTrigger[BUTTON_AXIS_UP] =
	(
		pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_UP,0) || 
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_UP,0) ||
		pKeyboard->GetTrigger(DIK_W)
	);

	// 下方向キー
	m_info.abTrigger[BUTTON_AXIS_DOWN] =
	(
		pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_DOWN, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, 0) ||
		pKeyboard->GetTrigger(DIK_S)
	);
}