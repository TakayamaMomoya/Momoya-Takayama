//*****************************************************
//
// �R���e�B�j���[�̏���[continue.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "continue.h"
#include "manager.h"
#include "object2D.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "checkPointManager.h"
#include "sound.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MENU_WIDTH	(200.0f)	// ���ڂ̕�
#define MENU_HEIGHT	(50.0f)	// ���ڂ̍���
#define MOVE_FACT	(0.15f)	// �ړ����x
#define LINE_ARRIVAL	(90.0f)	// ���������Ƃ���邵�����l
#define LINE_UNINIT	(3.0f)	// �I������܂ł̂������l
#define SPEED_FADE	(0.006f)	// �w�i�̃t�F�[�h���x
#define ALPHA_BG	(0.5f)	// �w�i�̕s�����x

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CContinue *CContinue::m_pContinue = nullptr;	// ���g�̃|�C���^

//====================================================
// �R���X�g���N�^
//====================================================
CContinue::CContinue()
{
	m_menu = MENU_RETRY_FROM_CHECKPOINT;
	m_state = STATE_NONE;
	ZeroMemory(&m_apMenu[0], sizeof(m_apMenu));
	ZeroMemory(&m_aPosDest[0], sizeof(D3DXVECTOR3) * MENU_MAX);
	m_bSound = false;
}

//====================================================
// �f�X�g���N�^
//====================================================
CContinue::~CContinue()
{

}

//====================================================
// ��������
//====================================================
CContinue *CContinue::Create(void)
{
	if (m_pContinue == nullptr)
	{
		m_pContinue = new CContinue;

		if (m_pContinue != nullptr)
		{
			m_pContinue->Init();
		}
	}

	return m_pContinue;
}

//====================================================
// ����������
//====================================================
HRESULT CContinue::Init(void)
{
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_SE_PAUSE_MENU);
	}

	// �w�i�̐���
	m_pBg = CObject2D::Create(7);

	if (m_pBg != nullptr)
	{
		m_pBg->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f));

		m_pBg->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		m_pBg->SetVtx();
	}

	char *pPath[MENU_MAX] =
	{// ���j���[���ڂ̃p�X
		"data\\TEXTURE\\UI\\menu_return.png",
		"data\\TEXTURE\\UI\\menu_retry.png",
		"data\\TEXTURE\\UI\\menu_quit.png",
	};

	int nIdxTexture;

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{// ���j���[���ڂ̃|���S���𐶐�
		if (m_apMenu[nCntMenu] == nullptr)
		{
			m_apMenu[nCntMenu] = CObject2D::Create(7);

			if (m_apMenu[nCntMenu] != nullptr)
			{
				// �|���S���̐ݒ�
				m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(-MENU_WIDTH, SCREEN_HEIGHT * 0.1f + MENU_HEIGHT * nCntMenu * 2, 0.0f));
				m_aPosDest[nCntMenu] = m_apMenu[nCntMenu]->GetPosition();

				m_apMenu[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);

				m_apMenu[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

				// �e�N�X�`���̐ݒ�
				nIdxTexture = CTexture::GetInstance()->Regist(pPath[nCntMenu]);

				m_apMenu[nCntMenu]->SetIdxTexture(nIdxTexture);

				m_apMenu[nCntMenu]->SetVtx();
			}
		}
	}

	m_state = STATE_IN;

	m_aPosDest[0].x = MENU_WIDTH;

	EnableNotStop(true);

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CContinue::Uninit(void)
{
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{// ���j���[���ڂ̔j��
		if (m_apMenu[nCnt] != nullptr)
		{
			m_apMenu[nCnt]->Uninit();

			m_apMenu[nCnt] = nullptr;
		}
	}

	m_pContinue = nullptr;

	Release();
}

//====================================================
// �X�V����
//====================================================
void CContinue::Update(void)
{
	// ��ԊǗ�
	ManageState();
}

