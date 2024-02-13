//*****************************************************
//
// �G�}�l�[�W���[[enemyManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "enemyThief.h"
#include "playerManager.h"
#include "manager.h"
#include "camera.h"
#include "universal.h"
#include <stdio.h>
#include "game.h"
#include "rocket.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int RAND_SPAWN = 400;	// �X�|�[���͈�

const float LIMID_RANGE_LEFT = -460.0f;
const float LIMID_RANGE_RIGHT = 460.0f;
const float LIMID_RANGE_UP = 460.0f;
const float LIMID_RANGE_DOWN = -460.0f;
const int NUM_SPAWNANGLE = 3;	// �X�|�[���p�x�̐�
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CEnemyManager *CEnemyManager::m_pEnemyManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyManager::CEnemyManager()
{
	m_fTimerSpawn = 0;
	m_nMinTimeSpawnThief = 0;
	m_nMaxTimeSpawnThief = 0;
	m_fTimerThief = 0.0f;
	m_fTimeSpawnThief = 0.0f;
	m_fDistSpawn = 0.0f;
	m_fRateProgress = 0.0f;
	m_fScaleSpawnTime = 0.0f;
	ZeroMemory(&m_afTime[0], sizeof(float) * NUM_PLAYER);
	m_nMaxEnemy = 0;

	m_pHead = nullptr;
	m_pTail = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyManager::~CEnemyManager()
{

}

//=====================================================
// ��������
//=====================================================
CEnemyManager *CEnemyManager::Create(void)
{
	if (m_pEnemyManager == nullptr)
	{// �C���X�^���X����
		m_pEnemyManager = new CEnemyManager;

		// ����������
		m_pEnemyManager->Init();
	}

	return m_pEnemyManager;
}

//=====================================================
// �G�̐�������
//=====================================================
CEnemy *CEnemyManager::CreateEnemy(D3DXVECTOR3 pos, CEnemy::TYPE type)
{
	char *apPath[CEnemy::TYPE_MAX] = 
	{
		nullptr,
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy01.txt",
	};

	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// �C���X�^���X����
		switch (type)
		{
		case CEnemy::TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			break;
		case CEnemy::TYPE_THIEF:

			pEnemy = new CEnemyThief;

			break;
		default:
			break;
		}

		if (pEnemy != nullptr)
		{
			// �ʒu�ݒ�
			pEnemy->SetPosition(pos);

			// ���[�V�����Ǎ�
			pEnemy->Load(apPath[type]);

			// ����������
			pEnemy->Init();
		}
	}

	return pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyManager::Init(void)
{
	// �Ǎ�����
	Load();

	// �ŏ��̏o�����Ԑݒ�
	m_fTimeSpawnThief = (float)universal::RandRange(m_nMaxTimeSpawnThief, m_nMinTimeSpawnThief);

	// �X�P�[���̃��Z�b�g
	m_fScaleSpawnTime = 1.0f;

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CEnemyManager::Load(void)
{
	// �ϐ��錾
	char cTemp[256];
	int nCntTime = 0;

	// �t�@�C������ǂݍ���
	FILE *pFile = fopen("data\\TEXT\\enemy.txt", "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "TIME_SPAWN_THIEF") == 0)
			{// �D�_�o������
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_nMinTimeSpawnThief);
				(void)fscanf(pFile, "%d", &m_nMaxTimeSpawnThief);
			}

			if (strcmp(cTemp, "TIME_SPAWN") == 0)
			{// �o������
				(void)fscanf(pFile, "%s", &cTemp[0]);
				
				(void)fscanf(pFile, "%f", &m_afTime[nCntTime]);

				nCntTime++;
			}

			if (strcmp(cTemp, "DIST_SPAWN") == 0)
			{// �o������
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_fDistSpawn);
			}

			if (strcmp(cTemp, "PROGRESS_TIME") == 0)
			{// �i�s�x�����̊���
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_fRateProgress);
			}

			if (strcmp(cTemp, "MAX_ENEMY") == 0)
			{// �G�̍ő吔
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_nMaxEnemy);
			}

			if (strcmp(cTemp, "NUM_SPAWN_ANGLE") == 0)
			{// �o���p�x�Ǎ�
				int nCntAngle = 0;

				// �o���p�x���Ǎ�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_nNumSpawnAngle);

				if (m_pAngleSpawn == nullptr)
				{// ������̐���
					m_pAngleSpawn = new float[m_nNumSpawnAngle];

					for (int i = 0; i < m_nNumSpawnAngle; i++)
					{// ���̃N���A
						m_pAngleSpawn[i] = 0.0f;
					}
				}
				else
				{
					break;
				}

				if (m_pAngleSpawn == nullptr)
				{// �����ł��Ȃ������ꍇ
					assert(("�G�o���p�x���Ǎ��Ɏ��s", false));
				}

				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "ANGLE") == 0)
					{// �p�x�Ǎ�
						float fAngle;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &fAngle);

						m_pAngleSpawn[nCntAngle] = D3DXToRadian(fAngle);

						nCntAngle++;
					}

					if (m_nNumSpawnAngle <= nCntAngle)
					{
						break;
					}
				}
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
void CEnemyManager::Uninit(void)
{
	m_pEnemyManager = nullptr;

	if (m_pAngleSpawn != nullptr)
	{
		delete[] m_pAngleSpawn;
		m_pAngleSpawn = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyManager::Update(void)
{
	// �Q�[����Ԃɂ���ăX�|�[�������Ȃ�
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL)
		{
			return;
		}
	}

	// �v���C���[���̎擾
	int nIdx = 1;

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{
		nIdx = pPlayerManager->GetNumPlayer() - 1;
	}

	// ���Ԃ̉��Z
	float fTick = CManager::GetTick();

	m_fTimerSpawn += fTick;

	// �X�|�[�����Ԃ̐ݒ�
	float fTime = m_afTime[nIdx];

	fTime *= m_fScaleSpawnTime;

	if (m_fTimerSpawn >= fTime)
	{// �ʏ�G�̃X�|�[��
		SpawnNormal();
	}

	// �D�_�G�̃X�|�[��
	CRocket *pRocket = CRocket::GetInstance();

	if (pRocket != nullptr)
	{// ���ɂ���ꍇ�͒ʂ�Ȃ�
		int nProgress = pRocket->GetProgress();

		if (nProgress > 0)
		{// ��x�ł��C������Ă���ꍇ�̂݃X�|�[��
			SpawnThief();
		}
	}
}

