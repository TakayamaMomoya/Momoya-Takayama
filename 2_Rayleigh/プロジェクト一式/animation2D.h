//*****************************************************
//
// 2Dアニメーションの処理[animation2D.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ANIM2D_H_
#define _ANIM2D_H_

#include "object2D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CAnim2D : public CObject2D
{
public:
	CAnim2D(int nPriotity = 5);	// コンストラクタ
	~CAnim2D();	// デストラクタ

	CAnim2D *Create(int nPtnAnim, int nNumAnim, int nTimeAnim, int nNumV);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetInfo(int nPtnAnim, int nNumAnim, int nTimeAnim, int nNumV);

private:
	int m_nCounterAnim;	// アニメーションカウンター
	int m_nPatternAnim;	// アニメパターンNO.
	int m_nTimerAnim;	// アニメーションする時間
	int m_nNumAnim;	// アニメーションの枚数
	int m_nNumV;	// アニメーションの縦の行で折り返すまでの枚数
};

#endif
