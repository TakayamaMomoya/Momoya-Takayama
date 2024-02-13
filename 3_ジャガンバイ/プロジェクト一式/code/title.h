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
namespace ENEMY
{
	const int NUM_ENEMY = 8;	// 敵の最大数
}

//*****************************************************
// クラスの定義
//*****************************************************
class CTitle : public CScene
{
public:

	enum CAMERA
	{
		CAMERA_PLAYER_ALL = 0,	// 全員が映る状態
		CAMERA_PLAYER_ONE,		// プレイヤー1が映る状態
		CAMERA_PLAYER_TWO,		// プレイヤー2が映る状態
		CAMERA_PLAYER_THREE,	// プレイヤー3が映る状態
		CAMERA_PLAYER_FOUR,		// プレイヤー4が映る状態
		CAMERA_MAX
	};

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

	enum TITLE_MOTION_PLAYER
	{
		TITLE_MOTION_PLAYER_NEUTRAL = 0,	// 待機状態
		TITLE_MOTION_PLAYER_MOVE,			// 移動状態
		TITLE_MOTION_PLAYER_MAX
	};

	struct SCameraInfo
	{
		CAMERA num;		// カメラ番号
		int nCount;		// カウント
	};

	void ManageStart(void);
	void UpdateFade(void);
	void UpdateCamera(void);
	void SetFadeIn(void);

	STATE m_state;				// 状態
	SCameraInfo m_cameraInfo;	// カメラ情報

	CObject2D* m_pLogo;		// タイトルロゴのポインタ
	CObject2D* m_pLogoLate; // 遅れてくるロゴのポインタ
	CObject2D *m_pStart;	// スタート表示のポインタ

	CMotion* m_apModelPlayer[NUM_PLAYER];		// プレイヤーモデルのポインタ
	CMotion* m_apModelEnemy[ENEMY::NUM_ENEMY];	// エネミーモデルのポインタ

	int m_nFadeCnt;			// フェードまでのカウント
	bool m_bIsAlphaChange;	// α値の変化

	float m_fSizeX;			// ロゴのサイズ用
	float m_fSizeY;			// ロゴのサイズ用
};

#endif