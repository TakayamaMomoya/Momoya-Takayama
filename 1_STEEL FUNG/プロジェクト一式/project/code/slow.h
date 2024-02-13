//*****************************************************
//
// スローの処理[slow.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SLOW_H_
#define _SLOW_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

namespace Slow
{
float GetScale(void);
}

//*****************************************************
// クラスの定義
//*****************************************************
class CSlow : public CObject
{
public:
	CSlow();	// コンストラクタ
	~CSlow();	// デストラクタ

	static CSlow *Create(void);
	static CSlow *GetInstance(void) { return m_pSlow; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float GetScale(void) { return m_fScale; }
	void SetScale(float fScale);
	void SetSlowTime(float fTime, float fScale);

private:
	float m_fScale;	// スローのスケール
	float m_fTimeSlow;	// スローにする時間

	static CSlow *m_pSlow;	// 自身のポインタ
};

#endif