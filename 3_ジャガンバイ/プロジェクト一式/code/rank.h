//*****************************************************
//
// ランクの処理[Rank.h]
// Author:酒井南勝
//
//*****************************************************

#ifndef _RANK_H_
#define _RANK_H_

//*****************************************************
// インクルード
//*****************************************************

#include "ranking.h"
#include "scene.h"
#include "object.h"
#include "record.h"

//*****************************************************
// 前方宣言
//*****************************************************

class CObject2D;
class CNumber;

//*****************************************************
// 定数定義
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CRank : public CObject
{
public:

	CRank();	// コンストラクタ
	~CRank();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRank* Create(CRecord::GENRE_TYPE typeGenre, const char* pFileName);

	void SetUi(void);
	void ReSetUi(void);

	void Set(void);
	void Reset(void);
	void Sort(void);
	void Save(void);
	void Load(void);

	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 posWhile, D3DXVECTOR3 posWhileRank);
	void SetSize(float width, float height, float widthRank, float heightRank);
	void SetColor(D3DXCOLOR col);
	void SetTex(char* fileNameTex);

	int GetUpdateIdx(int nIdx) { return m_aUpdateIdx[nIdx]; }
	bool GetIsUpdate(int nIdx) { return m_aIsUpdate[nIdx]; }
	CNumber* GetNum(int nIdx) { return m_infoVisualUi.apNum[nIdx]; }

private:

	struct SInfoVisualUi
	{
		CNumber* apNum[RANK::NUM_MAX];		// ランキングの数字
		CObject2D* apRank[RANK::NUM_MAX];	// ランキングの順位UI
	};

	SInfoVisualUi m_infoVisualUi;		// UIの見た目関連の情報

	CRecord::GENRE_TYPE m_typeGenre;	// 戦績種類の種類
	char* m_pFileName;					// ランキングファイルの名前

	int m_aRankScore[RANK::NUM_MAX];	// ランキングの値
	int m_aRankFace[RANK::NUM_MAX];		// 顔の番号
	int m_aUpdateIdx[NUM_PLAYER];		// 更新したランキングの番号
	bool m_aIsUpdate[NUM_PLAYER];		// ランキングの更新の有無
};

#endif