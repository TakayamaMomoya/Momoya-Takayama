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
		STATE_BOSS,	// ボス戦状態
		STATE_RESULT,	// リザルト状態
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
	static void SetProgress(int nProgress) { m_nProgress = nProgress; }
	void EnableStop(bool bStop) { m_bStop = bStop; }
	static CGame *GetInstance(void) { return m_pGame; }
	int GetProgress(void) { return m_nProgress; }

private:
	void LoadCheckPoint(void);
	void CreateTutorial(void);
	void UpdateCamera(void);
	void ManageState(void);
	void Debug(void);

	static STATE m_state;	// 状態
	static int m_nProgress;	// 進行度合い
	static CGame *m_pGame;	// 自身のポインタ

	D3DXVECTOR3 *m_pPosCheckPoint;	// チェックポイント座標のポインタ
	int m_nNumCheckPoint;	// チェックポイントの数
	int m_nCntState;	// 状態遷移カウンター
	bool m_bStop;	// 停止状態
};

#endif