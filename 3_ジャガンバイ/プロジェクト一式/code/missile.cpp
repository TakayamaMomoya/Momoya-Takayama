//*****************************************************
//
// �e����[Missile.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "missile.h"
#include "manager.h"
#include "renderer.h"
#include "object3D.h"
#include "objectX.h"
#include "particle.h"
#include "debugproc.h"
#include "animEffect3D.h"
#include "anim3D.h"
#include "record.h"
#include "sound.h"
#include "weaponManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* MISSILE_MODEL_PATH = "data\\MODEL\\weapon\\atomic.x";	// �~�T�C���{�̂̃��f���p�X
	const float INITIAL_LIFE = 1.0f;	// �����̗�
	const float INITIAL_SPEED = 12.0f;	// �������x
	const float INITIAL_SPEED_MAX = 12.0f;	// �����̍ő呬�x
	const float INITIAL_RADIUS_FUZE = 70.0f;	// �����N�����a
	const float INITIAL_RADIUS_EXPLOSION = 200.0f;	// �����������a
	const float INITIAL_DAMAGE = 10.0f;	// �����_���[�W
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CMissile::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CMissile::CMissile(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
	ZeroMemory(&m_infoVisual, sizeof(SInfoVisual));

	m_info.nIdxPlayer = -1;

	// �����J�E���g�A�b�v
	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMissile::~CMissile()
{
	m_nNumAll--;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMissile::Init(void)
{
	CWeaponManager* pWeaponManager = CWeaponManager::GetInstance();

	// �����ڂ̐���
	CreateVisual();

	m_info.fLife = INITIAL_LIFE;
	m_info.fSpeed = INITIAL_SPEED;
	m_info.fRadiusExplosion = INITIAL_RADIUS_FUZE;
	m_info.fDamage = INITIAL_DAMAGE;

	if (pWeaponManager != nullptr)
	{// �O���t�@�C���̏���ǂݍ���
		m_info.fLife = pWeaponManager->GetBaseInfo(CWeapon::TYPE_LAUNCHER).fLifeBullet;
		m_info.fDamage = pWeaponManager->GetBaseInfo(CWeapon::TYPE_LAUNCHER).fDamage;
	}

	if (m_info.pCollisionSphere != nullptr)
	{
		m_info.pCollisionSphere->SetRadius(m_info.fRadiusExplosion);
	}

	return S_OK;
}

//=====================================================
// �����ڂ̐���
//=====================================================
void CMissile::CreateVisual(void)
{
	if (m_infoVisual.pMissile == nullptr)
	{
		m_infoVisual.pMissile = CObjectX::Create();

		if (m_infoVisual.pMissile != nullptr)
		{
			// ���f���̓Ǎ�
			int nIdx = CModel::Load((char*)MISSILE_MODEL_PATH);
			m_infoVisual.pMissile->BindModel(nIdx);
		}
	}

	if (m_infoVisual.pBackLight == nullptr)
	{
		//m_infoVisual.pBackLight = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (m_infoVisual.pBackLight != nullptr)
		{

		}
	}
}

//=====================================================
// �I������
//=====================================================
void CMissile::Uninit(void)
{
	if (m_info.pCollisionSphere != nullptr)
	{// �����蔻��̔j��
		m_info.pCollisionSphere->Uninit();

		m_info.pCollisionSphere = nullptr;
	}

	// �����ڂ̔j��
	DeleteVisual();

	Release();
}

//=====================================================
// �����ڂ̔j��
//=====================================================
void CMissile::DeleteVisual(void)
{
	if (m_infoVisual.pMissile != nullptr)
	{
		m_infoVisual.pMissile->Uninit();
		m_infoVisual.pMissile = nullptr;
	}

	if (m_infoVisual.pBackLight != nullptr)
	{
		m_infoVisual.pBackLight->Uninit();
		m_infoVisual.pBackLight = nullptr;
	}
}

//=====================================================
// �X�V����
//=====================================================
void CMissile::Update(void)
{
	CSound* pSound = CSound::GetInstance();

	// �ϐ��錾
	bool bHit = false;

	// ��������
	float fTIck = CManager::GetTick();

	m_info.fLife -= fTIck;

	// �ړ��̊Ǘ�
	ManageMove();

	// �����ڂ̒Ǐ]
	FollowVisual();

	// �p�[�e�B�N���̐���
	CParticle::Create(m_info.pos, CParticle::TYPE_MISSILE_SMOKE);
	CParticle::Create(m_info.pos, CParticle::TYPE_MISSILE_SPARK);

	if (m_info.pCollisionSphere != nullptr)
	{// �����蔻��̊Ǘ�
		D3DXVECTOR3 pos = GetPosition();

		m_info.pCollisionSphere->SetPosition(pos);

		// �G�Ƃ̓����蔻��
		if (m_info.pCollisionSphere->OnEnter(CCollision::TAG_ENEMY))
		{// �ΏۂƂ̓����蔻��
			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_EXPLOSION);
			}

			bHit = true;

			Death();
		}
		else
		{
			if (m_info.pCollisionSphere->TriggerCube(CCollision::TAG_BLOCK))
			{// �u���b�N�Ƃ̓����蔻��
				if (pSound != nullptr)
				{
					pSound->Play(pSound->LABEL_SE_EXPLOSION);
				}

				bHit = true;

				Death();
			}
		}
	}

	if (bHit == false)
	{
		if (m_info.fLife < 0)
		{// �����ł̍폜
			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_EXPLOSION);
			}

			Death();
		}
	}
}

