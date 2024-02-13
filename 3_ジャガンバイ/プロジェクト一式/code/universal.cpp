//*****************************************
//
// いろいろ汎用処理詰め込み
// Author:髙山桃也
//
//*****************************************

//*****************************************
// インクルード
//*****************************************
#include "manager.h"
#include "renderer.h"
#include "universal.h"
#include <stdio.h>

namespace universal
{
//=========================================
// 回転制御処理
//=========================================
void LimitRot(float *fRot)
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

//=========================================
// 速度の制限
//=========================================
void LimitSpeed(D3DXVECTOR3 *pVec, float fSpeedMax)
{
	if (pVec == nullptr)
	{
		return;
	}

	// 速度の取得
	float fSpeed = D3DXVec3Length(pVec);

	if (fSpeed > fSpeedMax)
	{// 速度の制限
		D3DXVECTOR3 vecNew = *pVec;

		D3DXVec3Normalize(&vecNew, &vecNew);

		vecNew *= fSpeedMax;

		*pVec = vecNew;
	}
}

//========================================
// オフセット設定処理
//========================================
void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot)
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
void FactingRot(float *pfRot, float fRotDest, float rotateFact)
{
	// 引数の角度の補正
	LimitRot(pfRot);
	LimitRot(&fRotDest);

	//差分角度を取得
	float fRotDiff = fRotDest - *pfRot;

	//角度の修正
	LimitRot(&fRotDiff);

	//角度補正
	*pfRot += fRotDiff * rotateFact;

	LimitRot(pfRot);
}

//========================================
// 距離の比較
//========================================
bool DistCmp(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff)
{
	D3DXVECTOR3 vecDiff = posTarget - posOwn;
	float fLength = D3DXVec3Length(&vecDiff);

	if (fLength < fLengthMax)
	{
		if (fDiff != nullptr)
		{
			*fDiff = fLength;
		}

		return true;
	}
	else
	{
		return false;
	}
}

//========================================
// 外積の計算
//========================================
float CrossProduct(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	return pos1.z * pos2.x - pos1.x * pos2.z;
}

//=====================================================
// 外積の当たり判定計算
//=====================================================
bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate, D3DXVECTOR3 move)
{
	bool bHit = false;

	D3DXVECTOR3 vecLine = vecDest - vecSorce;
	D3DXVECTOR3 vecToPos = posTarget - vecSorce;
	float fArea = CrossProduct(vecLine, vecToPos);

	if (fArea > 0)
	{
		D3DXVECTOR3 vecToPos = posTarget - vecSorce;

		if (pRate != nullptr)
		{
			// 割合を算出
			float fAreaMax = (vecDest.z * move.x) - (vecDest.x * move.z);
			float fArea = (vecToPos.z * move.x) - (vecToPos.x * move.z);

			*pRate = fArea / fAreaMax;
		}

		bHit = true;
	}

	return bHit;
}

//========================================
// 矩形の中にいるかどうかの計算
//========================================
bool CubeCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 pos)
{
	bool bHit = false;
	int nHit = 0;

	D3DXVECTOR3 aVec[4] =
	{// 配列に格納
		vtx1,
		vtx2,
		vtx3,
		vtx4,
	};

	// ４辺のなかにいるかのチェック
	for (int i = 0; i < 4; i++)
	{
		int nNext = (i + 1) % 4;

		// 線分の向こうにいるかの判定
		bool bCross = IsCross(pos, aVec[i], aVec[nNext], nullptr);

		if (bCross)
		{
			nHit++;
		}
	}

	if (nHit >= 4)
	{
		bHit = true;
	}

	return bHit;
}

//========================================
// 範囲内のランダム数値を返す処理
//========================================
int RandRange(int nMax, int nMin)
{
	int nRange = nMax - nMin;
	int nRand = rand() % nRange + nMin;

	return nRand;
}
}	// namespace universal