//*****************************************************
//
// �{�X�G�̏���[enemyboss.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemyboss.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "universal.h"
#include "debugproc.h"
#include "game.h"
#include "bullet.h"
#include "effect3D.h"
#include "sound.h"
#include "frame.h"
#include "particle.h"
#include "stateEnemyBoss.h"
#include "beam.h"
#include "slow.h"
#include "fade.h"
#include "camera.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int INITIAL_SCORE = 30000;	// �����X�R�A
const float MOVE_FACT = 0.04f;	// �ړ��W��
const float LINE_END = 5.0f;	// �ړ��I���̂������l
const int DAMAGE_FRAME = 10;	// �_���[�W��Ԃ̎���
const float SPEED_BACK = 1.8f;	// ��ނ̈ړ����x
const float SPEED_DODGE = 2.1f;	// ��ގ��̉��ړ����x
const float DAMAGE_SLASH = 10.0f;	// �a���_���[�W
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CEnemyBoss *CEnemyBoss::m_pEnemyBoss = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyBoss::CEnemyBoss()
{
	// ���g�̏����N���A
	ZeroMemory(&m_info,sizeof(Sinfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyBoss::~CEnemyBoss()
{

}

//=====================================================
// ��������
//=====================================================
CEnemyBoss *CEnemyBoss::Create(void)
{
	if (m_pEnemyBoss == nullptr)
	{
		m_pEnemyBoss = new CEnemyBoss;

		if (m_pEnemyBoss != nullptr)
		{
			m_pEnemyBoss->Load("data\\MOTION\\robot01.txt");

			m_pEnemyBoss->Init();
		}
	}

	return m_pEnemyBoss;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyBoss::Init(void)
{
	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Stop();

		//pSound->Play(CSound::LABEL_BGM_BOSS);
	}

	// �t���[�����o�̐���
	CFrame::Create(100, 240, 70);

	// �p���N���X�̏�����
	CEnemy::Init();

	// �����̗̑͐ݒ�
	SetLife(Boss::INITIAL_LIFE);

	// ��Ԑݒ�
	ChangeState(new CStateBossApper);

	FollowCollision();

	SetPosition(D3DXVECTOR3(0.0f, 1500.0f, 0.0f));

	SetDistLock(50000.0f);

	EnableStamp(false);

	if (m_info.pClsnAttack == nullptr)
	{// ���̓����蔻�萶��
		m_info.pClsnAttack = CCollisionSphere::Create(CCollision::TAG_NONE, CCollision::TYPE_SPHERE, this);

		if (m_info.pClsnAttack != nullptr)
		{// ���̐ݒ�
			m_info.pClsnAttack->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_info.pClsnAttack->SetRadius(0.0f);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyBoss::Uninit(void)
{
	m_pEnemyBoss = nullptr;

	if (m_info.pState != nullptr)
	{
		delete m_info.pState;
		m_info.pState = nullptr;
	}

	if (m_info.pClsnAttack != nullptr)
	{
		m_info.pClsnAttack->Uninit();
		m_info.pClsnAttack = nullptr;
	}

	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyBoss::Update(void)
{
	// �p���N���X�̍X�V
	CEnemy::Update();

	if (m_info.pState != nullptr)
	{// ��Ԃ��Ƃ̍s��
		m_info.pState->Attack(this);
		m_info.pState->Move(this);
	}

	// �����蔻��Ǘ�
	ManageCollision();

	D3DXVECTOR3 pos = GetPosition();

	universal::LimitPosInSq(23000.0f, 23000.0f,&pos);

	SetPosition(pos);
}

//=====================================================
// �����蔻��Ǘ�
//=====================================================
void CEnemyBoss::ManageCollision(void)
{
	// �����蔻��Ǐ]
	FollowCollision();
}

//=====================================================
// �v���C���[��_������
//=====================================================
void CEnemyBoss::AimPlayer(float fSpeed, bool bPridict)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// �ڕW�����̎擾
		D3DXVECTOR3 pos = GetMtxPos(15);

		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		D3DXVECTOR3 posPridiction;

		if (bPridict)
		{
			posPridiction = universal::LinePridiction(pos, fSpeed, posPlayer, movePlayer);
		}
		else
		{
			posPridiction = posPlayer;
		}

		D3DXVECTOR3 vecDiff = posPridiction - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);
		rotDest.x -= D3DX_PI * 0.5f;
		rotDest.y -= D3DX_PI;

		// �����̕␳
		D3DXVECTOR3 rot = GetRotation();

		universal::FactingRot(&rot.x, rotDest.x, 0.15f);
		universal::FactingRot(&rot.y, rotDest.y, 0.15f);

		SetRotation(rot);
	}
}

//=====================================================
// ��ނ̏���
//=====================================================
void CEnemyBoss::Back(void)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 pos = GetPosition();

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �����x�N�g�����ړ��ʂɑ���
	D3DXVECTOR3 vecDiff = pos - posPlayer;

	D3DXVec3Normalize(&vecDiff,&vecDiff);

	vecDiff *= SPEED_BACK;
	vecDiff.y = 0.0f;

	move += vecDiff;

	// �������ɂ��ړ��ʂ𑫂�
	float fAngle = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI * 0.5f;

	universal::LimitRot(&fAngle);

	move += {sinf(fAngle + D3DX_PI * 0.5f) * SPEED_DODGE, 0.0f, cosf(fAngle + D3DX_PI * 0.5f) * SPEED_DODGE};
	move += {sinf(fAngle) * SPEED_DODGE, 0.0f, cosf(fAngle) * SPEED_DODGE};

	SetMove(move);
}

//=====================================================
// �r����r�[���u���[�h���o������
//=====================================================
void CEnemyBoss::BeamBlade(void)
{
	D3DXMATRIX mtxArm = *GetParts(IDX_HAND_L)->pParts->GetMatrix();
	D3DXMATRIX mtx;

	D3DXVECTOR3 vecBlade = universal::VecToOffset(mtxArm, D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	D3DXVECTOR3 rot = universal::VecToRot(vecBlade);
	m_info.posBlade =
	{
		mtxArm._41,
		mtxArm._42,
		mtxArm._43
	};

	rot.x *= -1;

	CParticle::Create(m_info.posBlade, CParticle::TYPE::TYPE_BEAM_BLADE,rot,&m_info.posBlade);
}

//=====================================================
// �C�x���g�̊Ǘ�
//=====================================================
void CEnemyBoss::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = *GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };
	float fRadius = pEventInfo->fRadius;

	if (nMotion == MOTION_SHOT)
	{
		// �r�[���̐���
		CBeam *pBeam = CBeam::Create();

		D3DXMATRIX mtxBeam;

		universal::SetOffSet(&mtxBeam, mtxParent, D3DXVECTOR3(0.0f, -1.0f, 1.0f));

		if (pBeam != nullptr)
		{
			D3DXVECTOR3 posMazzle = { mtxBeam._41,mtxBeam._42 ,mtxBeam._43 };
			D3DXVECTOR3 vecMazzle = posMazzle - pos;

			D3DXVECTOR3 rot = GetRotation();

			pBeam->SetCol(D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f));
			pBeam->SetMtx(mtxBeam);
			pBeam->SetRotation(rot);
			pBeam->SetPosition(posMazzle);
			pBeam->SetAnimSize(1000.0f, 5000.0f);
			pBeam->SetShrink(5.0f);
			pBeam->SetExpand(80.0f);
			pBeam->SetExtend(100.0f);
		}
	}

	if (nMotion == MOTION_SLASH)
	{// �a��
		if (m_info.pClsnAttack != nullptr)
		{// ���̐ݒ�
			m_info.pClsnAttack->SetPosition(pos);
			m_info.pClsnAttack->SetRadius(fRadius);

			if (m_info.pClsnAttack->OnEnter(CCollision::TAG_PLAYER))
			{
				CObject *pObj = m_info.pClsnAttack->GetOther();

				if (pObj != nullptr)
				{
					pObj->Hit(DAMAGE_SLASH);
				}
			}
		}
	}
}

//=====================================================
// �����蔻��̒Ǐ]
//=====================================================
void CEnemyBoss::FollowCollision(void)
{
	CCollisionSphere *pCollision = GetClsnSphere();

	if (pCollision != nullptr)
	{
		D3DXVECTOR3 pos = GetMtxPos(IDX_WAIST);

		pos.y -= 25.0f;

#ifdef _DEBUG
		//CEffect3D::Create(pos, pCollision->GetRadius(), 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif

		pCollision->SetPositionOld(pCollision->GetPosition());
		pCollision->SetPosition(pos);
		pCollision->SetRadius(Boss::RADIUS_COLLISION);
	}
}

//=====================================================
// �ڕW�ʒu�ւ̈ړ�
//=====================================================
bool CEnemyBoss::FollowDest(void)
{
	bool bEnd = false;

	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 vecDiff = m_info.posDest - pos;

	// ���������̎擾
	float fLength = D3DXVec3Length(&vecDiff);

	if (LINE_END > fLength)
	{
		bEnd = true;
	}

	vecDiff *= MOVE_FACT;

	pos += vecDiff;

	//SetPosition(pos);

	return bEnd;
}

//=====================================================
// ��Ԃ̐؂�ւ�
//=====================================================
void CEnemyBoss::ChangeState(CStateBoss *pNext)
{
	if (pNext == nullptr)
		assert(("�{�X�̏�Ԃ�nullptr�œn����܂���",false));

	if (m_info.pState != nullptr)
	{
		delete m_info.pState;
		m_info.pState = nullptr;
	}

	m_info.pState = pNext;
	
	if (m_info.pState != nullptr)
	{
		m_info.pState->Init(this);
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CEnemyBoss::Hit(float fDamage)
{
	CEnemy::STATE state = CEnemy::GetState();

	if (state == CEnemy::STATE_NORMAL)
	{
		float fLife = CEnemy::GetLife();

		fLife -= fDamage;

		CSound *pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{// �q�b�g�T�E���h
			//pSound->Play(CSound::LABEL_SE_HIT_BOSS);
		}

		if (fLife <= 0.0f)
		{// ���S���

			fLife = 0.0f;

			// �X�R�A�Ǘ�
			ManageScore();

			// �����蔻��폜
			DeleteCollision();

			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
				pSlow->SetSlowTime(0.5f,0.1f);

			if (m_info.bTrans == false)
			{
				ChangeState(new CStateBossTrans);

				CFade *pFade = CFade::GetInstance();

				if (pFade != nullptr)
				{
					pFade->SetFade(CScene::MODE_GAME, false);
				}
			}
			else
			{
				ChangeState(new CStateBossDeath);

				if (pSlow != nullptr)
					pSlow->SetSlowTime(3.0f, 0.1f);

				CCamera *pCamera = CManager::GetCamera();

				if (pCamera != nullptr)
					pCamera->SetQuake(1.5f, 1.5f, 160);
			}
		}
		else
		{
			state = CEnemy::STATE_DAMAGE;
		}

		CEnemy::SetLife(fLife);
	}

	CEnemy::SetState(state);
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyBoss::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\n�{�X�ʒu�F[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	CDebugProc::GetInstance()->Print("\n�{�X�̗́F[%f]", GetLife());
	CDebugProc::GetInstance()->Print("\n�{�X��ԁF[%d]",CEnemy::GetState());
	CDebugProc::GetInstance()->Print("\n�{�X���[�V�����F[%d]", GetMotion());
	CDebugProc::GetInstance()->Print("\n�L�[�F[%d]", GetKey());
	CDebugProc::GetInstance()->Print("\n�t���[���F[%d]", GetFrame());
	CDebugProc::GetInstance()->Print("\nIsFinish�F[%d]", IsFinish());
#endif
}