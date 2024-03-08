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
#include "camera.h"

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
	m_fFactSB = 0.0f;
	m_pVtxBuff = nullptr;
	m_nIdxTexture = -1;
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
	SetVtx();
}

//=====================================================
// 頂点設定
//=====================================================
void CObject3D::SetVtx(void)
{
	switch (m_mode)
	{
	case CObject3D::MODE_NORMAL:

		SetVtxNormal();

		break;
	case CObject3D::MODE_BILLBOARD:

		SetVtxNormal();

		break;
	case CObject3D::MODE_STRETCHBILLBOARD:

		SetVtxStretchBillboard();

		break;
	default:
		break;
	}
}

//=====================================================
// 通常頂点設定
//=====================================================
void CObject3D::SetVtxNormal(void)
{
	if (m_pVtxBuff == nullptr)
	{
		return;
	}

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_mode == MODE_BILLBOARD)
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
// ストレッチビルボード頂点設定
//=====================================================
void CObject3D::SetVtxStretchBillboard(void)
{
	if (m_pVtxBuff == nullptr)
	{
		return;
	}

	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 vecFront;
	D3DXVECTOR3 vecRear;

	vecFront =
	{// 前方ベクトル
		sinf(m_rot.x + D3DX_PI * 0.5f) * sinf(m_rot.y) * m_heigth,
		cosf(m_rot.x + D3DX_PI * 0.5f) * m_heigth,
		sinf(m_rot.x + D3DX_PI * 0.5f) * cosf(m_rot.y) * m_heigth
	};

	// 後方ベクトル
	vecRear = -vecFront;

	vecFront += pos;
	vecRear += pos;

	// カメラからのベクトル取得
	D3DXVECTOR3 vecEye = pInfoCamera->posV - vecFront;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 vecPolygon = vecFront - vecRear;

	// 法線の算出
	D3DXVec3Cross(&nor, &vecEye, &vecPolygon);

	// 法線正規化
	D3DXVec3Normalize(&nor, &nor);

	// 横幅をずらす計算
	D3DXVECTOR3 vecSide;

	D3DXVec3Cross(&vecSide, &nor, &-vecPolygon);

	D3DXVec3Normalize(&vecSide, &vecSide);

	vecSide *= m_fFactSB;

	vecFront -= vecSide;
	vecRear += vecSide;

	//頂点座標の設定
	pVtx[1].pos = vecFront + nor * m_width;
	pVtx[3].pos = vecFront - nor * m_width;
	pVtx[0].pos = vecRear + nor * m_width;
	pVtx[2].pos = vecRear - nor * m_width;

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

	if (m_mode == MODE_BILLBOARD)
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

	if (m_mode == MODE_STRETCHBILLBOARD)
	{
		// カリングを無効化
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	if (m_mode == MODE_STRETCHBILLBOARD)
	{
		// カリングを有効化
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//=====================================================
// マトリックスの設定
//=====================================================
void CObject3D::SetMtx(void)
{
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 rot = m_rot;

	if (m_mode == MODE_STRETCHBILLBOARD)
	{
		pos = { 0.0f,0.0f,0.0f };
		rot = { 0.0f,0.0f,0.0f };
	}

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
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
// モードの設定
//=====================================================
void CObject3D::SetMode(MODE mode)
{
	if (mode < MODE_MAX &&
		mode >= 0)
	{
		m_mode = mode;
	}
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

	if (m_mode == MODE_BILLBOARD)
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