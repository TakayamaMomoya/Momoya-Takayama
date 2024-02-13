//*****************************************
//
//いろいろ汎用処理詰め込み
//Author:髙山桃也
//
//*****************************************

//*****************************************
//インクルード
//*****************************************
#include "manager.h"
#include "renderer.h"
#include "universal.h"

//*****************************************
// 静的メンバ変数宣言
//*****************************************
CUniversal *CUniversal::m_pUniversal = nullptr;	// 自身のポインタ

//=========================================
// コンストラクタ
//=========================================
CUniversal::CUniversal()
{

}

//=========================================
// デストラクタ
//=========================================
CUniversal::~CUniversal()
{

}

//=========================================
// 生成処理
//=========================================
CUniversal *CUniversal::Create(void)
{
	if (m_pUniversal == nullptr)
	{
		m_pUniversal = new CUniversal;
	}

	return m_pUniversal;
}

//=========================================
// 終了処理
//=========================================
void CUniversal::Uninit(void)
{
	delete this;
}

//=========================================
//回転制御処理
//=========================================
void CUniversal::LimitRot(float *fRot)
{
	if (*fRot > D3DX_PI)
	{
		*fRot -= 6.28f;
	}
	if (*fRot < -D3DX_PI)
	{
		*fRot += 6.28f;
	}
}

//========================================
//オフセット設定処理
//========================================
void CUniversal::SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtxWorldOffset);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, posOffset.x, posOffset.y, posOffset.z);
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxTrans);

	//マトリックスをかけ合わせる
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxWorldOwner);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, pMtxWorldOffset);
}

//========================================
// 向き補正処理
//========================================
void CUniversal::FactingRot(float *pfRot, float fRotDest, float rotateFact)
{
	//差分角度を取得
	float fRotDiff = fRotDest - *pfRot;

	//角度の修正
	if (fRotDiff < 0)
	{
		fRotDiff += 6.28f;
	}
	else if (fRotDiff > 0)
	{
		fRotDiff -= 6.28f;
	}

	//角度補正
	*pfRot += fRotDiff * rotateFact;

	//角度の修正
	if (fRotDiff < 0)
	{
		fRotDiff += 6.28f;
	}
	else if (fRotDiff > 0)
	{
		fRotDiff -= 6.28f;
	}

	//角度補正
	*pfRot += fRotDiff * rotateFact;
}