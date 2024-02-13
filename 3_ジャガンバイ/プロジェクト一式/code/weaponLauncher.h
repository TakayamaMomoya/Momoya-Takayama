//*****************************************************
//
// ランチャーの処理[weaponLauncher.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _LAUNCHER_H_
#define _LAUNCHER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "weapon.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObjectX;

//*****************************************************
// クラスの定義
//*****************************************************
class CLauncher : public CWeapon
{
public:
	CLauncher(int nPriority = 3);	// コンストラクタ
	~CLauncher();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
};

#endif