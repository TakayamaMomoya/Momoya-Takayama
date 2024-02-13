//*****************************************************
//
// ランキングの処理[Ranking.h]
// Author:酒井南勝
//
//*****************************************************

#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************
// インクルード
//*****************************************************

#include "scene.h"

#include "record.h"

//*****************************************************
// 前方宣言
//*****************************************************

class CObject2D;
class CNumber;

class CRank;
class CRankActing;

//*****************************************************
// 定数定義
//*****************************************************
namespace RANK
{
	const int NUM_MAX = 3;	// 最大数
}

//*****************************************************
// クラスの定義
//*****************************************************
class CRanking : public CScene
{
public:
	
	CRanking();	// コンストラクタ
	~CRanking();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT InitUi(void);
	HRESULT InitRank(void);

	void SetUiRecord(void);
	void SetRank(void);

	void UpdateUiRecord(void);
	void UpdateNum(void);

private:

	struct SInfoVisualUi
	{
		CObject2D* pRecordText;		// レコードのテキスト
		CObject2D* pGenreText;		// 種類のテキスト
		CObject2D* pGenreIcon;		// 種類のアイコン 

		CObject2D* apFace[NUM_PLAYER];	// 各プレイヤーの顔
		CNumber* apNumber[NUM_PLAYER];	// 各プレイヤーの戦績
	};

	SInfoVisualUi m_infoVisualUi;				// UIの見た目関連の情報
	CRank* m_apRank[CRecord::GENRE_TYPE_MAX];	// ランクの情報
	CRankActing* m_pRankActing;					// 演技の情報

	CRecord::GENRE_TYPE m_typeGenre;	// 戦績の種類

	int m_nGenreCnt;					// 種類カウント

	bool m_aIsAlphaChange[CRecord::GENRE_TYPE_MAX][NUM_PLAYER];	// α値の変化
};

#endif