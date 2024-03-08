//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemy.h"
#include "manager.h"
#include "collision.h"
#include "debugproc.h"
#include "particle.h"
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "motion.h"
#include "universal.h"
#include "animEffect3D.h"
#include "anim3D.h"
#include "sound.h"
#include "particle.h"
#include "debrisSpawner.h"
#include "texture.h"
#include "UI.h"
#include "slow.h"
#include "meshfield.h"
#include "effect3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float INITIAL_LIFE = 5.0f;	// �����̗�
const float INITIAL_SPEED = 1.0f;	// �������x
const float LINE_CHASE = 470;	// �ǐՏ�ԂɈڍs����G���A
const float LINE_ARRIVAL = 20.0f;	// �ڕW�ʒu�ɓ��B��������̂������l
const int DAMAGE_FRAME = 10;	// �_���[�W��Ԃ̃t���[����
const int TIME_DEATH = 60;	// ���S�܂ł̎���
const float DAMAGE_THROWN = 10.0f;	// �����ł̃_���[�W
const float SIZE_CURSOR = 30.0f;	// �J�[�\���T�C�Y
const float INITIAL_DIST_LOCK = 5000.0f;	// �J�[�\���T�C�Y
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\lockon00.png";	// �J�[�\���̃e�N�X�`��
const float INITIAL_DIST_MOVESTATE[CEnemy::MOVESTATE_MAX] =
{// �ړ���Ԃ��؂�ւ�鋗��
	0.0f,
	3000.0f,
	5000.0f,
	7000.0f
};
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CEnemy::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemy::CEnemy()
{
	// �����J�E���g�A�b�v
	m_nNumAll++;

	ZeroMemory(&m_info, sizeof(SInfo));

	// ���X�g�ɒǉ�
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->AddToList(this);
	}
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemy::~CEnemy()
{
	m_nNumAll--;

	// ���X�g����폜
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->RemoveFromList(this);
	}
}

//=====================================================
// ��������
//=====================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// �C���X�^���X����
		switch (type)
		{
		case TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			pEnemy->Load("data\\Character\\robot00.txt");

			break;
		default:
			break;
		}

		// �ʒu�ݒ�
		pEnemy->SetPosition(pos);

		// ����������
		pEnemy->Init();
	}

	return pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemy::Init(void)
{
	// �p���N���X�̏�����
	CMotion::Init();

	// �^�C�v�̐ݒ�
	CObject::SetType(TYPE_ENEMY);

	// �����蔻�萶��
	CreateCollision();

	// �p�����[�^�[�����ݒ�
	SetLife(INITIAL_LIFE, true);
	SetMoveSpeed(INITIAL_SPEED);
	m_info.fDistLock = INITIAL_DIST_LOCK;
	m_info.bStamp = true;
	m_info.fFactMove = 0.98f;

	// �ʏ��Ԃɂ���
	m_info.state = STATE_NORMAL;
	m_info.moveState = MOVESTATE_WAIT;;

	SetPositionOld(GetPosition());

	// �e�̗L����
	//EnableShadow(true);
	SetPosShadow(D3DXVECTOR3(0.0f, 0.5f, 0.0f));

	for (int i = 0; i < MOVESTATE_MAX; i++)
	{
		m_info.aDistMoveState[i] = INITIAL_DIST_MOVESTATE[i];
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemy::Uninit(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
		pEnemyManager->CheckDeathLockon(this);

	if (m_info.pCursor != nullptr)
	{
		m_info.pCursor->Uninit();
		m_info.pCursor = nullptr;
	}

	// �����蔻��폜
	DeleteCollision();

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemy::Update(void)
{
	// �p���N���X�̍X�V
	CMotion::Update();

	// ��ԊǗ�
	ManageState();

	// �ړ���ԊǗ�
	ManageMoveState();

	// �ړ��ʂ𔽉f
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();

	SetPositionOld(pos);

	CSlow *pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		// �ʒu�̍X�V
		pos += move * fScale;
	}
	else
	{
		// �ʒu�̍X�V
		pos += move;
	}

	SetPosition(pos);

	// �����蔻��̊Ǘ�
	ManageCollision();

	// �ړ��ʂ̌���
	move *= m_info.fFactMove;
	SetMove(move);
}

//=====================================================
// �����蔻��̊Ǘ�
//=====================================================
void CEnemy::ManageCollision(void)
{
	if (m_info.pCollisionSphere != nullptr)
	{// ���̓����蔻��̊Ǘ�
		D3DXVECTOR3 posWaist = GetMtxPos(0);

		D3DXVECTOR3 pos = GetPosition();

		m_info.pCollisionSphere->SetPosition(pos);

		if (m_info.state != STATE_THROWN)
		{
			m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_PLAYER);
			m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_ENEMY);

			SetPosition(pos);
		}

		m_info.pCollisionSphere->SetPositionOld(m_info.pCollisionSphere->GetPosition());

		m_info.pCollisionSphere->SetPosition(posWaist);
	}

	if (m_info.pCollisionCube != nullptr)
	{// �����̂̓����蔻��̊Ǘ�
		// �����蔻��̈ʒu�ݒ�
		m_info.pCollisionCube->SetPositionOld(m_info.pCollisionCube->GetPosition());
		m_info.pCollisionCube->SetPosition(GetPosition());

		D3DXVECTOR3 move = GetMove();

		// �����o���̓����蔻��
		m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);
		SetMove(move);
	}

	// ���b�V���t�B�[���h�Ƃ̓����蔻��
	CMeshField *pMesh = CMeshField::GetInstance();

	if (pMesh != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 move = GetMove();

		float fHeight = pMesh->GetHeight(pos, &move);

		if (pos.y <= fHeight)
		{
			CDebugProc::GetInstance()->Print("\n���b�V���ɓ������Ă��[%f]",fHeight);

			move.y = 0.0f;

			pos.y = fHeight;
			SetPosition(pos);
			SetMove(move);

			HitField();
		}
	}
}

