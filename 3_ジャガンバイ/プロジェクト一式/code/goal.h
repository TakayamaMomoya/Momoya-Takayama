//*****************************************************
//
// ゴールの処理[goal.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _GOAL_H_
#define _GOAL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"
#include <stdio.h>

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CGoal : public CObjectX
{
public:
	CGoal(int nPriority = 3);	// コンストラクタ
	~CGoal();	// デストラクタ

	static CGoal *Create(void);
	static CGoal *GetInstance(void) { return m_pGoal; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DeadLine(void);
	bool IsFinish(void) { return m_bFinish; }
	void SetResult(void);
	float GetRadius(void) { return m_fRadius; }

private:
	void Load(void);
	void ApplyInfo(FILE *pFile,char *pTemp);
	void DetectPlayer(void);

	bool m_bFinish;	// カウントダウンが終わったかどうか
	float m_fRadius;	// 判定の半径
	CCollisionSphere *m_pCollisionGoal;	// ゴール判定
	CObject3D *m_pArea;	// 範囲の表示
	static CGoal *m_pGoal;	// 自身のポインタ
};

#endif

