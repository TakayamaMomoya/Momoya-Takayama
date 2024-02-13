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
#include "object2D.h"
#include "texture.h"
#include "player.h"
#include "fade.h"
#include "manager.h"
#include "enemyManager.h"
#include "enemyBoss.h"
#include "debugproc.h"
#include <stdio.h>

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* FILE_PATH = "data\\TEXT\\checkPoint.txt";	// �t�@�C���̃p�X
const float SIZE_CURSOR = 20.0f;	// �J�[�\���T�C�Y
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\checkPoint00.png";	// �J�[�\���̃e�N�X�`��
const float DIST_PROGRESS = 1000.0f;	// �i�s���鋗��
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
	m_pCursor = nullptr;
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
		Create();
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

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->SpawnGroup(m_nProgress);
	}


	if (m_pCursor == nullptr)
	{// �J�[�\������
		m_pCursor = CObject2D::Create(7);

		if (m_pCursor != nullptr)
		{
			m_pCursor->SetSize(SIZE_CURSOR, SIZE_CURSOR);
			m_pCursor->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
			m_pCursor->SetIdxTexture(nIdx);
			m_pCursor->SetVtx();
		}
	}

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

	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
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
			}
		}

		return;
	}

	D3DXVECTOR3 posNext = m_pPosCheckPoint[m_nProgress + 1];

	// �J�[�\���̕\��
	if (m_pCursor != nullptr)
	{
		D3DXMATRIX mtx;
		D3DXVECTOR3 posScreen;

		bool bInScreen = universal::IsInScreen(posNext, mtx, &posScreen);
		
		if (bInScreen == false)
		{
			// ��ʓ��ɓ���Ȃ��悤�ɐݒ�
			if (posScreen.x > -SIZE_CURSOR && posScreen.x < SCREEN_WIDTH + SIZE_CURSOR)
			{
				posScreen.x = -SIZE_CURSOR;
			}

			if (posScreen.y > -SIZE_CURSOR && posScreen.y < SCREEN_HEIGHT + SIZE_CURSOR)
			{
				posScreen.y = -SIZE_CURSOR;
			}
		}

		m_pCursor->SetPosition(posScreen);
		m_pCursor->SetVtx();
	}

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// �����̌v�Z
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
		D3DXVECTOR3 vecDiff = posNext - posPlayer;

		float fDist = D3DXVec3Length(&vecDiff);

		if (fDist < DIST_PROGRESS)
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
					pFade->SetFade(CScene::MODE_RANKING,false);

					if (m_pCursor != nullptr)
					{
						m_pCursor->Uninit();
						m_pCursor = nullptr;
					}
				}
			}
		}
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
			pEnemyBoss->SetPosition(D3DXVECTOR3(22000.0f,1000.0f,0.0f));
		}
	}

	// �v���C���[�̈ʒu�ݒ�
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		//pPlayer->SetPosition(BOSSBATTLE_POS_PLAYER);
		//pPlayer->SetPositionOld(BOSSBATTLE_POS_PLAYER);
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