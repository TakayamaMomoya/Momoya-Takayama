//*****************************************************
//
// ランキングの処理[Ranking.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************
// インクルード
//*****************************************************
#include"scene.h"
#include "number.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CSkybox;

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_RANK	(5) // ランキングの数

//*****************************************************
// クラスの定義
//*****************************************************
class CRanking : public CScene
{
public:
	//ニューレコード文字の状態
	typedef enum
	{
		STATE_NORMAL = 0,					//通常状態
		STATE_DIFF,							//色が変わっている状態
		STATE_MAX
	}STATE;

	CRanking();	// コンストラクタ
	~CRanking();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Reset(void);
	void Set(int nScore);
	void Save(void);
	void Load(void);

private:
	void Sort(void);

	int m_nScore;	// スコアの値
	int m_nRankUpdate = -1;	// 更新ランクNo.
	int m_aScore[NUM_RANK];	// ランキングスコア
	STATE m_state = STATE_NORMAL;	// ニューレコードの点滅状態
	int m_nCntState;	// 状態遷移カウンタ
	int m_nTimerTrans;	// 画面遷移カウンター
	CNumber *m_apNumber[NUM_RANK];	// ランキングの数字
	CSkybox *m_pSkybox;	// スカイボックス
};

#endif