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
#include "manager.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "enemyBomb.h"
#include "enemyDrone.h"
#include "enemyBoss.h"
#include "checkPointManager.h"
#include "inputkeyboard.h"
#include "effect3D.h"
#include "player.h"
#include "UI.h"
#include "texture.h"
#include "debugproc.h"
#include <stdio.h>

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* FILE_PATH = "data\\TEXT\\checkPoint.txt";	// �t�@�C���̃p�X
const float TIME_SPAWN = 5.0f;	// �G�̃X�|�[��
const float SIZE_CURSOR = 60.0f;	// �J�[�\���T�C�Y
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\lockon01.png";	// �J�[�\���̃e�N�X�`��
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
	m_pInfoGroup = nullptr;
	m_pEnemyLockon = nullptr;
	m_bLockTarget = false;
	m_pCursor = nullptr;
	m_fTimer = 0.0f;
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
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// �C���X�^���X����
		switch (type)
		{
		case CEnemy::TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			break;
		case CEnemy::TYPE_BOMB:

			pEnemy = new CEnemyBomb;

			break;
		case CEnemy::TYPE_DRONE:

			pEnemy = new CEnemyDrone;

			break;
		default:
			break;
		}

		if (pEnemy != nullptr)
		{
			// �ʒu�ݒ�
			pEnemy->SetPosition(pos);

			// ���[�V�����Ǎ�
			pEnemy->Load((char*)PATH_BODY_ENEMY[type]);

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
	// �`�F�b�N�|�C���g�̏��擾
	CCheckPointManager *pCheckPointManager = CCheckPointManager::GetInstance();

	if (pCheckPointManager != nullptr)
	{
		int nNumCheckPoint = pCheckPointManager->GetNumCheckPoint();

		if (m_pInfoGroup == nullptr)
		{
			m_pInfoGroup = new SInfoEnemyGroup[nNumCheckPoint];

			if (m_pInfoGroup != nullptr)
			{
				ZeroMemory(m_pInfoGroup, sizeof(SInfoEnemyGroup) * nNumCheckPoint);
			}
		}
	}

	// �Ǎ�����
	Load();

	// �ŏ��̓G���X�|�[��
	SpawnGroup(0);

	if (m_pCursor == nullptr)
	{// �J�[�\������
		m_pCursor = CUI::Create();

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
// �Ǎ�����
//=====================================================
void CEnemyManager::Load(void)
{
	if (m_pInfoGroup == nullptr)
	{
		return;
	}

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
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "CHECKPOINTSET") == 0)
			{// �p�����[�^�[�Ǎ��J�n
				int nNumEnemy = 0;

				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "NUM_ENEMY") == 0)
					{// �G�̐��擾
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_pInfoGroup[nCntPos].nNumEnemy);

						if (m_pInfoGroup[nCntPos].nNumEnemy > 0)
						{
							m_pInfoGroup[nCntPos].pInfoEnemy = new SInfoEnemy[m_pInfoGroup[nCntPos].nNumEnemy];
						}

					}

					if (strcmp(cTemp, "ENEMYSET") == 0)
					{// �G���
						SInfoEnemy *Info = &m_pInfoGroup[nCntPos].pInfoEnemy[nNumEnemy];

						while (true)
						{// �G�̏��Ǎ�
							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "POS") == 0)
							{// �ʒu
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%f", &Info->pos.x);
								(void)fscanf(pFile, "%f", &Info->pos.y);
								(void)fscanf(pFile, "%f", &Info->pos.z);
							}

							if (strcmp(cTemp, "TYPE") == 0)
							{// ���
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &Info->nType);
							}

							if (strcmp(cTemp, "END_ENEMYSET") == 0)
							{// �p�����[�^�[�Ǎ��I��
								nNumEnemy++;

								break;
							}
						}
					}

					if (strcmp(cTemp, "END_CHECKPOINTSET") == 0)
					{// �p�����[�^�[�Ǎ��I��
						nCntPos++;

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
void CEnemyManager::Uninit(void)
{
	m_pEnemyManager = nullptr;

	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

	if (m_pInfoGroup != nullptr)
	{
		delete[] m_pInfoGroup;
		m_pInfoGroup = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyManager::Update(void)
{
#ifdef _DEBUG

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_C))
		{
			CreateEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(0.0f, 500.0f, 500.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(-1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(-1200.0f, 400.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(1500.0f, 1000.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
		}
	}

	/*if(m_pEnemyLockon != nullptr)
		CEffect3D::Create(m_pEnemyLockon->GetPosition(), 30.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));*/
#endif
}

//=====================================================
// ���b�N�I������
//=====================================================
CEnemy *CEnemyManager::Lockon(CEnemy *pEnemyExclusive)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return nullptr;
	}

	bool bLock = IsLockTarget();

	bool bInAny = false;
	CEnemy *pEnemy = GetHead();
	float fDistMax = FLT_MAX;
	D3DXVECTOR3 posCenter = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
	D3DXVECTOR3 posScreen = posCenter;

	while (pEnemy != nullptr)
	{
		CEnemy::STATE state = pEnemy->GetState();

		CEnemy *pEnemyNext = pEnemy->GetNext();

		pEnemy->EnableLock(false);

		if (state != CEnemy::STATE::STATE_DEATH && pEnemyExclusive != pEnemy)
		{
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(0);
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXMATRIX mtx = *pEnemy->GetMatrix();

			D3DXVECTOR3 vecDiffPlayer = pos - posPlayer;
			float fDistPlayer = D3DXVec3Length(&vecDiffPlayer);
			D3DXVECTOR3 posScreenTemp;

			// ���b�N�I������G�̌���
			if (universal::IsInScreen(pos, mtx, &posScreenTemp))
			{
				float fDist = pEnemy->GetDistLock();

				if (fDist > fDistPlayer)
				{// �������ɓG��������
					bInAny = true;

					pEnemy->EnableLock(true);
					pEnemy->SetPositionCursor(posScreenTemp);

					if (bLock == false || (m_pEnemyLockon == nullptr && bLock == true))
					{// �����Ń��b�N�I���Ώۂ��؂�ւ�邩�ǂ���
						D3DXVECTOR3 vecDiff = posScreenTemp - posCenter;

						// ��ʒ��S����̋������v�Z
						float fDist = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

						if (fDist < fDistMax)
						{
							posScreen = posScreenTemp;

							m_pEnemyLockon = pEnemy;

							fDistMax = fDist;
						}
					}
				}
				else if (m_pEnemyLockon == pEnemy)
				{
					if (SwitchTarget(1, 1) == nullptr &&
						SwitchTarget(-1, -1) == nullptr)
					{
						m_pEnemyLockon = nullptr;
					}
				}
			}
		}

		pEnemy = pEnemyNext;
	}

	D3DXVECTOR3 posDestCursor = posCenter;

	if (bInAny == false)
	{
		if (bLock == false)
		{
			m_pEnemyLockon = nullptr;
		}
	}
	else
	{
		posDestCursor = posScreen;
	}

	if (m_pEnemyLockon != nullptr)
	{
		D3DXVECTOR3 pos = m_pEnemyLockon->GetMtxPos(0);
		D3DXMATRIX mtx = *m_pEnemyLockon->GetMatrix();

		universal::IsInScreen(pos, mtx, &posDestCursor);
	}

	if (m_pCursor != nullptr)
	{// �J�[�\���̈ʒu�␳
		D3DXVECTOR3 posCursor = m_pCursor->GetPosition();
		D3DXVECTOR3 vecDiffCursor = posDestCursor - posCursor;

		posCursor += vecDiffCursor * 0.1f;

		m_pCursor->SetPosition(posCursor);
		m_pCursor->SetVtx();
	}

	return m_pEnemyLockon;
}

//=====================================================
// �^�[�Q�b�g�̐؂�ւ�
//=====================================================
CEnemy *CEnemyManager::SwitchTarget(int nAxisX, int nAxisY, CEnemy *pEnemyExclusive)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (m_pEnemyLockon == nullptr)
	{
		return nullptr;
	}

	CEnemy *pEnemy = GetHead();
	CEnemy *pEnemyLock = nullptr;
	float fLengthDiff = FLT_MAX;

	// ���݃��b�N���Ă���G�̃X�N���[�����W
	D3DXMATRIX mtx = *m_pEnemyLockon->GetMatrix();
	D3DXVECTOR3 posScreenLockEnemy;
	D3DXVECTOR3 posLockEnemy = m_pEnemyLockon->GetMtxPos(0);
	universal::IsInScreen(posLockEnemy, mtx, &posScreenLockEnemy);

	while (pEnemy != nullptr)
	{
		CEnemy::STATE state = pEnemy->GetState();

		CEnemy *pEnemyNext = pEnemy->GetNext();

		if (state != CEnemy::STATE::STATE_DEATH && m_pEnemyLockon != pEnemy && pEnemyExclusive != pEnemy)
		{
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(0);

			D3DXVECTOR3 vecDiff = pos - pPlayer->GetPosition();
			float fDistLockon = D3DXVec3Length(&vecDiff);

			D3DXVECTOR3 posScreenTemp;

			// ���b�N�I������G�̌���
			if (universal::IsInScreen(pos, mtx, &posScreenTemp))
			{
				float fDist = pEnemy->GetDistLock();

				// ��������
				if (fDist > fDistLockon)
				{
					bool bOk = false;

					// �����b�N���Ă�G�Ƃ̈ʒu���ׂ�
					if (nAxisX > 0)
					{
						if (posScreenTemp.x > posScreenLockEnemy.x)
						{// ���̂��E�ɂ�����
							bOk = true;
						}
					}
					if (nAxisX < 0)
					{
						if (posScreenTemp.x < posScreenLockEnemy.x)
						{// ���̂�荶�ɂ�����
							bOk = true;
						}
					}
					if (nAxisY < 0)
					{
						if (posScreenTemp.y > posScreenLockEnemy.y)
						{// ���̂�艺�ɂ�����
							bOk = true;
						}
					}
					if (nAxisY > 0)
					{
						if (posScreenTemp.y < posScreenLockEnemy.y)
						{// ���̂���ɂ�����
							bOk = true;
						}
					}

					if (bOk)
					{
						// �����������擾
						D3DXVECTOR3 vecDIffScreen = posScreenTemp - posScreenLockEnemy;

						float fLengthScreen = D3DXVec3Length(&vecDIffScreen);

						if (fLengthScreen < fLengthDiff)
						{// �ő勗�����Z�������玟�̃��b�N�I���ɂ���
							pEnemyLock = pEnemy;
							fLengthDiff = fLengthScreen;
						}
					}
				}
			}
		}

		pEnemy = pEnemyNext;
	}

	if (pEnemyLock != nullptr)
	{
		m_pEnemyLockon = pEnemyLock;
	}

	return pEnemyLock;
}

//=====================================================
// �^�[�Q�b�g�̃��b�N
//=====================================================
void CEnemyManager::EnableLockTarget(bool bLock)
{
	m_bLockTarget = bLock;
}

//=====================================================
// �W�c�̃X�|�[��
//=====================================================
void CEnemyManager::SpawnGroup(int nIdx)
{
	if (m_pInfoGroup != nullptr)
	{
		if (m_pInfoGroup[nIdx].pInfoEnemy == nullptr)
			return;

		for (int i = 0; i < m_pInfoGroup[nIdx].nNumEnemy; i++)
		{
			D3DXVECTOR3 pos = m_pInfoGroup[nIdx].pInfoEnemy[i].pos;
			int nType = m_pInfoGroup[nIdx].pInfoEnemy[i].nType;

			CreateEnemy(pos, (CEnemy::TYPE)nType);
		}
	}
}

//=====================================================
// ���S�����G�����b�N�I�����Ă���̂Ɠ������ǂ���
//=====================================================
void CEnemyManager::CheckDeathLockon(CEnemy *pEnemy)
{
	if (pEnemy == m_pEnemyLockon)
	{
		m_pEnemyLockon = nullptr;

		if (SwitchTarget(1, 1) == nullptr &&
			SwitchTarget(-1, -1) == nullptr)
		{
			m_pEnemyLockon = nullptr;
		}
	}
}

//=====================================================
// �S�G���폜����
//=====================================================
void CEnemyManager::DeleteAll(void)
{
	CEnemy *pEnemy = GetHead();

	while (pEnemy != nullptr)
	{
		CEnemy *pEnemyNext = pEnemy->GetNext();

		pEnemy->Uninit();

		pEnemy = pEnemyNext;
	}

	m_pEnemyLockon = nullptr;
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyManager::Draw(void)
{
#ifdef _DEBUG

	if (m_pEnemyLockon != nullptr)
	{
		CDebugProc::GetInstance()->Print("\n���b�N�I�����Ă�G�����");
	}
#endif
}