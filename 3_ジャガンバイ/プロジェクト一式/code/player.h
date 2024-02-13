//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "characterDiv.h"
#include "weapon.h"
#include "itemRepair.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMotion;
class CCollisionSphere;
class CCollisionCube;
class CArrow;
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CCharacterDiv
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_BLOW,	// 吹き飛ばされている状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX
	};
	struct SParam
	{
		float fSpeedMove;		// 移動速度
		float fSpeedRot;		// 回転速度
		float fInitialLife;	// 初期体力
		float fTimeDamage;	// ダメージ状態の秒数
		float fPowBlow;	// 敵を吹き飛ばす力
		float fDamagePunch;	// パンチの威力
	};
	struct AttackInfo
	{// 攻撃の情報
		int nIdxMotion;	// モーション番号
		int nIdxParent;	// 親パーツ番号
		D3DXVECTOR3 pos;	// オフセット位置
		int nKey;	// キー番号
		int nFrame;	// 発生するフレーム
		float fRadius;	// 半径
	};

	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	static CPlayer *Create(int nID);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetWeapon(CWeapon::TYPE type);
	CWeapon* GetWeapon(void) { return m_info.pWeapon; }
	void SetItemRepair(CItemRepair* itemRepair) { m_info.pItemRepair = itemRepair; }
	void SetDoorPress(bool bDoorPress) { m_info.motionInfo.bDoorPress = bDoorPress; }
	void SetItemTrigger(bool bItemTrigger) { m_info.motionInfo.bItemTrigger = bItemTrigger; }
	void SetID(int nID) { m_info.nID = nID; }
	void SetIDJoypad(int nID) { m_info.nIDJoypad = nID; }
	int GetIDJoypad(void) { return m_info.nIDJoypad; }
	int GetID(void) { return m_info.nID; }
	bool GetDoorPress(void) { return m_info.motionInfo.bDoorPress; }
	bool GetItemTrigger(void) { return m_info.motionInfo.bItemTrigger; }
	void ReleaseWeapon(void) { m_info.pWeapon = nullptr; }
	void ReleaseItemRepair(void) { m_info.pItemRepair = nullptr; }
	void Hit(float fDamage);
	bool InputInteract(void);
	bool InputInteractPress(void);
	void EnableWeapon(bool bEnable);
	void SetState(STATE state) { m_info.state = state; }
	float GetLife(void) { return m_info.fLife; }
	void SetPosition(D3DXVECTOR3 pos);

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_WALK_FRONT,	// 前歩
		MOTION_WALK_BACK,	// 後ろ歩き
		MOTION_WALK_RIGHT,	// 右歩き
		MOTION_WALK_LEFT,	// 左歩き
		MOTION_DEAD,		// 死亡
		MOTION_PUNCH,		// 殴り

		MOTION_MAGNUM_NEUTRAL,		// マグナム持ち待機
		MOTION_MAGNUM_WALK_FRONT,	// マグナム持ち前歩
		MOTION_MAGNUM_WALK_BACK,	// マグナム持ち後ろ歩き
		MOTION_MAGNUM_WALK_RIGHT,	// マグナム持ち右歩き
		MOTION_MAGNUM_WALK_LEFT,	// マグナム持ち左歩き
		MOTION_MAGNUM_ATTACK,		// マグナム攻撃

		MOTION_RAILGUN_NEUTRAL,		// レールガン持ち待機
		MOTION_RAILGUN_WALK_FRONT,	// レールガン持ち前歩
		MOTION_RAILGUN_WALK_BACK,	// レールガン持ち後ろ歩き
		MOTION_RAILGUN_WALK_RIGHT,	// レールガン持ち右歩き
		MOTION_RAILGUN_WALK_LEFT,	// レールガン持ち左歩き
		MOTION_RAILGUN_ATTACK,		// レールガン攻撃

		MOTION_RIFLE_NEUTRAL,		// ライフル持ち待機
		MOTION_RIFLE_WALK_FRONT,	// ライフル持ち前歩
		MOTION_RIFLE_WALK_BACK,		// ライフル持ち後ろ歩き
		MOTION_RIFLE_WALK_RIGHT,	// ライフル持ち右歩き
		MOTION_RIFLE_WALK_LEFT,		// ライフル持ち左歩き
		MOTION_RIFLE_ATTACK,		// ライフル攻撃

		MOTION_OPEN_DOOR,			// ドア開け
		MOTION_ITEM_PICK_UP,		// 物を拾う

		MOTION_ITEM_NEUTRAL,		// 物持ち待機
		MOTION_ITEM_WALK_FRONT,		// 物持ち前歩
		MOTION_ITEM_WALK_BACK,		// 物持ち後ろ歩き
		MOTION_ITEM_WALK_RIGHT,		// 物持ち右歩き
		MOTION_ITEM_WALK_LEFT,		// 物持ち左歩き

		MOTION_EMOTE00,		// 固有エモート
		MOTION_EMOTE01,		// 固有エモート
		MOTION_EMOTE02,		// 固有エモート
		MOTION_EMOTE_UNIQUE,		// 固有エモート

		MOTION_MAX
	};
	struct SMotionInfo
	{
		bool bDoorPress;		// ドアへの入力情報
		bool bItemTrigger;		// 物への入力情報
		bool bPunch;	// パンチの入力情報
		bool bEmote;	// エモート
		float bRunawayProtect;	// 暴走入力防止
	};
	struct SInfo
	{
		int nID;	// 番号
		int nIDJoypad;	// コントローラーの番号
		float fLife;	// 体力
		float fTimerState;	// 状態遷移カウンタ
		STATE state;	// 状態
		SMotionInfo motionInfo;		// モーション情報
		int nNumAttack;	// 攻撃判定の数
		AttackInfo *pAttackInfo;	// 攻撃の情報ポインタ
		CCollisionSphere *pCollisionSphere;	// 球の当たり判定
		CCollisionSphere *pClsnAttack;	// 攻撃の当たり判定
		CCollisionCube *pCollisionCube;	// 立方体の当たり判定
		CWeapon* pWeapon;			// 武器
		CItemRepair* pItemRepair;	// 修理アイテム
		CArrow *pArrow;	// 矢印のポインタ
		CObject3D *pLifeFrame;	// ライフ表示のフレーム
		CObject3D *pLife;	// ライフ表示
	};

	void Input(void);
	void InputMove(void);
	void InputAttack(void);
	void InputEmote(void);
	void Aim(void);
	void ManageState(void);
	void ManageMotion(void);
	void ManageAttack(void);
	void ManageLife(void);
	void ResizeLife(void);
	void BlowEnemy(CObject *pObj);
	void BlowPlayer(CObject *pObj);
	void DispLife(void);
	void DeleteLife(void);
	void CheckDeathEnemy(CObject* pObj);
	void LimidPostion();
	void BoardingRocket(void);
	void Debug(void);

	SInfo m_info;	// 自身の情報
	SParam m_param;	// パラメーター情報

	bool m_bSound;
	bool m_bGoalJump;
};

#endif