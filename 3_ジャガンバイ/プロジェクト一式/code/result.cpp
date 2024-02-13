//*****************************************************
//
// ���U���g�̏���[result.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "result.h"
#include "manager.h"
#include "object2D.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "player.h"
#include "number.h"
#include "inputjoypad.h"
#include "playerManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float RESULT_WIDTH = 918.0f * 0.4f;	// ���ʂ̕�
	const float RESULT_HEIGHT = 178.0f * 0.4f;	// ���ʂ̍���
	const char* RESULT_PATH = "data\\TEXTURE\\UI\\result.png";	// ���ʂ̃p�X

	const float CAPTION_WIDTH = 100.0f;	// �L���v�V�����̕�
	const float CAPTION_HEIGHT = 35.0f;	// �L���v�V�����̍���
	const char* CAPTION_PATH = "data\\TEXTURE\\UI\\caption.png";	// �L���v�V�����̃p�X

	const int NUM_PLACE = 1;	// ����
	const float ICON_WIDTH = 40.0f;	// �����̕�
	const float ICON_HEIGHT = 40.0f;	// �����̍���

	const char* ICON_PATH[NUM_PLAYER] =
	{// �v���C���[�A�C�R���̃p�X
		"data\\TEXTURE\\UI\\player01.png",
		"data\\TEXTURE\\UI\\player02.png",
		"data\\TEXTURE\\UI\\player03.png",
		"data\\TEXTURE\\UI\\player04.png"
	};
}

//====================================================
// �R���X�g���N�^
//====================================================
CResult::CResult()
{
	m_state = STATE_NONE;
	m_pBg = nullptr;
	m_p2DResult = nullptr;
	ZeroMemory(&m_aInfoSurvived[0], sizeof(m_aInfoSurvived));
	m_nNumSuvived = 0;
}

//====================================================
// �f�X�g���N�^
//====================================================
CResult::~CResult()
{

}

//====================================================
// ��������
//====================================================
CResult *CResult::Create(bool bWin)
{
	CResult *pResult = nullptr;

	pResult = new CResult;

	if (pResult != nullptr)
	{
		pResult->Init();

		pResult->Create2D(bWin);
	}

	return pResult;
}

//====================================================
// �QD�I�u�W�F�N�g�̐���
//====================================================
void CResult::Create2D(bool bWin)
{
	int nIdxTexture;

	char *pPathCaption[2] =
	{
		"data\\TEXTURE\\UI\\result.png",
		"data\\TEXTURE\\UI\\result.png",
	};

	// �w�i�̐���
	m_pBg = CObject2D::Create(7);

	if (m_pBg != nullptr)
	{
		m_pBg->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f));

		m_pBg->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f));

		m_pBg->SetVtx();
	}

	// ���o���̐���
	m_p2DResult = CObject2D::Create(7);

	if (m_p2DResult != nullptr)
	{
		m_p2DResult->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.4f, 0.0f));

		m_p2DResult->SetSize(RESULT_WIDTH, RESULT_HEIGHT);

		m_p2DResult->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		nIdxTexture = CTexture::GetInstance()->Regist(pPathCaption[bWin]);

		m_p2DResult->SetIdxTexture(nIdxTexture);

		m_p2DResult->SetVtx();
	}
}

//====================================================
// ����������
//====================================================
HRESULT CResult::Init(void)
{
	m_state = STATE_WAIT;

	// �Q�[�������U���g��Ԃɂ���
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		pGame->SetState(CGame::STATE_RESULT);
	}

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CResult::Uninit(void)
{
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	if (m_p2DResult != nullptr)
	{
		m_p2DResult->Uninit();
		m_p2DResult = nullptr;
	}

	ZeroMemory(&m_aInfoSurvived[0], sizeof(m_aInfoSurvived));

	// �v���C���[�}�l�[�W���[�̏I��
	CPlayerManager *pPlayerManger = CPlayerManager::GetInstance();

	if (pPlayerManger != nullptr)
	{
		pPlayerManger->Uninit();
	}

	Release();
}

//====================================================
// �X�V����
//====================================================
void CResult::Update(void)
{
	// ���쏈��
	Input();
}

//====================================================
// ���쏈��
//====================================================
void CResult::Input(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
	{
		return;
	}

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, i))
		{
			// �Q�[�����I����Ԃɂ���
			CGame *pGame = CGame::GetInstance();

			if (pGame != nullptr)
			{
				pGame->SetState(CGame::STATE::STATE_END);
			}
		}
	}
}

//====================================================
// �`�揈��
//====================================================
void CResult::Draw(void)
{

}

//====================================================
// �����҂̐ݒ�
//====================================================
void CResult::SetSurvived(CPlayer *pPlayer)
{
	if (m_nNumSuvived == NUM_PLAYER)
	{// �ő�ɒB���Ă����ꍇ�̃G���[
		return;
	}

	if (m_aInfoSurvived[m_nNumSuvived].pSuvived == nullptr)
	{
		// ���̕ۑ�
		m_aInfoSurvived[m_nNumSuvived].pSuvived = pPlayer;

		// ���̕\��
		DispSuvived(&m_aInfoSurvived[m_nNumSuvived]);

		m_nNumSuvived++;
	}
	else
	{
		assert(("���U���g�Ńv���C���[���̎󂯎��Ɏ��s", false));
	}
}

//====================================================
// �����҂̕\��
//====================================================
void CResult::DispSuvived(SInfoSuvived *pInfo)
{
	int nIdx = m_nNumSuvived;

	if (pInfo->pSuvived != nullptr)
	{
		int nID = pInfo->pSuvived->GetID();

		if (pInfo->pIcon == nullptr)
		{// �A�C�R���̐���
			pInfo->pIcon = CObject2D::Create(7);

			if (pInfo->pIcon != nullptr)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.6f + ICON_HEIGHT * nIdx * 2, 0.0f);

				pInfo->pIcon->SetPosition(pos);
				pInfo->pIcon->SetSize(ICON_WIDTH, ICON_HEIGHT);

				int nIdx = CTexture::GetInstance()->Regist(ICON_PATH[nID]);
				pInfo->pIcon->SetIdxTexture(nIdx);
				
				pInfo->pIcon->SetVtx();
			}
		}
		
		if (pInfo->pCaption == nullptr)
		{// �L���v�V�����̐���
			pInfo->pCaption = CObject2D::Create(7);

			if (pInfo->pCaption != nullptr)
			{
				pInfo->pCaption->SetSize(CAPTION_WIDTH, CAPTION_HEIGHT);
				pInfo->pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.35f + ICON_WIDTH + CAPTION_WIDTH, SCREEN_HEIGHT * 0.6f + ICON_HEIGHT * nIdx * 2, 0.0f));

				int nIdx = CTexture::GetInstance()->Regist(CAPTION_PATH);
				pInfo->pCaption->SetIdxTexture(nIdx);
				pInfo->pCaption->SetVtx();
			}
		}
	}
}