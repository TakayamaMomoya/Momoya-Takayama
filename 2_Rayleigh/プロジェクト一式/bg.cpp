//*****************************************************
//
// 背景の処理[bg.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "bg.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
LPDIRECT3DTEXTURE9 CBg::m_pTexture = nullptr;	// テクスチャのポインタ

//=====================================================
// コンストラクタ
//=====================================================
CBg::CBg(int nPriority) : CObject2D(nPriority)
{
	m_fSpeedU = 0.0f;
	m_fSpeedV = 0.0f;
	m_texLeftUp = { 0.0f,0.0f };
	m_texRightDown = { 0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CBg::~CBg()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBg::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 継承クラスの初期化
	CObject2D::Init();

	m_texLeftUp = { 0.0f,0.0f };
	m_texRightDown = { 1.0f,1.0f };

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBg::Uninit(void)
{
	// 継承クラスの終了
	CObject2D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CBg::Update(void)
{
	m_texLeftUp.x += m_fSpeedU;
	m_texLeftUp.y += m_fSpeedV;
	m_texRightDown.x += m_fSpeedU;
	m_texRightDown.y += m_fSpeedV;

	if (m_texLeftUp.x > 1.0f)
	{
		m_texLeftUp.x -= 1.0f;
		m_texRightDown.x -= 1.0f;
	}

	if (m_texLeftUp.y > 1.0f)
	{
		m_texLeftUp.y -= 1.0f;
		m_texRightDown.y -= 1.0f;
	}

	// 継承クラスの更新
	CObject2D::Update();

	// 頂点情報設定
	SetVtx();

	// テクスチャ座標更新
	SetTex(m_texLeftUp, m_texRightDown);
}

//=====================================================
// 描画処理
//=====================================================
void CBg::Draw(void)
{
	// 継承クラスの描画
	CObject2D::Draw();
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CBg::Load(void)
{
	if (m_pTexture == nullptr)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\BG\\bg101.jpg",
			&m_pTexture
		);
	}

	return S_OK;
}

//=====================================================
// 破棄処理
//=====================================================
void CBg::Unload(void)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//=====================================================
// スクロール速度設定処理
//=====================================================
void CBg::SetScroll(float fSpeedV, float fSpeedU)
{
	m_fSpeedV = fSpeedV;
	m_fSpeedU = fSpeedU;
}

//=====================================================
// 生成処理
//=====================================================
CBg *CBg::Create(void)
{
	CBg *pBg = nullptr;

	if (pBg == nullptr)
	{// インスタンス生成
		pBg = new CBg;

		// 初期化処理
		pBg->Init();
		pBg->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		pBg->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	}

	return pBg;
}
