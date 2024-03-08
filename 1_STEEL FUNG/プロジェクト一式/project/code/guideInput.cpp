//*****************************************************
//
// ����K�C�h�̏���[guideInput.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "guideInput.h"
#include "texture.h"
#include "inputManager.h"
#include "UI.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR2 SIZE_GUIDE = { 90.0f,25.0f };	// �K�C�h�\���̃T�C�Y
const D3DXVECTOR3 POS_GUIDE = { SCREEN_WIDTH - SIZE_GUIDE.x,30.0f,0.0f };	// �K�C�h�\���̎n�߂̈ʒu
const char* PATH_GUIDE[CGuideInput::INPUT_MAX] =
{// �K�C�h�\���̃e�N�X�`���p�X
	"data\\TEXTURE\\UI\\tutorial00.png",
	"data\\TEXTURE\\UI\\tutorial01.png",
	"data\\TEXTURE\\UI\\tutorial02.png",
	"data\\TEXTURE\\UI\\tutorial03.png",
	"data\\TEXTURE\\UI\\tutorial04.png",
	"data\\TEXTURE\\UI\\tutorial05.png",
};
const char* PATH_FRAME = "data\\TEXTURE\\UI\\guideFrame00.png";	// �t���[���̃p�X
const D3DXCOLOR COL_INITIAL = { 0.2f,0.2f,0.2f,1.0f };	// �����F
const D3DXCOLOR COL_CURRENT = { 1.0f,1.0f,1.0f,1.0f };	// �I��F
const float TIME_GROW = 0.5f;	// ���鎞��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CGuideInput::CGuideInput(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_aGuide[0], sizeof(m_aGuide));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CGuideInput::~CGuideInput()
{

}

//=====================================================
// ��������
//=====================================================
CGuideInput *CGuideInput::Create(void)
{
	CGuideInput *pGuideInput = nullptr;

	if (pGuideInput == nullptr)
	{
		pGuideInput = new CGuideInput;

		if (pGuideInput != nullptr)
		{
			pGuideInput->Init();
		}
	}

	return pGuideInput;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGuideInput::Init(void)
{
	int nNum = 0;

	for (int i = 0; i < INPUT_MAX;i++)
	{
		m_aGuide[i].pFrame = CUI::Create();
		CUI *pCaption = CUI::Create();

		if (m_aGuide[i].pFrame != nullptr && pCaption != nullptr)
		{
			D3DXVECTOR3 pos = POS_GUIDE;

			pos.y += SIZE_GUIDE.y * nNum * 2;

			// �t���[��
			m_aGuide[i].pFrame->SetSize(SIZE_GUIDE.x, SIZE_GUIDE.y);
			m_aGuide[i].pFrame->SetPosition(pos);
			m_aGuide[i].pFrame->SetCol(COL_CURRENT);
			m_aGuide[i].pFrame->SetVtx();

			int nIdx = Texture::GetIdx(PATH_FRAME);
			m_aGuide[i].pFrame->SetIdxTexture(nIdx);

			// �L���v�V����
			pCaption->SetSize(SIZE_GUIDE.x, SIZE_GUIDE.y);
			pCaption->SetPosition(pos);
			pCaption->SetCol(COL_CURRENT);
			pCaption->SetVtx();

			nIdx = Texture::GetIdx(PATH_GUIDE[i]);
			pCaption->SetIdxTexture(nIdx);
		}

		nNum++;
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGuideInput::Uninit(void)
{
	for (int i = 0; i < INPUT_MAX; i++)
	{
		Object::DeleteObject((CObject**)&m_aGuide[i].pFrame);
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CGuideInput::Update(void)
{
	// ���͂̌��o
	CheckInput();

	for (int i = 0; i < INPUT_MAX; i++)
	{// �t���[���̐F�Ǘ�
		if (m_aGuide[i].pFrame != nullptr)
		{
			D3DXCOLOR col = m_aGuide[i].pFrame->GetCol();

			if (m_aGuide[i].fTime <= 0.0f)
			{
				col += (COL_INITIAL - col) * 0.05f;
			}
			else
			{
				col += (COL_CURRENT - col) * 0.4f;
			}

			m_aGuide[i].pFrame->SetCol(col);

			float fDeltaTime = CManager::GetDeltaTime();

			m_aGuide[i].fTime -= fDeltaTime;
		}
	}
}

//=====================================================
// ���͂̌��o
//=====================================================
void CGuideInput::CheckInput(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetPress(CInputManager::BUTTON_JUMP))
	{
		m_aGuide[INPUT_JUMP].fTime = TIME_GROW;
	}

	if (pInputManager->GetPress(CInputManager::BUTTON_SHOT))
	{
		m_aGuide[INPUT_SHOT].fTime = TIME_GROW;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_MELEE))
	{
		m_aGuide[INPUT_MELEE].fTime = TIME_GROW;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_DODGE))
	{
		m_aGuide[INPUT_DODGE].fTime = TIME_GROW;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_GRAB))
	{
		m_aGuide[INPUT_GRAB].fTime = TIME_GROW;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_LOCK))
	{
		m_aGuide[INPUT_LOCK].fTime = TIME_GROW;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CGuideInput::Draw(void)
{
	
}