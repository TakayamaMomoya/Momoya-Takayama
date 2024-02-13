//*****************************************************
//
// レーダーの処理[rader.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "meshcylinder.h"

//*****************************************************
// クラス定義
//*****************************************************
class CShockWave : public CMeshCylinder
{
public:
	CShockWave(void);	// コンストラクタ
	~CShockWave();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CShockWave *Create(void);

private:
	void Wave(void);
};
#endif