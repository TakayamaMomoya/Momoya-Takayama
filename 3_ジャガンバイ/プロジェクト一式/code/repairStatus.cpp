//=========================================================
//
// �C���󋵏��� [repairStatus.cpp]
// Author = �����đ�Y
//
//=========================================================

//===============================================
// �C���N���[�h
//===============================================
#include "repairStatus.h"
#include "rocket.h"
#include "texture.h"
#include "game.h"

//===============================================
// �萔��`
//===============================================
namespace
{
	// �C���󋵂̃e�N�X�`���̃p�X
	const char* TEX_PATH = "data\\TEXTURE\\UI\\repair_status.png";

	// �C���󋵂̃t���[���e�N�X�`���̃p�X
	const char* FRAME_PATH = "data\\TEXTURE\\UI\\repair_status_frame.png";

	// �����̈ʒu
	const float POS_Y = 300.0f;

	// �C�������Ƃ��̕ϓ���
	const float INCREASED_VALUE = 25.0f;
}

//===============================================
// �ÓI�����o�ϐ��錾
//===============================================
CRepairStatus* CRepairStatus::m_pRepairStatus = nullptr;	// ���g�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CRepairStatus::CRepairStatus(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_pStatus = nullptr;
	m_pFrame = nullptr;
}

//===============================================
// �f�X�g���N�^
//===============================================
CRepairStatus::~CRepairStatus()
{
	
}

//===============================================
// ��������
//===============================================
CRepairStatus *CRepairStatus::Create(int nPriority)
{
	if (m_pRepairStatus == nullptr)
	{
		// �C���󋵂̐���
		m_pRepairStatus = new CRepairStatus();

		if (m_pRepairStatus != nullptr)
		{
			// ����������
			m_pRepairStatus->Init();
		}
		else if (m_pRepairStatus == nullptr)
		{
			return nullptr;
		}
	}

	return m_pRepairStatus;
}

//===============================================
// ����������
//===============================================
HRESULT CRepairStatus::Init()
{
	m_pStatus = nullptr;
	m_pFrame = nullptr;

	// �C���X�^���X���擾
	CRocket* pRocket = pRocket->GetInstance();

	if (m_pRepairStatus != nullptr)
	{
		if (pRocket != nullptr)
		{
			D3DXVECTOR3 posRocket = pRocket->GetPosition();

			if (m_pStatus == nullptr)
			{// �Q�[�W�̐���
				m_pStatus = CObject3D::Create({ posRocket.x - INCREASED_VALUE * 3, POS_Y, posRocket.z - 50.0f });

				if (m_pStatus != nullptr)
				{// �������̐ݒ�
					m_pStatus->SetSize(0.0f, 50.0f);
					m_pStatus->SetPosition({ posRocket.x - INCREASED_VALUE * 3, POS_Y, posRocket.z - 50.0f });
					int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
					m_pStatus->SetIdxTexture(nIdx);
					m_pStatus->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
					m_pStatus->EnableBillboard(true);
					m_pStatus->SetVtx();
				}
			}

			if (m_pFrame == nullptr)
			{// �g�̐���
				m_pFrame = CObject3D::Create({ posRocket.x, 250.0f, posRocket.z - 50.0f });
				
				if (m_pFrame != nullptr)
				{// �g�̏��ݒ�
					m_pFrame->SetSize(3 * INCREASED_VALUE + INCREASED_VALUE * 0.1f, 20.0f + INCREASED_VALUE * 0.1f);
					m_pFrame->SetPosition({ posRocket.x, POS_Y, posRocket.z - 50.0f });
					int nIdx = CTexture::GetInstance()->Regist(FRAME_PATH);
					m_pFrame->SetIdxTexture(nIdx);
					m_pFrame->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
					m_pFrame->EnableBillboard(true);
					m_pFrame->SetVtx();
				}
			}
		}
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CRepairStatus::Uninit(void)
{
	if (m_pStatus != nullptr)
	{
		m_pStatus->Uninit();
		m_pStatus = nullptr;
	}

	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}

	m_pRepairStatus = nullptr;
}

//===============================================
// �X�V����
//===============================================
void CRepairStatus::Update(void)
{
	// �C���X�^���X���擾
	CRocket* pRocket = pRocket->GetInstance();
	CGame* pGame = pGame->GetInstance();

	if (pRocket != nullptr)
	{// �C���󋵂ɂ���ăQ�[�W�̏�Ԃ�ς���
		int nProgress = pRocket->GetProgress();
		D3DXVECTOR3 posRocket = pRocket->GetPosition();

		if (m_pStatus != nullptr)
		{
			m_pStatus->SetSize(nProgress * INCREASED_VALUE, 20.0f);
			m_pStatus->SetPosition({ posRocket.x - INCREASED_VALUE * 3 + nProgress * INCREASED_VALUE, POS_Y, posRocket.z - 50.0f });
		}
	}

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state == CGame::STATE_ESCAPE)
		{// ���U���g�Ŕ�\���ɂ���
			Uninit();
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CRepairStatus::Draw(void)
{
	
}