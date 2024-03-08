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
#include "debugproc.h"
#include <stdio.h>

namespace universal
{
//=========================================
// 値の制限
//=========================================
void LimitValue(float *pValue, float fMax, float fMin)
{
	if (fMax < *pValue)
	{
		*pValue = fMax;
	}

	if (fMin > *pValue)
	{
		*pValue = fMin;
	}
}

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
// 矩形内の位置制限
//========================================
void LimitPosInSq(float fWidth, float fHeight, D3DXVECTOR3 *pPos)
{
	if (pPos == nullptr)
		return;
	
	LimitValue(&pPos->x, fWidth, -fWidth);
	LimitValue(&pPos->z, fHeight, -fHeight);
}

//========================================
// 円柱の距離制限
//========================================
float LimitDistCylinder(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget)
{
	if (pPos == nullptr)
		return 0.0f;

	posTarget.y = pPos->y;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	float fDistDiff = D3DXVec3Length(&vecDiff);

	if (fLength >= fDistDiff)
	{
		fDistDiff = fLength;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= fLength;

		*pPos = posTarget - vecDiff;
	}

	return fDistDiff;
}

//========================================
// 球の距離制限（内側）
//========================================
float LimitDistSphereInSide(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget)
{
	if (pPos == nullptr)
		return 0.0f;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	float fDistDiff = D3DXVec3Length(&vecDiff);

	if (fLength <= fDistDiff)
	{
		fDistDiff = fLength;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= fLength;

		*pPos = posTarget - vecDiff;
	}

	return fDistDiff;
}

//========================================
// ホーミング
//========================================
void Horming(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, float fSpeedChase, D3DXVECTOR3 *pMove)
{
	if (pMove == nullptr)
		return;

	D3DXVECTOR3 vecDiff = posTarget - pos;

	VecConvertLength(&vecDiff, fSpeedChase);

	*pMove += vecDiff;
}

//========================================
// ベクトルを長さで補正する処理
//========================================
void VecConvertLength(D3DXVECTOR3 *pVec, float fLength)
{
	D3DXVec3Normalize(pVec, pVec);

	*pVec *= fLength;
}

//========================================
// 反対の相対座標を出す処理
//========================================
D3DXVECTOR3 RelativeInversPos(D3DXVECTOR3 pos, D3DXVECTOR3 posCenter, float fRate)
{
	D3DXVECTOR3 posInvers = { 0.0f,0.0f,0.0f };

	D3DXVECTOR3 vecDiff = posCenter - pos;

	vecDiff *= fRate;

	posInvers = posCenter + vecDiff;

	return posInvers;
}

//========================================
// 目標位置に向かう処理
//========================================
void MoveToDest(D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget, float fFact)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	*pPos += vecDiff * fFact;
}

