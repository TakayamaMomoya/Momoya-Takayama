//*****************************************
//
// 汎用処理セット
// Author:髙山桃也
//
//*****************************************
#ifndef _UNIVERSAL_H_
#define _UNIVERSAL_H_

#include "main.h"

//*****************************************
// クラスの定義
//*****************************************
namespace universal
{
	void LimitRot(float *fRot);	//回転制御処理
	void LimitSpeed(D3DXVECTOR3 *pVec, float fSpeedMax);
	void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });	//オフセット設定処理
	void FactingRot(float *pfRot, float fRotDest, float rotateFact);	//向き補正処理
	bool DistCmp(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget,float fLengthMax,float *fDiff);
	int RandRange(int nMax, int nMin);
	float CrossProduct(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
	bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });
	bool CubeCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 pos);
}

#endif