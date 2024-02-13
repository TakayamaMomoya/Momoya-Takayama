//*****************************************************
//
// ゴールタイマーの処理[goalGoalTimer.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _GOALTIMER_H_
#define _GOALTIMER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CNumber;
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CGoalTimer : public CObject
{
public:
	CGoalTimer();	// コンストラクタ
	~CGoalTimer();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGoalTimer *Create(void);
	void AddGoalTimer(int nValue);
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	float GetHeight(void) { return 0.0f; }	// サイズ取得
	float GetSecond(void) { return m_fSecond; }	// 時間取得
	void EnableStop(bool bStop) { m_bStop = bStop; }
	static CGoalTimer *GetInstance(void) { return m_pGoalTimer; }

private:
	CNumber *m_pObjSecond;	// 秒表示のポインタ
	CNumber *m_pObjDecimal;	// 小数点以下表示のポインタ
	CUI *m_pPoint;	// 小数点
	float m_fSecond;	// 現在の時間(秒)
	static CGoalTimer *m_pGoalTimer;	// 自身のポインタ
	bool m_bStop;	// 止めるかどうか
	bool m_bSound;
};

#endif
