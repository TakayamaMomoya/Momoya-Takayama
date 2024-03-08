//*****************************************************
//
// メッシュフィールドの処理[meshfield.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "player.h"
#include "debugproc.h"
#include "game.h"
#include <stdio.h>
#include "effect3D.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\MATERIAL\\field01.jpg"				// テクスチャファイル名
#define MOVE_SPEED					(1.0f)										// 移動速度
#define MESH_LENGTH					(300.0f)									// メッシュの一辺の長さ
#define MESH_U						(254)											// 横のブロック数
#define MESH_V						(254)											// 縦のブロック数
#define SPLIT_TEX					(10)										// テクスチャ分割数
#define CHENGE_LENGTH	(10000)	// 操作できる頂点までの距離
#define ANGLE_SLIP	(0.7f)	// 坂を滑る角度
#define CMP_LENGTH	(1000.0f)	// 判定する半径

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CMeshField *CMeshField::m_pMeshField = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CMeshField::CMeshField()
{
	ZeroMemory(&m_MeshField, sizeof(m_MeshField));
	m_pIdxBuff = nullptr;
	m_nIdxTexture = -1;
	m_pVtxBuff = nullptr;
	m_fLengthMesh = 0.0f;
	m_nDivNumU = 0;
	m_nDivNumV = 0;
	m_col = { 0.0f,0.0f,0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CMeshField::~CMeshField()
{

}

//=====================================================
// 生成処理
//=====================================================
CMeshField *CMeshField::Create(void)
{
	if (m_pMeshField == nullptr)
	{
		m_pMeshField = new CMeshField;

		if (m_pMeshField != nullptr)
		{
			m_pMeshField->Init();
		}
	}

	return m_pMeshField;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMeshField::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 値の初期設定
	m_fLengthMesh = MESH_LENGTH;
	m_nDivNumU = MESH_U;
	m_nDivNumV = MESH_V;
	m_col = { 1.0f,1.0f,1.0f,1.0f };

	// 読込処理
	Load("data\\MAP\\field00.bin");

	//Reset();

	m_nDivTex = SPLIT_TEX;

	// テクスチャの読込
	m_nIdxTexture = CTexture::GetInstance()->Regist(MESHFIELD_TEX_FILE);

	// 頂点数計算
	m_MeshField.nNumVtx = (m_nDivNumU + 1) * (m_nDivNumV + 1);

	if (m_pVtxBuff == nullptr)
	{
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshField.nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCountV, nCountU;
	int nCntVtx = 0;

	for (nCountV = 0;nCountV < m_nDivNumV + 1; nCountV++)
	{// 頂点座標の設定
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++)
		{
			// 頂点座標
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x = (nCountU - m_nDivNumU * 0.5f) * m_fLengthMesh;
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.z = (((m_nDivNumU) * 0.5f) - nCountV) * m_fLengthMesh;

			// テクスチャ座標
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].tex = D3DXVECTOR2
			(
			    ((float)m_nDivTex / (float)m_nDivNumU) * nCountU,
				((float)m_nDivTex / (float)m_nDivNumU) * nCountV
			);
		}
	}

	for (int nCnt = 0;nCnt < m_MeshField.nNumVtx;nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// 法線の設定
	SetNormal();

	m_MeshField.nNumIdx = (m_nDivNumU + 1) * (m_nDivNumV + 1) + ((m_nDivNumV + 1) - 2) * (m_nDivNumU + 1) + (((m_nDivNumV + 1) - 2) * 2);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshField.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	// インデックスバッファへのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、頂点番号へのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCount = 0; nCount < m_MeshField.nNumIdx / 2; nCount++)
	{// インデックス決定
		if (
			nCount % ((m_nDivNumU + 1) + (m_nDivNumU + 2) * (nCount / (m_nDivNumU + 2))) == 0
			&& nCount != 0
			)
		{
			pIdx[nCount * 2 + 1] = ((m_nDivNumU + 1) * 2) + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = m_nDivNumU + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));
		}
		else
		{
			pIdx[nCount * 2 + 1] = nCount - (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = (nCount - (nCount / (m_nDivNumU + 2))) + (m_nDivNumU + 1);
		}
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMeshField::Uninit(void)
{
	m_pMeshField = nullptr;

	if (m_pVtxBuff != nullptr)
	{// 頂点バッファポインタの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{// インデックスバッファポインタの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMeshField::Update(void)
{
	Edit();
}

//=====================================================
// エディットの処理
//=====================================================
void CMeshField::Edit(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	float fLength;
	D3DXVECTOR3 vecDiff;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 入力情報入手
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard->GetPress(DIK_UP))
	{
		for (int nCntVtx = 0; nCntVtx < m_MeshField.nNumVtx; nCntVtx++)
		{
			vecDiff = pPlayer->GetPosition() - pVtx[nCntVtx].pos;

			fLength = D3DXVec3Length(&vecDiff);

			if (fLength < CHENGE_LENGTH)
			{
				pVtx[nCntVtx].pos.y += 20.0f * (1.0f - (fLength / CHENGE_LENGTH));
			}
		}
	}

	if (pKeyboard->GetPress(DIK_DOWN))
	{
		for (int nCntVtx = 0; nCntVtx < m_MeshField.nNumVtx; nCntVtx++)
		{
			vecDiff = pPlayer->GetPosition() - pVtx[nCntVtx].pos;

			fLength = D3DXVec3Length(&vecDiff);

			if (fLength < CHENGE_LENGTH)
			{
				pVtx[nCntVtx].pos.y -= 20.0f * (1.0f - (fLength / CHENGE_LENGTH));
			}
		}
	}

	/*if (pKeyboard->GetPress(DIK_F))
	{
		m_col.r += 0.01f;
	}
	if (pKeyboard->GetPress(DIK_V))
	{
		m_col.r -= 0.01f;
	}

	if (pKeyboard->GetPress(DIK_G))
	{
		m_col.g += 0.01f;
	}
	if (pKeyboard->GetPress(DIK_B))
	{
		m_col.g -= 0.01f;
	}

	if (pKeyboard->GetPress(DIK_H))
	{
		m_col.b += 0.01f;
	}
	if (pKeyboard->GetPress(DIK_N))
	{
		m_col.b -= 0.01f;
	}*/

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	if (pKeyboard->GetTrigger(DIK_RIGHT))
	{
		// 法線の設定
		SetNormal();
	}

	if (pKeyboard->GetTrigger(DIK_F8))
	{// 保存処理
		Save();
	}

	if (pKeyboard->GetTrigger(DIK_F7))
	{// 読込処理
		Reset();
	}
}

//=====================================================
// 法線の設定
//=====================================================
void CMeshField::SetNormal(void)
{
	D3DXVECTOR3 vec1, vec2, nor1, nor2, nor3, nor4, nor5, nor6, nor7;
	int nCountU, nCountV, nCntVtx = 0;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;
	}

	for (nCountV = 0; nCountV < m_nDivNumV + 1; nCountV++)
	{// 法線の設定
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++, nCntVtx++)
		{
			if (nCntVtx == 0)
			{// 最初の場合================================================
			 // 下と右の法線を求め、足して割る
				// 左下の法線を求める
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 右の法線を求める
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// 法線を求める
				pVtx[nCntVtx].nor = (nor1 + nor2) * 0.5f;
			}
			else if (nCntVtx == m_MeshField.nNumVtx - 1)
			{// 最後の場合================================================
			 // 左と上の法線を求め、足して割る
			 // 左下の法線を求める
				vec1 = pVtx[nCntVtx - m_nDivNumU - 2].pos - pVtx[nCntVtx - 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx - 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 右の法線を求める
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx - m_nDivNumU - 1].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 2].pos - pVtx[nCntVtx - m_nDivNumU - 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// 法線を求める
				pVtx[nCntVtx].nor = (nor1 + nor2) * 0.5f;
			}
			else if (nCntVtx == m_nDivNumU + 1)
			{// 右上の面一つ================================================
				vec1 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 法線を求める
				pVtx[nCntVtx].nor = nor1;
			}
			else if (nCntVtx == m_nDivNumU * m_nDivNumV + m_nDivNumU)
			{// 左下の面一つ================================================
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 法線を求める
				pVtx[nCntVtx].nor = nor1;
			}
			else if (nCntVtx < m_nDivNumU)
			{// 上の辺で面が三つ================================================
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 右の法線を求める
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// 隣の法線をもらう
				nor3 = pVtx[nCntVtx - 1].nor;

				// 法線を求める
				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3) / 3;
			}
			else if (nCntVtx % m_nDivNumU + 1 == 0)
			{// 左の辺で面が三つ================================================
			 // 上の法線をもらう
				nor1 = pVtx[nCntVtx - m_nDivNumU - 1].nor;
				// 右の法線は上と同じ
				nor2 = nor1;

				// 下の法線を求める
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor3, &vec1, &vec2);

				D3DXVec3Normalize(&nor3, &nor3);

				// 右下の法線を求める
				nor4 = (nor3 + nor2) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor4) / 4;
			}
			else if (nCntVtx == m_nDivNumU + nCountV * (m_nDivNumU + 1))
			{// 右の辺で面三つ================================================
			 // 左上の法線をもらう
				nor1 = pVtx[nCntVtx - m_nDivNumU - 2].nor;
				// 上の法線をもらう
				nor2 = pVtx[nCntVtx - m_nDivNumU - 1].nor;
				// 左の法線をもらう
				nor3 = pVtx[nCntVtx - 1].nor;

				// 右上の法線を求める
				vec1 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// 左下の法線を求める
				vec1 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx + m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				// 下の法線を求める
				nor6 = (nor4 + nor5) / 2;

				// 法線を求める
				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor6) / 4;
			}
			else if (nCntVtx > m_nDivNumU * m_nDivNumV + m_nDivNumU)
			{// 下の辺で面三つ================================================
			 // 左の法線をもらう
				nor1 = pVtx[nCntVtx - 1].nor;
				// 左上の法線をもらう
				nor2 = pVtx[nCntVtx - m_nDivNumU - 2].nor;

				// 右上の法線を求める
				vec1 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// 左下の法線を求める
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor3 = (nor4 + nor5) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3) / 3;
			}
			else
			{// 面が六つの場合================================================
			 // 左の法線をもらう
				nor1 = pVtx[nCntVtx - 1].nor;
				// 左上の法線をもらう
				nor2 = pVtx[nCntVtx - m_nDivNumU - 2].nor;
				// 上の法線をもらう
				nor3 = pVtx[nCntVtx - m_nDivNumU - 1].nor;

				// 右上の四角形の法線を求める
				// 右上の法線を求める
				vec1 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// 左下の法線を求める
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor6 = (nor5 + nor4) / 2;

				// 左下の四角形の法線を求める
				// 右上の法線を求める
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// 左下の法線を求める
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor7 = (nor4 + nor5) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor4 + nor5 + nor7) / 6;
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 高さの取得
//=====================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor)
{
	if (m_pVtxBuff == nullptr)
	{
		return pos.y;
	}

	// 変数宣言
	D3DXVECTOR3 vec1, vec2, vec3, vec4, vec5, nor, vec6;
	D3DXVECTOR3 vecDiff;
	int nRu, nRd, nLu, nLd;
	float fHeight = pos.y;
	int nCntV, nCntU;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	nCntU = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x) / (int)m_fLengthMesh;
	nCntV = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z) / (int)m_fLengthMesh;

	// 頂点番号計算
	nLd = (m_nDivNumU + 1) * (nCntV + 1) + nCntU;
	nLu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU + 1) + nCntU;
	nRd = (m_nDivNumU + 1) * (nCntV + 1) + 1 + nCntU;
	nRu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU)+nCntU;

	if (pos.x < -m_fLengthMesh * m_nDivNumU / 2 || 
		pos.x > m_fLengthMesh * m_nDivNumU / 2 ||
		pos.z < -m_fLengthMesh * m_nDivNumU / 2 ||
		pos.z > m_fLengthMesh * m_nDivNumU / 2)
	{
		return 0.0f;
	}

	// 差分を取得
	vecDiff = pVtx[nLd].pos - pos;

	// 左上へのベクトル算出
	vec1 = pVtx[nLu].pos - pVtx[nLd].pos;
	// 右下へのベクトル算出
	vec2 = pVtx[nRd].pos - pVtx[nLd].pos;

	// 右下へのベクトル算出
	vec3 = pVtx[nRd].pos - pVtx[nRu].pos;
	// 左上へのベクトル算出
	vec4 = pVtx[nLu].pos - pVtx[nRu].pos;

	// 中心のベクトル算出
	vec5 = pVtx[nLu].pos - pVtx[nRd].pos;

	if ((vec5.z * (pos.x - pVtx[nLu].pos.x)) - (vec5.x * (pos.z - pVtx[nLu].pos.z)) < 0)
	{// 左下のポリゴンの上にいる
	 // 法線の算出
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// 法線正規化
		D3DXVec3Normalize(&nor, &nor);

		// 内積計算
		D3DXVECTOR3 vec = { -(pos.x - pVtx[nLd].pos.x) * nor.x,0.0f,-(pos.z - pVtx[nLd].pos.z) * nor.z };

		if (nor.y != 0)
		{// 高さの取得
			fHeight = (vec.x + vec.z + pVtx[nLd].pos.y * nor.y) / nor.y;

			if (pNor != nullptr)
			{
				*pNor = nor;
			}
		}
	}
	else
	{// 右上のポリゴンの上にいる
	 // 法線の算出
		D3DXVec3Cross(&nor, &vec3, &vec4);

		// 法線正規化
		D3DXVec3Normalize(&nor, &nor);

		// 内積計算
		D3DXVECTOR3 vec = { -(pos.x - pVtx[nRu].pos.x) * nor.x,0.0f,-(pos.z - pVtx[nRu].pos.z) * nor.z };

		if (nor.y != 0)
		{// 高さの取得
			fHeight = (vec.x + vec.z + pVtx[nRu].pos.y * nor.y) / nor.y;

			if (pNor != nullptr)
			{
				*pNor = nor;
			}
		}
	}

	if (pMove != nullptr)
	{
		if (nor.y * nor.y < ANGLE_SLIP * ANGLE_SLIP)
		{
			*pMove += nor * 0.5f;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=====================================================
// へこむ処理
//=====================================================
void CMeshField::Dent(D3DXVECTOR3 pos,float fRadius,float fDepth)
{
	// 計算用変数
	D3DXVECTOR3 vecDiff;
	float fLength;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 差分の長さを取得
		vecDiff = pos - pVtx[nCnt].pos;
		fLength = D3DXVec3Length(&vecDiff);

		if (fLength < fRadius)
		{
			pVtx[nCnt].pos.y -= fDepth - (fDepth * (fLength / fRadius));
		}
	}

	// 法線設定
	SetNormal();

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// レイで当たっているかのチェック
//=====================================================
bool CMeshField::RayCheck(D3DXVECTOR3 pos, D3DXVECTOR3 vecDir, D3DXVECTOR3 posOwn)
{
	int nCntV, nCntU;
	int nCntVStart, nCntUStart;
	int nCntVEnd, nCntUEnd;
	int nLd, nLu, nRd, nRu;
	int nTemp;
	float fLength = 0.0f;
	BOOL bLd,bRu;
	D3DXVECTOR3 vecDiff = vecDir;

	bool bHit = false;
	pos.y += 20;

	// 計算する範囲の設定
	nCntUStart = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x) / (int)m_fLengthMesh;
	nCntVStart = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z) / (int)m_fLengthMesh;

	nCntUEnd = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x + vecDir.x) / (int)m_fLengthMesh;
	nCntVEnd = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z - vecDir.z) / (int)m_fLengthMesh;

	if (nCntUEnd < nCntUStart)
	{
		nTemp = nCntUEnd;

		nCntUEnd = nCntUStart;

		nCntUStart = nTemp;
	}

	if (nCntVEnd < nCntVStart)
	{
		nTemp = nCntVEnd;

		nCntVEnd = nCntVStart;

		nCntVStart = nTemp;
	}

	// 向きの正規化
	D3DXVec3Normalize(&vecDir, &vecDir);

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntV = nCntVStart; nCntV < nCntVEnd; nCntV++)
	{// 全ポリゴンのチェック
		for (nCntU = nCntUStart; nCntU < nCntUEnd; nCntU++)
		{
			// 頂点番号計算
			nLd = (m_nDivNumU + 1) * (nCntV + 1) + nCntU;
			nLu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU + 1) + nCntU;
			nRd = (m_nDivNumU + 1) * (nCntV + 1) + 1 + nCntU;
			nRu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU) + nCntU;

			bLd = D3DXIntersectTri(&pVtx[nLd].pos, &pVtx[nLu].pos, &pVtx[nRd].pos, &pos, &vecDir, nullptr, nullptr, &fLength);
			bRu = D3DXIntersectTri(&pVtx[nLu].pos, &pVtx[nRu].pos, &pVtx[nRd].pos, &pos, &vecDir, nullptr, nullptr, &fLength);

			// 交差しているかチェック
			if (bLd || bRu)
			{
				if (D3DXVec3Length(&vecDiff) > fLength)
				{
					return true;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return bHit;
}

//=====================================================
// 色の変更
//=====================================================
void CMeshField::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色の変更
	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 高さのリセット
//=====================================================
void CMeshField::Reset(void)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色の変更
	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].pos.y = 0.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 描画処理
