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
#include "object.h"
#include "motion.h"
#include <stdio.h>

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionCube;
class CCollisionSphere;
class CShadow;

//*****************************************************
// マクロ定義
//*****************************************************
#define INITIAL_LIFE_PLAYER	(100)	// 初期体力

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CObject
{
public:
	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	static CPlayer *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	void SetMove(D3DXVECTOR3 move) { m_info.move = move; }
	D3DXVECTOR3 GetMove(void) { return m_info.move; }
	void SetPositionOld(D3DXVECTOR3 pos) { m_info.posOld = pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_info.posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_info.rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_info.rot; }
	static CPlayer *GetInstance(void) { return m_pPlayer; }
	int GetLife(void) { return m_info.nLife; }
	int GetinitialLife(void) { return m_info.nInitialLife; }
	CMotion *GetBody(void) { return m_info.pBody; }

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_MAX
	};
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機状態
		MOTION_MOVE,	// 移動モーション
		MOTION_JUMP,	// ジャンプモーション
		MOTION_FALL,	// 落下モーション
		MOTION_ATTACK,	// 攻撃モーション
		MOTION_ATTACKTURN,	// 反転攻撃モーション
		MOTION_AIRATTACK,	// 空中攻撃モーション
		MOTION_PARRY,	// パリィモーション
		MOTION_DASH,	// ダッシュモーション
		MOTION_TITLE_NEUTRAL,	// タイトル待機
		MOTION_TITLE_STANDUP,	// タイトル立ち上がり
		MOTION_APPER,	// 出現モーション
		MOTION_MAX
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
	enum JUMPSTATE
	{// ジャンプ状態
		JUMPSTATE_NONE = 0,
		JUMPSTATE_NORMAL,
		JUMPSTATE_ATTACK,
		JUMPSTATE_MAX
	};
	struct SInfo
	{
		int nLife;	// 体力
		int nInitialLife;	// 初期体力
		int nCntAfterImage;	// 残像を出すカウンター
		int nCntState;	// 状態遷移カウンター
		int nTimeParry;	// パリィ継続時間
		int nCntParry;	// パリィのカウンター
		int nTimeDash;	// ダッシュのクールタイム
		int nCntDash;	// ダッシュのカウンター
		int nTimeDamage;	// ダメージ状態の時間
		float fSpeedDash;	// ダッシュの速度
		float fPowJump;	// ジャンプ力
		bool bSprint;	// ダッシュ状態かどうか
		bool bAttack;	// 攻撃フラグ
		float fRadiusParry;	// パリィ判定の半径
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 前回の位置
		D3DXVECTOR3 offsetParry;	// パリィ判定のオフセット
		D3DXVECTOR3 move;	// 移動量
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 rotDest;	// 目標の向き
		CMotion *pBody;	// 体のポインタ
		CShadow *pShadow;	// 影のポインタ
		CCollisionCube *pCollisionCube;	// 立方体の当たり判定
		CCollisionSphere *pClsnAttack;	// 攻撃の当たり判定
		CCollisionSphere *pClsnHit;	// 被弾当たり判定
		int nNumAttack;	// 攻撃判定の数
		AttackInfo *pAttackInfo;	// 攻撃の情報ポインタ
		STATE state;	// 状態
		JUMPSTATE jump;	// ジャンプ状態
	};

	void Load(void);
	void LoadParam(FILE *pFile,char *pTemp);
	void ManageState(void);
	void Input(void);
	void InputMove(void);
	void InputAttack(void);
	void Parry(void);
	void InputCamera(void);
	void ManageMotion(void);
	void ManageCollision(void);
	void RotDest(void);
	void ManageAttack(void);
	void Death(void);
	void SetMotion(MOTION motion);

	SInfo m_info;	// 自身の情報

	static CPlayer *m_pPlayer;	// 自身のポインタ
};

#endif