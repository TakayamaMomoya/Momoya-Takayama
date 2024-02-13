//*****************************************************
//
// マシンガンの処理[weaponMachinegun.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MACHINEGUN_H_
#define _MACHINEGUN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "weapon.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CMachinegun : public CWeapon
{
public:
	CMachinegun(int nPriority = 3);	// コンストラクタ
	~CMachinegun();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
};

#endif

