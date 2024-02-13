//*****************************************************
//
// �D�_�G�̏���[enemyThief.h]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYThief_H_
#define _ENEMYThief_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyThief : public CEnemy
{
public:
	CEnemyThief();	// �R���X�g���N�^
	~CEnemyThief();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum STATE
	{
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_CHASE,	// �ǐՏ��
		STATE_ESCAPE,	// �������
		STATE_MAX
	};
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// �ҋ@���[�V����
		MOTION_WALK,	// �������[�V����
		MOTION_DEATH,	// ���S���[�V����
		MOTION_DANCE,	// �_���X���[�V����
		MOTION_MAX
	};
	void SwitchUpdate(void);
	void ChaseTarget(void);
	void CollisionRocket(void);
	void Escape(void);
	void Death(void);
	void LimidPostion(void);

	bool m_bTakeRepair;
	STATE m_state;
	CObject3D *m_pCaution;
	bool m_bSound;
};

#endif
