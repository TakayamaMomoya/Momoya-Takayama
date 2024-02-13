//*****************************************************
//
// ライフ管理の処理[life.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _LIFE_H_
#define _LIFE_H_

#include "Object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CLife : public CObject
{
public:
	CLife(int nPriority = 6);	// コンストラクタ
	~CLife();	// デストラクタ

	static CLife *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

private:
	enum PULSESTATE
	{
		PULSESTATE_IN = 0,	// フェードイン状態
		PULSESTATE_OUT,	// フェードアウト状態
		PULSESTATE_MAX,
	};

	void UpdatePulse(void);
	void FadePulse(void);

	CUI *m_pFrame;	// フレームのポインタ
	CUI *m_pPulse;	// 波形のポインタ
	PULSESTATE m_pulseState;	// 波形の状態
	int m_nCntPulse;	// 波形のカウンター
};

#endif
