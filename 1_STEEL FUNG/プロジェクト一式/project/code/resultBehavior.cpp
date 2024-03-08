//*****************************************************
//
// ���U���g��Behavior[resultBehavior.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultBehavior.h"
#include "number.h"
#include "result.h"
#include "saveDataManager.h"
#include "inputManager.h"
#include "fade.h"
#include "texture.h"
#include "string2D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_PLACE = 6;
const D3DXVECTOR3 POS_NUMBER[CResultPlayerScore::TYPE_MAX] =
{// �����\���X�R�A�̐��l�̈ʒu
	{ SCRN_MID.x, SCRN_MID.y - 250.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y - 130.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 30.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 210.0f, 0.0f },
};
const D3DXVECTOR2 SIZE_NUMBER = { 25.0f,50.0f };	// �����̃T�C�Y
const int COST_REPAIR = 50;	// ������1%�ɂ�����C����p
const char* PATH_TEXTURE_CAPTION[CResultPlayerScore::TYPE_MAX] =
{// ���ڂ̌��o���̃e�N�X�`���p�X
	"data\\TEXTURE\\UI\\resultCaption00.png",
	"data\\TEXTURE\\UI\\resultCaption01.png",
	"data\\TEXTURE\\UI\\resultCaption02.png",
	"data\\TEXTURE\\UI\\resultCaption03.png",
};
const D3DXVECTOR2 SIZE_CAPTION = { 70.0f,50.0f };	// ���ڌ��o���̃T�C�Y
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CResultBehavior::CResultBehavior()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CResultBehavior::~CResultBehavior()
{

}

//=====================================================
// �v���C���[�X�R�A�\��
//=====================================================
CResultPlayerScore::CResultPlayerScore()
{// �R���X�g���N�^
	ZeroMemory(&m_apNumber[0], sizeof(m_apNumber));
	ZeroMemory(&m_apCaption[0], sizeof(m_apCaption));
	m_pFrame = nullptr;
}

CResultPlayerScore::~CResultPlayerScore()
{// �f�X�g���N�^

}

void CResultPlayerScore::Init(CResult *pResult)
{// ������
	int nBaseReward = Result::BASE_REWARD;
	int nRepair = 0;
	int nAddReward = 0;

	// �Z�[�u�f�[�^�̎擾
	CSaveDataManager *pSave = CSaveDataManager::GetInstance();

	if (pSave == nullptr)
		assert(("�Z�[�u�f�[�^�̎擾���ł��܂���", false));

	CSaveDataManager::SInfo info = *pSave->GetInfo();

	nAddReward = info.nAddReward;

	// �C����p�v�Z
	float fRate = info.fLife / info.fInitialLife;

	fRate = (1.0f - fRate) * 100.0f;

	nRepair = COST_REPAIR * (int)fRate;

	// ���z
	int nResult = nBaseReward + nAddReward - nRepair;

	int aValue[TYPE_NUMBER::TYPE_MAX] =
	{// �l�̕ۑ��p
		nBaseReward,nAddReward,nRepair,nResult
	};

	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{// �����̐���
		m_apNumber[i] = CNumber::Create(NUM_PLACE, 0);

		if (m_apNumber[i] != nullptr)
		{
			m_apNumber[i]->SetSizeAll(SIZE_NUMBER.x, SIZE_NUMBER.y);
			m_apNumber[i]->SetPosition(POS_NUMBER[i]);
			m_apNumber[i]->SetValue(aValue[i],NUM_PLACE);
		}
	}

	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{// ���o���̐���
		m_apCaption[i] = CObject2D::Create(7);

		if (m_apCaption[i] != nullptr)
		{
			D3DXVECTOR3 pos = POS_NUMBER[i];

			pos.x -= SIZE_CAPTION.x * 2 - SIZE_NUMBER.x;

			m_apCaption[i]->SetSize(SIZE_CAPTION.x, SIZE_CAPTION.y);
			m_apCaption[i]->SetPosition(pos);
			m_apCaption[i]->SetVtx();

			int nIdx = Texture::GetIdx(PATH_TEXTURE_CAPTION[i]);
			m_apCaption[i]->SetIdxTexture(nIdx);
		}
	}

	// �t���[���̐���
	m_pFrame = CObject2D::Create(6);

	if (m_pFrame != nullptr)
	{
		D3DXVECTOR3 pos = SCRN_MID;
		pos.x += 50.0f;

		m_pFrame->SetSize(270.0f, 350.0f);
		m_pFrame->SetPosition(pos);
		m_pFrame->SetVtx();

		int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\resultCaptionFrame.png");
		m_pFrame->SetIdxTexture(nIdx);
	}
}

void CResultPlayerScore::Uninit(CResult *pResult)
{// �I��
	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{
		if (m_apNumber[i] != nullptr)
		{
			m_apNumber[i]->Uninit();
			m_apNumber[i] = nullptr;
		}

		if (m_apCaption[i] != nullptr)
		{
			m_apCaption[i]->Uninit();
			m_apCaption[i] = nullptr;
		}
	}

	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}
}

void CResultPlayerScore::Update(CResult *pResult)
{// �X�V
	// �t�F�[�h���鑀��
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}

		return;
	}
}

//=====================================================
// �����L���O�\��
//=====================================================
CResultRanking::CResultRanking()
{// �R���X�g���N�^

}

CResultRanking::~CResultRanking()
{// �f�X�g���N�^

}

void CResultRanking::Init(CResult *pResult)
{// ������
	CString2D::Create();

	std::list<CResult::SInfoRanking> listRanking = pResult->GetListRanking();

	for (auto item : listRanking)
	{

	}
}

void CResultRanking::Update(CResult *pResult)
{// �X�V
	// �t�F�[�h���鑀��
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}

		if (pInputManager->GetTrigger(CInputManager::BUTTON_BACK))
		{// ���X�R�A�\���ɖ߂�
			pResult->ChangeBehavior(new CResultPlayerScore);

			return;
		}
	}
}