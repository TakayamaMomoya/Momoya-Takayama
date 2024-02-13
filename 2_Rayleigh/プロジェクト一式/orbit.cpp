//******************************************
//
// 軌跡の処理
// Author : 髙山桃也
//
//******************************************

//******************************************
// インクルード
//******************************************
#include "orbit.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//******************************************
// マクロ定義
//******************************************
#define DELETE_LENGTH	(0.5f)	// 削除する長さ

//==========================================
// コンストラクタ
//==========================================
COrbit::COrbit(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_aColPoint[0], sizeof(m_aColPoint));
	ZeroMemory(&m_aMtxOffset[0], sizeof(m_aMtxOffset));
	ZeroMemory(&m_aPosPoint[0], sizeof(m_aPosPoint));
	ZeroMemory(&m_posOffset[0],sizeof(m_posOffset));
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_nIdxTexture = 0;
	m_nNumEdge = 0;
	m_nID = -1;
	m_pVtxBuff = nullptr;
	m_bEnd = false;
}

//==========================================
// デストラクタ
//==========================================
COrbit::~COrbit()
{

}

//==========================================
// 初期化処理
//==========================================
HRESULT COrbit::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// テクスチャ読込
	m_nIdxTexture = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\orbit001.png");

	if (m_pVtxBuff == nullptr)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer
		(
			sizeof(VERTEX_3D) * MAX_EDGE * NUM_OFFSET,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr
		);
	}

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0;nCntVtx < MAX_EDGE * NUM_OFFSET;nCntVtx++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f * (nCntVtx % NUM_OFFSET));

		pVtx += 1;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==========================================
// 終了処理
//==========================================
void COrbit::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{//頂点情報破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自身の破棄
	Release();
}

//==========================================
// 更新処理
//==========================================
void COrbit::Update(void)
{
	if (m_bEnd)
	{
		//ポリゴン更新処理
		UpdatePolygon();
	}
}

//==========================================
// ポリゴン更新処理
//==========================================
void COrbit::UpdatePolygon(void)
{
	//変数宣言
	int nCntOffset;
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DXVECTOR3 posTemp[NUM_OFFSET];

	//保存した座標をずらす==========
	for (int nCntVtx = 1; nCntVtx < m_nNumEdge; nCntVtx++)
	{
		for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{
			//一つ前の座標にずれる
			m_aPosPoint[nCntVtx - 1][nCntOffset] = m_aPosPoint[nCntVtx][nCntOffset];
		}
	}

	for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
	{
		//現在のフレームのオフセット位置を保存
		m_aPosPoint[m_nNumEdge - 1][nCntOffset] =
		{
			m_aMtxOffset[nCntOffset]._41,
			m_aMtxOffset[nCntOffset]._42,
			m_aMtxOffset[nCntOffset]._43
		};
	}
	//保存した座標をずらす==========

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bEnd)
	{// 切り離しからの自動削除
		D3DXVECTOR3 vecDiff;

		vecDiff = pVtx[0].pos - pVtx[m_nNumEdge * NUM_OFFSET].pos;

		if (D3DXVec3Length(&vecDiff) < DELETE_LENGTH)
		{
			Uninit();
		}
	}

	for (int nCntVtx = 0; nCntVtx < m_nNumEdge; nCntVtx++)
	{//辺ごとの頂点座標設定

		for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{//オフセットの数分設定

			 //頂点座標の設定
			pVtx[nCntOffset].pos = m_aPosPoint[nCntVtx][nCntOffset];

			//頂点カラーの設定
			pVtx[nCntOffset].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, (float)nCntVtx / m_nNumEdge);
		}

		//ポインタを進める
		pVtx += NUM_OFFSET;
	}
}

//==========================================
// 描画処理
//==========================================
void COrbit::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;						// 計算用マトリックス
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	// ライティング無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングを無効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ取得
	pTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,		//描画する最初のインデックス
		m_nNumEdge + (m_nNumEdge - 2)
	);

	// カリングを有効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティング有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================
// 生成処理
//==========================================
COrbit *COrbit::Create(D3DXMATRIX mtxWorld, D3DXVECTOR3 m_posOffset1, D3DXVECTOR3 m_posOffset2, D3DXCOLOR col,int nNumEdge)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// 変数宣言
	int nCounterOrbit = -1;
	COrbit *pOrbit = nullptr;

	if (pOrbit == nullptr)
	{
		pOrbit = new COrbit;

		if (pOrbit != nullptr)
		{
			// 初期化
			pOrbit->Init();

			// 色の代入
			pOrbit->m_col = col;

			// オフセットの代入
			pOrbit->m_posOffset[0] = m_posOffset1;
			pOrbit->m_posOffset[1] = m_posOffset2;

			// 辺の数の代入
			pOrbit->m_nNumEdge = nNumEdge;

			pOrbit->m_aMtxOffset[0]._41 = m_posOffset1.x + mtxWorld._41;
			pOrbit->m_aMtxOffset[0]._42 = m_posOffset1.y + mtxWorld._42;
			pOrbit->m_aMtxOffset[0]._43 = m_posOffset1.z + mtxWorld._43;
			pOrbit->m_aMtxOffset[1]._41 = m_posOffset2.x + mtxWorld._41;
			pOrbit->m_aMtxOffset[1]._42 = m_posOffset2.y + mtxWorld._42;
			pOrbit->m_aMtxOffset[1]._43 = m_posOffset2.z + mtxWorld._43;

			for (int nCntVtx = 0; nCntVtx < nNumEdge; nCntVtx++)
			{
				pOrbit->m_aPosPoint[nCntVtx][0].x = m_posOffset1.x + mtxWorld._41;
				pOrbit->m_aPosPoint[nCntVtx][0].y = m_posOffset1.y + mtxWorld._42;
				pOrbit->m_aPosPoint[nCntVtx][0].z = m_posOffset1.z + mtxWorld._43;
				pOrbit->m_aPosPoint[nCntVtx][1].x = m_posOffset2.x + mtxWorld._41;
				pOrbit->m_aPosPoint[nCntVtx][1].y = m_posOffset2.y + mtxWorld._42;
				pOrbit->m_aPosPoint[nCntVtx][1].z = m_posOffset2.z + mtxWorld._43;
			}

			pOrbit->UpdatePolygon();
		}
	}

	return pOrbit;
}

//==========================================
// 位置設定処理
//==========================================
void COrbit::SetPositionOffset(D3DXMATRIX mtxWorld,int nIdxOrbit)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_aMtxOffset[nCntOffset]);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_posOffset[nCntOffset].x, m_posOffset[nCntOffset].y, m_posOffset[nCntOffset].z);
		D3DXMatrixMultiply(&m_aMtxOffset[nCntOffset], &m_aMtxOffset[nCntOffset], &mtxTrans);

		//マトリックスをかけ合わせる
		D3DXMatrixMultiply(&m_aMtxOffset[nCntOffset], &m_aMtxOffset[nCntOffset], &mtxWorld);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_aMtxOffset[nCntOffset]);
	}

	//ポリゴン更新処理
	UpdatePolygon();
}