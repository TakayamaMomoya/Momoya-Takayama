//*****************************************************
//
// 煙の処理[smoke.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SMOKE_H_
#define _SMOKE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"

//*****************************************************
// クラス定義
//*****************************************************
class CSmoke : public CObject3D
{
public:
	CSmoke(int nPriority = 5);	// コンストラクタ
	~CSmoke();	// デストラクタ

	static CSmoke *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_move;	// 移動量
	bool m_bTurn;	// 反転
};

#endif