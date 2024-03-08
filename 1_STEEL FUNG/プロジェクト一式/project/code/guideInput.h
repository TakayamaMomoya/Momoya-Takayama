//*****************************************************
//
// 操作ガイドの処理[guideInput.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _GUIDEINPUT_H_
#define _GUIDEINPUT_H_

#include "Object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CGuideInput : public CObject
{
public:
	enum INPUT
	{// 表示する入力の種類
		INPUT_JUMP = 0,	// ジャンプ
		INPUT_SHOT,	// 射撃
		INPUT_MELEE,	// 近接攻撃
		INPUT_GRAB,	// 掴み攻撃
		INPUT_DODGE,	// 回避
		INPUT_LOCK,	// ロックオン
		INPUT_MAX
	};

	CGuideInput(int nPriority = 0);	// コンストラクタ
	~CGuideInput();	// デストラクタ

	static CGuideInput *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	struct SInfoGuide
	{
		CUI *pFrame;	// フレーム
		float fTime;	// 光る時間
	};

	void CheckInput(void);

	SInfoGuide m_aGuide[INPUT_MAX];	// ガイド表示の情報
};

#endif
