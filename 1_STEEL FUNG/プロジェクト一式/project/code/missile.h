//*****************************************************
//
// �~�T�C���̏���[enemymissile.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "enemy.h"

//*****************************************************
// �O���錾
//*****************************************************
class CMissileBehavior;
class COrbit;
class CBoostEffect;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMissile : public CEnemy
{
public:
	enum TYPE
	{
		TYPE_LINE = 0,	// �����U��
		TYPE_RIGHTANGLE,	// ���p�U��
		TYPE_MAX
	};
	CMissile();	// �R���X�g���N�^
	~CMissile();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMissile *Create(D3DXVECTOR3 pos,TYPE type = TYPE::TYPE_LINE);
	void Hit(float fDamage) { Death(); }
	void HitField(void) override;
	void SetChaseSpeed(float fSpeed) { m_fChaseSpeed = fSpeed; }
	float GetTimerHit(void) { return m_fTimerHit; }
	void SetTimerHit(float fTimer) { m_fTimerHit = fTimer; }
	float GetSpeedChase(void) { return m_fChaseSpeed; }
	void SetSpeedChase(float fSpeed) { m_fChaseSpeed = fSpeed; }
	float GetSpeedMax(void) { return m_fSpeedMax; }
	void SetSpeedMax(float fSpeed) { m_fSpeedMax = fSpeed; }

private:
	void ChasePlayer(void);	// �ǐՏ���
	void Tilt(void);	// �X������
	void Death(void);	// ���S����

	float m_fTimerHit;	// ���e�^�C�}�[
	int m_nDeathTimer;	// ���S�J�E���^�[
	bool m_bEndChase;	// �ǔ��I���t���O
	float m_fChaseSpeed;	// �ǔ����\
	float m_fSpeedMax;	// �ő呬�x
	CMissileBehavior *m_pBehavior;	// �s���̃|�C���^
	COrbit *m_pOrbit;	// �O��
	CBoostEffect *m_pFire;	// ���ˉ�
};

#endif
