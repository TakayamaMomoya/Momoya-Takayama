//*****************************************************
//
//meshcylinder.cpp
//Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "debugproc.h"
#include "universal.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MESHCYLINDER_TEX_FILE		"data\\TEXTURE\\EFFECT\\boost.png"				// テクスチャファイル名
#define MESH_LENGTH					(10.0f)									// メッシュの一辺の長さ
#define MESH_U						(256)										// 横のブロック数
#define MESH_V						(1)											// 縦のブロック数
#define SPLIT_TEX_U					(3)											// 横のテクスチャ分割数
#define SPLIT_TEX_V					(1)											// 縦のテクスチャ分割数
#define MESH_HEIGHT	(50.0f)	// メッシュの高さ
#define ADD_HEIGHT	(100.0f)	// 目標からの高さ
#define WAVE_ANGLE	(D3DX_PI * 0.1f)	// 波打つ範囲の角度
#define WAVE_HEIGHT	(50.0f)	// 波打つ高さ
#define WAVE_LENGTH	(100.0f)	// 波打つ範囲の距離

//=====================================================
// コンストラクタ
//=====================================================
CMeshCylinder::CMeshCylinder(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_meshCylinder, sizeof(m_meshCylinder));
	m_pIdxBuff = nullptr;
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_col = { 0.0f,0.0f,0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CMeshCylinder::~CMeshCylinder()
{

}

//=====================================================
// 生成処理
//=====================================================
CMeshCylinder *CMeshCylinder::Create(void)
{
	CMeshCylinder *pMeshCylinder = nullptr;

	if (pMeshCylinder == nullptr)
	{
		pMeshCylinder = new CMeshCylinder;

		if (pMeshCylinder != nullptr)
		{
			pMeshCylinder->Init();
		}
	}

	return pMeshCylinder;
}

//=====================================================
//初期化処理
//=====================================================
HRESULT CMeshCylinder::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESH_U + 1) * (MESH_V + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((MESH_U + 1) * (MESH_V + 1) + ((MESH_V + 1) - 2) * (MESH_U + 1) + (((MESH_V + 1) - 2) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// テクスチャの読込
	D3DXCreateTextureFromFile
	(pDevice, MESHCYLINDER_TEX_FILE, &m_pTexture);

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファへのポインタ
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号へのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//最大頂点数計算
	m_meshCylinder.nNumVtx = (MESH_U + 1) * (MESH_V + 1);

	//変数初期化
	m_meshCylinder.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//計算用変数
	float fRot;
	D3DXVECTOR3 VecRot;

	//頂点情報==================================================================================
	for (int nCountV = 0; nCountV < MESH_V + 1; nCountV++)
	{//頂点座標の設定
		for (int nCountU = 0; nCountU < MESH_U + 1; nCountU++)
		{
			//角度算出
			fRot = nCountU * (D3DX_PI / MESH_U) * 2;

			pVtx[nCountV * (MESH_U + 1) + nCountU].pos.x = (float)sin(fRot) * MESH_LENGTH;
			pVtx[nCountV * (MESH_U + 1) + nCountU].pos.y = (MESH_V - nCountV) * MESH_HEIGHT;
			pVtx[nCountV * (MESH_U + 1) + nCountU].pos.z = cosf(fRot) * MESH_LENGTH;

			//テクスチャ座標
			pVtx[nCountV * (MESH_U + 1) + nCountU].tex = D3DXVECTOR2
			(
				((float)SPLIT_TEX_U / MESH_U) * nCountU,
				((float)SPLIT_TEX_V / MESH_V) * nCountV
			);

			VecRot = D3DXVECTOR3
			(
				pVtx[nCountV * (MESH_U + 1) + nCountU].pos.x,
				0.0f,
				pVtx[nCountV * (MESH_U + 1) + nCountU].pos.z
			);

			//ベクトル正規化
			D3DXVec3Normalize(&VecRot, &VecRot);

			//法線ベクトルの設定
			pVtx[nCountV * (MESH_U + 1) + nCountU].nor = VecRot;
		}
	}

	for (int nCnt = 0; nCnt < m_meshCylinder.nNumVtx; nCnt++)
	{
		//頂点カラーの設定
		pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//インデックス==================================================================================
	m_meshCylinder.nNumIdx = (MESH_U + 1) * (MESH_V + 1) + ((MESH_V + 1) - 2) * (MESH_U + 1) + (((MESH_V + 1) - 2) * 2);

	for (int nCount = 0; nCount < m_meshCylinder.nNumIdx / 2; nCount++)
	{//インデックス決定
		if (
			nCount % ((MESH_U + 1) + (MESH_U + 2) * (nCount / (MESH_U + 2))) == 0
			&& nCount != 0
			)
		{
			pIdx[nCount * 2 + 1] = ((MESH_U + 1) * 2) + (MESH_U + 1) * (nCount / (MESH_U + 2));

			pIdx[nCount * 2] = MESH_U + (MESH_U + 1) * (nCount / (MESH_U + 2));
		}
		else
		{
			pIdx[nCount * 2 + 1] = nCount - (nCount / (MESH_U + 2));

			pIdx[nCount * 2] = (nCount - (nCount / (MESH_U + 2))) + (MESH_U + 1);
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMeshCylinder::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pVtxBuff != NULL)
	{//頂点バッファポインタの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pIdxBuff != NULL)
	{//インデックスバッファポインタの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMeshCylinder::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CMeshCylinder::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングを無効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_meshCylinder.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_meshCylinder.rot.y, m_meshCylinder.rot.x, m_meshCylinder.rot.z);
	D3DXMatrixMultiply(&m_meshCylinder.mtxWorld, &m_meshCylinder.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_meshCylinder.pos.x, m_meshCylinder.pos.y, m_meshCylinder.pos.z);
	D3DXMatrixMultiply(&m_meshCylinder.mtxWorld, &m_meshCylinder.mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_meshCylinder.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_meshCylinder.nNumVtx,								//頂点数
		0,													//最初のインデックス
		m_meshCylinder.nNumIdx - 2							//ポリゴン数
	);

	// カリングを有効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}