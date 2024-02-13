//*****************************************************
//
// 2D円の処理[Fan3D.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fan3D.h"
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
CFan3D::CFan3D(int nPriority) : CObject(nPriority)
{
	// 変数のクリア
	m_pVtxBuff = nullptr;
	m_bBillboard = false;
	m_pos = { 0,0,0 };
	m_rot = { 0.0f,0.0f,0.0f };
	m_fRadius = 0.0f;
	m_nNumVtx = 0;
	m_fAngleMax = 1.0f;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_nIdxTexture = -1;
}

//=====================================================
// デストラクタ
//=====================================================
CFan3D::~CFan3D()
{

}

//=====================================================
// 生成処理
//=====================================================
CFan3D *CFan3D::Create(int nPriority, int nNumVtx)
{
	CFan3D *pFan3D = nullptr;

	// インスタンス生成
	pFan3D = new CFan3D(nPriority);

	if (pFan3D != nullptr)
	{
		pFan3D->m_nNumVtx = nNumVtx;

		// 初期化処理
		pFan3D->Init();
	}

	return pFan3D;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CFan3D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		if (m_pVtxBuff == nullptr)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nNumVtx + 2),
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				nullptr);
		}
	}

	// テクスチャ読み込み
	int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
	SetIdxTexture(nIdx);

	m_fRadius = INITIAL_RADIUS;

	// 色初期化
	SetCol(m_col);

	// 頂点設定
	SetVtx();

	EnableLighting(false);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFan3D::Uninit(void)
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
void CFan3D::Update(void)
{
	SetVtx();
}

//=====================================================
// 頂点位置設定
//=====================================================
void CFan3D::SetVtx(void)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 中心の頂点の設定
		pVtx[0].pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pVtx[0].tex = D3DXVECTOR2{ 0.5f,0.5f };

		for (int i = 1;i < m_nNumVtx + 2;i++)
		{// 円周の頂点の設定
			float fAngle = (6.28f * m_fAngleMax) * ((float)(i - 1) / (float)m_nNumVtx);

			universal::LimitRot(&fAngle);

			pVtx[i].pos =
			{
				sinf(fAngle) * m_fRadius,
				cosf(fAngle) * m_fRadius,
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
void CFan3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{

		//ワールドマトリックス初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		if (m_bBillboard)
		{// ビルボード描画
			DrawBillboard();
		}
		else
		{// 通常の描画
			DrawNormal();
		}

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャ設定
		LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);
		pDevice->SetTexture(0, pTexture);

		// カリングのOFF
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// 描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_nNumVtx);

		// カリングのON
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_FORCE_DWORD);
	}
}

//=====================================================
// 通常描画
//=====================================================
void CFan3D::DrawNormal(void)
{
	D3DXMATRIX mtxRot, mtxTrans;

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//=====================================================
// ビルボード描画
//=====================================================
void CFan3D::DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;

	//ビューマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに向ける
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//=====================================================
// 設定処理
//=====================================================
void CFan3D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
// 色設定処理
//=====================================================
void CFan3D::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nNumVtx + 2; nCnt++)
		{
			pVtx[nCnt].col = (D3DCOLOR)m_col;

			pVtx[nCnt].nor = D3DXVECTOR3{ 0.0f, 0.0f, -1.0f };
		}

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}
}