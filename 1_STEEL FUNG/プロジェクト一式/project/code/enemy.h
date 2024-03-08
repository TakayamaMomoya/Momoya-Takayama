//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CBlock;
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemy : public CMotion
{
public:
	enum TYPE
	{// �G�̎��
		TYPE_NONE = 0,	// ���ł��Ȃ�
		TYPE_NORMAL,	// �ʏ�G
		TYPE_BOMB,	// ���e�G
		TYPE_DRONE,	// �h���[���G
		TYPE_TUTORIAL,	// �`���[�g���A���G
		TYPE_BOSS,		// �{�X�G
		TYPE_MAX
	};
	enum STATE
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_THROWN,	// ������ꂽ���
		STATE_DAMAGE,	// �_���[�W���
		STATE_DEATH,	// ���S���
		STATE_MAX
	};
	enum MOVESTATE
	{// �ړ����
		MOVESTATE_NONE = 0,	// ���ł��Ȃ����
		MOVESTATE_WAIT,	// �ҋ@���
		MOVESTATE_CHASE,	// �ǐՏ��
		MOVESTATE_ATTACK,	// �U�����
		MOVESTATE_MAX
	};

	CEnemy();	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	static CEnemy *Create(D3DXVECTOR3 pos,TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }
	void SetLife(float fLife,bool bInit = false);
	float GetLife(void) { return m_info.fLife; }
	float GetLifeInitial(void) { return m_info.fLifeInitial; }
	void Hit(float fDamage);
	void SetSpherePosition(D3DXVECTOR3 pos);
	STATE GetState(void) { return m_info.state; }
	void SetState(STATE state) { m_info.state = state; }
	CCollisionSphere *GetClsnSphere(void) { return m_info.pCollisionSphere; }
	int GetCntState(void) { return m_info.nTimerState; }
	void SetCntState(int nCnt) { m_info.nTimerState = nCnt; }
	float GetSpeed(void) { return m_info.fMoveSpeed; }
	void SetMoveSpeed(float fSpeed) { m_info.fMoveSpeed = fSpeed; }
	void SetMoveState(MOVESTATE state) { m_info.moveState = state; }
	MOVESTATE GetMoveState(void) { return m_info.moveState; }
	void SetPosDest(D3DXVECTOR3 pos) { m_info.posDest = pos; }
	void CreateCollision(float fRadius = 90.0f);
	void DeleteCollision(void);
	void EnableLock(bool bLock);
	void SetPositionCursor(D3DXVECTOR3 pos);
	void SetDistMoveState(float fValue, MOVESTATE moveState) { m_info.aDistMoveState[moveState] = fValue; }
	bool AttackTimer(float fTime);
	void SetDistLock(float fDist) { m_info.fDistLock = fDist; }
	float GetDistLock(void) { return m_info.fDistLock; }
	bool IsStamp(void) { return m_info.bStamp; }
	void EnableStamp(bool bStamp) { m_info.bStamp = bStamp; }
	void SetMovefact(float fFact) { m_info.fFactMove = fFact; }
	void SetType(TYPE type) { m_info.type = type; }
	TYPE GetType(void) { return m_info.type; }

protected:
	void ManageScore(void);
	virtual void Wait(void);
	virtual void Chase(void);
	virtual void Attack(void);
	virtual void Death(void);
	virtual void TransferChase(void);
	virtual void HitField(void);
	bool MoveToDest(D3DXVECTOR3 posDest,float fSpeed);

private:
	struct SInfo
	{
		float fLife;	// �̗�
		float fLifeInitial;	// �����̗�
		float fMoveSpeed;	// �ړ����x
		int nTimerState;	// ��ԑJ�ڃJ�E���^�[
		float fCntAttack;	// �U���^�C�}�[
		float fRadius;	// ���蔼�a
		CCollisionSphere *pCollisionSphere;	// ���̓����蔻��
		CCollisionCube *pCollisionCube;	// �����̂̓����蔻��
		STATE state;	// ���
		MOVESTATE moveState;	// �ړ����
		D3DXVECTOR3 posDest;	// �ڕW�ʒu
		CUI *pCursor;	// �ߑ��\�J�[�\��
		float fDistLock;	// ���b�N�\����
		float aDistMoveState[MOVESTATE_MAX];	// �ړ���Ԃ�����鋗��
		bool bStamp;	// ���߂邩�ǂ���
		float fFactMove;	// �ړ������W��
		TYPE type;	// ���
	};
	void ManageState(void);
	void ManageMoveState(void);
	void ManageCollision(void);
	bool IsInArea(void);
	void CollisionThrown(void);

	static int m_nNumAll;	// ����
	SInfo m_info;	// ���
};

#endif