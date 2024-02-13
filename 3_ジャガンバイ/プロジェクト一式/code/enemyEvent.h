//*****************************************************
//
// 敵イベントの処理[enemyEvent.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYEVENT_H_
#define _ENEMYEVENT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyEvent : public CObject
{
public:
	CEnemyEvent(int nPriority = 6);	// コンストラクタ
	~CEnemyEvent();	// デストラクタ

	static CEnemyEvent *Create(float fLife = 10.0f,float fDelay = 10.0f);
	static CEnemyEvent *GetInstace(void) { return m_pEnemyEvent; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScale(float fScale) { m_fScaleTime = fScale; }

private:
	void StartEvent(void);

	float m_fScaleTime;	// タイムスケール
	float m_fDelay;	// イベント発生までのディレイ
	float m_fLife;	// 寿命

	static CEnemyEvent *m_pEnemyEvent;	// 自身のポインタ
};

#endif
