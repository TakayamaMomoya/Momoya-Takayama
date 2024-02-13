//*****************************************************
//
// ３Dエフェクト処理[effect3D.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(7.0f)	// 移動速度

//=====================================================
// コンストラクタ
//=====================================================
CEffect3D::CEffect3D(int nPriority) : CBillboard(nPriority)
{
	m_nLife = 0;
	m_fDecreaseRadius = 0.0f;
	m_fDecreaseAlpha = 0.0f;
	m_move = { 0.0f,0.0f,0.0f };
	m_relPos = { 0.0f,0.0f,0.0f };
	m_bAdd = true;
	m_fGravity = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CEffect3D::~CEffect3D()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEffect3D::Init(void)
{
	// 継承クラスの初期化
	CBillboard::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEffect3D::Uninit(void)
{
	// 継承クラスの終了
	CBillboard::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEffect3D::Update(void)
{
	// 継承クラスの更新
	CBillboard::Update();

	// 寿命減衰
	m_nLife--;

	// サイズ縮小
	SetSize(GetWidth() - m_fDecreaseRadius, GetHeight() - m_fDecreaseRadius);

	if (GetWidth() < 0.0f)
	{// 大きさの補正
		SetSize(0.0f, 0.0f);
	}

	// 重力加算
	m_move.y -= m_fGravity;

	// 色減少
	SetColor(D3DXCOLOR(GetColor().r, GetColor().g, GetColor().b, GetColor().a - m_fDecreaseAlpha));

	if (m_pPosOwner != nullptr)
	{
		// 相対位置更新
		m_relPos += m_move;

		// 位置更新
		SetPosition(*m_pPosOwner + m_relPos + m_move);
	}
	else
	{
		// 位置更新
		SetPosition(GetPosition() + m_move);
	}

	if (m_nLife < 0)
	{// 自分の削除
		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEffect3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (m_bAdd)
	{
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//Zテストを無効化
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	// 継承クラスの描画
	CBillboard::Draw();

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	if (m_bAdd)
	{
		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=====================================================
// 生成処理
//=====================================================
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move, float fGravity, bool bAdd,float fDecrease, D3DXVECTOR3 *pPosOwner, int nPriority, bool bTurn)
{
	CEffect3D *pEffect3D = nullptr;

	if (pEffect3D == nullptr)
	{// インスタンス生成
		pEffect3D = new CEffect3D(nPriority);

		if (pEffect3D != nullptr)
		{
			pEffect3D->SetPosition(pos);
			pEffect3D->SetSize(fRadius, fRadius);

			// 初期化処理
			pEffect3D->Init();

			pEffect3D->SetColor(col);

			// テクスチャの読込
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\effect000.png");
			pEffect3D->SetIdxTexture(nIdx);

			pEffect3D->m_nLife = nLife;

			pEffect3D->m_move = move;

			pEffect3D->m_fDecreaseRadius = fDecrease;

			pEffect3D->m_fDecreaseAlpha = 1.0f / nLife;

			pEffect3D->m_fGravity = fGravity;

			pEffect3D->m_bAdd = bAdd;
			
			pEffect3D->m_pPosOwner = pPosOwner;

			if (pPosOwner != nullptr)
			{// 位置のリセット
				pEffect3D->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				if (bTurn)
				{
					pEffect3D->m_relPos = move * (float)nLife;

					pEffect3D->m_move *= -1;
				}
			}
		}
	}
	
	return pEffect3D;
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CEffect3D::Load(void)
{
	return S_OK;
}

//=====================================================
// テクスチャ破棄
//=====================================================
void CEffect3D::Unload(void)
{

}