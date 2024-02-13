//*****************************************************
//
// マグナムの処理[weaponMagnum.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MAGNUM_H_
#define _MAGNUM_H_

//*****************************************************
// インクルード
//*****************************************************
#include "weapon.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CMagnum : public CWeapon
{
public:
	CMagnum(int nPriority = 3);	// コンストラクタ
	~CMagnum();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
};

#endif