//=====================================================
// �ړ��̊Ǘ�
//=====================================================
void CMissile::ManageMove(void)
{
	D3DXVECTOR3 move = m_info.move;
	D3DXVECTOR3 rot = m_info.rot;

	m_info.posOld = m_info.pos;

	// �ړ��ʂ̐ݒ�
	move += D3DXVECTOR3
	{
		sinf(m_info.rot.y) * m_info.fSpeed,
		0.0f,
		cosf(m_info.rot.y) * m_info.fSpeed
	};

	m_info.move -= move;

	// ���x����
	universal::LimitSpeed(&m_info.move, INITIAL_SPEED_MAX);

	// �ړ��ʂ����Z
	m_info.pos += m_info.move;
}

//=====================================================
// �����ڂ̒Ǐ]
//=====================================================
void CMissile::FollowVisual(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = m_info.rot;

	if (m_infoVisual.pMissile != nullptr)
	{
		m_infoVisual.pMissile->SetPosition(pos);
		m_infoVisual.pMissile->SetRot(rot);
	}
}

//=====================================================
// ���S����
//=====================================================
void CMissile::Death(void)
{
	D3DXVECTOR3 pos = GetPosition();

	// �A�j���[�V�����G�t�F�N�g�̐���
	CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

	if (pAnimManager != nullptr)
	{
		CAnim3D *pAnim3D = pAnimManager->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);

		if (pAnim3D != nullptr)
		{
			pAnim3D->SetSize(INITIAL_RADIUS_EXPLOSION, INITIAL_RADIUS_EXPLOSION);
			pAnim3D->EnableZtest(true);
		}
	}

	// �����Ń_���[�W��^����
	if (m_info.pCollisionSphere != nullptr)
	{
		m_info.pCollisionSphere->SetRadius(INITIAL_RADIUS_EXPLOSION);

		CRecord* pRecord = CRecord::GetInstance();

		if (pRecord != nullptr)
		{
			pRecord->CheckDeathEnemyAll(
				m_info.pCollisionSphere->GetCollision(),
				m_info.pCollisionSphere->GetPosition(),
				m_info.pCollisionSphere->GetRadius(),
				m_info.nIdxPlayer);
		}

		m_info.pCollisionSphere->DamageAll(CCollision::TAG_ENEMY,m_info.fDamage);
	}

	// �I������
	Uninit();
}

//=====================================================
// �`�揈��
//=====================================================
void CMissile::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_info.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_info.rot.y, m_info.rot.x, m_info.rot.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_info.pos.x, m_info.pos.y, m_info.pos.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_info.mtxWorld);

#ifdef _DEBUG
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\n�~�T�C���ʒu[%f,%f,%f]", m_info.pos.x, m_info.pos.y, m_info.pos.z);
#endif
}

//=====================================================
// ��������
//=====================================================
CMissile *CMissile::Create(void)
{
	CMissile *pMissile = nullptr;

	if (pMissile == nullptr)
	{// �C���X�^���X����
		pMissile = new CMissile;

		if (pMissile != nullptr)
		{
			if (pMissile->m_info.pCollisionSphere == nullptr)
			{// �����蔻�萶��
				pMissile->m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYERBULLET, CCollision::TYPE_SPHERE, pMissile);
			}

			if (pMissile->m_info.pCollisionSphere != nullptr)
			{
				pMissile->m_info.pCollisionSphere->SetPosition(pMissile->m_info.pos);

				pMissile->m_info.pCollisionSphere->SetRadius(20.0f);
			}

			// ����������
			pMissile->Init();
		}
	}

	return pMissile;
}