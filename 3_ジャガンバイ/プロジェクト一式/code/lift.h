//*****************************************************
//
// リフト処理[lift.h]
// Author:小笠原彪
//
//*****************************************************

#ifndef _STARTLOCATION_H_
#define _STARTLOCATION_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CLift : public CObjectX
{
public:
	enum STATE
	{
		STATE_NONE = 0,
		STATE_IN,
		STATE_UP,
		STATE_END,
		STATE_MAX
	};

public:
	CLift(int nPriority = 6);	// コンストラクタ
	~CLift();	// デストラクタ

	static CLift* Create(D3DXVECTOR3 pos, int nPriority = 3);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);	// 読込

	static void SetjoinPlayer(int nPlayer) { m_nNumJoinPlayer = nPlayer; }
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }

	static int GetInPlayer(void);
	static bool GetIsIn(void);

private:
	D3DXVECTOR3 m_pos;
	static int m_nNumJoinPlayer;
	static int m_nInCnt;
	static bool m_abJoin[NUM_PLAYER];
	STATE m_state;

};

#endif