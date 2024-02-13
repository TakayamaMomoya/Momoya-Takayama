//*****************************************************
//
// 残像の処理[alterimage.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "afterimage.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include <stdio.h>

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CAfterImage::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CAfterImage::CAfterImage(int nPriority)
{
	m_nLife = 0;
	m_fDecAlpha = 0.0f;

	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CAfterImage::~CAfterImage()
{
	m_nNumAll--;
}

//=====================================================
// 生成処理
//=====================================================
CAfterImage *CAfterImage::Create(D3DXMATRIX mtx, int nIdxModel, D3DXCOLOR col, int nLife)
{
	CAfterImage *pAlterImage = nullptr;

	if (pAlterImage == nullptr)
	{// インスタンス生成
		pAlterImage = new CAfterImage;

		if (pAlterImage != nullptr)
		{
			// マトリックス適用
			pAlterImage->SetMatrix(mtx);

			// モデル設定
			pAlterImage->SetIdxModel(nIdxModel);
			pAlterImage->BindModel(nIdxModel);

			// パラメーター設定
			pAlterImage->m_nLife = nLife;

			// 初期化処理
			pAlterImage->Init();
			
			pAlterImage->SetEmissiveCol(col);
		}
	}

	return pAlterImage;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CAfterImage::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	m_fDecAlpha = 1.0f / m_nLife;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CAfterImage::Uninit(void)
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CAfterImage::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();

	D3DXCOLOR col = GetEmissiveCol();

	m_nLife--;

	col.a -= m_fDecAlpha;

	SetEmissiveCol(col);

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CAfterImage::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 継承クラスの描画
	CObjectX::JustDraw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}