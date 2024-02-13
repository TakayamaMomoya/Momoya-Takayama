//*****************************************************
//
// 2D円の処理[fan2D.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fan2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "universal.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float INITIAL_RADIUS = 50.0f;	// 初期の半径
	const char* TEX_PATH = "data\\TEXTURE\\UI\\gauge.png";	// テクスチャのパス
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CFan2D::CFan2D(int nPriority) : CObject(nPriority)
{
	// 変数のクリア
	m_pVtxBuff = nullptr;
	m_pos = { 0,0,0 };
	m_fRot = 0.0f;
	m_fRadius = 0.0f;
	m_nNumVtx = 0;
	m_fAngleMax = 1.0f;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_nIdxTexture = -1;
}

//=====================================================
// デストラクタ
//=====================================================
CFan2D::~CFan2D()
{

}

//=====================================================
// 生成処理
//=====================================================
CFan2D *CFan2D::Create(int nPriority, int nNumVtx)
{
	CFan2D *pFan2D = nullptr;

	// インスタンス生成
	pFan2D = new CFan2D(nPriority);

	if (pFan2D != nullptr)
	{
		pFan2D->m_nNumVtx = nNumVtx;

		// 初期化処理
		pFan2D->Init();
	}

	return pFan2D;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CFan2D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		if (m_pVtxBuff == nullptr)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (m_nNumVtx + 2),
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				nullptr);
		}
	}

	// テクスチャ読み込み
	int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
	SetIdxTexture(nIdx);

	m_fRadius = INITIAL_RADIUS;
	m_pos = D3DXVECTOR3{ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };

	// 色初期化
	SetCol(m_col);

	// 頂点設定
	SetVtx();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFan2D::Uninit(void)
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
void CFan2D::Update(void)
{

}

//=====================================================
// 頂点位置設定
//=====================================================
void CFan2D::SetVtx(void)
{
	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		
		// 中心の頂点の設定
		pVtx[0].pos = pos;
		pVtx[0].tex = D3DXVECTOR2{ 0.5f,0.5f };

		for (int i = 1;i < m_nNumVtx + 2;i++)
		{// 円周の頂点の設定
			float fAngle = (6.28f * m_fAngleMax) * ((float)(i - 1) / (float)m_nNumVtx);

			universal::LimitRot(&fAngle);

			pVtx[i].pos =
			{
				pos.x + sinf(fAngle) * m_fRadius,
				pos.y - cosf(fAngle) * m_fRadius,
				0.0f,
			};

			D3DXVECTOR2 tex =
			{
				0.5f + sinf(fAngle) * 0.5f,
				0.5f - cosf(fAngle) * 0.5f,
			};

			pVtx[i].tex = tex;
		}

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}
}

//=====================================================
// 描画
//=====================================================
void CFan2D::Draw(void)
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
 		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_nNumVtx);
	}
}

//=====================================================
// 設定処理
//=====================================================
void CFan2D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
// 向き設定処理
//=====================================================
void CFan2D::SetRot(float fRot)
{
	m_fRot = fRot;
}

//=====================================================
// 色設定処理
//=====================================================
void CFan2D::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nNumVtx + 2; nCnt++)
		{
			pVtx[nCnt].col = (D3DCOLOR)m_col;

			pVtx[nCnt].rhw = 1.0;
		}

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}
}