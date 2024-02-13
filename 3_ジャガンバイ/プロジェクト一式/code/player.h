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
#include "characterDiv.h"
#include "weapon.h"
#include "itemRepair.h"

//*****************************************************
// �O���錾
//*****************************************************
class CMotion;
class CCollisionSphere;
class CCollisionCube;
class CArrow;
class CObject3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayer : public CCharacterDiv
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_BLOW,	// ������΂���Ă�����
		STATE_DAMAGE,	// �_���[�W���
		STATE_DEATH,	// ���S���
		STATE_MAX
	};
	struct SParam
	{
		float fSpeedMove;		// �ړ����x
		float fSpeedRot;		// ��]���x
		float fInitialLife;	// �����̗�
		float fTimeDamage;	// �_���[�W��Ԃ̕b��
		float fPowBlow;	// �G�𐁂���΂���
		float fDamagePunch;	// �p���`�̈З�
	};
	struct AttackInfo
	{// �U���̏��
		int nIdxMotion;	// ���[�V�����ԍ�
		int nIdxParent;	// �e�p�[�c�ԍ�
		D3DXVECTOR3 pos;	// �I�t�Z�b�g�ʒu
		int nKey;	// �L�[�ԍ�
		int nFrame;	// ��������t���[��
		float fRadius;	// ���a
	};

	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

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
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_WALK_FRONT,	// �O��
		MOTION_WALK_BACK,	// ������
		MOTION_WALK_RIGHT,	// �E����
		MOTION_WALK_LEFT,	// ������
		MOTION_DEAD,		// ���S
		MOTION_PUNCH,		// ����

		MOTION_MAGNUM_NEUTRAL,		// �}�O�i�������ҋ@
		MOTION_MAGNUM_WALK_FRONT,	// �}�O�i�������O��
		MOTION_MAGNUM_WALK_BACK,	// �}�O�i������������
		MOTION_MAGNUM_WALK_RIGHT,	// �}�O�i�������E����
		MOTION_MAGNUM_WALK_LEFT,	// �}�O�i������������
		MOTION_MAGNUM_ATTACK,		// �}�O�i���U��

		MOTION_RAILGUN_NEUTRAL,		// ���[���K�������ҋ@
		MOTION_RAILGUN_WALK_FRONT,	// ���[���K�������O��
		MOTION_RAILGUN_WALK_BACK,	// ���[���K������������
		MOTION_RAILGUN_WALK_RIGHT,	// ���[���K�������E����
		MOTION_RAILGUN_WALK_LEFT,	// ���[���K������������
		MOTION_RAILGUN_ATTACK,		// ���[���K���U��

		MOTION_RIFLE_NEUTRAL,		// ���C�t�������ҋ@
		MOTION_RIFLE_WALK_FRONT,	// ���C�t�������O��
		MOTION_RIFLE_WALK_BACK,		// ���C�t������������
		MOTION_RIFLE_WALK_RIGHT,	// ���C�t�������E����
		MOTION_RIFLE_WALK_LEFT,		// ���C�t������������
		MOTION_RIFLE_ATTACK,		// ���C�t���U��

		MOTION_OPEN_DOOR,			// �h�A�J��
		MOTION_ITEM_PICK_UP,		// �����E��

		MOTION_ITEM_NEUTRAL,		// �������ҋ@
		MOTION_ITEM_WALK_FRONT,		// �������O��
		MOTION_ITEM_WALK_BACK,		// ������������
		MOTION_ITEM_WALK_RIGHT,		// �������E����
		MOTION_ITEM_WALK_LEFT,		// ������������

		MOTION_EMOTE00,		// �ŗL�G���[�g
		MOTION_EMOTE01,		// �ŗL�G���[�g
		MOTION_EMOTE02,		// �ŗL�G���[�g
		MOTION_EMOTE_UNIQUE,		// �ŗL�G���[�g

		MOTION_MAX
	};
	struct SMotionInfo
	{
		bool bDoorPress;		// �h�A�ւ̓��͏��
		bool bItemTrigger;		// ���ւ̓��͏��
		bool bPunch;	// �p���`�̓��͏��
		bool bEmote;	// �G���[�g
		float bRunawayProtect;	// �\�����͖h�~
	};
	struct SInfo
	{
		int nID;	// �ԍ�
		int nIDJoypad;	// �R���g���[���[�̔ԍ�
		float fLife;	// �̗�
		float fTimerState;	// ��ԑJ�ڃJ�E���^
		STATE state;	// ���
		SMotionInfo motionInfo;		// ���[�V�������
		int nNumAttack;	// �U������̐�
		AttackInfo *pAttackInfo;	// �U���̏��|�C���^
		CCollisionSphere *pCollisionSphere;	// ���̓����蔻��
		CCollisionSphere *pClsnAttack;	// �U���̓����蔻��
		CCollisionCube *pCollisionCube;	// �����̂̓����蔻��
		CWeapon* pWeapon;			// ����
		CItemRepair* pItemRepair;	// �C���A�C�e��
		CArrow *pArrow;	// ���̃|�C���^
		CObject3D *pLifeFrame;	// ���C�t�\���̃t���[��
		CObject3D *pLife;	// ���C�t�\��
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

	SInfo m_info;	// ���g�̏��
	SParam m_param;	// �p�����[�^�[���

	bool m_bSound;
	bool m_bGoalJump;
};

#endif