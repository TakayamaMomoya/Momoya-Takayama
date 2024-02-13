//*****************************************************
//
// ショットガンの処理[weaponShotgun.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SHOTGUN_H_
#define _SHOTGUN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "weapon.h"
#include "weaponManager.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CShotgun : public CWeapon
{
public:
	CShotgun(int nPriority = 3);	// コンストラクタ
	~CShotgun();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
	float m_fAngleDiffuse;	// 拡散する角度
	int m_nNumPellet;	// 一度に発射する弾の数
};

#endif

