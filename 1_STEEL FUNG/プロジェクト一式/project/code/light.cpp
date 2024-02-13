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
// 定数定義
//*****************************************************
namespace
{
const float SPEED_CHANGE_COL = 0.1f;	// 色を変える速度
const D3DXCOLOR INTIIAL_COLOR[MAX_LIGHT] =
{// 初期の色
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f),
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f),
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f)
};
}

//====================================================
//初期化処理
//====================================================
HRESULT CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXVECTOR3 vecDir;

	//ライト１の設定============================================
	//ライトの種類設定
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光の設定
	m_aLight[0].Diffuse = INTIIAL_COLOR[0];

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
	m_aLight[1].Diffuse = INTIIAL_COLOR[1];

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
	m_aLight[2].Diffuse = INTIIAL_COLOR[2];

	//ライトの方向設定
	vecDir = D3DXVECTOR3(-0.29f, -0.8f, 0.55f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
	m_aLight[2].Direction = vecDir;

	//ライト設定
	pDevice->SetLight(2, &m_aLight[2]);

	//ライト有効化
	pDevice->LightEnable(2, TRUE);

	// 色の初期化
	ResetColDest();

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		m_aInfo[i].col = INTIIAL_COLOR[i];
	}

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
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		// 色の補正
		m_aInfo[i].col.r += (m_aInfo[i].colDest.r - m_aInfo[i].col.r) * SPEED_CHANGE_COL;
		m_aInfo[i].col.g += (m_aInfo[i].colDest.g - m_aInfo[i].col.g) * SPEED_CHANGE_COL;
		m_aInfo[i].col.b += (m_aInfo[i].colDest.b - m_aInfo[i].col.b) * SPEED_CHANGE_COL;
		m_aInfo[i].col.a += (m_aInfo[i].colDest.a - m_aInfo[i].col.a) * SPEED_CHANGE_COL;

		// ライトの拡散光の設定
		m_aLight[i].Diffuse = m_aInfo[i].col;

		// ライト設定
		pDevice->SetLight(i, &m_aLight[i]);
	}
}

//====================================================
// 色の設定
//====================================================
void CLight::SetColDest(int nIdx, D3DXCOLOR col)
{
	m_aInfo[nIdx].colDest = col;
}

//====================================================
// 色のリセット
//====================================================
void CLight::ResetColDest(void)
{
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		m_aInfo[i].colDest = INTIIAL_COLOR[i];
	}
}