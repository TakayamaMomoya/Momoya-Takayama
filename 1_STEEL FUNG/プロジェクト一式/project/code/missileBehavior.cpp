//*****************************************************
//
// �~�T�C���̃r�w�C�r�A[missileBehavior.xpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "missileBehavior.h"
#include "missile.h"
#include "player.h"
#include "manager.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float DIST_STARTCHASE = 500.0f;	// �ҋ@�܂ł̋���
const float TIME_START_CHASE = 1.0f;	// �ǐՊJ�n�܂ł̎���
const float SPEED_CHASE = 100.0f;	// �ǐՃX�s�[�h
const int RANGE_RANDOM = 500;	// �����_���͈�
}

//=====================================================
// ���N���X
//=====================================================
CMissileBehavior::CMissileBehavior()
{// �R���X�g���N�^
	m_bEndChase = false;
}

CMissileBehavior::~CMissileBehavior()
{// �f�X�g���N�^

}

//=====================================================
// ���������̃~�T�C��
//=====================================================
CMissileLine::CMissileLine()
{// �R���X�g���N�^
	
}

CMissileLine::~CMissileLine()
{// �f�X�g���N�^

}

void CMissileLine::Init(CMissile *pMissile)
{// ������

}

void CMissileLine::Uninit(CMissile *pMissile)
{// �I��

}

void CMissileLine::Update(CMissile *pMissile)
{// �X�V
	bool bEndChase = IsEndChase();

	if (bEndChase)
		return;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// �ڕW�����̎擾
		D3DXVECTOR3 pos = pMissile->GetMtxPos(0);
		D3DXVECTOR3 move = pMissile->GetMove();

		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();

		float fSpeedMax = pMissile->GetSpeedMax();

		posPlayer = universal::LinePridiction(pos, fSpeedMax, posPlayer, movePlayer * 0.1f);

		float fSpeed = D3DXVec3Length(&move);

		D3DXVECTOR3 vecDiff = posPlayer - pos;
		float fLengthDiff = D3DXVec3Length(&vecDiff);

		if (fLengthDiff < 1000.0f)
		{
			bEndChase = true;

			EnableChase(bEndChase);
		}

		float fDeltaTime = CManager::GetDeltaTime();

		float fTime = pMissile->GetTimerHit();

		fTime -= fDeltaTime;

		pMissile->SetTimerHit(fTime);

		float fSpeedChase = pMissile->GetSpeedChase();

		if (fTime > 0.0f)
		{
			D3DXVECTOR3 acceleration = 2.0f * (vecDiff - move);

			if (D3DXVec3Length(&acceleration) > fSpeedChase)
			{
				D3DXVec3Normalize(&acceleration, &acceleration);

				acceleration *= fSpeedChase;
			}

			// �ړ��ʂ𐳖ʂɑ���
			move += acceleration;

			if (D3DXVec3Length(&move) > fSpeedMax)
			{
				D3DXVec3Normalize(&move, &move);

				move *= fSpeedMax;
			}

			pMissile->SetMove(move);
		}
	}
}

//=====================================================
// ���p�ɋȂ���~�T�C��
//=====================================================
CMissileRightAngle::CMissileRightAngle()
{// �R���X�g���N�^
	m_state = STATE_MID;
	m_posMid = { 0.0f,0.0f,0.0f };
	m_fTimerStartChase = 0.0f;
}

CMissileRightAngle::~CMissileRightAngle()
{// �f�X�g���N�^

}

void CMissileRightAngle::Init(CMissile *pMissile)
{// ������
	m_state = STATE_MID;

	DecidePosMid(pMissile);
}

void CMissileRightAngle::DecidePosMid(CMissile *pMissile)
{// ���Ԓn�_�̐ݒ�
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 pos = pMissile->GetPosition();
	D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	m_posMid = posPlayer;

	m_posMid +=
	{
		(float)universal::RandRange(RANGE_RANDOM, -RANGE_RANDOM),
		(float)universal::RandRange(RANGE_RANDOM, RANGE_RANDOM / 2),
		(float)universal::RandRange(RANGE_RANDOM, -RANGE_RANDOM),
	};

	float fLength = D3DXVec3Length(&vecDiff) * 0.8f;

	universal::LimitDistSphereInSide(fLength, &m_posMid, pos);
}

void CMissileRightAngle::Uninit(CMissile *pMissile)
{// �I��

}

void CMissileRightAngle::Update(CMissile *pMissile)
{// �X�V
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posDest = m_posMid;
	D3DXVECTOR3 posMissile = pMissile->GetPosition();
	D3DXVECTOR3 move = pMissile->GetMove();
	D3DXVECTOR3 vecDiff = posDest - posMissile;

	switch (m_state)
	{// �ڕW�n�_�̐ݒ�
	case CMissileRightAngle::STATE_MID:	// ���Ԓn�_�Ɍ�����
	{
		float fSpeed = pMissile->GetSpeedMax();
		universal::VecConvertLength(&vecDiff, fSpeed);

		move += vecDiff;

		if (universal::DistCmp(posMissile, posDest, DIST_STARTCHASE, nullptr))
		{// �ǐՊJ�n�܂ňڍs����P�\
			move *= 0.0f;

			pMissile->SetMove(move);

			m_state = STATE_WAIT;
		}
		
		pMissile->SetMove(move);
	}
		break;
	case CMissileRightAngle::STATE_WAIT:	// �ҋ@���
	{
		posDest = pPlayer->GetMtxPos(0);

		vecDiff = posDest - posMissile;

		universal::VecConvertLength(&vecDiff, 0.01f);

		move += vecDiff;

		pMissile->SetMove(move);

		float fDeltaTime = CManager::GetDeltaTime();

		m_fTimerStartChase += fDeltaTime;

		if (m_fTimerStartChase >= TIME_START_CHASE)
		{
			D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

			vecDiff = posPlayer - posMissile;

			m_state = STATE_CHASE;

			universal::VecConvertLength(&vecDiff, SPEED_CHASE);

			move += vecDiff;

			pMissile->SetMove(move);

			Sound::Play(CSound::LABEL_SE_SHOT03);
		}
	}
	break;
	case CMissileRightAngle::STATE_CHASE:	// �v���C���[�ǐ�
	{
		posDest = pPlayer->GetMtxPos(0);

		// ���z�[�~���O������
		universal::Horming(posMissile, posDest, 3.0f, &move);
		
		pMissile->SetMove(move);
	}
		break;
	default:
		break;
	}

	
}