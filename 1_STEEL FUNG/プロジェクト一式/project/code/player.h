//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CObject3D;
class CEnemy;
class CHeat;
class CBoostEffect;
class COrbit;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayer : public CMotion
{
public:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_WALK_FRONT,	// �O��
		MOTION_SHOT,	// �ˌ�
		MOTION_JUMP,	// �W�����v
		MOTION_AIR,	// �؋�
		MOTION_ASSAULT,	// �ːi���
		MOTION_MELEE,	// �ߐڍU��
		MOTION_MELEE2,	// �Q��ڂ̋ߐڍU��
		MOTION_STOP,	// �}��~���[�V����
		MOTION_GRAB,	// �͂݃��[�V����
		MOTION_THROW,	// �������[�V����
		MOTION_STAMP,	// ���݂����[�V����
		MOTION_DEATH,	// ���S���[�V����
		MOTION_NEUTRAL_TITLE,	// �^�C�g���ҋ@
		MOTION_LAUNCH,	// �o��
		MOTION_DODGE,	// ���
		MOTION_APPER,	// �o��
		MOTION_MAX
	};
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_BLOW,	// ������΂���Ă�����
		STATE_DAMAGE,	// �_���[�W���
		STATE_DEATH,	// ���S���
		STATE_MAX
	};
	enum STATEBOOST
	{
		STATEBOOST_NONE = 0,	// ���ł��Ȃ����
		STATEBOOST_NORMAL,	// �ʏ���
		STATEBOOST_OVERHEAT,	// �I�[�o�[�q�[�g���
		STATEBOOST_MAX
	};
	enum PARAM
	{
		PARAM_GUN = 0,	// �e�̔M
		PARAM_MELEE,	// �ߐڕ���̔M
		PARAM_HAND,	// �r�̔M
		PARAM_MAX
	};
	struct SParam
	{
		float fSpeedMove;		// �ړ����x
		float fInitialBoost;		// �u�[�X�g�c�ʏ����l
		float fInitialLife;	// �����̗�
	};

	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return m_pPlayer; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);
	bool IsTargetLock(void) { return m_info.bLockTarget; }
	void EnableLock(bool bLock) { m_info.bLockTarget = bLock; }
	float GetBoost(void) { return m_info.fBoost; }
	float GetLife(void) { return m_info.fLife; }
	SParam GetParam(void) { return m_param; }
	STATEBOOST GetStateBoost(void) { return m_info.stateBoost; }
	bool IsLock(void) { return m_info.bLockTarget; }

private:
	struct SFragMotion
	{
		bool bMove;	// �ړ�
		bool bJump;	// �W�����v
		bool bDodge;	// ���
		bool bStamp;	// ���݂�
		bool bShot;	// �ˌ�
		bool bMelee;	// �ߐڍU��
		bool bAddAttack;	// �ǉ��U��
		bool bGrab;	// �͂�
		bool bAir;	// �؋�
		bool bStop;	// �}��~
	};
	struct SInfoThruster
	{
		CBoostEffect *pFire;	// ���ˉ�
		int nIdxParent;	// �e�p�[�c�ԍ�
		D3DXVECTOR3 offset;	// �I�t�Z�b�g
		D3DXVECTOR3 vector;	// ���˃x�N�g��
		D3DXVECTOR2 size;	// �T�C�Y
		int nRot;	// ����ID
	};
	struct SInfo
	{
		float fLife;	// �̗�
		float fBoost;	// �u�[�X�g�c��
		float aParam[PARAM_MAX];	// �p�����[�^
		bool aHeat[PARAM_MAX];	// �I�[�o�[�q�[�g�t���O
		CHeat *apHeatUI[PARAM_MAX];	// �p�����[�^
		STATE state;	// ���
		STATEBOOST stateBoost;	// �u�[�X�g�̏��
		CCollisionSphere *pCollisionSphere;	// ���̓����蔻��
		CCollisionSphere *pClsnAttack;	// �U���̓����蔻��
		CCollisionCube *pCollisionCube;	// �����̂̓����蔻��
		SInfoThruster *pThruster;	// �X���X�^�[���
		int nNumThruster;	// �X���X�^�[�̐�
		bool bLockTarget;	// �^�[�Q�b�g���b�N���邩�ǂ���
		bool bLand;	// ���n���Ă��邩�ǂ���
		bool bMelee;	// �ߐڍU���������������ǂ���
		CEnemy *pEnemyGrab;	// �͂�ł���G
		D3DXVECTOR3 rotDest;	// �ڕW�̌���
		COrbit *pOrbitWeapon;	// ����̋O��
	};

	void Load(void);
	void Lockon(void);
	void Input(void);
	void InputMove(void);
	void Stamp(void);
	void InputCamera(void);
	void SwitchLockEnemy(void);
	void InputAttack(void);
	void Rotation(void);
	void ManageCollision(void);
	void ManageState(void);
	void ManageMotion(void);
	void ManageParam(void);
	void Boost(void);
	void StartMelee(void);
	void AddMoveForward(float fSpeed);
	void AddMoveUp(float fSpeed);
	void Event(EVENT_INFO *pEventInfo);
	void Shot(D3DXVECTOR3 posMazzle);
	void ManageAttack(D3DXVECTOR3 pos,float fRadius);
	CEnemy *GetLockOn(void);
	void EndMelee(void);
	void AddBoost(float fValue);
	void ToggleLockTarget(void);
	void AddMoveStamp(void);
	void Death(void);
	void Debug(void);

	SInfo m_info;	// ���g�̏��
	SParam m_param;	// �p�����[�^���
	SFragMotion m_fragMotion;	// ���[�V�����̃t���O

	static CPlayer *m_pPlayer;	// ���g�̃|�C���^
};

#endif