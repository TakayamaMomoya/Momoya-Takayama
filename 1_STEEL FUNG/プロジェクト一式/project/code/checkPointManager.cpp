//*****************************************************
//
// �`�F�b�N�|�C���g�Ǘ��̏���[checkPointmanager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "checkPointManager.h"
#include "checkPointBehavior.h"
#include "saveDataManager.h"
#include "UI.h"
#include "texture.h"
#include "player.h"
#include "fade.h"
#include "manager.h"
#include "enemyManager.h"
#include "enemyBoss.h"
#include "debugproc.h"
#include <stdio.h>
#include "cameraBehavior.h"
#include "camera.h"
#include "UIManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* FILE_PATH = "data\\TEXT\\checkPoint.txt";	// �t�@�C���̃p�X
const float SIZE_CURSOR = 20.0f;	// �J�[�\���T�C�Y
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\checkPoint00.png";	// �J�[�\���̃e�N�X�`��
const D3DXVECTOR3 BOSSBATTLE_POS_PLAYER = { 0.0f,0.0f,-2000.0f };	// �{�X��ڍs���̃v���C���[�ʒu
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CCheckPointManager *CCheckPointManager::m_pCheckPointManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CCheckPointManager::CCheckPointManager()
{
	m_nProgress = 0;
	m_nNumCheckPoint = 0;
	m_pPosCheckPoint = nullptr;
	m_pBehavior = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CCheckPointManager::~CCheckPointManager()
{

}

//=====================================================
// ��������
//=====================================================
CCheckPointManager *CCheckPointManager::Create(void)
{
	if (m_pCheckPointManager == nullptr)
	{
		m_pCheckPointManager = new CCheckPointManager;

		if (m_pCheckPointManager != nullptr)
		{
			m_pCheckPointManager->Init();
		}
	}

	return m_pCheckPointManager;
}

//=====================================================
// �C���X�^���X�̎擾
//=====================================================
CCheckPointManager *CCheckPointManager::GetInstance(void)
{
	if (m_pCheckPointManager == nullptr)
	{
		//Create();
	}

	return m_pCheckPointManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CCheckPointManager::Init(void)
{
	// �Ǎ�
	Load();

	// �Z�[�u�f�[�^�̎擾
	CSaveDataManager *pSave = CSaveDataManager::GetInstance();

	if (pSave != nullptr)
	{
		m_nProgress = pSave->GetProgress() - 1;

		CPlayer *pPlayer = CPlayer::GetInstance();

		if (pPlayer != nullptr)
		{
			D3DXVECTOR3 pos = m_pPosCheckPoint[m_nProgress + 1];

			pPlayer->SetPosition(pos);
			
			if (m_nProgress == -1)
			{
				pPlayer->SetMotion(CPlayer::MOTION_APPER);

				Camera::ChangeBehavior(new CApperPlayer);

				CUIManager *pUIManager = CUIManager::GetInstance();

				if (pUIManager != nullptr)
				{
					pUIManager->EnableDisp(false);
				}
			}
		}
	}

	ChangeBehavior(new CCheckPointMove);
	
	return S_OK;
}

//=====================================================
// �Ǎ�
//=====================================================
void CCheckPointManager::Load(void)
{
	// �`�F�b�N�|�C���g���Ǎ�
	LoadInfoCheckPoint();
}

//=====================================================
// �`�F�b�N�|�C���g���Ǎ�
//=====================================================
void CCheckPointManager::LoadInfoCheckPoint(void)
{
	// �ϐ��錾
	char cTemp[256];
	int nCntPos = 0;

	// �t�@�C������ǂݍ���
	FILE *pFile = fopen(FILE_PATH, "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "NUM_CHECKPOINT") == 0)
			{// �`�F�b�N�|�C���g���Ǎ�
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_nNumCheckPoint);

				if (m_pPosCheckPoint == nullptr)
				{// ������̐���
					m_pPosCheckPoint = new D3DXVECTOR3[m_nNumCheckPoint];

					for (int i = 0; i < m_nNumCheckPoint; i++)
					{// ���̃N���A
						m_pPosCheckPoint[i] = { 0.0f,0.0f,0.0f };
					}
				}
				else
				{
					break;
				}

				if (m_pPosCheckPoint == nullptr)
				{// �����ł��Ȃ������ꍇ
					break;
				}

				while (true)
				{
					// �����ǂݍ���
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "CHECKPOINTSET") == 0)
					{// �p�����[�^�[�Ǎ��J�n
						while (true)
						{
							// �����ǂݍ���
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "POS") == 0)
							{//�ʒu�ǂݍ���
								fscanf(pFile, "%s", &cTemp[0]);

								for (int i = 0; i < 3; i++)
								{
									fscanf(pFile, "%f", &m_pPosCheckPoint[nCntPos][i]);
								}

								nCntPos++;
								break;
							}

							if (strcmp(cTemp, "END_CHECKPOINTSET") == 0)
							{// �p�����[�^�[�Ǎ��I��
								nCntPos++;

								break;
							}
						}
					}

					if (m_nNumCheckPoint <= nCntPos)
					{
						break;
					}
				}
			}
			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// �I������
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
}

