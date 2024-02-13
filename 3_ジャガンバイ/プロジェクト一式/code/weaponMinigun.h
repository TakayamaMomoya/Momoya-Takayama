//*****************************************************
//
// ミニガンの処理[weaponMinigun.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MINIGUN_H_
#define _MINIGUN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "weapon.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CMinigun : public CWeapon
{
public:
	CMinigun(int nPriority = 3);	// コンストラクタ
	~CMinigun();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
};

#endif

