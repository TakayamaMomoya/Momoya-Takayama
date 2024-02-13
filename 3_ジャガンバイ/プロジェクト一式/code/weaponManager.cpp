//*****************************************************
//
// ����}�l�[�W���[[weaponManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weaponManager.h"
#include "Weapon.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* PARAM_PATH = "data\\TEXT\\weapon.txt";	// �p�����[�^�[�̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CWeaponManager *CWeaponManager::m_pWeaponManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CWeaponManager::CWeaponManager()
{
	ZeroMemory(&m_aInfo[0], sizeof(m_aInfo));
	ZeroMemory(&m_infoShotgun, sizeof(SInfoShotgun));
	ZeroMemory(&m_infoRailgun, sizeof(SInfoRailgun));
	ZeroMemory(&m_anProb[0], sizeof(m_anProb));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CWeaponManager::~CWeaponManager()
{

}

//=====================================================
// ��������
//=====================================================
CWeaponManager *CWeaponManager::Create(void)
{
	if (m_pWeaponManager == nullptr)
	{// �C���X�^���X����
		m_pWeaponManager = new CWeaponManager;

		// ����������
		m_pWeaponManager->Init();
	}

	return m_pWeaponManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CWeaponManager::Init(void)
{
	// �p�����[�^�[�ǂݍ���
	LoadParam();

	return S_OK;
}

//=====================================================
// �p�����[�^�[�ǂݍ��ݏ���
//=====================================================
void CWeaponManager::LoadParam(void)
{
	// �ϐ��錾
	char cTemp[256];
	int nCntParam = 0;

	// �t�@�C������ǂݍ���
	FILE* pFile = fopen(PARAM_PATH, "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "MAGNUMSET") == 0)
			{// �p�����[�^�[�Ǎ��J�n
				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// ���p�����[�^�[�ǂݍ���
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_MAGNUM);

					if (strcmp(cTemp, "END_MAGNUMSET") == 0)
					{// �p�����[�^�[�Ǎ��I��
						break;
					}
				}
			}

			if (strcmp(cTemp, "MACHINEGUNSET") == 0)
			{// �p�����[�^�[�Ǎ��J�n
				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// ���p�����[�^�[�ǂݍ���
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_MACHINEGUN);

					if (strcmp(cTemp, "END_MACHINEGUNSET") == 0)
					{// �p�����[�^�[�Ǎ��I��
						break;
					}
				}
			}

			if (strcmp(cTemp, "SHOTGUNSET") == 0)
			{// �p�����[�^�[�Ǎ��J�n
				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// ���p�����[�^�[�ǂݍ���
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_SHOTGUN);

					// �V���b�g�K������p�����[�^�[�Ǎ�
					LoadShotgunInfo(pFile, &cTemp[0]);

					if (strcmp(cTemp, "END_SHOTGUNSET") == 0)
					{// �p�����[�^�[�Ǎ��I��
						break;
					}
				}
			}

			if (strcmp(cTemp, "RAILGUNSET") == 0)
			{// �p�����[�^�[�Ǎ��J�n
				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// ���p�����[�^�[�ǂݍ���
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_RAILGUN);

					// �ŗL�p�����[�^�[�Ǎ�
					LoadRailgunInfo(pFile, &cTemp[0]);

					if (strcmp(cTemp, "END_RAILGUNSET") == 0)
					{// �p�����[�^�[�Ǎ��I��
						break;
					}
				}
			}

			if (strcmp(cTemp, "MINIGUNSET") == 0)
			{// �p�����[�^�[�Ǎ��J�n
				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// ���p�����[�^�[�ǂݍ���
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_MINIGUN);

					if (strcmp(cTemp, "END_MINIGUNSET") == 0)
					{// �p�����[�^�[�Ǎ��I��
						break;
					}
				}
			}

			if (strcmp(cTemp, "LAUNCHERSET") == 0)
			{// �p�����[�^�[�Ǎ��J�n
				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// ���p�����[�^�[�ǂݍ���
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_LAUNCHER);

					if (strcmp(cTemp, "END_LAUNCHERSET") == 0)
					{// �p�����[�^�[�Ǎ��I��
						break;
					}
				}
			}

			if (strcmp(cTemp, "PROBSET") == 0)
			{// �m���Ǎ��J�n
				int nCntProb = 0;

				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PROB") == 0)
					{
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_anProb[nCntProb]);

						nCntProb++;
					}

					if (strcmp(cTemp, "END_PROBSET") == 0 ||
						nCntProb >= CWeapon::TYPE_MAX)
					{// �p�����[�^�[�Ǎ��I��
						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// �ǂݍ��݂̏I��
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		assert(("������ǂݍ��݂Ɏ��s", false));
	}
}

//=====================================================
// ���p�����[�^�[�Ǎ�
//=====================================================
void CWeaponManager::LoadBaseInfo(FILE* pFile, char* pTemp, int nCntParam)
{
	if (strcmp(pTemp, "BULLET") == 0)
	{// �e��
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%d", &m_aInfo[nCntParam].nMaxBullet);
	}

	if (strcmp(pTemp, "RAPID") == 0)
	{// �A�˗�
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%d", &m_aInfo[nCntParam].nRapid);
	}

	if (strcmp(pTemp, "DAMAGE") == 0)
	{// �З�
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_aInfo[nCntParam].fDamage);
	}

	if (strcmp(pTemp, "BULLET_LIFE") == 0)
	{// �e�̎���
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_aInfo[nCntParam].fLifeBullet);
	}

	if (strcmp(pTemp, "VIB_POWER") == 0)
	{// �R���g���[���[�̐U���̋���
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_aInfo[nCntParam].fVibPower);
	}

	if (strcmp(pTemp, "VIB_TIME") == 0)
	{// �R���g���[���[�̐U������
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%d", &m_aInfo[nCntParam].nVibTime);
	}
}

//=====================================================
// �V���b�g�K���p�����[�^�[�Ǎ�
//=====================================================
void CWeaponManager::LoadShotgunInfo(FILE *pFile, char* pTemp)
{
	if (strcmp(pTemp, "NUM_PELLET") == 0)
	{// ���˂���e��
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%d", &m_infoShotgun.nNumPellet);
	}

	if (strcmp(pTemp, "ANGLE_DIFFUSE") == 0)
	{// �g�U�p�x
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_infoShotgun.fAngleDiffuse);
	}
}

//=====================================================
// ���[���K���p�����[�^�[�Ǎ�
//=====================================================
void CWeaponManager::LoadRailgunInfo(FILE *pFile, char* pTemp)
{
	if (strcmp(pTemp, "WIDTH") == 0)
	{// ��
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_infoRailgun.fWidth);
	}

	if (strcmp(pTemp, "LENGTH") == 0)
	{// ����
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_infoRailgun.fLength);
	}
}

//=====================================================
// �I������
//=====================================================
void CWeaponManager::Uninit(void)
{
	m_pWeaponManager = nullptr;

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CWeaponManager::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CWeaponManager::Draw(void)
{

}