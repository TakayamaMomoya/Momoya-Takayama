//*****************************************************
//
// ライトの処理[light.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"
#include "debugproc.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define ROLL_SPEED							(0.03f)						//回るスピード

//====================================================
//初期化処理
//====================================================
HRESULT CLight::Init(void)
{
	m_nCurrent = 0;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXVECTOR3 vecDir;

	//ライトの種類設定
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光の設定
	m_aLight[0].Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	//ライトの方向設定
	vecDir = D3DXVECTOR3(-1.4f, 0.24f, -2.21f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
	m_aLight[0].Direction = vecDir;

	//ライト設定
	pDevice->SetLight(0, &m_aLight[0]);

	//ライト有効化
	pDevice->LightEnable(0, TRUE);

	//ライト２の設定============================================
	//ライトの種類設定
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光の設定
	m_aLight[1].Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	//ライトの方向設定
	vecDir = D3DXVECTOR3(1.42f, -0.8f, 0.08f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
	m_aLight[1].Direction = vecDir;

	//ライト設定
	pDevice->SetLight(1, &m_aLight[1]);

	//ライト有効化
	pDevice->LightEnable(1, TRUE);

	//ライト３の設定============================================
	//ライトの種類設定
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光の設定
	m_aLight[2].Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	//ライトの方向設定
	vecDir = D3DXVECTOR3(-0.59f, -0.8f, 1.55f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
	m_aLight[2].Direction = vecDir;

	//ライト設定
	pDevice->SetLight(2, &m_aLight[2]);

	//ライト有効化
	pDevice->LightEnable(2, TRUE);

	return S_OK;
}

//====================================================
//終了処理
//====================================================
void CLight::Uninit(void)
{

}

//====================================================
//更新処理
//====================================================
void CLight::Update(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXVECTOR3 vecDir;

	// 入力情報入手
	//CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	//ライト移動================================================
	//if (pKeyboard->GetPress(DIK_J) == true)
	//{//左移動
	//	m_aLight[m_nCurrent].Direction.x -= ROLL_SPEED;
	//}
	//if (pKeyboard->GetPress(DIK_L) == true)
	//{//右移動
	//	m_aLight[m_nCurrent].Direction.x += ROLL_SPEED;
	//}

	//if (pKeyboard->GetPress(DIK_I) == true)
	//{//奥移動
	//	m_aLight[m_nCurrent].Direction.z += ROLL_SPEED;
	//}
	//if (pKeyboard->GetPress(DIK_K) == true)
	//{//手前移動
	//	m_aLight[m_nCurrent].Direction.z -= ROLL_SPEED;
	//}

	//if (pKeyboard->GetPress(DIK_U) == true)
	//{//上移動
	//	m_aLight[m_nCurrent].Direction.y += ROLL_SPEED;
	//}
	//if (pKeyboard->GetPress(DIK_M) == true)
	//{//下移動
	//	m_aLight[m_nCurrent].Direction.y -= ROLL_SPEED;
	//}

	////ライト切り替え================================================
	//if (pKeyboard->GetTrigger(DIK_O) == true)
	//{//ライト切り替え
	//	m_nCurrent = (m_nCurrent + 1) % MAX_LIGHT;
	//}

	//for (int nCntLight = 0;nCntLight < MAX_LIGHT;nCntLight++)
	//{
	//	//ベクトル正規化
	//	D3DXVec3Normalize
	//	(
	//		&(D3DXVECTOR3)m_aLight[nCntLight].Direction,
	//		&(D3DXVECTOR3)m_aLight[nCntLight].Direction
	//	);

	//	//ライト設定
	//	pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);
	//}

	//pDebugProc->Print("選択ライト[%d]\n", m_nCurrent);
	//pDebugProc->Print("ライトの向き[%f,%f,%f]\n",m_aLight[m_nCurrent].Direction.x, m_aLight[m_nCurrent].Direction.y, m_aLight[m_nCurrent].Direction.z);
}