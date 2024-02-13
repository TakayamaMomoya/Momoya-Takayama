//=========================================================
//
// �Q�[���I�[�o�[���� [gameover.cpp]
// Author = �����đ�Y
//
//=========================================================

//===============================================
// �C���N���[�h
//===============================================
#include "gameover.h"
#include "enemyManager.h"
#include "playerManager.h"
#include "particle.h"
#include "scene.h"
#include "fade.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "camera.h"
#include "manager.h"
#include "goalTimer.h"
#include "UIManager.h"
#include "goal.h"
#include "texture.h"
#include "game.h"
#include "gimmickManager.h"
#include "block.h"

//===============================================
// �萔��`
//===============================================
namespace
{
	// �v���C���[�̑̂̃p�X
	const char* PLAYER_BODY_PATH[NUM_PLAYER] =
	{
		"data\\MOTION\\motionPotatoman01.txt",
		"data\\MOTION\\motionPotatoman02.txt",
		"data\\MOTION\\motionPotatoman03.txt",
		"data\\MOTION\\motionPotatoman04.txt"
	};

	// �v���C���[�̈ʒu
	const D3DXVECTOR3 PLAYER_POS[NUM_PLAYER] =
	{
		D3DXVECTOR3(-35.0f, 0.0f, -215.0f),
		D3DXVECTOR3(35.0f, 0.0f, -215.0f),
		D3DXVECTOR3(90.0f, 0.0f, -215.0f),
		D3DXVECTOR3(-90.0f, 0.0f, -215.0f)
	};

	// �v���C���[�̌���
	const D3DXVECTOR3 PLAYER_ROT[NUM_PLAYER] =
	{
		D3DXVECTOR3(0.0f, D3DX_PI * -0.10f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.10f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.15f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * -0.15f, 0.0f)
	};

	// �G�̑̂̃p�X
	const char* ENEMY_BODY_PATH = "data\\MOTION\\motionEnemy00.txt";

	// �G�̈ʒu
	const D3DXVECTOR3 ENEMY_POS[NUM_ENEMY] =
	{
		D3DXVECTOR3(-140.0f, 0.0f, -160.0f),
		D3DXVECTOR3(-70.0f, 0.0f, -130.0f),
		D3DXVECTOR3(0.0f, 0.0f, -100.0f),
		D3DXVECTOR3(70.0f, 0.0f, -130.0f),
		D3DXVECTOR3(140.0f, 0.0f, -160.0f)
	};

	// �G�̌���
	const D3DXVECTOR3 ENEMY_ROT[NUM_ENEMY] =
	{
		D3DXVECTOR3(0.0f, D3DX_PI * -0.2f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * -0.1f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.1f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.2f, 0.0f)
	};

	// ���_�J�����̈ʒu
	const D3DXVECTOR3 CAMERA_POSV = D3DXVECTOR3(0.0f, 110.0f, -340.0f);

	// �����_�J�����̈ʒu
	const D3DXVECTOR3 CAMERA_POSR = D3DXVECTOR3(0.0f, -50.0f, 50.0f);

	// �Q�[���I�[�o�[�̃e�N�X�`���̃p�X
	const char* LOGO_PATH = "data\\TEXTURE\\UI\\gameover.png";

	// �Q�[���I�[�o�[�̉���
	const float LOGO_WIDTH = 480.0f;

	// �Q�[���I�[�o�[�̏c��
	const float LOGO_HEIGHT = 270.0f;

	// �Q�[���I�[�o�[�̈ʒu
	const D3DXVECTOR3 LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f, 0.0f);

	// �R���e�B�j���[�̃e�N�X�`���̃p�X
	const char* CONTINY_PATH = "data\\TEXTURE\\UI\\pressAbutton.png";

	// �R���e�B�j���[�̉���
	const float CONTINY_WIDTH = 322.0f;

	// �R���e�B�j���[�̏c��
	const float CONTINY_HEIGHT = 30.0f;

	// �R���e�B�j���[�̈ʒu
	const D3DXVECTOR3 CONTINY_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 250.0f, 0.0f);

	// �R���e�B�j���[�̕\���^�C�~���O
	const float CONTINY_DISPTIMING = 0.7f;
}

//===============================================
// �ÓI�����o�ϐ��錾
//===============================================
CGameover* CGameover::m_pGameover = nullptr;	// ���g�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CGameover::CGameover()
{
	// �l�̃N���A
	ZeroMemory(&m_apModelPlayer[0], sizeof(m_apModelPlayer));
	ZeroMemory(&m_apModelEnemy[0], sizeof(m_apModelEnemy));
	m_pBlack = nullptr;
	m_pLogo = nullptr;
	m_pContiny = nullptr;
	m_bDeathPlayer[0] = {};
	m_bFlash = false;
	m_fFadeCounter = 0.0f;
	m_fFlashCounter = 0.0f;
}

