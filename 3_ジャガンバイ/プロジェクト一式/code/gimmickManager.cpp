//*****************************************************
//
// �M�~�b�N�}�l�[�W���[[gimmickManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "GimmickManager.h"
#include "Gimmick.h"
#include "door.h"
#include "container.h"
#include "itemRepair.h"
#include "box.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* MAP_PATH = "data\\MAP\\gimmick1.txt";	// �z�u���̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CGimmickManager *CGimmickManager::m_pGimmickManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CGimmickManager::CGimmickManager()
{
	m_pHead = nullptr;
	m_pTail = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CGimmickManager::~CGimmickManager()
{

}

//=====================================================
// ��������
//=====================================================
CGimmickManager *CGimmickManager::Create(void)
{
	if (m_pGimmickManager == nullptr)
	{// �C���X�^���X����
		m_pGimmickManager = new CGimmickManager;

		// ����������
		m_pGimmickManager->Init();
	}

	return m_pGimmickManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGimmickManager::Init(void)
{
	// �z�u�f�[�^�̓Ǎ�
	Load();

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CGimmickManager::Load(void)
{
	// �ϐ��錾
	char cTemp[256];

	// �t�@�C������ǂݍ���
	FILE* pFile = fopen(MAP_PATH, "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// �M�~�b�N�̐���
			CreateGimmick(pFile, &cTemp[0]);

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
		assert(("�M�~�b�N�z�u���ǂݍ��݂Ɏ��s", false));
	}
}

//=====================================================
// �f�[�^���Ƃɐ������鏈��
//=====================================================
void CGimmickManager::CreateGimmick(FILE *pFile, char *pTemp)
{
	if (strcmp(pTemp, "DOORSET") == 0)
	{// �h�A�̐ݒu
		LoadDoor(pFile, pTemp);
	}

	if (strcmp(pTemp, "CONTAINERSET") == 0)
	{// �R���e�i�̐ݒu
		LoadContainer(pFile, pTemp);
	}

	if (strcmp(pTemp, "REPAIRSET") == 0)
	{// ���i�̐ݒu
		LoadRepair(pFile, pTemp);
	}

	if (strcmp(pTemp, "BOXSET") == 0)
	{// �ؔ��̐ݒu
		LoadBox(pFile, pTemp);
	}
}

//=====================================================
// �h�A�Ǎ�����
//=====================================================
void CGimmickManager::LoadDoor(FILE *pFile, char *pTemp)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	float rotDest = 0.0f;

	while (true)
	{
		(void)fscanf(pFile, "%s", pTemp);

		if (strcmp(pTemp, "POS") == 0)
		{// �ʒu
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				(void)fscanf(pFile, "%f", &pos[i]);
			}
		}

		if (strcmp(pTemp, "ROT") == 0)
		{// ����
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				float fAngle;

				(void)fscanf(pFile, "%f", &fAngle);

				rot[i] = D3DXToRadian(fAngle);
			}
		}

		if (strcmp(pTemp, "ROTDEST") == 0)
		{// �ڕW�̌���
			(void)fscanf(pFile, "%s", pTemp);

			float fAngle;

			(void)fscanf(pFile, "%f", &fAngle);

			rotDest = D3DXToRadian(fAngle);
		}

		if (strcmp(pTemp, "END_DOORSET") == 0)
		{// �h�A�ݒ�I��
			CDoor *pDoor = CDoor::Create();

			if (pDoor != nullptr)
			{
				pDoor->SetPosition(pos);
				pDoor->SetRot(rot);
				pDoor->SetOrgRot(rot.y);
				pDoor->SetDestRot(rotDest);
			}

			break;
		}
	}
}

//=====================================================
// �R���e�i�Ǎ�����
//=====================================================
void CGimmickManager::LoadContainer(FILE *pFile, char *pTemp)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	while (true)
	{
		(void)fscanf(pFile, "%s", pTemp);

		if (strcmp(pTemp, "POS") == 0)
		{// �ʒu
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				(void)fscanf(pFile, "%f", &pos[i]);
			}
		}

		if (strcmp(pTemp, "ROT") == 0)
		{// ����
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				float fAngle;

				(void)fscanf(pFile, "%f", &fAngle);

				rot[i] = D3DXToRadian(fAngle);
			}
		}

		if (strcmp(pTemp, "END_CONTAINERSET") == 0)
		{// �R���e�i�ݒ�I��
			CContainer *pContainer = CContainer::Create();

			if (pContainer != nullptr)
			{
				pContainer->SetPosition(pos);
				pContainer->SetRot(rot);
			}

			break;
		}
	}
}

//=====================================================
// ���i�Ǎ�����
//=====================================================
void CGimmickManager::LoadRepair(FILE *pFile, char *pTemp)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	while (true)
	{
		(void)fscanf(pFile, "%s", pTemp);

		if (strcmp(pTemp, "POS") == 0)
		{// �ʒu
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				(void)fscanf(pFile, "%f", &pos[i]);
			}
		}

		if (strcmp(pTemp, "ROT") == 0)
		{// ����
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				float fAngle;

				(void)fscanf(pFile, "%f", &fAngle);

				rot[i] = D3DXToRadian(fAngle);
			}
		}

		if (strcmp(pTemp, "END_REPAIRSET") == 0)
		{// ���i�ݒ�I��
			CItemRepair *pRapair = CItemRepair::Create();

			if (pRapair != nullptr)
			{
				pRapair->SetPosition(pos);
				pRapair->SetRot(rot);
			}

			break;
		}
	}
}

//=====================================================
// �ؔ��Ǎ�����
//=====================================================
void CGimmickManager::LoadBox(FILE *pFile, char *pTemp)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	int nType = CBox::TYPE::TYPE_RANDOM;

	while (true)
	{
		(void)fscanf(pFile, "%s", pTemp);

		if (strcmp(pTemp, "POS") == 0)
		{// �ʒu
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				(void)fscanf(pFile, "%f", &pos[i]);
			}
		}

		if (strcmp(pTemp, "ROT") == 0)
		{// ����
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				float fAngle;

				(void)fscanf(pFile, "%f", &fAngle);

				rot[i] = D3DXToRadian(fAngle);
			}
		}

		if (strcmp(pTemp, "TYPE") == 0)
		{// ���
			(void)fscanf(pFile, "%s", pTemp);

			(void)fscanf(pFile, "%d", &nType);
		}

		if (strcmp(pTemp, "END_BOXSET") == 0)
		{// �ݒ�I��
			CBox *pBox = CBox::Create((CBox::TYPE)nType);

			if (pBox != nullptr)
			{
				pBox->SetPosition(pos);
				pBox->SetRot(rot);
			}

			break;
		}
	}
}

//=====================================================
// �I������
//=====================================================
void CGimmickManager::Uninit(void)
{
	m_pGimmickManager = nullptr;

	Release();
}

//=====================================================
// �S�M�~�b�N�폜����
//=====================================================
void CGimmickManager::DeleteAllGimmick(void)
{
	CGimmick *pGimmick = GetHead();

	while (pGimmick != nullptr)
	{
		CGimmick *pGimmickNext = pGimmick->GetNext();

		if (pGimmick != nullptr)
		{
			pGimmick->Uninit();
		}

		pGimmick = pGimmickNext;
	}
}

//=====================================================
// �X�V����
//=====================================================
void CGimmickManager::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CGimmickManager::Draw(void)
{

}