//=====================================================
// �ʏ�G�̃X�|�[��
//=====================================================
void CEnemyManager::SpawnNormal(void)
{
	if (m_pAngleSpawn == nullptr)
	{// �p�x�̃G���[
		return;
	}

	int nNumEnemy = CEnemy::GetNumAll();

	if (nNumEnemy >= m_nMaxEnemy)
	{// �ő吔����
		return;
	}

	int nIdxAngle = universal::RandRange(m_nNumSpawnAngle, 0);

	D3DXVECTOR3 posCenter = { 0.0f,0.0f,0.0f };

	// �o��������W��ݒ�
	posCenter.x = sinf(m_pAngleSpawn[nIdxAngle]) * m_fDistSpawn;
	posCenter.z = cosf(m_pAngleSpawn[nIdxAngle]) * m_fDistSpawn;

	// �G�X�|�[��
	CreateEnemy(posCenter, CEnemy::TYPE::TYPE_NORMAL);

	m_fTimerSpawn = 0.0f;
}

//=====================================================
// �D�_�̃X�|�[��
//=====================================================
void CEnemyManager::SpawnThief(void)
{
	CRocket* pRocket = CRocket::GetInstance();
	int nProgress = 0;
	int nNumEnemy = CEnemy::GetNumAll();

	if (nNumEnemy >= m_nMaxEnemy)
	{// �ő吔����
		return;
	}

	if (pRocket != nullptr)
	{// ���P�b�g�̏C���󋵂��擾
		nProgress = pRocket->GetProgress();
	}

	float fTick = CManager::GetTick();
	m_fTimerThief += fTick;

	if (m_fTimerThief > m_fTimeSpawnThief && nProgress < 3)
	{
		D3DXVECTOR3 posCenter = { 0.0f,0.0f,0.0f };

		int nIdxAngle = universal::RandRange(m_nNumSpawnAngle, 0);

		// �o��������W��ݒ�
		posCenter.x = sinf(m_pAngleSpawn[nIdxAngle]) * m_fDistSpawn;
		posCenter.z = cosf(m_pAngleSpawn[nIdxAngle]) * m_fDistSpawn;

		// �G�X�|�[��
		CreateEnemy(posCenter, CEnemy::TYPE::TYPE_THIEF);

		m_fTimerThief = 0.0f;

		// ���ɏo�����鎞�Ԃ�ݒ�
		m_fTimeSpawnThief = (float)universal::RandRange(m_nMaxTimeSpawnThief,m_nMinTimeSpawnThief);
	}
}

//=====================================================
// �X�|�[�����Ԃ̐i�s
//=====================================================
void CEnemyManager::ProgressTimeSpawn(bool bAdd)
{
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if (bAdd)
		{
			m_afTime[i] += m_fRateProgress * m_afTime[i];
		}
		else
		{
			m_afTime[i] *= m_fRateProgress;
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyManager::Draw(void)
{
#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n�G�̈ʒu�F[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
#endif
}