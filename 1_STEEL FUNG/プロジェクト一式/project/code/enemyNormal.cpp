//*****************************************************
//
// �ʏ�G�̏���[enemyNormal.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemyNormal.h"
#include "player.h"
#include "effect3D.h"
#include "debugproc.h"
#include "bullet.h"
#include "manager.h"
#include "slow.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float DIST_FIRE = 500.0f;	// ���˂���܂ł̎����ƃv���C���[�̍�������
const float SPEED_BULLET = 150.0f;	// �e�̑��x
const float TIME_FIRE = 0.5f;	// ���ˊԊu
const float DIST_KEEP = 3000.0f;	// �퓬���ɕۂ���
const float SPEED_MOVE = 0.4f;	// �ړ����x
const int ACCURACY = 500;	// ���x
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyNormal::CEnemyNormal()
{
	m_fTimeFire = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyNormal::Init(void)
{
	// �p���N���X�̏�����
	CEnemy::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyNormal::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyNormal::Update(void)
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
}

//=====================================================
// �ҋ@
//=====================================================
void CEnemyNormal::Wait(void)
{
	CEnemy::Wait();
}

//=====================================================
// �ڕW�̒ǐ�
//=====================================================
void CEnemyNormal::Chase(void)
{
	CEnemy::Chase();
}

//=====================================================
// �U��
//=====================================================
void CEnemyNormal::Attack(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();
	D3DXVECTOR3 rot = GetRotation();

	// ������ۂ�
	KeepDistance();

	if (pPlayer != nullptr)
	{
		// �v���C���[�̕�������
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// �����p�x�̎擾
		D3DXVECTOR3 vecDiff = posPlayer - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);

		universal::FactingRot(&rot.x, -rotDest.x + D3DX_PI * 0.5f, 0.1f);
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
				Sound::Play(CSound::LABEL_SE_SHOT01);

				D3DXVECTOR3 posMazzle = GetMtxPos(0);
				D3DXVECTOR3 moveBullet;
				D3DXVECTOR3 movePlayer = pPlayer->GetMove();

				D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);

				posPrediction.x += (float)universal::RandRange(ACCURACY, -ACCURACY);
				posPrediction.y += (float)universal::RandRange(ACCURACY, -ACCURACY);
				posPrediction.z += (float)universal::RandRange(ACCURACY, -ACCURACY);

				D3DXVECTOR3 vecDiffBullet = pos - posPrediction;
				D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

				moveBullet = vecDiffBullet * SPEED_BULLET;

				CBullet::Create(posMazzle, -moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f, 0.25f);

				m_fTimeFire = 0;
			}
		}
	}

	CEnemy::Attack();
}

//=====================================================
// ������ۂ���
//=====================================================
void CEnemyNormal::KeepDistance(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 move = GetMove();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// �����p�x�̎擾
		D3DXVECTOR3 vecDiff = pos - posPlayer;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= DIST_KEEP;

		D3DXVECTOR3 posDest = posPlayer + vecDiff;

		posDest.y = pos.y;

		// �ړ��ʂ�␳����
		vecDiff = (posDest - pos);

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= SPEED_MOVE;

		move += vecDiff;

		SetMove(move);

#ifdef _DEBUG
		CEffect3D::Create(posDest, 20.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.01f));
#endif

	}
}

//=====================================================
// �n�ʂɂԂ������Ƃ��̏���
//=====================================================
void CEnemyNormal::HitField(void)
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
void CEnemyNormal::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}