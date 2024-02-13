//*****************************************************
//
// ジョイパッド入力処理[inputjoypad.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "inputjoypad.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float LINE_TRIGGER = 100;	// トリガーボタンのトリガー判定のしきい値
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CInputJoypad *CInputJoypad::m_pJoyPad = nullptr;

//====================================================
// コンストラクタ
//====================================================
CInputJoypad::CInputJoypad()
{
	ZeroMemory(&m_aState, sizeof(m_aState));
	ZeroMemory(&m_aStateRelease, sizeof(m_aStateRelease));
	ZeroMemory(&m_aStateTrigger, sizeof(m_aStateTrigger));
	ZeroMemory(&m_aStateRepeat, sizeof(m_aStateRepeat));
	ZeroMemory(&m_aVibration, sizeof(m_aVibration));
	ZeroMemory(&m_aVibState,sizeof(m_aVibState));
	ZeroMemory(&m_aVibTimer, sizeof(m_aVibTimer));
	ZeroMemory(&m_abTriggerTB, sizeof(m_abTriggerTB));
	ZeroMemory(&m_abPressTB, sizeof(m_abPressTB));
	ZeroMemory(&m_abTrigggerLStick, sizeof(m_abTrigggerLStick));
	ZeroMemory(&m_abTrigggerRStick, sizeof(m_abTrigggerRStick));
}

//====================================================
// デストラクタ
//====================================================
CInputJoypad::~CInputJoypad()
{

}

