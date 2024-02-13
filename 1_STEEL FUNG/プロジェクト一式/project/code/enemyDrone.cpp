//*****************************************************
//
// �h���[���G�̏���[enemyDrone.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemyDrone.h"
#include "enemyManager.h"
#include "player.h"
#include "effect3D.h"
#include "debugproc.h"
#include "bullet.h"
#include "manager.h"
#include "slow.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float DIST_FIRE = 500.0f;	// ���˂���܂ł̎����ƃv���C���[�̍�������
const float SPEED_BULLET = 150.0f;	// �e�̑��x
const float TIME_FIRE = 1.0f;	// ���ˊԊu
const float DIST_KEEP = 3000.0f;	// �퓬���ɕۂ���
const float SPEED_MOVE = 2.0f;	// �ړ����x
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyDrone::CEnemyDrone()
{
	m_fTimeFire = 0;
	m_posDest = { 0.0f,0.0f,0.0f };
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyDrone::~CEnemyDrone()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyDrone::Init(void)
{
	Load((char*)PATH_BODY_ENEMY[TYPE_DRONE]);

	// �p���N���X�̏�����
	CEnemy::Init();

	DeleteCollision();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyDrone::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyDrone::Update(void)
{
	// �p���N���X�̍X�V
	CEnemy::Update();

	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_DEATH)
	{// ���S���[�V����
		int nMotion = GetMotion();

		if (nMotion != MOTION_DEATH)
		{
			SetMotion(MOTION_DEATH);
		}
	}

	if (state != STATE_THROWN)
	{
		if (MoveToDest(m_posDest, SPEED_MOVE))
		{
			int nMotion = GetMotion();

			if (nMotion != 1 && state != STATE_DEATH)
			{
				CreateCollision(240.0f);

				SetMotion(1);
			}
		}
	}
}

//=====================================================
// �ҋ@
//=====================================================
void CEnemyDrone::Wait(void)
{
	CEnemy::Wait();
}

//=====================================================
// �ڕW�̒ǐ�
//=====================================================
void CEnemyDrone::Chase(void)
{
	CEnemy::Chase();
}

//=====================================================
// �U��
//=====================================================
void CEnemyDrone::Attack(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();
	D3DXVECTOR3 rot = GetRotation();

	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (nMotion != 1 || bFinish == false)
		return;

	if (pPlayer != nullptr)
	{
		// �v���C���[�̕�������
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// �����p�x�̎擾
		D3DXVECTOR3 vecDiff = posPlayer - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);

		universal::FactingRot(&rot.y, rotDest.y, 0.1f);

		SetRotation(rot);

		// �v���C���[�Ɍ������Ēe������
		float fDist = D3DXVec3Length(&vecDiff);

		D3DXMATRIX mtxEye;
		D3DXMATRIX mtx = *GetMatrix();

		universal::SetOffSet(&mtxEye, mtx, D3DXVECTOR3(0.0f, 0.0f, fDist));

		D3DXVECTOR3 vecEye = { mtxEye._41,mtxEye._42 ,mtxEye._43 };

		D3DXVECTOR3 vecDiffEye = posPlayer - vecEye;

		float fDistEyeToPlayer = D3DXVec3Length(&vecDiffEye);

		// �_�����߂������甭��
		if (fDistEyeToPlayer < DIST_FIRE)
		{
			float fDeltaTime = CManager::GetDeltaTime();

			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{// �^�C���X�P�[����������
				float fScale = pSlow->GetScale();

				fDeltaTime *= fScale;
			}

			// �^�C�}�[���Z
			m_fTimeFire += fDeltaTime;

			if (m_fTimeFire > TIME_FIRE)
			{// �e�̔���
				D3DXVECTOR3 posMazzle = GetMtxPos(0);
				D3DXVECTOR3 moveBullet;
				D3DXVECTOR3 movePlayer = pPlayer->GetMove();

				D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);
				D3DXVECTOR3 vecDiffBullet = pos - posPrediction;
				D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

				moveBullet = vecDiffBullet * SPEED_BULLET;

				CBullet::Create(posMazzle, -moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f, 0.2f);

				m_fTimeFire = 0;
			}
		}
	}

	CEnemy::Attack();
}

//=====================================================
// �n�ʂɂԂ������Ƃ��̏���
//=====================================================
void CEnemyDrone::HitField(void)
{
	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_THROWN)
	{
		// ���S
		Hit(20.0f);

		// �ړ��ʂ�����
		D3DXVECTOR3 move = GetMove();

		move *= 0.1f;

		SetMove(move);
	}

	CEnemy::HitField();
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyDrone::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();

	//CDebugProc::GetInstance()->Print("\n�ڕW�ʒu�F[%f,%f,%f]", m_posDest.x, m_posDest.y, m_posDest.z);
}