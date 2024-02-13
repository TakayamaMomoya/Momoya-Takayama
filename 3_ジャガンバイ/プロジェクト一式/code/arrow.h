//*****************************************************
//
// 矢印の処理[arrow.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ARROW_H_
#define _ARROW_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CArrow : public CObject3D
{
public:
	CArrow(int nPriority = 6);	// コンストラクタ
	~CArrow();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CArrow *Create(D3DXVECTOR3 pos,float width,float height);

private:
};

#endif
