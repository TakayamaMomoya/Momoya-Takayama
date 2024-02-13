//*****************************************************
//
// 残像の処理[afterimage.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _AFTERIMAGE_H_
#define _AFTERIMAGE_H_

#include "ObjectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionCube;

//*****************************************************
// クラスの定義
//*****************************************************
class CAfterImage : public CObjectX
{
public:
	CAfterImage(int nPriority = 6);	// コンストラクタ
	~CAfterImage();	// デストラクタ

	static CAfterImage *Create(D3DXMATRIX mtx, int nIdxModel,D3DXCOLOR col,int m_nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_nNumAll;	// 総数
	int m_nLife;	// 体力
	float m_fDecAlpha;	// 透明度減少量
};

#endif
