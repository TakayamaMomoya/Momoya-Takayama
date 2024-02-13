//*****************************************************
//
// スカイボックスの処理[skybox.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SKYBOX_H_
#define _SKYBOX_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSkybox : public CObjectX
{
public:
	CSkybox(int nPriority = 3);	// コンストラクタ
	~CSkybox();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSkybox *Create();

private:
};

#endif

