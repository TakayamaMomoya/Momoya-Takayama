//*****************************************************
//
// Xファイルの処理[objectX.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "texture.h"
#include "model.h"

//====================================================
// コンストラクタ
//====================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_nIdxModel = -1;
	m_fRadius = 0.0f;
	m_fScale = 0.0f;
	m_bChangeCol = false;
	m_bShadow = false;
	m_pModel = nullptr;
}

//====================================================
// デストラクタ
//====================================================
CObjectX::~CObjectX()
{

}

//====================================================
//初期化処理
//====================================================
HRESULT CObjectX::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	m_fScale = 1.0f;

	if (m_pModel != nullptr)
	{
		int nIdx = CModel::Load("data\\MODEL\\weapon\\minigun.x");

		// モデル読込
		BindModel(nIdx);
	}

	return S_OK;
}

//====================================================
//終了処理
//====================================================
void CObjectX::Uninit(void)
{
	// 自身の破棄
	Release();
}

//====================================================
//更新処理
//====================================================
void CObjectX::Update(void)
{

}

//====================================================
// 描画処理
//====================================================
void CObjectX::Draw(void)
{
	if (m_pModel != nullptr)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		if (m_bShadow)
		{
			D3DXMATRIX *pMtx = GetMatrix();
			D3DXMATRIX mtxShadow;
			D3DLIGHT9 light;
			D3DXVECTOR4 posLight;
			D3DXVECTOR3 normal;
			D3DXPLANE plane;

			// ライトの位置設定
			pDevice->GetLight(2, &light);
			posLight = { -light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f };

			// 平面情報の生成
			normal = { 0.0f,1.0f,0.0f };
			D3DXPlaneFromPointNormal(&plane, &D3DXVECTOR3(0.0f, 0.5f, 0.0f), &normal);

			// シャドウマトリックス初期化
			D3DXMatrixIdentity(&mtxShadow);

			// シャドウマトリックスの作成
			D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
			D3DXMatrixMultiply(&mtxShadow, pMtx, &mtxShadow);

			// シャドウマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

			DrawShadow();
		}

		// マトリックスの設定
		SetMatrix();

		D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
		D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
		LPDIRECT3DTEXTURE9 pTexture;

		// 現在のマテリアル取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_pModel->dwNumMat; nCntMat++)
		{
			// マテリアルの保存
			matDef = pMat[nCntMat].MatD3D;

			if (m_bChangeCol)
			{
				// 色の設定
				pMat[nCntMat].MatD3D.Diffuse = m_col;
			}

			// マテリアル設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの取得
			pTexture = CTexture::GetInstance()->GetAddress(m_pModel->pIdxTexture[nCntMat]);

			// テクスチャ設定
			pDevice->SetTexture(0, pTexture);

			// モデル（パーツ）描画
			m_pModel->pMesh->DrawSubset(nCntMat);

			// 色を戻す
			pMat[nCntMat].MatD3D = matDef;
		}

		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//====================================================
// 影用の描画処理
//====================================================
void CObjectX::DrawShadow(void)
{
	if (m_pModel == nullptr)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	LPDIRECT3DTEXTURE9 pTexture;

	// 現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModel->dwNumMat; nCntMat++)
	{
		// マテリアルの保存
		matDef = pMat[nCntMat].MatD3D;

		// 色の設定
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの取得
		pTexture = CTexture::GetInstance()->GetAddress(m_pModel->pIdxTexture[nCntMat]);

		//テクスチャ設定
		pDevice->SetTexture(0, pTexture);

		//モデル（パーツ）描画
		m_pModel->pMesh->DrawSubset(nCntMat);

		// 色を戻す
		pMat[nCntMat].MatD3D = matDef;

		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=====================================================
// 描画のみ行う
//=====================================================
void CObjectX::JustDraw(void)
{
	if (m_pModel != nullptr)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		// ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
		D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
		LPDIRECT3DTEXTURE9 pTexture;

		// 現在のマテリアル取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_pModel->dwNumMat; nCntMat++)
		{
			// マテリアルの保存
			matDef = pMat[nCntMat].MatD3D;

			if (m_bChangeCol)
			{
				// 色の設定
				pMat[nCntMat].MatD3D.Diffuse = m_col;
			}

			// マテリアル設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの取得
			pTexture = CTexture::GetInstance()->GetAddress(m_pModel->pIdxTexture[nCntMat]);

			// テクスチャ設定
			pDevice->SetTexture(0, pTexture);

			// モデル（パーツ）描画
			m_pModel->pMesh->DrawSubset(nCntMat);

			// 色を戻す
			pMat[nCntMat].MatD3D = matDef;
		}

		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=====================================================
// マトリックス設定処理
//=====================================================
void CObjectX::SetMatrix(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//変数宣言
	D3DXMATRIX mtxRot, mtxTrans;
	
	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケール反映
	D3DXMatrixScaling(&m_mtxWorld, m_fScale, m_fScale, m_fScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
//生成処理
//=====================================================
CObjectX *CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObjectX *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{
		// インスタンス生成
		pObjectX = new CObjectX;

		pObjectX->m_pos = pos;
		pObjectX->m_rot = rot;

		// 初期化処理
		pObjectX->Init();
	}

	return pObjectX;
}

//=====================================================
// モデルの最長頂点取得
//=====================================================
void CObjectX::SetRadius(void)
{
	//計算用変数
	int nNumVtx;			//頂点数
	DWORD dwSizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ
	float fLength = 0.0f;
	D3DXVECTOR3 vtxMin = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vtxMax = { 0.0f,0.0f,0.0f };

	//頂点数の取得
	nNumVtx = m_pModel->pMesh->GetNumVertices();

	//フォーマットサイズ入手
	dwSizeFVF = D3DXGetFVFVertexSize(m_pModel->pMesh->GetFVF());

	//頂点バッファのロック
	m_pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x > vtxMax.x)
		{//Xの最大値
			vtxMax.x = vtx.x;
		}
		else if (vtx.x < vtxMin.x)
		{//Xの最小値
			vtxMin.x = vtx.x;
		}

		if (vtx.z > vtxMax.z)
		{//Zの最大値
			vtxMax.z = vtx.z;
		}
		else if (vtx.z < vtxMin.z)
		{//Zの最小値
			vtxMin.z = vtx.z;
		}

		if (vtx.y > vtxMax.y)
		{//Yの最大値
			vtxMax.y = vtx.y;
		}
		else if (vtx.y < vtxMin.y)
		{//Yの最小値
			vtxMin.y = vtx.y;
		}

		if (D3DXVec3Length(&vtx) > fLength)
		{
			fLength = D3DXVec3Length(&vtx);
		}

		pVtxBuff += dwSizeFVF;
	}

	//最大・最小頂点設定
	m_vtxMax = vtxMax;
	m_vtxMin = vtxMin;

	//頂点バッファのアンロック
	m_pModel->pMesh->UnlockVertexBuffer();

	// 長さを代入
	m_fRadius = fLength;
}

//=====================================================
// モデルの割り当て
//=====================================================
void CObjectX::BindModel(int nIdx)
{
	// モデル割り当て
	m_pModel = CModel::GetModel(nIdx);

	// 番号記憶
	m_nIdxModel = nIdx;

	// 半径設定
	SetRadius();
}