//=====================================================
// �I������
//=====================================================
void CCheckPointManager::Uninit(void)
{
	m_pCheckPointManager = nullptr;

	if (m_pPosCheckPoint != nullptr)
	{
		delete[] m_pPosCheckPoint;
		m_pPosCheckPoint = nullptr;
	}

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Uninit(this);
		delete m_pBehavior;
		m_pBehavior = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CCheckPointManager::Update(void)
{
	if (m_nProgress >= m_nNumCheckPoint - 1)
	{
		CFade *pFade = CFade::GetInstance();

		if (pFade != nullptr)
		{
			CFade::FADE state = pFade->GetState();

			if (state == CFade::FADE::FADE_OUT)
			{// �{�X�G�ֈڍs
				TransBossBattle();

				if (m_pBehavior != nullptr)
				{
					m_pBehavior->Uninit(this);
					delete m_pBehavior;
					m_pBehavior = nullptr;
				}
			}
		}

		return;
	}

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Update(this);
	}
}

//=====================================================
// �{�X��ֈڍs
//=====================================================
void CCheckPointManager::TransBossBattle(void)
{
	// �{�X�̐���
	CEnemyBoss *pEnemyBoss = CEnemyBoss::GetInstance();

	if (pEnemyBoss == nullptr)
	{
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			pEnemyManager->DeleteAll();
		}

		pEnemyBoss = CEnemyBoss::Create();

		if (pEnemyBoss != nullptr)
		{
			pEnemyBoss->SetPosition(D3DXVECTOR3(22000.0f,0.0f,0.0f));
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CCheckPointManager::Draw(void)
{
#ifdef _DEBUG

	CDebugProc::GetInstance()->Print("\n�i�s��[%d]", m_nProgress);

#endif
}

//=====================================================
// �`�F�b�N�|�C���g�ʒu�擾
//=====================================================
D3DXVECTOR3 CCheckPointManager::GetCheckPosition(int nProgress)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (m_pPosCheckPoint == nullptr)
		return pos;

	if (nProgress == -1)
	{
		pos = m_pPosCheckPoint[m_nProgress];
	}
	else
	{
		pos = m_pPosCheckPoint[nProgress];
	}

	return pos;
}

//=====================================================
// �i�s�x���Z
//=====================================================
void CCheckPointManager::AddProgress(void)
{
	m_nProgress++;

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		if (m_nProgress > 0)
		{
			pEnemyManager->SpawnGroup(m_nProgress);
		}
	}

	if (m_nProgress >= m_nNumCheckPoint - 1)
	{// �Ō�̃`�F�b�N�|�C���g�ɓ���
		CFade *pFade = CFade::GetInstance();

		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_RESULT, false);
		}
	}
}

//=====================================================
// �r�w�C�r�A�ύX
//=====================================================
void CCheckPointManager::ChangeBehavior(CCheckPointBehavior *pBehavior)
{
	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Uninit(this);
		delete m_pBehavior;
	}

	m_pBehavior = pBehavior;

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Init(this);
	}
}

namespace CheckPoint
{
void SetProgress(int nProgress)
{
	CCheckPointManager *pCheck = CCheckPointManager::GetInstance();

	if (pCheck != nullptr)
	{
		pCheck->SetProgress(nProgress);
	}
}
}