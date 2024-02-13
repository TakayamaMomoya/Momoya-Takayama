//*****************************************************
//
// 幽霊の処理[ghost.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _GHOST_H_
#define _GHOST_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CGhost : public CMotion
{
public:
	CGhost(int nPriority = 3);	// コンストラクタ
	~CGhost();	// デストラクタ

	static CGhost *Create(int nIdxPlayer);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
};

#endif

