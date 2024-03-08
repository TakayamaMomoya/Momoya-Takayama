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
class CMotion;
class CMotion;
class CTitleBehavior;

//*****************************************************
// クラスの定義
//*****************************************************
class CTitle : public CScene
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// 何もしてない状態
		STATE_OUT,		// フェードアウト状態
		START_MAX
	};

	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void ChangeBehavior(CTitleBehavior *pBehavior);
	STATE GetState(void) { return m_state; }

private:
	STATE m_state;				// 状態
	CObject2D* m_pLogo;		// タイトルロゴのポインタ
	CMotion *m_pMotion;	// プレイヤーモデル
	CTitleBehavior *m_pBehavior;	// ビヘイビア
	float m_fTImerSmoke;	// 煙のスポーンタイマー
};

class CTitleBehavior
{// 基本ビヘイビア
public:
	CTitleBehavior();	// コンストラクタ
	virtual ~CTitleBehavior();	// デストラクタ

	virtual void Update(CTitle *pTItle) = 0;

private:
};

class CTitleStart : public CTitleBehavior
{// スタート表示状態
public:
	CTitleStart();	// コンストラクタ
	virtual ~CTitleStart();	// デストラクタ
	void Update(CTitle *pTItle) override;

private:
	CObject2D *m_pStart;	// スタート表示のポインタ
	CObject2D *m_pAfter;	// スタート表示の残像
};

class CTitleMenu : public CTitleBehavior
{// メニュー
public:
	enum MENU
	{
		MENU_GAME = 0,	// ゲーム
		MENU_TRANING,	// 訓練場
		MENU_MAX
	};

	CTitleMenu();	// コンストラクタ
	virtual ~CTitleMenu();	// デストラクタ
	void Update(CTitle *pTItle) override;

private:
	void Input(void);
	void ManageCursor(void);
	void Fade(void);

	CObject2D *m_apMenu[MENU_MAX];	// メニュー項目
	CObject2D *m_pCursor;	// カーソル
	MENU m_menu;	// 選択メニュー項目
};

#endif