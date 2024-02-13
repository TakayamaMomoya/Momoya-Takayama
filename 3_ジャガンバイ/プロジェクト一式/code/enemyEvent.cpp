//*****************************************************
//
// �G�C�x���g�̏���[enemyEvent.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemyEvent.h"
#include "manager.h"
#include "light.h"
#include "enemyManager.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float INITIAL_SCALE = 0.5f;	// �����̃^�C���X�P�[��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CEnemyEvent *CEnemyEvent::m_pEnemyEvent = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyEvent::CEnemyEvent(int nPriority) : CObject(nPriority)
{
	m_fLife = 0.0f;
	m_fDelay = 0.0f;
	m_fScaleTime = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyEvent::~CEnemyEvent()
{

}

//=====================================================
// ��������
//=====================================================
CEnemyEvent *CEnemyEvent::Create(float fLife, float fDelay)
{
	if (m_pEnemyEvent == nullptr)
	{
		m_pEnemyEvent = new CEnemyEvent;

		if (m_pEnemyEvent != nullptr)
		{
			m_pEnemyEvent->m_fLife = fLife;
			m_pEnemyEvent->m_fDelay = fDelay;

			// ������
			m_pEnemyEvent->Init();
		}
	}

	return m_pEnemyEvent;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyEvent::Init(void)
{
	m_fScaleTime = INITIAL_SCALE;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyEvent::Uninit(void)
{
	// ���C�g�̐F��߂�
	CLight *pLight = CManager::GetLight();

	if (pLight != nullptr)
	{
		pLight->ResetColDest();
	}

	// �G�̏o���p�x��߂�
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->SetTimeScale(1.0f);
	}

	m_pEnemyEvent = nullptr;

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyEvent::Update(void)
{
	// �����ł̏���
	float fTick = CManager::GetTick();

	if (m_fDelay > 0.0f)
	{
		// �f�B���C
		m_fDelay -= fTick;

		if (m_fDelay <= 0.0f)
		{// �C�x���g�̊J�n
			StartEvent();
		}
	}
	else
	{// �C�x���g�̍X�V
		m_fLife -= fTick;

		if (m_fLife <= 0.0f)
		{
			Uninit();
		}
	}
}

//=====================================================
// �C�x���g�̊J�n
//=====================================================
void CEnemyEvent::StartEvent(void)
{
	// ���C�g��Ԃ�����
	CLight *pLight = CManager::GetLight();

	if (pLight != nullptr)
	{
		for (int i = 0; i < MAX_LIGHT; i++)
		{
			pLight->SetColDest(i, D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f));
		}
	}

	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_SE_SIREN);
	}

	// �G�̏o���p�x��ύX
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->SetTimeScale(m_fScaleTime);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyEvent::Draw(void)
{

}