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
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_width = 0.0f;
	m_heigth = 0.0f;
	m_pVtxBuff = nullptr;
	m_nIdxTexture = -1;
	m_bBillboard = false;
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
	pVtx[0].pos = D3DXVECTOR3(-m_width, 0.0f, m_heigth);
	pVtx[1].pos = D3DXVECTOR3(m_width, 0.0f, m_heigth);
	pVtx[2].pos = D3DXVECTOR3(-m_width, 0.0f, -m_heigth);
	pVtx[3].pos = D3DXVECTOR3(m_width, 0.0f, -m_heigth);

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

}

//=====================================================
// 頂点設定
//=====================================================
void CObject3D::SetVtx(void)
{
	if (m_pVtxBuff == nullptr)
	{
		return;
	}

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bBillboard)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_width, m_heigth, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_width, m_heigth, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_width, -m_heigth, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_width, -m_heigth, 0.0f);
	}
	else
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_width, 0.0f, m_heigth);
		pVtx[1].pos = D3DXVECTOR3(m_width, 0.0f, m_heigth);
		pVtx[2].pos = D3DXVECTOR3(-m_width, 0.0f, -m_heigth);
		pVtx[3].pos = D3DXVECTOR3(m_width, 0.0f, -m_heigth);
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 描画処理
//=====================================================
void CObject3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bBillboard)
	{
		SetMtxBillboard();
	}
	else
	{
		SetMtx();
	}

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);
	pDevice->SetTexture(0, pTexture);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

//=====================================================
// マトリックスの設定
//=====================================================
void CObject3D::SetMtx(void)
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
// ビルボード版のマトリックスの設定
//=====================================================
void CObject3D::SetMtxBillboard(void)
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
// 生成処理
//=====================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		// インスタンス生成
		pObject3D = new CObject3D;

		// 初期化処理
		pObject3D->Init();

		// サイズ設定
		pObject3D->SetSize(500.0f, 500.0f);
	}

	return pObject3D;
}

//=====================================================
// サイズ設定処理
//=====================================================
void CObject3D::SetSize(float width, float height)
{
	if (m_pVtxBuff == nullptr)
	{
		return;
	}

	m_width = width;
	m_heigth = height;

	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bBillboard)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_width, m_heigth, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_width, m_heigth, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_width, -m_heigth, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_width, -m_heigth, 0.0f);
	}
	else
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_width, 0.0f, m_heigth);
		pVtx[1].pos = D3DXVECTOR3(m_width, 0.0f, m_heigth);
		pVtx[2].pos = D3DXVECTOR3(-m_width, 0.0f, -m_heigth);
		pVtx[3].pos = D3DXVECTOR3(m_width, 0.0f, -m_heigth);
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
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
// テクスチャ座標設定
//=====================================================
void CObject3D::SetTex(D3DXVECTOR2 rd, D3DXVECTOR2 lu)
{
	if (m_pVtxBuff == nullptr)
	{
		return;
	}

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(lu.x, lu.y);
	pVtx[1].tex = D3DXVECTOR2(rd.x, lu.y);
	pVtx[2].tex = D3DXVECTOR2(lu.x, rd.y);
	pVtx[3].tex = D3DXVECTOR2(rd.x, rd.y);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}