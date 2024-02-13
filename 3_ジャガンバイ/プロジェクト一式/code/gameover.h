//=========================================================
//
// ゲームオーバー処理 [gameover.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _GAMEOVER_H_     // このマクロ定義がされてなかったら
#define _GAMEOVER_H_     // 2重インクルード防止のマクロ定義する

//===============================================
// インクルード
//===============================================
#include "main.h"
#include "motion.h"
#include "object2D.h"

//===============================================
// 定数定義
//===============================================
namespace
{
	const int NUM_ENEMY = 5;		// 生成する敵の総数
}

//===============================================
// ゲームオーバークラス
//===============================================
class CGameover
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CGameover();						// デフォルトコンストラクタ
	~CGameover();						// デストラクタ

	static CGameover* Create(int nPriority = 3);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BlackOut(void);

	static CGameover* GetInstance(void) { return m_pGameover; }
	void SetDeathPlayer(int nID, bool bDeath) { m_bDeathPlayer[nID] = bDeath; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CGameover* m_pGameover;	// 自身のポインタ
	CMotion* m_apModelPlayer[NUM_PLAYER];		// プレイヤーモデルのポインタ
	CMotion* m_apModelEnemy[NUM_ENEMY];			// エネミーモデルのポインタ
	CObject2D* m_pBlack;						// 黒ポリゴン
	CObject2D* m_pLogo;							// ゲームオーバー表示
	CObject2D* m_pContiny;						// コンティニュー表示
	bool m_bDeathPlayer[NUM_PLAYER];			// 死亡したプレイヤー
	bool m_bFlash;								// 点滅切り替え
	float m_fFadeCounter;						// ゆっくり表示する
	float m_fFlashCounter;						// 点滅させる
};

#endif