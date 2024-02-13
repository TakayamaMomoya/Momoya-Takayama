
//*****************************************************
//
// ランキングの演技の処理[rank_acting.h]
// Author:酒井南勝
//
//*****************************************************

#ifndef _RANK_ACTING_H_
#define _RANK_ACTING_H_

//*****************************************************
// インクルード
//*****************************************************

#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************

class CObject3D;
class CMotion;

//*****************************************************
// 定数定義
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CRankActing
{
public:

	// 演出の種類
	enum ACTING_TYPE
	{
		ACTING_TYPE_MARCH = 0,	// 行進
		ACTING_TYPE_RUN_AWAY,	// 逃げる
		ACTING_TYPE_CHASE,		// 追いかける
		ACTING_TYPE_MAX
	};

	// 役者の種類
	enum ACTOR_TYPE
	{
		ACTOR_TYPE_PLAYER_ONE = 0,	// プレイヤー01
		ACTOR_TYPE_PLAYER_TWO,		// プレイヤー02
		ACTOR_TYPE_PLAYER_THREE,	// プレイヤー03
		ACTOR_TYPE_PLAYER_FOUR,		// プレイヤー04
		ACTOR_TYPE_ENEMY_ONE,		// 敵01
		ACTOR_TYPE_ENEMY_TWO,		// 敵02
		ACTOR_TYPE_ENEMY_THREE,		// 敵03
		ACTOR_TYPE_MAX
	};

	CRankActing();	// コンストラクタ
	~CRankActing();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRankActing* Create(void);

	HRESULT InitObj(void);

	void SetObjActing(void);

	void CountActing(void);

private:

	struct SInfoVisualObj
	{
		CObject3D* pField;				// 地面
		CObject3D* pWall;				// 壁
		CMotion* apModelActor[ACTOR_TYPE_MAX];	// プレイヤー
	};

	SInfoVisualObj m_infoVisualObj;		// オブジェクトの見た目関連の処理

	ACTING_TYPE m_typeActing;			// 演技の種類

	int m_nActingCnt;					// 演技カウント
};

#endif