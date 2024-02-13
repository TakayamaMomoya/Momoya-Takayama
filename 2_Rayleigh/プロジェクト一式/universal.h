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
class CUniversal
{
public:
	CUniversal();	// コンストラクタ
	~CUniversal();	// デストラクタ

	static CUniversal *Create(void);
	void Uninit(void);
	void LimitRot(float *fRot);	//回転制御処理
	void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });	//オフセット設定処理
	void FactingRot(float *pfRot, float fRotDest, float rotateFact);	//向き補正処理
	static CUniversal *GetInstance(void) { return m_pUniversal; }

private:
	static CUniversal *m_pUniversal;	// 自身のポインタ
};

#endif