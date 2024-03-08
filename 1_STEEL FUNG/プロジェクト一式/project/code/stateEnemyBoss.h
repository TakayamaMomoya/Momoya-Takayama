//*****************************************************
//
// �{�X�G���[stateEnemyBoss.h]
// Author:���R����
//
//*****************************************************

#ifndef _STATEENEMYBOSS_H_
#define _STATEENEMYBOSS_H_

#include <assert.h>
#include "main.h"

class CEnemyBoss;
class COrbit;
class CBeamBlade;
class CAnim3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CStateBoss
{
public:
	CStateBoss();
	virtual ~CStateBoss();

	virtual void Init(CEnemyBoss *pBoss) {}
	virtual void Attack(CEnemyBoss *pBoss) {}
	virtual void Move(CEnemyBoss *pBoss) {}

protected:
	void CheckPointer(CEnemyBoss *pBoss) {
		if(pBoss == nullptr)assert(("�{�X���̎Q�Ƃ��ł��܂���", false));
	};

private:
};

//=====================================================
// �s���I��
//=====================================================
class CStateBossSelect : public CStateBoss
{// �s���I��
	void Init(CEnemyBoss *pBoss) override;

	virtual void Close(int nRand, CEnemyBoss *pBoss) = 0;
	virtual void Middle(int nRand, CEnemyBoss *pBoss) = 0;
	virtual void Far(int nRand, CEnemyBoss *pBoss) = 0;
};

class CStateBossSelect1st : public CStateBossSelect
{// ���`�Ԃ̍s���I��
	void Close(int nRand, CEnemyBoss *pBoss) override;
	void Middle(int nRand, CEnemyBoss *pBoss) override;
	void Far(int nRand, CEnemyBoss *pBoss) override;
};

class CStateBossSelect2nd : public CStateBossSelect
{// ���`�Ԃ̍s���I��
	void Close(int nRand, CEnemyBoss *pBoss) override;
	void Middle(int nRand, CEnemyBoss *pBoss) override;
	void Far(int nRand, CEnemyBoss *pBoss) override;
};

//=====================================================
// ���`��
//=====================================================
class CStateBossApper : public CStateBoss
{// �o�����
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

class CStateBossAttackBeam : public CStateBoss
{// �r�[���U��
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;
};

class CStateBossAttackMissile : public CStateBoss
{// �~�T�C���U��
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // �ˏo�J�E���^�[
};

class CStateBossLaunchDrone : public CStateBoss
{// �q�@�̎ˏo
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // �ˏo�J�E���^�[
};

class CStateBossAttackMachinegun : public CStateBoss
{// �}�V���K��
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // �ˏo�J�E���^�[
};

class CStateBossStep1st : public CStateBoss
{// �X�e�b�v�̓���
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	D3DXVECTOR3 m_posDestMid;
	D3DXVECTOR3 m_posDest;
	bool m_bMid;	// ���Ԃ̃X�e�b�v���I������
};

class CStateBossTrans : public CStateBoss
{// ���`�Ԃֈڍs����
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	void TransMovie(CEnemyBoss *pBoss);
	void Evolve(CEnemyBoss *pBoss);

	float m_fTimeTrans;	// �J�ڃ^�C�}�[
};

//=====================================================
// ��j�`��
//=====================================================
class CStateBossBeforeTrans : public CStateBoss
{// ���`�Ԃւ̈ڍs��
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

class CStateBossSlash : public CStateBoss
{// �a��
	~CStateBossSlash();
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	COrbit *m_pOrbit;
	CBeamBlade *m_pBlade;
};

class CStateBossBeamSmall : public CStateBoss
{// ���r�[��
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt;
};

class CStateBossStep : public CStateBoss
{// �X�e�b�v�̓���
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	D3DXVECTOR3 m_posDestMid;
	D3DXVECTOR3 m_posDest;
	bool m_bMid;	// ���Ԃ̃X�e�b�v���I������
};

class CStateBossJump : public CStateBoss
{// ��яオ��
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;

	D3DXVECTOR3 m_posDest;
};

class CStateBossBeamAir : public CStateBoss
{// �󒆃r�[��
	~CStateBossBeamAir();
	void Init(CEnemyBoss *pBoss) override;
	void DisideRotDest(CEnemyBoss *pBoss);
	void Attack(CEnemyBoss *pBoss) override;
	void Rotation(CEnemyBoss *pBoss);
	void Radiation(CEnemyBoss *pBoss);

	CAnim3D *m_pAnim;
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_posAim;
	D3DXVECTOR3 m_moveAim;
	float m_fTimer;
};

//=====================================================
// ���j
//=====================================================
class CStateBossDeath : public CStateBoss
{// ���S����
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

class CStateBossAfterDeath : public CStateBoss
{// ���S��
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

#endif