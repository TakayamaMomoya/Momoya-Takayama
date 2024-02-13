//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "character.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CArrow;
class CBlock;

//*****************************************************
// �}�N����`
//*****************************************************
#define LOCKON_RANGE	(4000.0f)	// ���b�N�I������
#define AIM_LOCKON_RANGE	(LOCKON_RANGE * 2.0f)	// �G�C�����̃��b�N�I������

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemy : public CCharacter
{
public:
	enum TYPE
	{// �G�̎��
		TYPE_NONE = 0,	// ���ł��Ȃ�
		TYPE_NORMAL,	// �ʏ�G
		TYPE_THIEF,		// �D�_�G
		TYPE_MAX
	};

	enum STATE
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_DAMAGE,	// �_���[�W���
		STATE_DEATH,	// ���S���
		STATE_MAX
	};

	enum MOVESTATE
	{// �ړ����
		MOVESTATE_NONE = 0,	// ���ł��Ȃ����
		MOVESTATE_INTRUSION,	// �N�����悤�Ƃ��Ă���
		MOVESTATE_CHASE,	// �ǐՏ��
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
	void SetLife(float fLife);
	float GetLife(void) { return m_fLife; }
	void Hit(float fDamage);
	void SetSpherePosition(D3DXVECTOR3 pos);
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }
	CCollisionSphere *GetClsnSphere(void) { return m_pCollisionSphere; }
	CEnemy *GetNext(void) { return m_pNext; }
	int GetCntState(void) { return m_nTimerState; }
	void SetCntState(int nCnt) { m_nTimerState = nCnt; }
	void DeleteCollision(void);
	float GetSpeed(void) { return m_fMoveSpeed; }
	void SetMoveSpeed(float fSpeed) { m_fMoveSpeed = fSpeed; }
	MOVESTATE GetMoveState(void) { return m_moveState; }
	void SetPosDest(D3DXVECTOR3 pos) { m_posDest = pos; }

protected:
	CArrow *GetShadow(void) { return m_pShadow; }
	void ManageScore(void);
	CBlock *GetTouchBlock(void) { return m_pBlock; }
	virtual void ChaseTarget(void);
	virtual void Death(void);
	virtual void TransferChase(void);

private:
	void ManageState(void);
	void ManageMoveState(void);
	void ManageCollision(void);
	bool IsInArea(void);

	static int m_nNumAll;	// ����
	float m_fLife;	// �̗�
	float m_fMoveSpeed;	// �ړ����x
	int m_nTimerState;	// ��ԑJ�ڃJ�E���^�[
	CCollisionSphere *m_pCollisionSphere;	// ���̓����蔻��
	CCollisionCube *m_pCollisionCube;	// �����̂̓����蔻��
	CArrow *m_pShadow;	// �e�̃|�C���^
	CBlock *m_pBlock;	// �u���b�N�̃|�C���^
	STATE m_state;	// ���
	MOVESTATE m_moveState;	// �ړ����
	D3DXVECTOR3 m_posDest;	// �ڕW�ʒu

	CEnemy *m_pPrev;	// �O�̃A�h���X
	CEnemy *m_pNext;	// ���̃A�h���X
};

#endif
