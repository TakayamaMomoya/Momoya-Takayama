//*****************************************************
//
// �R���e�i�}�l�[�W���[[containerManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "containerManager.h"
#include "container.h"
#include "manager.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CContainerManager *CContainerManager::m_pContainerManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CContainerManager::CContainerManager()
{
	ZeroMemory(&m_apInfoSave[0], sizeof(m_apInfoSave));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CContainerManager::~CContainerManager()
{

}

//=====================================================
// ��������
//=====================================================
CContainerManager *CContainerManager::Create(void)
{
	if (m_pContainerManager == nullptr)
	{// �C���X�^���X����
		m_pContainerManager = new CContainerManager;

		// ����������
		m_pContainerManager->Init();
	}

	return m_pContainerManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CContainerManager::Init(void)
{
	// �Ǎ�����
	Load();

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CContainerManager::Load(void)
{
	// �ϐ��錾
	char cTemp[256];
	int nCntAttack = 0;

	// �t�@�C������ǂݍ���
	FILE *pFile = fopen("data\\TEXT\\container.txt", "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "TIME_SPAWN") == 0)
			{// ���X�|�[������
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_fTimeSpawm);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		assert(("�G���ǂݍ��݂Ɏ��s", false));
	}
}

//=====================================================
// �I������
//=====================================================
void CContainerManager::Uninit(void)
{
	for (int i = 0; i < containermanager::MAX_SAVE; i++)
	{
		DeleteSaveInfo(i);
	}

	m_pContainerManager = nullptr;

	Release();
}

//=====================================================
// �ʂ̍폜
//=====================================================
void CContainerManager::DeleteSaveInfo(int nIdx)
{
	if (m_apInfoSave[nIdx] != nullptr)
	{
		delete m_apInfoSave[nIdx];
		m_apInfoSave[nIdx] = nullptr;
	}
}

//=====================================================
// �X�V����
//=====================================================
void CContainerManager::Update(void)
{
	for (int i = 0; i < containermanager::MAX_SAVE; i++)
	{
		if (m_apInfoSave[i] != nullptr)
		{
			float fTick = CManager::GetTick();

			m_apInfoSave[i]->fTimeSpawn -= fTick;

			if (m_apInfoSave[i]->fTimeSpawn <= 0.0f)
			{// �R���e�i���X�|�[��������
				CContainer *pContainer = CContainer::Create();

				if (pContainer != nullptr)
				{
					pContainer->SetPosition(m_apInfoSave[i]->pos);
				}

				// ���̍폜
				DeleteSaveInfo(i);
			}
		}
	}
}

//=====================================================
// �ʒu�̕ۑ�
//=====================================================
void CContainerManager::SavePosition(D3DXVECTOR3 pos)
{
	for (int i = 0; i < containermanager::MAX_SAVE; i++)
	{
		if (m_apInfoSave[i] == nullptr)
		{
			m_apInfoSave[i] = new SInfoSave;

			m_apInfoSave[i]->pos = pos;

			m_apInfoSave[i]->fTimeSpawn = m_fTimeSpawm;

			break;
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CContainerManager::Draw(void)
{

}