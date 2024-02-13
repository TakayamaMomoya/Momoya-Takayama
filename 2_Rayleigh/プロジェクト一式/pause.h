//*****************************************************
//
// ポーズの処理[pause.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;

//*****************************************************
// クラス定義
//*****************************************************
class CPause : CObject
{
public:
	CPause();	// コンストラクタ
	~CPause();	// デストラクタ

	static CPause *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *GetInstance(void) { return m_pPause; }

private:
	enum MENU
	{// 項目の種類
		MENU_RESUME = 0,	// 再開
		MENU_RESTART,	// リトライ
		MENU_CHECKPOINT,	// チェックポイントから再開
		MENU_QUIT,	// 終了
		MENU_MAX
	};
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// フェードイン状態
		STATE_OUT,	// フェードアウト状態
		STATE_MAX
	};

	void ManageState(void);
	void ManageBg(void);
	void Input(void);
	void Fade(MENU menu);

	MENU m_menu;	// 選択項目
	CObject2D *m_apMenu[MENU_MAX];	// メニュー項目の２Dオブジェクト
	CObject2D *m_pBg;	// 背景２Dオブジェクト
	D3DXVECTOR3 m_aPosDest[MENU_MAX];	// 目標の位置
	STATE m_state;	// 状態
	static CPause *m_pPause;	// 自身のポインタ
};

#endif