//=====================================================
// �n�ʂɓ��������Ƃ��̏���
//=====================================================
void CEnemy::HitField(void)
{

}

//=====================================================
// ��ԊǗ�
//=====================================================
void CEnemy::ManageState(void)
{
	D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	switch (m_info.state)
	{
	case STATE_NORMAL:
		break;
	case STATE_THROWN:

		CollisionThrown();

		break;
	case STATE_DAMAGE:
		if (m_info.nTimerState > DAMAGE_FRAME)
		{// �ʏ��Ԃɖ߂�
			m_info.nTimerState = 0;
			m_info.state = STATE_NORMAL;

			ResetAllCol();
		}
		else
		{// �J�E���g�A�b�v
			m_info.nTimerState++;
		}

		break;
	case STATE_DEATH:

		m_info.nTimerState++;

		// �����ɂȂ�Ȃ��������
		col = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

		col.a = 1.0f - (float)((float)(m_info.nTimerState) / (TIME_DEATH));

		SetAllCol(col);

		if (m_info.nTimerState >= TIME_DEATH)
		{// ���S
			Uninit();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// �ړ���ԊǗ�
//=====================================================
void CEnemy::ManageMoveState(void)
{
	switch (m_info.moveState)
	{
	case MOVESTATE_WAIT:
		// �ҋ@���
		Wait();

		break;
	case MOVESTATE_CHASE:
		// �ǐ�
		Chase();

		break;
	case MOVESTATE_ATTACK:
		// �U��
		Attack();

		break;
	}
}

//=====================================================
// �ǐՂɈڍs����
//=====================================================
void CEnemy::TransferChase(void)
{
	m_info.moveState = MOVESTATE_CHASE;
}

//=====================================================
// �G���A�ɓ���������
//=====================================================
bool CEnemy::IsInArea(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (pos.x <= LINE_CHASE && pos.x >= -LINE_CHASE && pos.z <= LINE_CHASE && pos.z >= -LINE_CHASE)
	{
		return true;
	}

	return false;
}

//=====================================================
// �������Ă鎞�̔���
//=====================================================
void CEnemy::CollisionThrown(void)
{
	if (m_info.pCollisionSphere != nullptr)
	{
		m_info.pCollisionSphere->SetRadius(200.0f);

		if (m_info.pCollisionSphere->OnEnter(CCollision::TAG_ROCKET))
		{// �~�T�C���̔j��
			m_info.pCollisionSphere->DamageAll(CCollision::TAG_ROCKET, DAMAGE_THROWN);
		}

		if (m_info.pCollisionSphere->OnEnter(CCollision::TAG_ENEMY))
		{
			// ���������G�Ƀ_���[�W��^����
			m_info.pCollisionSphere->DamageAll(CCollision::TAG_ENEMY, DAMAGE_THROWN);

			D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

			SetMove(move);
		}

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(90.0f);
		}
	}
}

//=====================================================
// �ҋ@
//=====================================================
void CEnemy::Wait(void)
{
	MoveToDest(m_info.posDest, 1.0f);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	float fDist = 0.0f;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	fDist = D3DXVec3Length(&vecDiff);

	float fRot = atan2f(vecDiff.x, vecDiff.z);

	D3DXVECTOR3 rot = GetRotation();

	universal::FactingRot(&rot.y, fRot, 0.1f);

	SetRotation(rot);

	if (m_info.aDistMoveState[MOVESTATE_CHASE] > fDist)
	{// �ǐՂɈڍs
		m_info.moveState = MOVESTATE_CHASE;
	}
}

//=====================================================
// �ڕW�̒ǐ�
//=====================================================
void CEnemy::Chase(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	float fDist = 0.0f;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	fDist = D3DXVec3Length(&vecDiff);

	if (m_info.aDistMoveState[MOVESTATE_ATTACK] > fDist)
	{// �U���Ɉڍs
		m_info.moveState = MOVESTATE_ATTACK;
	}
}

//=====================================================
// �ڕW�Ɍ�����
//=====================================================
bool CEnemy::MoveToDest(D3DXVECTOR3 posDest, float fSpeed)
{
	CEffect3D::Create(posDest, 20.0f, 1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	bool bArrival = false;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posDest - pos;

	// �ړ��I���̔���
	float fLength = D3DXVec3Length(&vecDiff);
	bArrival = fLength < LINE_ARRIVAL;

	// �ړ��ʂ̉��Z
	D3DXVec3Normalize(&vecDiff, &vecDiff);

	vecDiff *= fSpeed;

	D3DXVECTOR3 move = GetMove();

	move += vecDiff;

	SetMove(move);

	return bArrival;
}

//=====================================================
// �U���̃^�C�}�[
//=====================================================
bool CEnemy::AttackTimer(float fTime)
{
	float fDeltaTime = CManager::GetDeltaTime();
	float fScaleTime = Slow::GetScale();

	m_info.fCntAttack += fDeltaTime * fScaleTime;

	if (m_info.fCntAttack > fTime)
	{
		m_info.fCntAttack = 0.0f;

		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================
// �U��
//=====================================================
void CEnemy::Attack(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	float fDist = 0.0f;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	fDist = D3DXVec3Length(&vecDiff);

	if (m_info.aDistMoveState[MOVESTATE_CHASE] < fDist)
	{// �ҋ@�Ɉڍs
		m_info.moveState = MOVESTATE_WAIT;
	}
}

//=====================================================
// �̗͐ݒ�
//=====================================================
void CEnemy::SetLife(float fLife, bool bInit)
{
	if (bInit)
		m_info.fLifeInitial = fLife;

	m_info.fLife = fLife;

	if (m_info.fLife < 0)
	{
		m_info.fLife = 0;
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CEnemy::Hit(float fDamage)
{
	if (m_info.state != STATE_DEATH)
	{
		m_info.fLife -= fDamage;

		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			//pSound->Play(pSound->LABEL_SE_HIT);
		}

		if (m_info.fLife <= 0.0f)
		{// ���S���
			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			// �X�R�A�Ǘ�
			ManageScore();

			// ���S����
			Death();

			// �����蔻��폜
			DeleteCollision();
		}
		else
		{
			m_info.state = STATE_DAMAGE;

			// �q�b�g�F
			D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			SetAllCol(col);
		}
	}
}

//=====================================================
// ���S����
//=====================================================
void CEnemy::Death(void)
{
	Sound::Play(CSound::LABEL_SE_EXPLOSION00);

	D3DXVECTOR3 pos = GetMtxPos(0);

	// �A�j���[�V�����G�t�F�N�g����
	CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

	if (pAnim3D != nullptr)
	{
		CAnim3D *pAnim = pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);
	}

	// �j�А���
	CDebrisSpawner::Create(pos, CDebrisSpawner::TYPE::TYPE_DEATH);

	m_info.state = STATE_DEATH;

	Uninit();
}

//=====================================================
// �X�R�A�Ǘ�
//=====================================================
void CEnemy::ManageScore(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CEnemy::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();

#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n�G�̈ʒu�F[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	//CDebugProc::GetInstance()->Print("\n�G�̈ړ��ʁF[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
#endif
}

//=====================================================
// ���̓����蔻��̈ʒu�ݒ�
//=====================================================
void CEnemy::SetSpherePosition(D3DXVECTOR3 pos)
{
	if (m_info.pCollisionSphere != nullptr)
	{// �ʒu�ݒ�
		m_info.pCollisionSphere->SetPosition(pos);
	}
}

//=====================================================
// �����蔻�萶��
//=====================================================
void CEnemy::CreateCollision(float fRadius)
{
	m_info.fRadius = fRadius;

	if (m_info.pCollisionSphere == nullptr)
	{// ���̓����蔻�萶��
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMY, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{// ���̐ݒ�
			m_info.pCollisionSphere->SetPosition(GetPosition());
			m_info.pCollisionSphere->SetRadius(fRadius);
		}
	}

	if (m_info.pCollisionCube == nullptr)
	{// �����̂̓����蔻��
		m_info.pCollisionCube = CCollisionCube::Create(CCollision::TAG_ENEMY, this);

		D3DXVECTOR3 vtxMax = { 20,20,20 };
		D3DXVECTOR3 vtxMin = { -20,0.0f,-20 };

		if (m_info.pCollisionCube != nullptr)
		{
			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}
}

//=====================================================
// �����蔻��폜
//=====================================================
void CEnemy::DeleteCollision(void)
{
	if (m_info.pCollisionSphere != nullptr)
	{// �����蔻��̏���
		m_info.pCollisionSphere->Uninit();

		m_info.pCollisionSphere = nullptr;
	}

	if (m_info.pCollisionCube != nullptr)
	{// �����蔻��̏���
		m_info.pCollisionCube->Uninit();

		m_info.pCollisionCube = nullptr;
	}
}

//=====================================================
// ���b�N�ł��邩�ǂ����̐ݒ�
//=====================================================
void CEnemy::EnableLock(bool bLock)
{
	if (bLock)
	{
		if (m_info.pCursor == nullptr)
		{// �J�[�\������
			m_info.pCursor = CUI::Create();

			if (m_info.pCursor != nullptr)
			{
				m_info.pCursor->SetSize(SIZE_CURSOR, SIZE_CURSOR);
				int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
				m_info.pCursor->SetIdxTexture(nIdx);
				m_info.pCursor->SetVtx();
			}
		}
	}
	else
	{
		if (m_info.pCursor != nullptr)
		{
			m_info.pCursor->Uninit();
			m_info.pCursor = nullptr;
		}
	}
}

//=====================================================
// �J�[�\���ʒu�ݒ�
//=====================================================
void CEnemy::SetPositionCursor(D3DXVECTOR3 pos)
{
	if (m_info.pCursor != nullptr)
	{
		m_info.pCursor->SetPosition(pos);
		m_info.pCursor->SetVtx();
	}
}