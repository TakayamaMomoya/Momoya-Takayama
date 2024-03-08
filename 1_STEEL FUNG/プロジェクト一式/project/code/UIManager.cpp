//*****************************************************
//
// UI�}�l�[�W���[[UIManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "UIManager.h"
#include "UI.h"
#include "inputkeyboard.h"
#include "boost.h"
#include "life.h"
#include "texture.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_FRAME = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
const D3DXCOLOR COL_NORMAL = { 1.0f,1.0f,1.0f,1.0f };
const D3DXCOLOR COL_DAMAGE = { 1.0f,0.0f,0.0f,1.0f };
const float SPEED_FRAME = 0.1f;	// �t���[���̑��x
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CUIManager *CUIManager::m_pUIManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIManager::CUIManager()
{
	m_fCntFrame = 0.0f;
	m_bDisp = false;
	m_pCockpit = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUIManager::~CUIManager()
{

}

//=====================================================
// ��������
//=====================================================
CUIManager *CUIManager::Create(void)
{
	if (m_pUIManager == nullptr)
	{// �C���X�^���X����
		m_pUIManager = new CUIManager;

		// ����������
		m_pUIManager->Init();
	}

	return m_pUIManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUIManager::Init(void)
{
	m_bDisp = true;

	// �R�b�N�s�b�g���o�\��
	m_pCockpit = CUI::Create();

	if (m_pCockpit != nullptr)
	{
		m_pCockpit->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		m_pCockpit->SetPosition(POS_FRAME);
		m_pCockpit->SetVtx();

		int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\Frame03.png");
		m_pCockpit->SetIdxTexture(nIdx);
		m_pCockpit->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
	}

	CBoost::Create();
	CLife::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUIManager::Uninit(void)
{
	m_pUIManager = nullptr;

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CUIManager::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_1))
		{
			m_bDisp = m_bDisp ? false : true;
		}
	}
#endif

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		float fLifeInitial = pPlayer->GetParam().fInitialLife;
		float fLife = pPlayer->GetLife();

		float fRate = fLife / fLifeInitial;

		if (m_pFrame == nullptr)
		{
			if (fRate <= 0.3f)
			{// �t���[���̐���
				CreateFrame();
			}
		}
	}

	// �t���[���̐���
	ManageFrame();
}

//=====================================================
// �t���[������
//=====================================================
void CUIManager::CreateFrame(void)
{
	if (m_pFrame == nullptr)
	{
		m_pFrame = CUI::Create();

		if (m_pFrame != nullptr)
		{
			m_pFrame->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
			m_pFrame->SetPosition(POS_FRAME);
			m_pFrame->SetVtx();

			int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\frame.png");
			m_pFrame->SetIdxTexture(nIdx);
			m_pFrame->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		}
	}
}

//=====================================================
// �t���[���̐���
//=====================================================
void CUIManager::ManageFrame(void)
{
	if (m_pFrame == nullptr)
		return;

	m_fCntFrame += SPEED_FRAME;

	universal::LimitRot(&m_fCntFrame);

	D3DXCOLOR col = m_pFrame->GetCol();

	col.a = sinf(m_fCntFrame);

	m_pFrame->SetCol(col);
}

//=====================================================
// �`�揈��
//=====================================================
void CUIManager::Draw(void)
{
#ifdef _DEBUG

#endif
}