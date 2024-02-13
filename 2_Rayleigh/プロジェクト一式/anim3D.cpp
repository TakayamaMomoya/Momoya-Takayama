//*****************************************************
//
// 爆発処理[explosion.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "anim3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"

//=====================================================
// コンストラクタ
//=====================================================
CAnim3D::CAnim3D(int nPriority) : CObject3D(nPriority)
{
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nSpeedAnim = 0;
	m_nNumAnim = 0;
	m_bFinish = false;
	m_bLoop = false;
}

//=====================================================
// デストラクタ
//=====================================================
CAnim3D::~CAnim3D()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CAnim3D::Init(void)
{
	// 継承クラスの初期化
	CObject3D::Init();

	SetAnim(m_nPatternAnim, m_nNumAnim);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CAnim3D::Uninit(void)
{
	// 継承クラスの終了
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CAnim3D::Update(void)
{
	// カウンターを加算
	m_nCounterAnim++;

	if (m_nCounterAnim % m_nSpeedAnim == 0)
	{// 一定時間ごとにアニメーションを行う
		m_nCounterAnim = 0;
		m_nPatternAnim++;

		// アニメーション数を超えたら0枚に戻す
		m_nPatternAnim = m_nPatternAnim % m_nNumAnim;

		// アニメーション設定
		SetAnim(m_nPatternAnim, m_nNumAnim);

		if (m_nPatternAnim == m_nNumAnim - 1)
		{// アニメーション終了
			if (m_bLoop == false)
			{
				Uninit();
			}
		}
	}

	// 継承クラスの更新
	CObject3D::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CAnim3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 継承クラスの描画
	CObject3D::Draw();

	// ライティングを戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\nパス：[%d]", GetIdxTexture());
#endif
}

//=====================================================
// 生成処理
//=====================================================
CAnim3D *CAnim3D::Create(D3DXVECTOR3 pos, int nNumAnim, int nTimeAnim, bool bLoop)
{
	CAnim3D *pAnim3D = nullptr;

	pAnim3D = new CAnim3D;

	if (pAnim3D != nullptr)
	{
		pAnim3D->SetPosition(pos);
		pAnim3D->SetSize(20.0f, 20.0f);
		pAnim3D->m_nNumAnim = nNumAnim;
		pAnim3D->m_nSpeedAnim = nTimeAnim;
		pAnim3D->m_bLoop = bLoop;

		// 初期化処理
		pAnim3D->Init();
	}

	return pAnim3D;
}

//=====================================================
// アニメーション設定
//=====================================================
void CAnim3D::SetAnim(int nAnim, int nNumAnim)
{
	// 頂点バッファ取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	if (pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2
		(
			(1.0f / nNumAnim) * nAnim,
			0.0f
		);
		pVtx[1].tex = D3DXVECTOR2
		(
			1.0f / nNumAnim + 1.0f / nNumAnim * nAnim,
			0.0f
		);
		pVtx[2].tex = D3DXVECTOR2
		(
			1.0f / nNumAnim * nAnim,
			1.0f
		);
		pVtx[3].tex = D3DXVECTOR2
		(
			1.0f / nNumAnim + 1.0f / nNumAnim * nAnim,
			1.0f
		);

		// 頂点バッファのアンロック
		pVtxBuff->Unlock();
	}
}