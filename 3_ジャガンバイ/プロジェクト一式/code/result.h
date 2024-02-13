//*****************************************************
//
// リザルトの処理[result.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;
class CPlayer;
class CNumber;

//*****************************************************
// クラス定義
//*****************************************************
class CResult : CObject
{
public:
	CResult();	// コンストラクタ
	~CResult();	// デストラクタ

	static CResult *Create(bool bWin = false);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSurvived(CPlayer *pPlayer);

private:
	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_WAIT,	// 入力待機状態
		STATE_CURRENT,	// 入力している状態
		STATE_MAX
	};
	struct SInfoSuvived
	{// 生存者情報
		CPlayer *pSuvived;	// 生存者のポインタ
		CObject2D *pIcon;	// プレイヤーのアイコン
		CObject2D *pCaption;	// キャプション
	};

	void Input(void);
	void Create2D(bool bWin);
	void DispSuvived(SInfoSuvived *pInfo);

	CObject2D *m_pBg;	// 背景のポインタ
	CObject2D *m_p2DResult;	// 見出しのポインタ
	STATE m_state;	// 状態
	SInfoSuvived m_aInfoSurvived[NUM_PLAYER];	// 生存者情報
	int m_nNumSuvived;	// 生存者数
};

#endif