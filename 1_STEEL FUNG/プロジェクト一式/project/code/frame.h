//*****************************************************
//
// フレームの処理[frame.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FRAME_H_
#define _FRAME_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;

//*****************************************************
// クラスの定義
//*****************************************************
class CFrame	: public CObject
{
public:
	CFrame(int nPriority = 0);
	~CFrame();

	static CFrame *Create(int nTimeIn,int nTimeWait,int nTimeOut);	// 生成処理
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// イン状態
		STATE_WAIT,	// 待機状態
		STATE_OUT,	// アウト状態
		STATE_MAX
	};

	struct SInfo
	{
		CObject2D *pUpper;
		CObject2D *pUnder;
		STATE state;
		int nTimeIn;
		int nTimeWait;
		int nTimeOut;
		int nCntFade;
	};

	void ManageState(void);
	void ManageUpper(void);
	void ManageUnder(void);

	SInfo m_info;
};

#endif