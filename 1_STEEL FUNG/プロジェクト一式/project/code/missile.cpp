//*****************************************************
//
// �~�T�C���̏���[enemymissile.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "missile.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "universal.h"
#include "debugproc.h"
#include "particle.h"
#include "collision.h"
#include "orbit.h"
#include "anim3D.h"
#include "animEffect3D.h"
#include "missileBehavior.h"
#include "boostEffect.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float MAX_SPEED = 100.0f;	// �ړ����x
const float MAX_ACCL = 8.0f;	// �ő�����x
const float CHASE_SPEED = 3.0f;	// �ǐՑ��x
const int INITIAL_LIFE = 1;	// �����̗�
const int DEATH_TIME = 240;	// ���ł܂ł̎���
const float WIDTH_ORBIT = 30.0f;	// �O�Ղ̕�
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CMissile::CMissile()
{
	m_fTimerHit = 0.0f;
	m_nDeathTimer = 0;
	m_bEndChase = false;
	m_fChaseSpeed = 0.0f;
	m_fSpeedMax = 0.0f;
	m_pBehavior = nullptr;
	m_pOrbit = nullptr;
	m_pFire = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMissile::~CMissile()
{

}

//=====================================================
// ��������
//=====================================================
CMissile *CMissile::Create(D3DXVECTOR3 pos, TYPE type)
{
	CMissile *pMissile = nullptr;

	pMissile = new CMissile;

	if (pMissile != nullptr)
	{
		pMissile->SetPosition(pos);

		// ����������
		pMissile->Init();

		switch (type)
		{
		case CMissile::TYPE_LINE:
			pMissile->m_pBehavior = new CMissileLine;

			break;
		case CMissile::TYPE_RIGHTANGLE:
			pMissile->m_pBehavior = new CMissileRightAngle;

			break;
		default:
			assert(("�s���ȃ~�T�C���������Ă܂���", false));
			break;
		}

		if (pMissile->m_pBehavior != nullptr)
		{
			pMissile->m_pBehavior->Init(pMissile);
		}
	}

	return pMissile;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMissile::Init(void)
{
	Sound::Play(CSound::LABEL_SE_SHOT02);

	m_fChaseSpeed = MAX_ACCL;
	m_fSpeedMax = MAX_SPEED;

	Load("data\\MOTION\\motionMissileSmall.txt");

	// �p���N���X�̏�����
	CEnemy::Init();

	// �����蔻����~�T�C���ɕύX
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		pCollision->SetTag(CCollision::TAG::TAG_ROCKET);
	}

	m_fTimerHit = 5.0f;

	MultiplyMtx();

	if (m_pOrbit == nullptr)
	{
		D3DXMATRIX mtx = *GetMatrix();
		D3DXVECTOR3 offset = { WIDTH_ORBIT,0.0f,0.0f };
		D3DXCOLOR col = { 1.0f, 1.0f,1.0f,1.0f };

		m_pOrbit = COrbit::Create(mtx, offset, -offset, col, 20);
	}

	if (m_pFire == nullptr)
	{
		m_pFire = CBoostEffect::Create();

		if (m_pFire != nullptr)
		{
			D3DXCOLOR col = universal::ConvertRGB(255, 102, 0, 255);

			m_pFire->SetColor(col);
			m_pFire->EnableZtestBoost(true);
		}
	}

	return S_OK;
}

//=====================================================
// ���S����
//=====================================================
void CMissile::Death(void)
{
	Sound::Play(CSound::LABEL_SE_EXPLOSION00);

	D3DXVECTOR3 pos = GetPosition();

	// �A�j���G�t�F�N�g����
	CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

	if (pAnim3D != nullptr)
	{
		pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);
	}

	// ���g�̏I��
	Uninit();
}

//=====================================================
// �I������
//=====================================================
void CMissile::Uninit(void)
{
	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Uninit(this);
		delete m_pBehavior;
		m_pBehavior = nullptr;
	}

	if (m_pOrbit != nullptr)
	{
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}

	if (m_pFire != nullptr)
	{
		m_pFire->Uninit();
		m_pFire = nullptr;
	}

	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CMissile::Update(void)
{
	// �ʒu�ۑ�
	SetPositionOld(GetPosition());

	// �ǐՏ���
	ChasePlayer();

	if (m_pOrbit != nullptr)
	{
		D3DXMATRIX mtx = *GetMatrix();

		m_pOrbit->SetPositionOffset(mtx, 0);
	}

	if (m_pFire != nullptr)
	{
		D3DXVECTOR3 move = GetMove();
		D3DXMATRIX mtxParent = *GetMatrix();
		D3DXMATRIX mtx;
		D3DXVECTOR3 offset = { 0.0f,-10.0f,0.0f };

		universal::SetOffSet(&mtx, mtxParent, offset);

		D3DXVECTOR3 posBoost = { mtx._41, mtx._42 ,mtx._43 };

		D3DXVECTOR3 rot = universal::VecToRot(-move);
		rot.x *= -1;
		rot.x += D3DX_PI;

		m_pFire->SetRotation(rot);
		m_pFire->SetPosition(posBoost);
	}

	m_nDeathTimer++;

	if (m_nDeathTimer > DEATH_TIME)
	{
		Death();
	}

	// �X���̐���
	Tilt();

	// �����蔻����~�T�C���ɕύX
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		float fRadius = pCollision->GetRadius();

		pCollision->SetRadius(fRadius * 1.4f);
		
		if (pCollision->OnEnter(CCollision::TAG_PLAYER))
		{
			CObject *pObj = pCollision->GetOther();

			if (pObj != nullptr)
			{
				// ���������I�u�W�F�N�g�̃q�b�g����
				pObj->Hit(5.0f);

				Death();

				return;
			}
		}

		pCollision->SetRadius(fRadius);
	}

	// �p���N���X�̍X�V
	CEnemy::Update();
}

//=====================================================
// �ǐՏ���
//=====================================================
void CMissile::ChasePlayer(void)
{
	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Update(this);
	}
}

//=====================================================
// �X���̊Ǘ�
//=====================================================
void CMissile::Tilt(void)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = GetMove();
	float fLength;

	// Y�����ʂ̊p�x���擾
	rot.y = atan2f(move.x, move.z);

	// Y�����ʂ̈ړ��ʂ��擾
	fLength = D3DXVec2Length(&D3DXVECTOR2(move.x,move.z));

	// �����ʂ̊p�x���擾
	rot.x = atan2f(fLength,move.y);

	// ��]�̐ݒ�
	SetRotation(rot);
}

//=====================================================
// �`�揈��
//=====================================================
void CMissile::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}

//=====================================================
// �n�ʂɓ��������Ƃ��̏���
//=====================================================
void CMissile::HitField(void)
{
	Death();
}