//*****************************************************
//
// �h�A�̏���[door.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "door.h"
#include "playerManager.h"
#include "player.h"
#include "manager.h"
#include "collision.h"
#include "universal.h"
#include "fan3D.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* BODY_PATH = "data\\MODEL\\gimmick\\door000.x";	// ���f���̃p�X
	const float INITIAL_LIFE = 5.0f;	// �̗�
	const float ROLL_SPEED = 0.3f;	// ��]���x
	const float OPEN_ANGLE = D3DX_PI * 0.5f;	// ��]����
	const float OPEN_LINE = 0.1f;	// �J���Ă邵�����l
	const float OPEN_SPEED = 0.05f;	// �J�����x
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CDoor::CDoor(int nPriority) : CGimmick(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
	m_fCtr = 0.0f;
	m_bSound = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CDoor::~CDoor()
{

}

//=====================================================
// ��������
//=====================================================
CDoor *CDoor::Create(void)
{
	CDoor *pDoor = nullptr;

	if (pDoor == nullptr)
	{
		pDoor = new CDoor;

		if (pDoor != nullptr)
		{
			// ������
			pDoor->Init();
		}
	}

	return pDoor;
}

//=====================================================
// ����������
//=====================================================
HRESULT CDoor::Init(void)
{
	// �p���N���X�̏�����
	CGimmick::Init();

	// ���f���̓Ǎ�
	int nIdx = CModel::Load((char*)BODY_PATH);
	SetIdxModel(nIdx);
	BindModel(nIdx);

	// �����蔻��̐���
	if (m_info.pCollisionCube == nullptr)
	{
		m_info.pCollisionCube = CCollisionCube::Create(CCollision::TAG_BLOCK, this);

		m_info.pCollisionCube->SetPosition(GetPosition());

		m_info.pCollisionCube->SetVtx(GetVtxMax(), GetVtxMin());
	}

	// �l�̏�����
	m_info.fLife = INITIAL_LIFE;
	m_info.state = STATE_NORMAL;
	m_info.rotDestY = D3DX_PI * 0.5f;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CDoor::Uninit(void)
{
	if (m_info.pCollisionCube != nullptr)
	{
		m_info.pCollisionCube->Uninit();
		m_info.pCollisionCube = nullptr;
	}

	// �p���N���X�̏I��
	CGimmick::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CDoor::Update(void)
{
	// �p���N���X�̍X�V
	CGimmick::Update();

	if (m_info.state == STATE_OPEN)
	{// �J������
		Open();
	}

	if (GetGuide() == nullptr)
	{// �v���C���[�����ꂽ��Q�[�W������
		if (m_info.pGauge != nullptr)
		{
			m_info.pGauge->Uninit();
			m_info.pGauge = nullptr;
		}
		m_fCtr = 0.0f;
	}
}

//=====================================================
// �J������
//=====================================================
void CDoor::Open(void)
{
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr && m_bSound == false)
	{
		pSound->Play(pSound->LABEL_SE_DOOR_OPEN);
		m_bSound = true;
	}

	// �����p�x�̎擾
	D3DXVECTOR3 rot = GetRot();

	float fRotDiff = (m_info.rotDestY - rot.y);

	universal::LimitRot(&fRotDiff);

	if (fRotDiff * fRotDiff <= OPEN_LINE * OPEN_LINE)
	{// �J������������
		m_info.state = STATE_NONE;
		m_bSound = false;
	}

	fRotDiff *= OPEN_SPEED;

	// �p�x�̔��f�Ɣ���
	rot.y += fRotDiff;

	SetRot(rot);
}

//=====================================================
// �C���^���N�g
//=====================================================
void CDoor::Interact(CObject *pObj)
{
	if (m_info.state != STATE_NORMAL)
	{
		return;
	}

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pObj == nullptr || pPlayerManager == nullptr)
	{
		return;
	}

	// �v���C���[�擾
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			if ((CObject*)pPlayer == pObj)
			{// �v���C���[�̃C���^���N�g���o
				bool bInteract = pPlayer->InputInteractPress();

				if (bInteract)
				{// �s�b�L���O��i�߂�
					CWeapon *pWeapon = pPlayer->GetWeapon();

					if (pWeapon != nullptr)
					{
						bool bEnable = pWeapon->IsEnable();

						if (bEnable)
						{
							pWeapon->SetEnable(false);
						}
					}
						
					float Tick = CManager::GetTick();
					m_fCtr += Tick;

					if (m_fCtr >= 0.5f)
					{
						CSound* pSound = CSound::GetInstance();

						if (pSound != nullptr)
						{
							pSound->Play(pSound->LABEL_SE_DOOR_TOUCH);
						}

						m_fCtr = 0.0f;
					}

					// �v���C���[�̃h�A���͏��
					pPlayer->SetDoorPress(true);

					proceed(pPlayer);
				}
				else
				{
					CWeapon *pWeapon = pPlayer->GetWeapon();

					if (pWeapon != nullptr)
					{
						bool bEnable = pWeapon->IsEnable();

						if (bEnable == false)
						{
							pWeapon->SetEnable(true);
						}
					}
				}
			}
		}
	}
}

