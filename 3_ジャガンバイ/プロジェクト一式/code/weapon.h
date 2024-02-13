//*****************************************************
//
// 武器の処理[weapon.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _WEAPON_H_
#define _WEAPON_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;

//*****************************************************
// クラスの定義
//*****************************************************
class CWeapon : public CObjectX
{
public:
	enum TYPE
	{
		TYPE_MAGNUM = 0,	// マグナム
		TYPE_MACHINEGUN,	// マシンガン
		TYPE_SHOTGUN,	// ショットガン
		TYPE_RAILGUN,	// レールガン
		TYPE_MINIGUN,	// ミニガン
		TYPE_LAUNCHER,	// ランチャー
		TYPE_MAX
	};
	struct SInfo
	{// 自身の情報
		int nNumBullet;	// 弾数
		int nMaxBullet;	// 最大弾数
		int nRapid;	// 連射
		float fDamage;	// 威力
		float fLifeBullet;	// 弾の寿命
		int nCntShot;	// 連射カウンタ
		int nIdxHand;	// 手の番号
		int nIdxJoypad;	// コントローラー番号
		float fVibPower;	// コントローラーの振動の強さ
		int nVibTime;		// コントローラーの振動時間
		bool bEnable;	// 使用できるかどうか
		CPlayer *pPlayer;	// 所持してるプレイヤー
		TYPE type;	// 種類
	};

	CWeapon(int nPriority = 3);	// コンストラクタ
	~CWeapon();	// デストラクタ

	static CWeapon *Create(CWeapon::TYPE type,int nIdxhand);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void Attack(void) {};
	CPlayer *GetPlayer(void) { return m_info.pPlayer; }
	void SetPlayer(CPlayer *pPlayer);
	int GetID(void) { return m_info.nIdxJoypad; }
	void SetBullet(int nBullet);
	int GetBullet(void) { return m_info.nNumBullet; }
	int GetMaxBullet(void) { return m_info.nMaxBullet; }
	void SetMaxBullet(int nBullet);
	void SetRapid(int nRapid) { m_info.nRapid = nRapid; }
	int GetRapid(void) { return m_info.nRapid; }
	void SetCntShot(int nCntShot) { m_info.nCntShot = nCntShot; }
	int GetCntShot(void) { return m_info.nCntShot; }
	void FollowPlayerHand(void);
	void SetVibPower(float fVib) { m_info.fVibPower = fVib; }
	void SetVibTime(int nTime) { m_info.nVibTime = nTime; }
	void SetEnable(bool bEnable);
	bool IsEnable(void) { return m_info.bEnable; }
	void SetDamage(float fDamage) { m_info.fDamage = fDamage; }
	float GetDamage(void) { return m_info.fDamage; }
	void SetBulletLife(float fBulletLife) { m_info.fLifeBullet = fBulletLife; }
	TYPE GetType(void) { return m_info.type; }
	SInfo GetInfo(void) { return m_info; }
	void SetInfo(SInfo info) { m_info = info; }

protected:
	virtual void ToggleEnable(bool bEnable) {};

private:
	void Break(void);

	SInfo m_info;
};

#endif

