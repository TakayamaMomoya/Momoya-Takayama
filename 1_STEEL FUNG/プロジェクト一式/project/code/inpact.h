//*****************************************************
//
// 衝撃波の処理[inpact.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _INPACT_H_
#define _INPACT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "meshcylinder.h"

//*****************************************************
// クラス定義
//*****************************************************
class CInpact : public CMeshCylinder
{
public:
	CInpact(int nPriority = 5);	// コンストラクタ
	~CInpact();	// デストラクタ
	static CInpact *Create(float nLife = 0.4f,D3DXMATRIX *pMtx = nullptr);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSpeedExpand(float fSpeed) { m_info.fSpeedExpand = fSpeed; }

private:
	struct SInfo
	{
		float fRadiusDiff;	// 半径にどれだけ差をつけるか
		float fLife;	// 寿命
		float fLifeInitial;	// 初期寿命
		float fSpeedExpand;	// 膨らむ速度
	};

	void SetVtx(void);
	void ManageLife(void);

	SInfo m_info;
};
#endif