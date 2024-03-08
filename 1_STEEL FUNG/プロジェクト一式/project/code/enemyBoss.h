//*****************************************************
//
// �{�X�G�̏���[enemyboss.h]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYBOSS_H_
#define _ENEMYBOSS_H_

#include "enemy.h"

class CStateBoss;
class CCollisionSphere;
class CObject3D;

namespace Boss
{
const float RADIUS_COLLISION = 300.0f;	// �����蔻��̔��a
const float INITIAL_LIFE = 75.0f;	// �����̗�
}

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyBoss : public CEnemy
{
public:
	enum IDXPARTS
	{// �p�[�c�̔ԍ�
		IDX_WAIST = 0,	// ��
		IDX_BODY,	// ��
		IDX_HEAD,	// ��
		IDX_SHOULDER_L,	// ����
		IDX_ARM_L,	// ���r
		IDX_HAND_L,	// ����
		IDX_SHOULDER_R,	// �E��
		IDX_ARM_R,	// �E��
		IDX_HAND_R,	// �E��
		IDX_LEG_L,	// ����
		IDX_FOOT_L,	// ����
		IDX_TOE_L,	// ������
		IDX_LEG_R,	// �E��
		IDX_FOOT_R,	// �E��
		IDX_TOE_R,	// �E����
		IDX_WEAPON,	// ����
		IDX_MAX
	};
	enum MOTION
	{// ���[�V����
		MOTION_APPER = 0,	// �o�����[�V����
		MOTION_MISSILE,	// �~�T�C�����[�V����
		MOTION_DASH,	// �ːi���[�V����
		MOTION_SHOT,	// �ˌ����[�V����
		MOTION_SLASH,	// �a��
		MOTION_PRE_SLASH,	// �a���\��
		MOTION_DEATH,	// ���S���[�V����
		MOTION_BEAMSMALL,	// ���r�[��
		MOTION_JUMP,	// ��яオ��
		MOTION_RADIATION,	// ���M
		MOTION_PRE_AIRBEAM,	// �󒆃r�[���\������
		MOTION_LAST_SHOOTING,	// ���X�g�V���[�e�B���O���[�V����
		MOTION_AFTER_DEATH,	// ����
		MOTION_MAX
	};

	CEnemyBoss();	// �R���X�g���N�^
	~CEnemyBoss();	// �f�X�g���N�^

	static CEnemyBoss *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemyBoss *GetInstance(void) { return m_pEnemyBoss; }
	void Hit(float fDamage);
	void ChangeState(CStateBoss *pNext);
	void Event(EVENT_INFO *pEventInfo);
	void AimPlayer(float fSpeed = 500.0f, bool bPridict = true, float fFact = 0.15f);
	void AimPlayerFlat(float fSpeed = 500.0f, bool bPridict = true, float fFact = 0.15f, D3DXVECTOR3 rotAdd = { 0.0f,0.0f,0.0f });
	void Back(void);
	void EnableTrans(bool bTrans) { m_info.bTrans = bTrans; }
	void BeamBlade(void);

private:
	struct Sinfo
	{// ���g�̏��
		CStateBoss *pState;	// ���
		D3DXVECTOR3 posDest;	// �ڕW�ʒu
		D3DXVECTOR3 posBlade;	// �u���[�h�̈ʒu
		int nCntAttack;	// �U���J�E���^�[
		int nNumAttack;	// �U��������
		int nCntState;	// ��ԑJ�ڃJ�E���^�[
		bool bTrans;	// �`�ԕω��ォ�ǂ���
		CCollisionSphere *pClsnAttack;	// �U������
		CObject3D *pBlade;	// �u���[�h
	};

	void ManageCollision(void);
	void FollowCollision(void);
	bool FollowDest(void);
	void Wait(void) override {};

	static CEnemyBoss *m_pEnemyBoss;	// ���g�̃|�C���^
	Sinfo m_info;	// ���g�̏��
};

#endif
