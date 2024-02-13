//*****************************************************
//
// �D�_�G�̏���[enemyThief.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemyThief.h"
#include "enemyManager.h"
#include <stdio.h>
#include "playerManager.h"
#include "game.h"
#include "block.h"
#include "universal.h"
#include "rocket.h"
#include "motion.h"
#include "itemRepair.h"
#include "telop.h"
#include "object3D.h"
#include "texture.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace 
{
const float TIME_CAMERAEVENT = 4.0f;	// �J�����̃C�x���g�p������
const float MOVE_SPEED = 1.7f;			// �ړ����x

const float LIMID_RANGE_LEFT = -460.0f;
const float LIMID_RANGE_RIGHT = 460.0f;
const float LIMID_RANGE_UP = 460.0f;
const float LIMID_RANGE_DOWN = -460.0f;
const char* CAUTION_PATH = "data\\TEXTURE\\UI\\steal.png";	// �x���\���̃e�N�X�`���p�X
const D3DXVECTOR3 POS_CAUTION = { 0.0f,150.0f,100.0f };	// �x���̈ʒu
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyThief::CEnemyThief()
{
	m_bTakeRepair = false;
	m_state = STATE_NONE;
	m_pCaution = nullptr;
	m_bSound = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyThief::~CEnemyThief()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyThief::Init(void)
{
	// �p���N���X�̏�����
	CEnemy::Init();

	// �̗͂��ʐݒ�
	SetLife(50.0f);

	// �������[�V�����ɐݒ�
	CMotion *pBody = GetBody();

	if (pBody != nullptr)
	{
		pBody->SetMotion(MOTION_WALK);
	}

	// �ǐՏ�Ԃɐݒ�
	m_state = STATE_CHASE;

	// �ړ����x�̐ݒ�
	SetMoveSpeed(MOVE_SPEED);

	// �J�����C�x���g�̐ݒ�
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		D3DXVECTOR3 posOwn = GetPosition();
		D3DXVECTOR3 posV = posOwn;
		posV.x += 100.0f;
		posV.y += 150.0f;
		posV.z -= 150.0f;
		posOwn.y += 60.0f;

		pGame->SetEventCamera(TIME_CAMERAEVENT, posOwn, posV);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyThief::Uninit(void)
{
	if (m_pCaution != nullptr)
	{
		m_pCaution->Uninit();
		m_pCaution = nullptr;
	}

	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// ���S���̏���
//=====================================================
void CEnemyThief::Death(void)
{
	int nMotion = GetMotion();

	if (nMotion != MOTION_DEATH)
	{// ���S���[�V�����̐ݒ�
		SetMotion(MOTION_DEATH);

		if (m_bTakeRepair)
		{// �C���A�C�e���̃h���b�v
			CItemRepair *pRepair = CItemRepair::Create();

			if (pRepair != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();

				pRepair->SetPosition(pos);
			}
		}
	}

	CEnemy::Death();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyThief::Update(void)
{
	// �p���N���X�̍X�V
	CEnemy::Update();

	CBlock *pBlock = GetTouchBlock();

	if (pBlock != nullptr)
	{
		// �����`�F�b�N
		CBlock **ppBlock = CBlock::GetBlock();

		for (int i = 0; i < NUM_OBJECT; i++)
		{
			if (ppBlock[i] == pBlock)
			{
				pBlock->Hit(5.0f);
			}
		}
	}

	CEnemy::STATE state = GetState();

	if (state != CEnemy::STATE::STATE_DEATH)
	{
		// �X�V�����̕���
		SwitchUpdate();
	}

	if (m_pCaution != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		pos += POS_CAUTION;

		m_pCaution->SetPosition(pos);
	}
}

//=====================================================
// �X�V�����̕���
//=====================================================
void CEnemyThief::SwitchUpdate(void)
{
	switch (m_state)
	{
	case CEnemyThief::STATE_CHASE:	// ���P�b�g�̒ǐ�
		// ���P�b�g�Ƃ̓����蔻��
		CollisionRocket();

		break;
	case CEnemyThief::STATE_ESCAPE:	// �������
		// �����鏈��
		Escape();
		break;
	default:
		break;
	}
}

//=====================================================
// �ڕW��ǐՂ��鏈��
//=====================================================
void CEnemyThief::ChaseTarget(void)
{
	// �Q�[����Ԃɂ���ĒǐՂ����Ȃ�
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL || m_state != STATE_CHASE)
		{
			return;
		}
	}

	// �ڕW�ʒu�̎擾
	D3DXVECTOR3 posTarget = { 0.0f,0.0f,0.0f };

	CRocket *pRocket = CRocket::GetInstance();

	if (pRocket != nullptr)
	{
		posTarget = pRocket->GetPosition();
	}

	// �ړ��ʂ̐ݒ�
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posTarget - pos;
	D3DXVECTOR3 move = GetMove();

	D3DXVec3Normalize(&vecDiff, &vecDiff);

	float fSpeed = GetSpeed();

	vecDiff *= fSpeed;

	move += vecDiff;

	SetMove(move);

	// ������ڕW�����ɕ␳
	float fAngleDist = atan2f(vecDiff.x, vecDiff.z);
	D3DXVECTOR3 rot = GetRot();

	fAngleDist += D3DX_PI;

	universal::FactingRot(&rot.y, fAngleDist, 0.1f);

	SetRot(rot);
}

//=====================================================
// ���P�b�g�Ƃ̓����蔻��
//=====================================================
void CEnemyThief::CollisionRocket(void)
{
	CRocket *pRocket = CRocket::GetInstance();

	if (pRocket == nullptr)
	{
		return;
	}

	// �����������擾
	D3DXVECTOR3 posRocket = pRocket->GetPosition();
	D3DXVECTOR3 posOwn = GetPosition();
	D3DXVECTOR3 vecDiff = posOwn - posRocket;

	float fLegnthDiff = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);
	float fLegnth = pRocket->GetRadius();

	if (fLegnthDiff < fLegnth)
	{// �i�s�󋵂�߂��A������ԂɈڍs
		pRocket->AddProgress(-1);

		m_state = STATE_ESCAPE;

		m_bTakeRepair = true;

		// �J�����C�x���g�̐ݒ�
		CGame *pGame = CGame::GetInstance();

		if (pGame != nullptr)
		{
			D3DXVECTOR3 posV = posOwn;
			posV.x += 100.0f;
			posV.y += 150.0f;
			posOwn.y += 60.0f;

			pGame->SetEventCamera(TIME_CAMERAEVENT, posOwn, posV);
		}

		// �e���b�v�̐���
		CTelop::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_STAMP);
			m_bSound = true;
		}

		if (m_pCaution == nullptr)
		{// �x���̐���
			D3DXVECTOR3 pos = GetPosition();

			pos += POS_CAUTION;

			m_pCaution = CObject3D::Create(pos);

			if (m_pCaution != nullptr)
			{
				int nIdx = CTexture::GetInstance()->Regist(CAUTION_PATH);
				m_pCaution->SetIdxTexture(nIdx);

				m_pCaution->SetSize(100.0f, 50.0f);
				m_pCaution->EnableBillboard(true);
				m_pCaution->EnableZtest(true);
				m_pCaution->EnableLighting(false);
				m_pCaution->SetVtx();
			}
		}
	}
}

