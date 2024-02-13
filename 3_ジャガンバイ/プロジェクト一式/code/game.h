//*****************************************************
//
// ゲーム処理[game.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CScore;
class CTimer;
class CGame;
class CPlayer;

//*****************************************************
// クラスの定義
//*****************************************************
class CGame : public CScene
{
public:
	enum STATE
	{// 状態
		STATE_NONE,	// 何もしていない状態
		STATE_NORMAL,	// 通常状態
		STATE_ESCAPE,	// 脱出状態
		STATE_RESULT,	// リザルト状態
		STATE_GAMEOVER,	// ゲームオーバー状態
		STATE_END,	// 終了状態
		STATE_MAX
	};

	CGame();	// コンストラクタ
	~CGame();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static void SetState(STATE state) { m_state = state; }
	static STATE GetState(void) { return m_state; }
	void EnableStop(bool bStop) { m_bStop = bStop; }
	bool GetStop(void) { return m_bStop; }
	void SetEventCamera(float fTime, D3DXVECTOR3 posRDest, D3DXVECTOR3 posVDest);
	static CGame *GetInstance(void) { return m_pGame; }

private:
	void UpdateCamera(void);
	void ManageState(void);
	void Debug(void);

	static STATE m_state;	// 状態
	static CGame *m_pGame;	// 自身のポインタ

	int m_nCntState;	// 状態遷移カウンター
	bool m_bStop;	// 停止状態
	bool m_bGameover;	// ゲームオーバーのフラグ
};

#endif