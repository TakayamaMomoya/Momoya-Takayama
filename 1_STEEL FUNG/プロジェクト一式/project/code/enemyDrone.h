//*****************************************************
//
// �ʏ�G�̏���[enemyNormal.h]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYDRONE_H_
#define _ENEMYDRONE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyDrone : public CEnemy
{
public:
	CEnemyDrone();	// �R���X�g���N�^
	~CEnemyDrone();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPositionDest(D3DXVECTOR3 pos) { m_posDest = pos; }
	void HitField(void) override;

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// �ҋ@���[�V����
		MOTION_WALK,	// �������[�V����
		MOTION_ATTACK,	// �U�����[�V����
		MOTION_DEATH,	// ���S���[�V����
		MOTION_MAX
	};

	virtual void Wait(void);
	virtual void Chase(void);
	virtual void Attack(void);

	float m_fTimeFire;	// �ˌ��^�C�}�[
	D3DXVECTOR3 m_posDest;	// �ڕW�ʒu
	float m_fLengthDest;	// �ڕW�ʒu�̋���
	float m_fAngleDest;	// �ڕW�ʒu�̊p�x
};

#endif
