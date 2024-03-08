//*****************************************************
//
// �Z�[�u�f�[�^�Ǘ�[saveDataManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "saveDataManager.h"
#include "checkPointManager.h"
#include "player.h"
#include "game.h"
#include <stdio.h>

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* SAVE_PATH = "data\\TEXT\\save.txt";	// �Z�[�u�f�[�^�̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CSaveDataManager *CSaveDataManager::m_pSaveDataManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CSaveDataManager::CSaveDataManager()
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSaveDataManager::~CSaveDataManager()
{

}

//=====================================================
// ��������
//=====================================================
CSaveDataManager *CSaveDataManager::Create(void)
{
	if (m_pSaveDataManager == nullptr)
	{
		m_pSaveDataManager = new CSaveDataManager;

		if (m_pSaveDataManager != nullptr)
		{
			m_pSaveDataManager->Init();
		}
	}

	return m_pSaveDataManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSaveDataManager::Init(void)
{
	Load();

	return S_OK;
}

//=====================================================
// �Ǎ�
//=====================================================
void CSaveDataManager::Load(void)
{
	// �ϐ��錾
	char cTemp[256];
	int nCntPos = 0;

	// �t�@�C������ǂݍ���
	FILE *pFile = fopen(SAVE_PATH, "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "PROGRESS") == 0)
			{// �i�s��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.nProgress);
			}

			if (strcmp(cTemp, "LIFE_PLAYER") == 0)
			{// �̗�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_info.fLife);
			}

			if (strcmp(cTemp, "INITIALLIFE_PLAYER") == 0)
			{// �����̗�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_info.fInitialLife);
			}

			if (strcmp(cTemp, "ADDREWARD") == 0)
			{// ���Z��V
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.nAddReward);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
}

//=====================================================
// �I������
//=====================================================
void CSaveDataManager::Uninit(void)
{
	m_pSaveDataManager = nullptr;

	delete this;
}

//=====================================================
// �ۑ�
//=====================================================
void CSaveDataManager::Save(void)
{
	CCheckPointManager *pCheck = CCheckPointManager::GetInstance();
	CPlayer *pPlayer = CPlayer::GetInstance();
	CGame *pGame = CGame::GetInstance();

	if (pCheck == nullptr || pGame == nullptr)
		assert(("�ۑ����s�I",false));

	float fIntialLife = 0;
	float fLife = 0;

	if (pPlayer != nullptr)
	{
		fIntialLife = pPlayer->GetParam().fInitialLife;
		fLife = pPlayer->GetLife();
	}

	int nProgress = pCheck->GetProgress();

	int nAddReward = pGame->GetAddReward();

	FILE *pFile = nullptr;

	pFile = fopen(SAVE_PATH, "w");

	if (pFile != nullptr)
	{
		fprintf(pFile, "PROGRESS = %d\n", nProgress);

		fprintf(pFile, "LIFE_PLAYER = %.2f\n", fLife);

		fprintf(pFile, "INITIALLIFE_PLAYER = %.2f\n", fIntialLife);

		fprintf(pFile, "ADDREWARD = %d\n", nAddReward);

		fprintf(pFile, "END_SCRIPT");

		fclose(pFile);
	}
}