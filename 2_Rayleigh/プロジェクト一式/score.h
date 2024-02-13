//*****************************************************
//
// スコアの処理[score.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CNumber;

//*****************************************************
// クラスの定義
//*****************************************************
class CScore : public CObject
{
public:
	CScore();	// コンストラクタ
	~CScore();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {}
	static CScore *Create(void);
	int AddScore(int nValue);
	void SetPosition(D3DXVECTOR3 pos) {};
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); };	// 位置取得処理
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	float GetHeight(void) { return 0.0f; }	// サイズ取得
	int GetScore(void) { return m_nSocreDest; }
	static CScore *GetInstance(void) { return m_pScore; }
	void SetResultScore(void);

private:
	CNumber *m_pObjNumber;	// スコア値のポインタ
	int m_nScore;	// スコアの値
	int m_nSocreDest;	// 目標のスコア値
	int m_nIdxTexture;	// テクスチャ番号
	static CScore *m_pScore;	// 自身のポインタ
};

#endif