//=====================================================
void CMeshField::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	// 入力情報入手
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_MeshField.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll (&mtxRot,
		m_MeshField.rot.y, m_MeshField.rot.x, m_MeshField.rot.z);
	D3DXMatrixMultiply(&m_MeshField.mtxWorld, &m_MeshField.mtxWorld,&mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_MeshField.pos.x, m_MeshField.pos.y, m_MeshField.pos.z);
	D3DXMatrixMultiply(&m_MeshField.mtxWorld, &m_MeshField.mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD,&m_MeshField.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	
	// テクスチャ設定
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);
	pDevice->SetTexture(0, pTexture);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive (D3DPT_TRIANGLESTRIP, 0, 0, m_MeshField.nNumVtx, 0, m_MeshField.nNumIdx - 2);
}

//=====================================================
// 読込処理
//=====================================================
void CMeshField::Load(std::string path)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen((const char*)&path[0], "rb");

	if (pFile != nullptr)
	{
		fread(&m_nDivNumU, sizeof(int), 1, pFile);
		fread(&m_nDivNumV, sizeof(int), 1, pFile);
		fread(&m_nDivTex, sizeof(int), 1, pFile);
		fread(&m_fLengthMesh, sizeof(float), 1, pFile);

		// 頂点情報のポインタ
		VERTEX_3D *pVtx;

		m_MeshField.nNumVtx = (m_nDivNumU + 1) * (m_nDivNumV + 1);

		if (m_pVtxBuff == nullptr)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshField.nNumVtx * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				nullptr);
		}

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点の位置情報を保存
		fread(&pVtx[0].pos, sizeof(D3DXVECTOR3), m_MeshField.nNumVtx * 4, pFile);

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		// ファイルを閉じる
		fclose(pFile);
	}

}

//=====================================================
// 保存処理
//=====================================================
void CMeshField::Save(void)
{
	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen("data\\MAP\\field00.bin", "wb");

	if (pFile != nullptr)
	{
		fwrite(&m_nDivNumU, sizeof(int), 1, pFile);
		fwrite(&m_nDivNumV, sizeof(int), 1, pFile);
		fwrite(&m_nDivTex, sizeof(int), 1, pFile);
		fwrite(&m_fLengthMesh, sizeof(float), 1, pFile);

		// 頂点情報のポインタ
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点の位置情報を保存
		fwrite(&pVtx[0].pos, sizeof(D3DXVECTOR3),(m_nDivNumU + 1) * (m_nDivNumV + 1) * 4, pFile);

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		// ファイルを閉じる
		fclose(pFile);
	}
}