//====================================================
// ��ԊǗ�
//====================================================
void CContinue::ManageState(void)
{
	if (m_state != STATE_OUT)
	{
		Input();
	}

	// �w�i�̊Ǘ�
	ManageBg();

	// �I���t���O�p
	int nEnd = 0;

	// �|���S����ڕW�ʒu�Ɍ����킹��
	for (int i = 0;i < MENU_MAX;i++)
	{
		if (m_apMenu[i] != nullptr)
		{
			D3DXVECTOR3 pos = m_apMenu[i]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[i] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;

			vecDiff += pos;

			m_apMenu[i]->SetPosition(vecDiff);

			m_apMenu[i]->SetVtx();

			float fDiff = m_aPosDest[i].x - vecDiff.x;

			if (fDiffOld * fDiffOld >= LINE_ARRIVAL * LINE_ARRIVAL &&
				fDiff * fDiff < LINE_ARRIVAL * LINE_ARRIVAL &&
				i < MENU_MAX - 1)
			{// �������������l��艺�ɂȂ����牺�̂��̂𓮂���
				if (m_state == STATE_IN)
				{
					m_aPosDest[i + 1].x = MENU_WIDTH;
				}
				else if (m_state == STATE_OUT)
				{
					m_aPosDest[i + 1].x = -MENU_WIDTH;
				}
			}

			if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
				m_state == STATE_OUT)
			{// �I���̃��C��
				nEnd++;
			}
		}
	}

	if (nEnd == MENU_MAX &&
		m_state == STATE_OUT)
	{
		Uninit();
	}
}

//====================================================
// �w�i�̊Ǘ�
//====================================================
void CContinue::ManageBg(void)
{
	if (m_pBg == nullptr)
	{
		return;
	}

	D3DXCOLOR col = m_pBg->GetCol();

	switch (m_state)
	{
	case CContinue::STATE_IN:

		col.a += SPEED_FADE;

		if (col.a > ALPHA_BG)
		{
			// �Q�[�����~����
			CGame *pGame = CGame::GetInstance();

			if (pGame != nullptr)
			{
				pGame->EnableStop(true);
			}

			col.a = ALPHA_BG;
		}

		break;
	case CContinue::STATE_OUT:

		col.a -= SPEED_FADE;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;
		}

		break;
	default:
		break;
	}

	m_pBg->SetCol(col);
}

//====================================================
// ���쏈��
//====================================================
void CContinue::Input(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();

	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}
	else
	{
		if (pFade->GetState() != CFade::FADE_NONE)
		{
			return;
		}
	}

	if (pInputManager == nullptr)
	{
		return;
	}

	if (m_apMenu[m_menu] != nullptr)
	{// �ȑO�ɑI�����Ă����ڂ��I��F�ɂ���
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));
	}

	// ���ڐ؂�ւ�
	if (pInputManager->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{
		m_menu = (MENU)((m_menu + 1) % MENU_MAX);

		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_PAUSE_ARROW);

			m_bSound = true;
		}
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_AXIS_UP))
	{
		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);
	}

	if (m_apMenu[m_menu] != nullptr)
	{// �I�����Ă��鍀�ڂ̐F�ύX
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
	{// �I�����ڂɃt�F�[�h����

		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_PAUSE_ENTER);
		}
		Fade(m_menu);
	}
}

//====================================================
// �t�F�[�h���鏈��
//====================================================
void CContinue::Fade(MENU menu)
{
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}

	switch (menu)
	{
	case CContinue::MENU_RETRY_FROM_CHECKPOINT:

		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CContinue::MENU_RESTART:

		CGame::SetState(CGame::STATE_END);
		pFade->SetFade(CScene::MODE_GAME);

		CheckPoint::SetProgress(0);

		break;
	case CContinue::MENU_QUIT:
	{
		pFade->SetFade(CScene::MODE_TITLE);
	}
		break;
	default:
		break;
	}
}

//====================================================
// �`�揈��
//====================================================
void CContinue::Draw(void)
{

}