//========================================
// オフセットへのベクトル
//========================================
D3DXVECTOR3 VecToOffset(D3DXMATRIX mtx, D3DXVECTOR3 posOffset)
{
	D3DXVECTOR3 pos =
	{
		mtx._41,
		mtx._42,
		mtx._43,
	};

	D3DXMATRIX mtxOffset;

	SetOffSet(&mtxOffset, mtx, posOffset);

	D3DXVECTOR3 offset =
	{
		mtxOffset._41,
		mtxOffset._42,
		mtxOffset._43,
	};

	D3DXVECTOR3 vecDiff = offset - pos;

	return vecDiff;
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
// 目標方向を向く処理
//========================================
void FactingRotTarget(D3DXVECTOR3 *pRot, D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fFact)
{
	if (pRot == nullptr)
	{
		return;
	}

	// 差分角度の取得
	D3DXVECTOR3 vecDiff = posTarget - posOwn;

	D3DXVECTOR3 rotDest = VecToRot(vecDiff);

	FactingRot(&pRot->x, rotDest.x, fFact);
	FactingRot(&pRot->y, rotDest.y, fFact);
}

//========================================
// 距離の比較
//========================================
D3DXVECTOR3 VecToRot(D3DXVECTOR3 vec)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	rot.y = atan2f(vec.x, vec.z);

	float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

	rot.x = atan2f(fLength, -vec.y);

	return rot;
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
// 距離の比較平面
//========================================
bool DistCmpFlat(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff)
{
	D3DXVECTOR3 vecDiff = posTarget - posOwn;
	float fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

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

//========================================
// 外積の当たり判定計算
//========================================
bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate, D3DXVECTOR3 move)
{
	bool bHit = false;

	D3DXVECTOR3 vecLine = vecDest - vecSorce;
	D3DXVECTOR3 vecToPos = posTarget - vecSorce;
	float fArea = CrossProduct(vecLine, vecToPos);

	if (fArea > 0)
	{
		if (pRate != nullptr)
		{
			// 割合を算出
			float fAreaMax = (vecDest.z * move.x) - (vecDest.x * move.z);
			fArea = (vecToPos.z * move.x) - (vecToPos.x * move.z);

			*pRate = fArea / fAreaMax;
		}

		bHit = true;
	}

	return bHit;
}

//========================================
// 三角形の中にいるかの判定
//========================================
bool IsInTriangle(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 posTarget)
{
	int nHit = 0;

	if(IsCross(posTarget, vtx1, vtx2, nullptr) == false)
		nHit++;
	if (IsCross(posTarget, vtx2, vtx3, nullptr) == false)
		nHit++;
	if (IsCross(posTarget, vtx3, vtx1, nullptr) == false)
		nHit++;

	if (nHit == 3)
	{
		return true;
	}

	return false;
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
// 画面内判定
//========================================
bool IsInScreen(const D3DXVECTOR3 pos, D3DXMATRIX mtx, D3DXVECTOR3 *pPos)
{
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer == nullptr)
	{
		return false;
	}

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	D3DXMATRIX projectionMatrix;
	pDevice->GetTransform(D3DTS_PROJECTION, &projectionMatrix);

	D3DXMATRIX viewMatrix;
	pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXVECTOR3 screenPosition;
	D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	if (pPos != nullptr)
	{
		*pPos = screenPosition;
	}

	if (screenPosition.x >= viewport.X && 
		screenPosition.x <= viewport.X + viewport.Width &&
		screenPosition.y >= viewport.Y && 
		screenPosition.y <= viewport.Y + viewport.Height &&
		screenPosition.z >= 0.0f && screenPosition.z <= 1.0f)
	{
		return true;
	}

	return false;
}

//=====================================================
// 偏差撃ちの処理
//=====================================================
D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget)
{
	// 変数宣言
	D3DXVECTOR3 vecDiff;
	float fSpeedTarget;
	float fDataA, fDataB, fDataC, fDataD;
	int nFrame1, nFrame2;

	// 目標の速度を計算
	fSpeedTarget = D3DXVec3Length(&moveTarget);

	// 目標との位置の差分を取得
	vecDiff = posTarget - pos;

	fDataA = (moveTarget.x * moveTarget.x + moveTarget.y * moveTarget.y + moveTarget.z * moveTarget.z) - fSpeedBullet * fSpeedBullet;

	fDataB = 2 * (posTarget.x * moveTarget.x + posTarget.y * moveTarget.y + posTarget.z * moveTarget.z);

	fDataC = (posTarget.x * posTarget.x + posTarget.y * posTarget.y + posTarget.z * posTarget.z);

	if (fDataA == 0)
	{// 0割り防止
		if (fDataB == 0)
		{
			return posTarget;
		}
		else
		{
			return posTarget + moveTarget * (-fDataC / fDataB);
		}
	}

	//二次方程式の解の公式の判別式で分類
	fDataD = fDataB * fDataB - 4 * fDataA * fDataC;

	if (fDataD > 0)
	{
		float fDataE = (float)sqrt(fDataD);
		nFrame1 = (int)((-fDataB - fDataE) / (2 * fDataA));
		nFrame2 = (int)((-fDataB + fDataE) / (2 * fDataA));
		//解は2つなので正の数の最小値を使う
		nFrame1 = (int)PlusMin((float)nFrame1, (float)nFrame2);
	}
	else
	{
		//虚数解
		//当たらないので今の位置を狙う
		nFrame1 = 0;
	}

	//予想位置を返す
	return posTarget + moveTarget * (float)nFrame1;
}

//=====================================================
// プラスの最小値を返す処理
//=====================================================
float PlusMin(float fData1, float fData2)
{
	if (fData1 < 0 && fData2 < 0)
	{// どちらも負の値なら０を返す
		return 0;
	}
	else if (fData1 < 0)
	{
		return fData2;
	}
	else if (fData2 < 0)
	{
		return fData1;
	}

	return fData1 < fData2 ? fData1 : fData2;
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

//========================================
// RGBをD3DXCOLORに変換する処理
//========================================
D3DXCOLOR ConvertRGB(BYTE r, BYTE g, BYTE b, BYTE a)
{
	D3DXCOLOR col;

	col =
	{
		(float)r / 255.0f,
		(float)g / 255.0f,
		(float)b / 255.0f,
		(float)a / 255.0f,
	};

	return col;
}

//========================================
// 3D座標の線形補完
//========================================
D3DXVECTOR3 Lerp(D3DXVECTOR3 start, D3DXVECTOR3 end, float fTime)
{
	D3DXVECTOR3 pos = start;

	D3DXVECTOR3 vecDiff = end - start;

	pos += vecDiff * fTime;

	return pos;
}
}	// namespace universal