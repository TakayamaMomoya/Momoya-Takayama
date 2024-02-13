//*****************************************************
//
// ３Dポリゴンの処理[object3D.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"

//=====================================================
// コンストラクタ
//=====================================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_width = 0.0f;
	m_heigth = 0.0f;
	m_pVtxBuff = nullptr;
	m_nIdxTexture = -1;
	m_bBillboard = false;
	m_bZTest = false;
	m_bAdd = false;
}

//=====================================================
// デストラクタ
//=====================================================
CObject3D::~CObject3D()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CObject3D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (m_pVtxBuff == nullptr)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_width, m_heigth, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_width, m_heigth, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_width, -m_heigth, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_width, -m_heigth, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f, 1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f, 1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f, 1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CObject3D::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{//頂点バッファポインタの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CObject3D::Update(void)
{
	if (m_pVtxBuff != nullptr)
	{
		//頂点情報のポインタ
		VERTEX_3D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_width, m_heigth, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_width, m_heigth, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_width, -m_heigth, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_width, -m_heigth, 0.0f);

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CObject3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (m_bAdd)
	{// 加算合成に設定する
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	if (m_bBillboard)
	{
		DrawBillboard();
	}
	else
	{
		DrawNormal();
	}

	if (m_bAdd)
	{// 加算合成を戻す
		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=====================================================
// 通常の描画
//=====================================================
void CObject3D::DrawNormal(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

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

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);
	pDevice->SetTexture(0, pTexture);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================================
// ビルボード描画
//=====================================================
void CObject3D::DrawBillboard(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;

	//ライティング無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_bZTest)
	{
		//Zテストの無効化
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	if (CRenderer::IsFog())
	{
		// フォグを無効化
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

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

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);
	pDevice->SetTexture(0, pTexture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ライティング有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_bZTest)
	{
		//Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	if (CRenderer::IsFog())
	{
		// フォグを有効化
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}
}

//=====================================================
// 生成処理
//=====================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		// インスタンス生成
		pObject3D = new CObject3D;

		// サイズ設定
		pObject3D->SetSize(500.0f,500.0f);

		// 初期化処理
		pObject3D->Init();
	}

	return pObject3D;
}

//=====================================================
// サイズ設定処理
//=====================================================
void CObject3D::SetSize(float width, float height)
{
	m_width = width;
	m_heigth = height;
}

//=====================================================
// 色設定処理
//=====================================================
void CObject3D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// テクスチャ座標設定処理
//=====================================================
void CObject3D::SetTex(D3DXVECTOR2 texLeftUp, D3DXVECTOR2 texRightDown)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

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