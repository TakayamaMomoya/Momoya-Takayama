//*****************************************************
//
// UIマネージャー[UIManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CLife;
class CScore;
class CTimer;

//*****************************************************
// クラスの定義
//*****************************************************
class CUIManager : public CObject
{
public:
	CUIManager();	// コンストラクタ
	~CUIManager();	// デストラクタ

	static CUIManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUIManager *GetInstance(void) { return m_pUIManager; }
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }

private:
	static CUIManager *m_pUIManager;	// 自身のポインタ
	bool m_bDisp;	// UIを表示するかどうか
	CLife *m_pLife;	// ライフ
	CScore *m_pScore;	// スコア
	CTimer *m_pTimer;	// タイマー
};

#endif