//===============================================
// �f�X�g���N�^
//===============================================
CGameover::~CGameover()
{
	
}

//===============================================
// ��������
//===============================================
CGameover *CGameover::Create(int nPriority)
{
	if (m_pGameover == nullptr)
	{
		// �Q�[���I�[�o�[�̐���
		m_pGameover = new CGameover;

		if (m_pGameover != nullptr)
		{
			// ����������
			m_pGameover->Init();
		}
		else if (m_pGameover == nullptr)
		{
			return nullptr;
		}
	}

	return m_pGameover;
}

//===============================================
// ����������
//===============================================
HRESULT CGameover::Init()
{
	// �l�̏�����
	m_pBlack = nullptr;
	m_pLogo = nullptr;
	m_pContiny = nullptr;
	m_bDeathPlayer[0] = {};
	m_bFlash = false;
	m_fFadeCounter = 0.0f;
	m_fFlashCounter = 0.0f;

	// �C���X�^���X���擾
	CEnemyManager* pEnemyManager = CEnemyManager::GetInstance();
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CGame* pGame = CGame::GetInstance();
	CUIManager* pUIManager = CUIManager::GetInstance();
	CGoal* pGoal = CGoal::GetInstance();
	CGimmickManager* pGimmickManager = CGimmickManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		// �擪�I�u�W�F�N�g����
		CEnemy* pEnemy = pEnemyManager->GetHead();

		// ���ݏo�����Ă���G���폜
		while (pEnemy != nullptr)
		{
			// ���̃A�h���X��ۑ�
			CEnemy* pEnemyNext = pEnemy->GetNext();

			// �I������
			pEnemy->Uninit();

			// ���̃A�h���X����
			pEnemy = pEnemyNext;
		}

		// �Q�[���I�[�o�[��Ԃɂ��ēG�̃X�|�[�����~����
		if (pGame != nullptr)
		{
			pGame->SetState(CGame::STATE_GAMEOVER);
		}

		// �G�𐶐�
		for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
		{
			m_apModelEnemy[nCntEnemy] = CMotion::Create((char*)ENEMY_BODY_PATH);

			// �G�̏��̐ݒ�
			m_apModelEnemy[nCntEnemy]->SetPosition(ENEMY_POS[nCntEnemy]);	// �ʒu
			m_apModelEnemy[nCntEnemy]->SetRot(ENEMY_ROT[nCntEnemy]);		// ����
			m_apModelEnemy[nCntEnemy]->SetMotion(0);						// ���[�V����
		}
	}
	else if (pEnemyManager == nullptr)
	{
		return E_FAIL;
	}

	// �M�~�b�N�̍폜
	if (pGimmickManager != nullptr)
	{
		pGimmickManager->DeleteAllGimmick();
	}
	else if (pGimmickManager == nullptr)
	{
		return E_FAIL;
	}

	// ���S���[�V�����̃v���C���[�̐���
	if (pPlayerManager != nullptr)
	{
		int nNumPlayer = 4;

		for (int nCntPlayer = 0; nCntPlayer < nNumPlayer; nCntPlayer++)
		{
			bool bDeathPlayer = pPlayerManager->GetDeathPlayer(nCntPlayer);		// ���񂾃v���C���[���擾

			if (bDeathPlayer == true)
			{
				m_apModelPlayer[nCntPlayer] = CMotion::Create((char*)PLAYER_BODY_PATH[nCntPlayer]);	// ����

				// �v���C���[�̏��̐ݒ�
				m_apModelPlayer[nCntPlayer]->SetPosition(PLAYER_POS[nCntPlayer]);	// �ʒu
				m_apModelPlayer[nCntPlayer]->SetRot(PLAYER_ROT[nCntPlayer]);		// ����
				m_apModelPlayer[nCntPlayer]->SetMotion(3);							// ���[�V����
			}
		}
	}
	else if (pPlayerManager == nullptr)
	{
		return E_FAIL;
	}

	// ���|���S���̕\��
	if (m_pBlack == nullptr)
	{
		m_pBlack = CObject2D::Create(7);
	}

	if (m_pBlack != nullptr)
	{// ���|���S���̏���ݒ�
		m_pBlack->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		m_pBlack->SetPosition({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f });
		m_pBlack->SetVtx();
		m_pBlack->SetCol({ 0.0, 0.0, 0.0, 0.0 });
	}
	else if (m_pBlack == nullptr)
	{
		return E_FAIL;
	}


	// �Q�[���I�[�o�[�̕\��
	if (m_pLogo == nullptr)
	{
		m_pLogo = CObject2D::Create(7);
	}

	if (m_pLogo != nullptr)
	{// �Q�[���I�[�o�[�̏���ݒ�
		m_pLogo->SetSize(LOGO_WIDTH, LOGO_HEIGHT);
		m_pLogo->SetPosition(LOGO_POS);
		int nIdx = CTexture::GetInstance()->Regist(LOGO_PATH);
		m_pLogo->SetIdxTexture(nIdx);
		m_pLogo->SetVtx();
		m_pLogo->SetCol({1.0, 1.0, 1.0, 0.0});
	}
	else if (m_pLogo == nullptr)
	{
		return E_FAIL;
	}

	// �R���e�B�j���[�̕\��
	if (m_pContiny == nullptr)
	{
		m_pContiny = CObject2D::Create(7);
	}

	if (m_pContiny != nullptr)
	{// �R���e�B�j���[�̏���ݒ�
		m_pContiny->SetSize(CONTINY_WIDTH, CONTINY_HEIGHT);
		m_pContiny->SetPosition(CONTINY_POS);
		int nIdx = CTexture::GetInstance()->Regist(CONTINY_PATH);
		m_pContiny->SetIdxTexture(nIdx);
		m_pContiny->SetVtx();
		m_pContiny->SetCol({ 1.0, 1.0, 1.0, 0.0 });
	}
	else if (m_pContiny == nullptr)
	{
		return E_FAIL;
	}

	// UI���\��
	if (pUIManager != nullptr)
	{
		pUIManager->EnableDisp(false);
	}
	else if (pUIManager == nullptr)
	{
		return E_FAIL;
	}
	if (pGoal != nullptr)
	{
		pGoal->Uninit();
	}
	else if (pGoal == nullptr)
	{
		return E_FAIL;
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CGameover::Uninit(void)
{
	m_pGameover = nullptr;
	m_pBlack = nullptr;
	m_pLogo = nullptr;
	m_pContiny = nullptr;

	delete this;
}

//===============================================
// �X�V����
//===============================================
void CGameover::Update(void)
{
	// �C���X�^���X���擾
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CCamera* pCamera = CManager::GetCamera();
	
	// �J���������P�b�g�t�߂ֈړ�
	if (pCamera != nullptr)
	{
		// �J�����̐ݒ�
		pCamera->UpdateGameover(CAMERA_POSV, CAMERA_POSR);
	}

	int nNumBlock = CBlock::GetNumAll();

	if (nNumBlock > 0)
	{
		CBlock** pBlock = CBlock::GetBlock();

		for (int nCntBlock = 0; nCntBlock < NUM_OBJECT; nCntBlock++)
		{
			if (pBlock[nCntBlock] != nullptr)
			{
				pBlock[nCntBlock]->Hit(100);
			}
		}
	}

	bool bFinish = m_apModelPlayer[0]->IsFinish();		// ���S���[�V�������I��������

	if (bFinish == true)
	{
		// �Ó]����
		BlackOut();
	}
}

//===============================================
// �`�揈��
//===============================================
void CGameover::Draw(void)
{
	
}

//===============================================
// �Ó]����
//===============================================
void CGameover::BlackOut(void)
{
	// �C���X�^���X���擾
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	CFade* pFade = CFade::GetInstance();

	// �J�E���g���X�V
	m_fFadeCounter += 0.01f;

	if (m_pBlack != nullptr)
	{// ���|���S�����������\������
		m_pBlack->SetCol({ 0.0, 0.0, 0.0, m_fFadeCounter });
	}

	if (m_pLogo != nullptr)
	{// �Q�[���I�[�o�[���������\������
		m_pLogo->SetCol({ 1.0, 1.0, 1.0, m_fFadeCounter - 0.1f });

		if (m_fFadeCounter - 0.1f >= 0.7f)
		{// �ړ��ʂ�����������
			m_pLogo->DicMove(0.1f);
			m_pLogo->SetVtx();
		}
		else
		{// ���������ֈړ�
			m_pLogo->SetMove({ 0.0f, 0.8f, 0.0f });
			m_pLogo->SetVtx();
		}
	}

	if (m_fFadeCounter >= CONTINY_DISPTIMING)
	{
		if (m_pContiny != nullptr)
		{// �R���e�B�j���[���������\������
			if (m_fFlashCounter > 1.0f || m_fFlashCounter < 0.0f)
			{// �����x���ŏ����ő�ɂȂ���
				m_bFlash = m_bFlash ? false : true;		// ��Ԃ�؂�ւ���
			}

			if (m_bFlash == false)
			{
				m_fFlashCounter += 0.02f;
			}
			else if(m_bFlash == true)
			{
				m_fFlashCounter -= 0.02f;
			}

			m_pContiny->SetCol({ 1.0, 1.0, 1.0, m_fFlashCounter });
		}
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
	{//ENTER�L�[�������ꂽ��
		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_RANKING);	// �����L���O�ɑJ��

			// �v���C���[�}�l�[�W���[�̏I��
			CPlayerManager* pPlayerManger = CPlayerManager::GetInstance();

			if (pPlayerManger != nullptr)
			{
				pPlayerManger->Uninit();
			}
		}
	}
}