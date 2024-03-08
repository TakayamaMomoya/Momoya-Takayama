//*****************************************************
//
// リザルトのBehavior[resultBehavior.h]
// Author:高山桃也
//
//*****************************************************

#ifndef _RESULTBEHAVIOR_H_
#define _RESULTBEHAVIOR_H_

//*****************************************************
// インクルード
//*****************************************************
#include <iostream>
#include <list>

//*****************************************************
// 前方宣言
//*****************************************************
class CResult;
class CNumber;
class CObject2D;

//*****************************************************
// クラスの定義
//*****************************************************
class CResultBehavior
{
public:
	CResultBehavior();
	virtual ~CResultBehavior();

	virtual void Init(CResult *pResult) = 0;
	virtual void Update(CResult *pResult) = 0;
	virtual void Uninit(CResult *pResult) = 0;
private:

};

class CResultPlayerScore : public CResultBehavior
{
public:
	enum TYPE_NUMBER
	{
		TYPE_ALL = 0,	// 総額
		TYPE_ADD,	// 加算報酬
		TYPE_REPAIR,	// 修理費
		TYPE_RESULT,	// 総額
		TYPE_MAX
	};

	CResultPlayerScore();
	~CResultPlayerScore() override;

private:
	void Init(CResult *pResult) override;
	void Update(CResult *pResult) override;
	void Uninit(CResult *pResult) override;

	CNumber *m_apNumber[TYPE_MAX];	// 数値
	CObject2D *m_apCaption[TYPE_MAX];	// 項目の見出し
	CObject2D *m_pFrame;	// フレーム
};

class CResultRanking : public CResultBehavior
{
public:
	CResultRanking();
	~CResultRanking() override;

private:
	void Init(CResult *pResult) override;
	void Update(CResult *pResult) override;
	void Uninit(CResult *pResult) override {};
};

#endif