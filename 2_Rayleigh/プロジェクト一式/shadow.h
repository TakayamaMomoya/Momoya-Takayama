//*****************************************************
//
// 影の処理[shadow.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CShadow : public CObject3D
{
public:
	CShadow(int nPriority = 6);	// コンストラクタ
	~CShadow();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CShadow *Create(D3DXVECTOR3 pos,float width,float height);

private:
};

#endif