//====================================================
// 生成処理
//====================================================
CInputJoypad *CInputJoypad::Create(void)
{
	if (m_pJoyPad == nullptr)
	{
		m_pJoyPad = new CInputJoypad;

		if (m_pJoyPad != nullptr)
		{
			m_pJoyPad->Init();
		}
	}

	return m_pJoyPad;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CInputJoypad::Init(void)
{
	//xinputを有効化する
	XInputEnable(true);

	//メモリのクリア
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		memset(&m_aState[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aVibration[nCount], 0, sizeof(XINPUT_VIBRATION));
		memset(&m_aStateTrigger[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aVibState[nCount], 0, sizeof(PADVIB));
	}

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CInputJoypad::Uninit(void)
{
	m_pJoyPad = nullptr;

	//xinputを無効化する
	XInputEnable(false);
}

//====================================================
// 更新処理
//====================================================
void CInputJoypad::Update(void)
{
	//変数宣言
	XINPUT_STATE aState[MAX_PLAYER];

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntKey = 0; nCntKey < CInputJoypad::PADBUTTONS_MAX; nCntKey++)
		{
			if ((m_aState[nCntPlayer].Gamepad.wButtons & (0x01 << nCntKey)))
			{// リピートのカウンター
				m_aCntRepeat[nCntPlayer][nCntKey]++;
			}
			else
			{
				m_aCntRepeat[nCntPlayer][nCntKey] = 0;
			}
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (m_aVibTimer[nCntPlayer] > 0)
		{
			m_aVibTimer[nCntPlayer]--;
		}
		else if (m_aVibTimer[nCntPlayer] <= 0)
		{
			memset(&m_aVibState[nCntPlayer], 0, sizeof(PADVIB));
			m_aVibration[nCntPlayer].wLeftMotorSpeed = 0;
			m_aVibration[nCntPlayer].wRightMotorSpeed = 0;
			//振動状態を伝達
			XInputSetState(nCntPlayer, &m_aVibration[nCntPlayer]);
			m_aVibTimer[nCntPlayer] = 0;
		}

		if (m_aVibration[nCntPlayer].wLeftMotorSpeed < 0 ||
			m_aVibration[nCntPlayer].wRightMotorSpeed < 0)
		{
			m_aVibration[nCntPlayer].wLeftMotorSpeed = 0;
			m_aVibration[nCntPlayer].wRightMotorSpeed = 0;
			//振動状態を伝達
			XInputSetState(nCntPlayer, &m_aVibration[nCntPlayer]);
		}

		//入力デバイスからデータを取得
		if (XInputGetState(nCntPlayer, &aState[nCntPlayer]) == ERROR_SUCCESS)
		{
			// スティックのトリガー判定
			CheckStickTrigger(aState[nCntPlayer], nCntPlayer);

			// トリガーボタンの判定
			CheckTrigger(aState[nCntPlayer], nCntPlayer);

			//トリガー
			m_aStateTrigger[nCntPlayer].Gamepad.wButtons =
				(m_aState[nCntPlayer].Gamepad.wButtons ^ aState[nCntPlayer].Gamepad.wButtons)
				& aState[nCntPlayer].Gamepad.wButtons;

			//リリース
			m_aStateRelease[nCntPlayer].Gamepad.wButtons =
				(m_aState[nCntPlayer].Gamepad.wButtons ^ aState[nCntPlayer].Gamepad.wButtons)
				& m_aState[nCntPlayer].Gamepad.wButtons;			

			//プレス
			m_aState[nCntPlayer] = aState[nCntPlayer];
		}
	}

	CDebugProc::GetInstance()->Print("\nトリガー左[%d]右[%d]",m_aState[0].Gamepad.bLeftTrigger, m_aState[0].Gamepad.bRightTrigger);
}

//====================================================
// トリガー判定取得
//====================================================
bool CInputJoypad::GetPressTB(TRIGGER trigger, int nPlayer)
{
	return m_abPressTB[nPlayer][trigger];
}

//====================================================
// トリガー判定取得
//====================================================
bool CInputJoypad::GetTriggerTB(TRIGGER trigger, int nPlayer)
{
	return m_abTriggerTB[nPlayer][trigger];
}

//====================================================
// トリガーの入力判定
//====================================================
void CInputJoypad::CheckTrigger(XINPUT_STATE state, int nPlayer)
{
	int nDiffRT, nDiffLT;

	nDiffRT = state.Gamepad.bRightTrigger - m_aState[nPlayer].Gamepad.bRightTrigger;
	nDiffLT = state.Gamepad.bLeftTrigger - m_aState[nPlayer].Gamepad.bLeftTrigger;

	// 一気に押されていたらトリガー判定
	m_abTriggerTB[nPlayer][TRIGGER_RT] = nDiffRT > LINE_TRIGGER;
	m_abTriggerTB[nPlayer][TRIGGER_LT] = nDiffLT > LINE_TRIGGER;

	// プレス判定
	m_abPressTB[nPlayer][TRIGGER_RT] = m_aState[nPlayer].Gamepad.bRightTrigger > LINE_TRIGGER;
	m_abPressTB[nPlayer][TRIGGER_LT] = m_aState[nPlayer].Gamepad.bLeftTrigger > LINE_TRIGGER;

	CDebugProc::GetInstance()->Print("\n差分左[%d]右[%d]", nDiffLT, nDiffRT);
}

//====================================================
// ジョイスティックトリガー判定処理
//====================================================
void CInputJoypad::CheckStickTrigger(XINPUT_STATE state, int nPlayer)
{
	// 左スティック==========================================
	// 左右
	float fDiff = (float)state.Gamepad.sThumbLX / USHRT_MAX * 2 - (float)m_aState[nPlayer].Gamepad.sThumbLX / USHRT_MAX * 2;
	
	m_abTrigggerLStick[nPlayer][DIRECTION_RIGHT] = fDiff > 0.5f && (float)m_aState[nPlayer].Gamepad.sThumbLX >= 0.0f;

	m_abTrigggerLStick[nPlayer][DIRECTION_LEFT] = fDiff < -0.5f && (float)m_aState[nPlayer].Gamepad.sThumbLX <= 0.0f;

	// 上下
	fDiff = (float)state.Gamepad.sThumbLY / USHRT_MAX * 2 - (float)m_aState[nPlayer].Gamepad.sThumbLY / USHRT_MAX * 2;

	m_abTrigggerLStick[nPlayer][DIRECTION_UP] = fDiff > 0.5f && (float)m_aState[nPlayer].Gamepad.sThumbLY >= 0.0f;

	m_abTrigggerLStick[nPlayer][DIRECTION_DOWN] = fDiff < -0.5f && (float)m_aState[nPlayer].Gamepad.sThumbLY <= 0.0f;

	// 左スティック==========================================
	// 左右
	fDiff = (float)state.Gamepad.sThumbRX / USHRT_MAX * 2 - (float)m_aState[nPlayer].Gamepad.sThumbRX / USHRT_MAX * 2;

	m_abTrigggerRStick[nPlayer][DIRECTION_RIGHT] = fDiff > 0.5f && (float)m_aState[nPlayer].Gamepad.sThumbRX >= 0.0f;

	m_abTrigggerRStick[nPlayer][DIRECTION_LEFT] = fDiff < -0.5f && (float)m_aState[nPlayer].Gamepad.sThumbRX <= 0.0f;

	// 上下
	fDiff = (float)state.Gamepad.sThumbRY / USHRT_MAX * 2 - (float)m_aState[nPlayer].Gamepad.sThumbRY / USHRT_MAX * 2;

	m_abTrigggerRStick[nPlayer][DIRECTION_UP] = fDiff > 0.5f && (float)m_aState[nPlayer].Gamepad.sThumbRY >= 0.0f;

	m_abTrigggerRStick[nPlayer][DIRECTION_DOWN] = fDiff < -0.5f && (float)m_aState[nPlayer].Gamepad.sThumbRY <= 0.0f;
}

//====================================================
// ジョイスティックトリガー情報
//====================================================
bool CInputJoypad::GetLStickTrigger(DIRECTION direction, int nPlayer)
{
	return m_abTrigggerLStick[nPlayer][direction];
}

//====================================================
// ジョイスティックトリガー情報
//====================================================
bool CInputJoypad::GetRStickTrigger(DIRECTION direction, int nPlayer)
{
	return m_abTrigggerRStick[nPlayer][direction];
}

//====================================================
// ジョイスティック情報取得
//====================================================
float CInputJoypad::GetJoyStickLX(int nPlayer)
{
	//情報を返す
	return (float)m_aState[nPlayer].Gamepad.sThumbLX / USHRT_MAX * 2;
}

//====================================================
// ジョイスティック情報取得
//====================================================
float CInputJoypad::GetJoyStickLY(int nPlayer)
{
	//情報を返す
	return (float)m_aState[nPlayer].Gamepad.sThumbLY / USHRT_MAX * 2;
}

//====================================================
// ジョイスティック情報取得
//====================================================
float CInputJoypad::GetJoyStickRX(int nPlayer)
{
	//情報を返す
	return (float)m_aState[nPlayer].Gamepad.sThumbRX / USHRT_MAX * 2;
}

//====================================================
// ジョイスティック情報取得
//====================================================
float CInputJoypad::GetJoyStickRY(int nPlayer)
{
	//情報を返す
	return (float)m_aState[nPlayer].Gamepad.sThumbRY / USHRT_MAX * 2;
}

//====================================================
// ジョイスティック移動量取得（L）
//====================================================
D3DXVECTOR3 CInputJoypad::GetVecStickL(void)
{
	float fAngle = atan2f(GetJoyStickLX(0), GetJoyStickLY(0));
	float fLength = sqrtf(GetJoyStickLX(0) * GetJoyStickLX(0) + GetJoyStickLY(0) * GetJoyStickLY(0));

	D3DXVECTOR3 vec =
	{
		sinf(fAngle) * fLength,
		cosf(fAngle) * fLength,
		0.0f
	};

	return vec;
}

//====================================================
// プレス情報取得
//====================================================
bool CInputJoypad::GetPress(PADBUTTOS nKey, int nPlayer)
{
	//三項演算子
	return(m_aState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//トリガー情報取得
//====================================================
bool CInputJoypad::GetTrigger(PADBUTTOS nKey, int nPlayer)
{
	//三項演算子
	return(m_aStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//リリース情報取得
//====================================================
bool CInputJoypad::GetRelease(PADBUTTOS nKey, int nPlayer)
{
	//三項演算子
	return(m_aStateRelease[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//リピート情報取得
//====================================================
int CInputJoypad::GetRepeat(PADBUTTOS nKey, int nPlayer)
{
	//三項演算子
	return m_aCntRepeat[nPlayer][nKey];
}

//====================================================
// バイブ情報設定
//====================================================
void CInputJoypad::Vibration(int nPlayer, PADVIB state, float fVib,int nTime)
{
	switch (state)
	{
	case PADVIB_USE:
		m_aVibration[nPlayer].wLeftMotorSpeed = (WORD)(USHRT_MAX * fVib);
		m_aVibration[nPlayer].wRightMotorSpeed = (WORD)(USHRT_MAX * fVib);
		m_aVibState[nPlayer] = state;
		break;
	default:
		break;
	}

	m_aVibTimer[nPlayer] = nTime;

	//振動状態を伝達
	XInputSetState(nPlayer, &m_aVibration[nPlayer]);
}