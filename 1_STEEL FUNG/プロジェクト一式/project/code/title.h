//*****************************************************
//
// タイトル処理[title.h]
// Author:酒井南勝
//
//*****************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMenu;
class CObject2D;

//*****************************************************
// 定数定義
//*****************************************************

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
	virtual void Update(void);
	virtual void Draw(void);

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何もしてない状態
		STATE_OUT,		// フェードアウト状態
		START_MAX
	};

	void ManageStart(void);
	void UpdateFade(void);
	void SetFadeIn(void);

	STATE m_state;				// 状態

	CObject2D* m_pLogo;		// タイトルロゴのポインタ
	CObject2D* m_pLogoLate; // 遅れてくるロゴのポインタ
	CObject2D *m_pStart;	// スタート表示のポインタ

	int m_nFadeCnt;			// フェードまでのカウント
	bool m_bIsAlphaChange;	// α値の変化

	float m_fSizeX;			// ロゴのサイズ用
	float m_fSizeY;			// ロゴのサイズ用
};

#endif