//*****************************************************
//
// チェックポイントのBehavior[checkpointBehavior.h]
// Author:高山桃也
//
//*****************************************************

#ifndef _CHECKPOINTBEHAVIOR_H_
#define _CHECKPOINTBEHAVIOR_H_

//*****************************************************
// 前方宣言
//*****************************************************
class CCheckPointManager;
class CLimit;
class CUI;

//*****************************************************
// 定数定義
//*****************************************************
namespace CheckPointBehavior
{
const int NUM_LIMIT = 2;	// 位置制限の数
}

//*****************************************************
// クラスの定義
//*****************************************************
class CCheckPointBehavior
{
public:
	CCheckPointBehavior();
	virtual ~CCheckPointBehavior();

	virtual void Init(CCheckPointManager *pCheckPoint) = 0;
	virtual void Update(CCheckPointManager *pCheckPoint) = 0;
	virtual void Uninit(CCheckPointManager *pCheckPoint) = 0;
private:

};

class CCheckPointMove : public CCheckPointBehavior
{// 移動パート
public:
	CCheckPointMove();
	~CCheckPointMove() override;

private:
	void Init(CCheckPointManager *pCheckPoint) override;
	void Update(CCheckPointManager *pCheckPoint) override;
	void Uninit(CCheckPointManager *pCheckPoint) override;

	CUI *m_pCursor;	// カーソルのポインタ
};

class CCheckPointBattle : public CCheckPointBehavior
{// 戦闘パート
public:
	CCheckPointBattle();
	~CCheckPointBattle() override;

private:
	void Init(CCheckPointManager *pCheckPoint) override;
	void Update(CCheckPointManager *pCheckPoint) override;
	void Uninit(CCheckPointManager *pCheckPoint) override;
	CLimit *m_apLimit[CheckPointBehavior::NUM_LIMIT];	// 位置制限のポインタ
};

#endif