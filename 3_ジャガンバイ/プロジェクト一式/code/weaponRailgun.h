//*****************************************************
//
// レールガンの処理[weaponRailgun.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _RAILGUN_H_
#define _RAILGUN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "weapon.h"

//*****************************************************
// 前方定義
//*****************************************************
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CRailgun : public CWeapon
{
public:
	struct SInfoRailgun
	{
		float fWidth;	// 判定の幅
		float fLength;	// 判定の長さ
	};

	CRailgun(int nPriority = 3);	// コンストラクタ
	~CRailgun();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
	void Shot(void);
	void SetVtx(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4);
	void ToggleEnable(bool bEnable);

	SInfoRailgun m_info;
	CObject3D *m_pRange;	// 範囲表示のポリゴン
};

#endif