//=====================================================
// �v���C���[���o�����o
//=====================================================
void CDoor::Exit(CObject* pObj)
{
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pObj == nullptr || pPlayerManager == nullptr)
	{
		return;
	}

	// �v���C���[�擾
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			if ((CObject*)pPlayer == pObj)
			{// �v���C���[�̌��o
				CWeapon *pWeapon = pPlayer->GetWeapon();

				if (pWeapon != nullptr)
				{
					bool bEnable = pWeapon->IsEnable();

					if (bEnable == false)
					{
						pWeapon->SetEnable(true);
					}
				}
			}
		}
	}
}

//=====================================================
// �s�b�L���O��i�߂�
//=====================================================
void CDoor::proceed(CPlayer *pPlayer)
{
	if (m_info.pGauge == nullptr)
	{// �Q�[�W�̐���
		D3DXVECTOR3 pos = GetPosition();

		pos.y += 150.0f;
		pos.x += 50.0f;

		m_info.pGauge = CFan3D::Create();
		m_info.pGauge->SetPosition(pos);
		m_info.pGauge->SetRadius(30.0f);
		m_info.pGauge->EnableBillboard(true);
		m_info.pGauge->EnableZtest(true);
	}

	if (m_info.pGauge != nullptr)
	{// �Q�[�W�̊p�x�ݒ�
		float fRate = m_info.fLife / INITIAL_LIFE;
		m_info.pGauge->SetAngleMax(fRate);
	}

	float fTick = CManager::GetTick();

	m_info.fLife -= fTick;

	if (m_info.fLife < 0.0f)
	{
		SetEnable(false);

		m_info.state = STATE_OPEN;
		SetEnable(false);

		if (m_info.pCollisionCube != nullptr)
		{// �����蔻��̍폜
			m_info.pCollisionCube->Uninit();
			m_info.pCollisionCube = nullptr;
		}

		CWeapon *pWeapon = pPlayer->GetWeapon();

		if (pWeapon != nullptr)
		{
			bool bEnable = pWeapon->IsEnable();

			if (bEnable == false)
			{
				pWeapon->SetEnable(true);
			}
		}
	}
}

//=====================================================
// �ʒu�̐ݒ�
//=====================================================
void CDoor::SetPosition(D3DXVECTOR3 pos)
{
	// �p���N���X�̐ݒu
	CObjectX::SetPosition(pos);

	if (m_info.pCollisionCube != nullptr)
	{// �����蔻��̒Ǐ]
		D3DXVECTOR3 pos = GetPosition();

		m_info.pCollisionCube->SetPosition(pos);
	}
}

//=====================================================
// ���̊p�x�ݒ�
//=====================================================
void CDoor::SetOrgRot(float rotY)
{
	m_info.orgRotY = rotY;

	universal::LimitRot(&m_info.rotDestY);

	if (rotY >= 3.0f || rotY <= -3.0f)
	{
		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 vtxMax = GetVtxMax();
			D3DXVECTOR3 vtxMin = GetVtxMin();
			D3DXVECTOR3 vtxTemp = vtxMin;

			vtxMin = { -vtxMax.x,vtxMin.y,vtxMin.z };
			vtxMax = { -vtxTemp.x,vtxMax.y,vtxMax.z };

			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}
	else if (rotY >= 1.0f)
	{
		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 vtxMax = GetVtxMax();
			D3DXVECTOR3 vtxMin = GetVtxMin();
			D3DXVECTOR3 vtxTemp = vtxMin;

			vtxMin = { -vtxMax.z,vtxMin.y,-vtxMax.x };
			vtxMax = { -vtxTemp.z,vtxMax.y,-vtxTemp.x };

			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}
	else if (rotY <= -1.0f)
	{
		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 vtxMax = GetVtxMax();
			D3DXVECTOR3 vtxMin = GetVtxMin();
			D3DXVECTOR3 vtxTemp = vtxMin;

			vtxMin = { -vtxMax.z,vtxMin.y,vtxMin.x };
			vtxMax = { -vtxTemp.z,vtxMax.y,vtxMax.x };

			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}
}

//=====================================================
// �ڕW�̊p�x�ݒ�
//=====================================================
void CDoor::SetDestRot(float fRot)
{
	m_info.rotDestY = fRot;
}

//=====================================================
// �`�揈��
//=====================================================
void CDoor::Draw(void)
{
	// �p���N���X�̕`��
	CGimmick::Draw();
}