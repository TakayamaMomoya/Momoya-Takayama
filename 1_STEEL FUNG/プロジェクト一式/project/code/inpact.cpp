//*****************************************************
//
// 衝撃波の処理[inpact.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "inpact.h"
#include "manager.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int MESH_U = 32;	// 横の分割数
const int MESH_V = 1;	// 縦の分割数
const float MESH_HEIGHT = 50.0f;	// メッシュの高さ
const float INITIAL_EXPAND = 15.0f;	// 初期の膨らむ速度
const char* TEX_PATH = "data\\TEXTURE\\EFFECT\\inpact.png";	// テクスチャパス
}

//=====================================================
// コンストラクタ
//=====================================================
CInpact::CInpact(int nPriority) : CMeshCylinder(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CInpact::~CInpact()
{

}

//=====================================================
// 生成処理
//=====================================================
CInpact *CInpact::Create(float nLife, D3DXMATRIX *pMtx)
{
	CInpact *pInpact = nullptr;

	if (pInpact == nullptr)
	{
		pInpact = new CInpact;

		if (pInpact != nullptr)
		{
			if (pMtx != nullptr)
			{
				pInpact->SetMtx(*pMtx);
			}

			pInpact->m_info.fLife = nLife;
			pInpact->m_info.fLifeInitial = nLife;

			pInpact->Init();
		}
	}

	return pInpact;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CInpact::Init(void)
{
	// 分割数設定
	SetNumMeshU(MESH_U);
	SetNumMeshV(MESH_V);

	// 継承クラスの初期化
	CMeshCylinder::Init();

	m_info.fRadiusDiff = 40.0f;
	m_info.fSpeedExpand = INITIAL_EXPAND;

	// 頂点位置設定
	SetVtx();

	// テクスチャの読込
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture != nullptr)
	{
		int nIdx = pTexture->Regist(TEX_PATH);
		SetIdxTexture(nIdx);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CInpact::Uninit(void)
{
	CMeshCylinder::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CInpact::Update(void)
{
	CMeshCylinder::Update();

	// 体力管理
	ManageLife();

	// 頂点設定
	SetVtx();
}

//=====================================================
// 頂点位置の設定
//=====================================================
void CInpact::SetVtx(void)
{
	// 情報取得
	MeshCylinder *pMesh = GetMeshCylinder();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pMesh == nullptr || pVtxBuff == nullptr)
	{
		return;
	}

	int nMeshU = pMesh->nMeshU;
	int nMeshV = pMesh->nMeshV;
	float fRadius = pMesh->fRadius;
	float fHeight = pMesh->fHeight;

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//計算用変数
	float fRot;
	D3DXVECTOR3 VecRot;

	//頂点情報==================================================================================
	for (int nCountV = 0; nCountV < nMeshV + 1; nCountV++)
	{//頂点座標の設定
		// 半径の設定
		if (nCountV == 0)
		{
			fRadius -= m_info.fRadiusDiff;
		}
		else
		{
			fRadius += m_info.fRadiusDiff;
		}

		for (int nCountU = 0; nCountU < nMeshU + 1; nCountU++)
		{
			//角度算出
			fRot = nCountU * (D3DX_PI / nMeshU) * 2;

			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x = (float)sin(fRot) * fRadius;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.y = (nMeshV - nCountV) * fHeight;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z = cosf(fRot) * fRadius;

			VecRot = D3DXVECTOR3
			(
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x,
				0.0f,
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z
			);

			//ベクトル正規化
			D3DXVec3Normalize(&VecRot, &VecRot);

			//法線ベクトルの設定
			pVtx[nCountV * (nMeshU + 1) + nCountU].nor = VecRot;
		}
	}

	//頂点バッファをアンロック
	pVtxBuff->Unlock();
}

//=====================================================
// ライフの管理
//=====================================================
void CInpact::ManageLife(void)
{
	// 半径取得
	MeshCylinder *pMesh = GetMeshCylinder();

	if (pMesh == nullptr)
	{
		return;
	}

	float fRadius = pMesh->fRadius;

	// 半径を加算する
	fRadius += m_info.fSpeedExpand;
	SetRadius(fRadius);

	// 寿命を減らす
	float fDeltaTime = CManager::GetDeltaTime();

	m_info.fLife -= fDeltaTime;

	// 色の設定
	float fRate = m_info.fLife / m_info.fLifeInitial;

	D3DXCOLOR col = GetCol();

	col.a = fRate;

	SetCol(col);

	if (m_info.fLife <= 0.0f)
	{
		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CInpact::Draw(void)
{
	CMeshCylinder::JustDraw();
}