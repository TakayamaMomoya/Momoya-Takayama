//*****************************************************
//
// タイトル処理[title.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMenu;
class CObject2D;
class CMotion;

//*****************************************************
// クラスの定義
//*****************************************************
class CTitle : public CScene
{
public:
	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何もしてない状態
		STATE_MOTION,	// モーションしてる状態
		STATE_OUT,	// フェードアウト状態
		START_MAX
	};

	void ManageStart(void);

	CObject2D *m_pStart;	// スタート表示のポインタ
	CObject2D *m_pLogo;	// ロゴのポインタ
	STATE m_state;	// 状態
	CMotion *m_pMotion;	// モーション
};

#endif