//*****************************************************
//
// 武器マネージャー[weaponManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _WEAPONMANAGER_H_
#define _WEAPONMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "weapon.h"
#include <stdio.h>

//*****************************************************
// クラスの定義
//*****************************************************
class CWeaponManager : public CObject
{
public:
	struct SInfoShotgun
	{
		float fAngleDiffuse;	// 拡散角度
		int nNumPellet;	// 一度に発射する弾の数
	};
	struct SInfoRailgun
	{
		float fWidth;	// 幅
		float fLength;	// 長さ
	};

	CWeaponManager();	// コンストラクタ
	~CWeaponManager();	// デストラクタ

	static CWeaponManager*Create(void);
	HRESULT Init(void);
	void LoadParam(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CWeapon::SInfo GetBaseInfo(int nCnt) { return m_aInfo[nCnt]; }
	SInfoShotgun GetShotgunInfo(void) { return m_infoShotgun; }
	SInfoRailgun GetRailgunInfo(void) { return m_infoRailgun; }
	int GetProb(int nIdx) { return m_anProb[nIdx]; }
	static CWeaponManager*GetInstance(void) { return m_pWeaponManager; }

private:
	void LoadBaseInfo(FILE *pFile, char* pTemp, int nCntParam);
	void LoadShotgunInfo(FILE *pFile, char* pTemp);
	void LoadRailgunInfo(FILE *pFile, char* pTemp);

	CWeapon::SInfo m_aInfo[CWeapon::TYPE_MAX];	// 基底のパラメーター
	SInfoShotgun m_infoShotgun;	// ショットガンの情報
	SInfoRailgun m_infoRailgun;	// レールガンの情報
	int m_anProb[CWeapon::TYPE_MAX];	// 確率の配列

	static CWeaponManager *m_pWeaponManager;	// 自身のポインタ
};

#endif
