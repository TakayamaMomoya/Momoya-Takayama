//*****************************************************
//
// �ʏ�G�̏���[enemyNormal.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemyNormal.h"
#include "manager.h"
#include "collision.h"
#include "debugproc.h"
#include "particle.h"
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "enemyManager.h"
#include "debrisSpawner.h"
#include "block.h"
#include "motion.h"
#include "playerManager.h"

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyNormal::CEnemyNormal()
{

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

	// �������[�V�����ɐݒ�
	CMotion *pBody = GetBody();

	if (pBody != nullptr)
	{
		pBody->SetMotion(MOTION_WALK);
	}

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

	if (state == CEnemy::STATE::STATE_DEATH)
	{// ���S���[�V����
		int nMotion = GetMotion();

		if (nMotion != MOTION_DEATH)
		{
			SetMotion(MOTION_DEATH);
		}
	}
	else
	{

	}
}

//=====================================================
// �ڕW�̒ǐ�
//=====================================================
void CEnemyNormal::ChaseTarget(void)
{
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr)
	{
		return;
	}

	MOVESTATE moveState = GetMoveState();

	if (moveState == MOVESTATE_CHASE)
	{
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
					SetPosDest(posPlayer);
				}
			}
		}
	}

	CEnemy::ChaseTarget();
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyNormal::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}