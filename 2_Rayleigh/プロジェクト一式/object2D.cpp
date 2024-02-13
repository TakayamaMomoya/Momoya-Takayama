//*****************************************************
//
// 2Dオブジェクトの処理[object2D.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "object2D.h"
#include "manager.h"
#include "renderer.h"
#include "bg.h"
#include "texture.h"

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	// 変数のクリア
	m_pVtxBuff = nullptr;
	m_pos = { 0,0,0 };
	m_move = { 0,0,0 };
	m_fRot = 0;
	m_heigth = 0;
	m_width = 0;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_fCenterHeight = 0.5f;
	m_nIdxTexture = -1;
}

//=====================================================
// デストラクタ
//=====================================================
CObject2D::~CObject2D()
{

}

//=====================================================
// 初期化
//=====================================================
HRESULT CObject2D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		if (m_pVtxBuff == nullptr)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				nullptr);
		}
	}

	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].pos = { 0.0f,0.0f,0.0f };
			pVtx[nCnt].rhw = 1.0;
			pVtx[nCnt].col = m_col;
			pVtx[nCnt].tex = D3DXVECTOR2(0.0f, 0.0f);
		}

		// 対角線の角度取得
		float fAngleUp = atan2f(m_width, m_heigth);
		// 長さの取得
		float fLengthUp = sqrtf(m_width * m_width + m_heigth * m_heigth);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3
		(
			m_pos.x + sinf(m_fRot - D3DX_PI + fAngleUp) * fLengthUp,
			m_pos.y + cosf(m_fRot - D3DX_PI + fAngleUp) * fLengthUp,
			0.0f
		);
		pVtx[1].pos = D3DXVECTOR3
		(
			m_pos.x + sinf(m_fRot + D3DX_PI - fAngleUp) * fLengthUp,
			m_pos.y + cosf(m_fRot + D3DX_PI - fAngleUp) * fLengthUp,
			0.0f
		);
		pVtx[2].pos = D3DXVECTOR3
		(
			m_pos.x + sinf(m_fRot - fAngleUp) * fLengthUp,
			m_pos.y + cosf(m_fRot - fAngleUp) * fLengthUp,
			0.0f
		);
		pVtx[3].pos = D3DXVECTOR3
		(
			m_pos.x + sinf(m_fRot + fAngleUp) * fLengthUp,
			m_pos.y + cosf(m_fRot + fAngleUp) * fLengthUp,
			0.0f
		);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CObject2D::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自分自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CObject2D::Update(void)
{
	// 前回の位置を保存
	m_posOld = m_pos;

	// 位置を移動
	m_pos += m_move;
}

//=====================================================
// 頂点位置設定
//=====================================================
void CObject2D::SetVtx(void)
{
	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 対角線の角度取得
		float fAngleUp = atan2f(m_width, m_heigth);
		// 長さの取得
		float fLengthUp = sqrtf(m_width * m_width + m_heigth * m_heigth);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3
		(
			m_pos.x + sinf(m_fRot - D3DX_PI + fAngleUp) * fLengthUp,
			m_pos.y + cosf(m_fRot - D3DX_PI + fAngleUp) * fLengthUp,
			0.0f
		);
		pVtx[1].pos = D3DXVECTOR3
		(
			m_pos.x + sinf(m_fRot + D3DX_PI - fAngleUp) * fLengthUp,
			m_pos.y + cosf(m_fRot + D3DX_PI - fAngleUp) * fLengthUp,
			0.0f
		);
		pVtx[2].pos = D3DXVECTOR3
		(
			m_pos.x + sinf(m_fRot - fAngleUp) * fLengthUp,
			m_pos.y + cosf(m_fRot - fAngleUp) * fLengthUp,
			0.0f
		);
		pVtx[3].pos = D3DXVECTOR3
		(
			m_pos.x + sinf(m_fRot + fAngleUp) * fLengthUp,
			m_pos.y + cosf(m_fRot + fAngleUp) * fLengthUp,
			0.0f
		);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].col = m_col;
		}

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}

}

//=====================================================
// 描画
//=====================================================
void CObject2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);

		// テクスチャ設定
		pDevice->SetTexture(0, pTexture);

		// 背景の描画
 		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================
// 設定処理
//=====================================================
void CObject2D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
// 中心の高さ
//=====================================================
void CObject2D::SetCenterHeight(float fHeight)
{
	m_fCenterHeight = fHeight;
}

//=====================================================
// 生成処理
//=====================================================
CObject2D *CObject2D::Create(int nPriority, float fCenterHeight)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		// インスタンス生成
		pObject2D = new CObject2D(nPriority);

		if (pObject2D != nullptr)
		{
			pObject2D->m_fCenterHeight = fCenterHeight;

			// 初期化処理
			pObject2D->Init();
		}
	}

	return pObject2D;
}

//=====================================================
// サイズ設定処理
//=====================================================
void CObject2D::SetSize(float width, float height)
{
	m_width = width;
	m_heigth = height;
}

//=====================================================
// テクスチャ座標設定処理
//=====================================================
void CObject2D::SetTex(D3DXVECTOR2 texLeftUp, D3DXVECTOR2 texRightDown)
{
	if (m_pVtxBuff == nullptr)
	{
		return;
	}

	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = texLeftUp;
	pVtx[1].tex = D3DXVECTOR2(texRightDown.x, texLeftUp.y);
	pVtx[2].tex = D3DXVECTOR2(texLeftUp.x, texRightDown.y);
	pVtx[3].tex = texRightDown;

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// アニメーション設定処理
//=====================================================
void CObject2D::SetAnim(int nAnim,int nNumAnim,int nNumV)
{
	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
		m_pVtxBuff->Unlock();
	}
}

//=====================================================
// 移動量設定処理
//=====================================================
void CObject2D::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=====================================================
// 移動量加算処理
//=====================================================
void CObject2D::AddMove(D3DXVECTOR3 move)
{
	m_move += move;
}

//=====================================================
// 移動量減衰処理
//=====================================================
void CObject2D::DicMove(float fDicrease)
{
	m_move.x *= fDicrease;
	m_move.y *= fDicrease;
	m_move.z *= fDicrease;
}

//=====================================================
// 向き設定処理
//=====================================================
void CObject2D::SetRot(float fRot)
{
	m_fRot = fRot;
}

//=====================================================
// 色設定処理
//=====================================================
void CObject2D::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].col = m_col;
		}

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}
}

//=====================================================
// 移動制限処理
//=====================================================
void CObject2D::LimitPos(void)
{
	if (m_pos.y < 0 + m_heigth)
	{// 画面上の判定
		m_pos.y = m_heigth;

		m_move.y = 0.0f;
	}

	if (m_pos.y > SCREEN_HEIGHT - m_heigth)
	{// 画面下の判定
		m_pos.y = SCREEN_HEIGHT - m_heigth;

		m_move.y = 0.0f;
	}

	if (m_pos.x < 0 + m_width)
	{// 画面左の判定
		m_pos.x = m_width;

		m_move.x = 0.0f;
	}

	if (m_pos.x > SCREEN_WIDTH - m_width)
	{// 画面右の判定
		m_pos.x = SCREEN_WIDTH - m_width;

		m_move.x = 0.0f;
	}
}