//=====================================================
// �����鏈��
//=====================================================
void CEnemyThief::Escape(void)
{
	// �Q�[����Ԃɂ���ē������Ȃ�
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL)
		{
			return;
		}
	}

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posTarget = { 0.0f,0.0f,0.0f };

	// �ŉ������̐錾
	float fLengthMax = FLT_MAX;

	for (int i = 0; i < NUM_PLAYER; i++)
	{// �ł��߂��v���C���[���Q��
		CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

			// �����̔�r
			bool bNear = universal::DistCmp(pos, posPlayer, fLengthMax, &fLengthMax);

			if (bNear)
			{
				posTarget = posPlayer;
			}
		}
	}

	// �ړ��ʂ̐ݒ�
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posTarget - pos;
	D3DXVECTOR3 move = GetMove();

	D3DXVec3Normalize(&vecDiff, &vecDiff);

	float fSpeed = GetSpeed();

	vecDiff *= fSpeed;

	move -= vecDiff;

	SetMove(move);

	// ������ڕW�����ɕ␳
	float fAngleDist = atan2f(vecDiff.x, vecDiff.z);
	D3DXVECTOR3 rot = GetRot();

	universal::FactingRot(&rot.y, fAngleDist, 0.1f);

	SetRot(rot);

	LimidPostion();
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyThief::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}

//=====================================================
// �ړ��͈�
//=====================================================
void CEnemyThief::LimidPostion(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (pos.x <= LIMID_RANGE_LEFT)
	{
		pos.x = LIMID_RANGE_LEFT;
	}
	else if (pos.x >= LIMID_RANGE_RIGHT)
	{
		pos.x = LIMID_RANGE_RIGHT;
	}
	if (pos.z >= LIMID_RANGE_UP)
	{
		pos.z = LIMID_RANGE_UP;
	}
	else if (pos.z <= LIMID_RANGE_DOWN)
	{
		pos.z = LIMID_RANGE_DOWN;
	}
	SetPosition(pos);
}