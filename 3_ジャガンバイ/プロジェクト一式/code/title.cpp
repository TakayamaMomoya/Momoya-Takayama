//*****************************************************
//
// �^�C�g������[title.cpp]
// Author:����쏟
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "title.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "manager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "renderer.h"
#include "sound.h"

#include "object3D.h"
#include "objectX.h"
#include "skybox.h"

//*****************************************************
// �}�N����`
//*****************************************************

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* PLAYER_BODY_PATH[NUM_PLAYER] =
	{// �G�̑̂̃p�X
		"data\\MOTION\\motionTitle01.txt",
		"data\\MOTION\\motionTitle02.txt",
		"data\\MOTION\\motionTitle03.txt",
		"data\\MOTION\\motionTitle04.txt",
	};
	const D3DXVECTOR3 PLAYER_POS[NUM_PLAYER] =
	{// �v���C���[�̈ʒu
		D3DXVECTOR3(   0.0f, 0.0f, -150.0f),
		D3DXVECTOR3(-125.0f, 0.0f, -100.0f),
		D3DXVECTOR3( 100.0f, 0.0f,  -50.0f),
		D3DXVECTOR3(-75.0f, 0.0f,  100.0f),
	};
	const D3DXVECTOR3 PLAYER_ROT[NUM_PLAYER] =
	{// �v���C���[�̌���
		D3DXVECTOR3(0.0f, D3DX_PI *  0.0f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * -0.25f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI *  0.25f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * -0.10f, 0.0f),
	};
	const D3DXVECTOR3 PLAYER_ESC_MOVE = D3DXVECTOR3(0.0f, 0.0f, 8.0f);		// �v���C���[�̓�����Ƃ��̈ړ��� 
	const D3DXVECTOR3 PLAYER_ESC_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// �v���C���[�̓�����Ƃ��̌���

	const char* ENEMY_BODY_PATH[ENEMY::NUM_ENEMY] =
	{// �G�̑̂̃p�X
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
	};
	const D3DXVECTOR3 ENEMY_POS[ENEMY::NUM_ENEMY] =
	{// �G�̈ʒu
		D3DXVECTOR3(-250.0f, 0.0f, -600.0f),
		D3DXVECTOR3(-150.0f,  0.0f, -450.0f),
		D3DXVECTOR3(-90.0f,  0.0f, -850.0f),
		D3DXVECTOR3(-30.0f, 0.0f, -600.0f),
		D3DXVECTOR3( 40.0f, 0.0f, -800.0f),
		D3DXVECTOR3( 80.0f,  0.0f, -950.0f),
		D3DXVECTOR3( 120.0f,  0.0f, -750.0f),
		D3DXVECTOR3( 250.0f, 0.0f, -500.0f),
	};
	const D3DXVECTOR3 ENEMY_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// �G�̌���
	const D3DXVECTOR3 ENEMY_MOVE = D3DXVECTOR3(0.0f, 0.0f, 7.5f);	// �G�̈ړ���

	const float FIELD_WIDTH = 10000.0f;		// �t�B�[���h�̕�
	const float FIELD_HEIGHT = 10000.0f;	// �t�B�[���h�̍���

	const D3DXVECTOR3 LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 150.0f, 0.0f);	// ���S�̈ʒu
	const float LOGO_WIDTH = 875.0f * 0.35f;	// ���S�̕�
	const float LOGO_HEIGHT = 320.0f * 0.35f;	// ���S�̍���
	const char* LOGO_PATH = "data\\TEXTURE\\UI\\logo000.png";	// ���S�̃p�X

	const D3DXVECTOR3 STATE_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f, 0.0f);	// �X�^�[�g�\���̈ʒu
	const float START_WIDTH = 200.0f;	// �X�^�[�g�\���̕�
	const float START_HEIGHT = 50.0f;	// �X�^�[�g�\���̍���
	const float ADD_WIDTH = 16.0f;		// ���̊g�剻
	const float ADD_HEIGHT = 10.0f;		// �����̊g�剻  
	const char* START_PATH = "data\\TEXTURE\\UI\\gamestart.png";	// �X�^�[�g�\���̃p�X

	const int FADE_COUNT = 120;			// �t�F�[�h�܂ł̎���

	const float ALPHA_UPPER = 1.0f;			// ���l�̏����
	const float ALPHA_LOWER = 0.25f;		// ���l�̉�����
	const float ALPHA_CHANGE = 0.1f;		// ���l�̕ω���
	const float ALPHA_CHANGE_LOGO = 0.01f;	// ���l�̕ω���

	const D3DXVECTOR3 CAMERA_POSV[CTitle::CAMERA_MAX] =
	{// ���_�J�����̈ʒu
		D3DXVECTOR3(0.0f, 75.0f, -400.0f),
		D3DXVECTOR3(75.0f, 75.0f, -250.0f),
		D3DXVECTOR3(-30.0f, 60.0f, -175.0f),
		D3DXVECTOR3(-30.0f, 60.0f, -105.0f),
		D3DXVECTOR3(0.0f, 30.0f, 0.0f),
	};
	const D3DXVECTOR3 CAMERA_POSR[CTitle::CAMERA_MAX] =
	{// �����_�J�����̈ʒu
		D3DXVECTOR3(0.0f, 25.0f, 0.0f),
		D3DXVECTOR3(0.0f, 35.0f, 0.0f),
		D3DXVECTOR3(0.0f, 25.0f, 0.0f),
		D3DXVECTOR3(0.0f, 35.0f, 0.0f),
		D3DXVECTOR3(0.0f, 35.0f, 0.0f),
	};
	const D3DXVECTOR3 CAMERA_MOVE[CTitle::CAMERA_MAX] =
	{// �J�����̈ړ���
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	};
	const int CAMERA_SWITCH_COUNT[CTitle::CAMERA_MAX] =
	{// �J�����؂�ւ��̃J�E���g
		720,
		360,
		360,
		360,
		360,
	};

	const D3DXVECTOR3 CAMERA_ESC_POS_V = D3DXVECTOR3(0.0f, 200.0f, -800.0f);
	const D3DXVECTOR3 CAMERA_ESC_POS_R = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	const D3DXVECTOR3 CAMERA_ESC_MOVE = D3DXVECTOR3(0.0f, 0.0f, 7.0f);
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CTitle::CTitle()
{
	m_state = STATE_NONE;
	ZeroMemory(&m_cameraInfo, sizeof(m_cameraInfo));
	m_pLogo = nullptr;
	m_pLogoLate = nullptr;
	m_pStart = nullptr;
	ZeroMemory(&m_apModelPlayer[0], sizeof(m_apModelPlayer));
	ZeroMemory(&m_apModelEnemy[0], sizeof(m_apModelEnemy));
	m_nFadeCnt = 0;
	m_bIsAlphaChange = false;
	m_fSizeX = LOGO_WIDTH;
	m_fSizeY = LOGO_HEIGHT;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTitle::~CTitle()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CTitle::Init(void)
{
	// ���擾
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		// �J�����̐ݒ�
		pCamera->SetTitle(
			CAMERA_POSV[m_cameraInfo.num],
			CAMERA_POSR[m_cameraInfo.num]);
	}
	else if (pCamera == nullptr)
	{
		return E_FAIL;
	}

	// ���S�̐���
	m_pLogo = CObject2D::Create(7);

	if (m_pLogo != nullptr)
	{
		m_pLogo->SetSize(LOGO_WIDTH, LOGO_HEIGHT);
		m_pLogo->SetPosition(LOGO_POS);
		int nIdx = CTexture::GetInstance()->Regist(LOGO_PATH);
		m_pLogo->SetIdxTexture(nIdx);
		m_pLogo->SetVtx();
	}
	if (m_pLogo == nullptr)
	{
		return E_FAIL;
	}

	// �X�^�[�g�\���̐���
	m_pStart = CObject2D::Create(7);

	if (m_pStart != nullptr)
	{
		m_pStart->SetSize(START_WIDTH, START_HEIGHT);
		m_pStart->SetPosition(STATE_POS);
		int nIdx = CTexture::GetInstance()->Regist(START_PATH);
		m_pStart->SetIdxTexture(nIdx);
		m_pStart->SetVtx();
	}
	else if (m_pStart == nullptr)
	{
		return E_FAIL;
	}

	// �X�J�C�{�b�N�X�̐���
	CSkybox* pSkyBox = CSkybox::Create();

	if (pSkyBox == nullptr)
	{
		return E_FAIL;
	}

	// �n�ʂ̐���
	CObject3D* pField = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pField != nullptr)
	{
		pField->SetSize(FIELD_WIDTH, FIELD_HEIGHT);
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\field00.jpg");
		pField->SetIdxTexture(nIdx);
		pField->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f));
	}
	else if (pField == nullptr)
	{
		return E_FAIL;
	}

	// �^�C�g�����f���̐���
	CObjectX* pTitleModel = CObjectX::Create();

	if (pTitleModel != nullptr)
	{
		int nIdx = CModel::Load("data\\MODEL\\title\\title_model.x");
		pTitleModel->BindModel(nIdx);
	}
	else if (pTitleModel == nullptr)
	{
		return E_FAIL;
	}

	// �L�����N�^�[�̐�������
	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		m_apModelPlayer[nCount] = CMotion::Create((char*)PLAYER_BODY_PATH[nCount]);

		if (m_apModelPlayer[nCount] != nullptr)
		{
			m_apModelPlayer[nCount]->SetPosition(PLAYER_POS[nCount]);
			m_apModelPlayer[nCount]->SetRot(PLAYER_ROT[nCount]);
			m_apModelPlayer[nCount]->SetMotion(TITLE_MOTION_PLAYER_NEUTRAL);
		}
		else if (m_apModelPlayer[nCount] == nullptr)
		{
			return E_FAIL;
		}
	}

	// �t�H�O��������
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
	}
	else if (pRenderer == nullptr)
	{
		return E_FAIL;
	}

	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		// BGM�̍Đ�
		pSound->Play(pSound->LABEL_BGM_TITLE);
	}
	else if (pSound == nullptr)
	{
		return E_FAIL;
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTitle::Uninit(void)
{
	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();

	// �t�H�O��������
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(false);
	}
}

//=====================================================
// �X�V����
//=====================================================
void CTitle::Update(void)
{
	// ���擾
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	CCamera* pCamera = CManager::GetCamera();

	// �V�[���̍X�V
	CScene::Update();

	if (pCamera != nullptr)
	{
		// �J�����̍X�V
		UpdateCamera();
	}

	if (m_state == STATE_NONE)
	{
		if (pKeyboard != nullptr && pMouse != nullptr && pJoypad != nullptr)
		{
			if (pKeyboard->GetTrigger(DIK_RETURN) ||
				pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
				pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
			{// �t�F�[�h�J�n

				// �T�E���h�C���X�^���X�̎擾
				CSound* pSound = CSound::GetInstance();

				if (pSound != nullptr)
				{
					pSound->Play(pSound->LABEL_SE_START_GAME);
				}

				// �t�F�[�h�ݒ�
				SetFadeIn();
			}
		}
	}
	else if (m_state == STATE_OUT)
	{
		// �t�F�[�h�X�V����
		UpdateFade();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CTitle::Draw(void)
{

}

//=====================================================
// �X�^�[�g�\���̊Ǘ�
//=====================================================
void CTitle::ManageStart(void)
{
	// �F�̏��擾
	D3DXCOLOR colStart = m_pStart->GetCol();
	D3DXCOLOR colLogo = m_pLogo->GetCol();
	D3DXCOLOR colLogoLate = m_pLogoLate->GetCol();

	if (m_pStart != nullptr && m_pLogo != nullptr)
	{
		if (m_bIsAlphaChange == false)
		{
			colStart.a -= ALPHA_CHANGE;
		}
		else if (m_bIsAlphaChange == true)
		{
			colStart.a += ALPHA_CHANGE;
		}

		if (colStart.a <= ALPHA_LOWER)
		{
			m_bIsAlphaChange = true;
		}
		else if (colStart.a >= ALPHA_UPPER)
		{
			m_bIsAlphaChange = false;
		}

		if (colLogo.a >= 0.0f)
		{
			// ���S�̃��l������
			colLogo.a -= ALPHA_CHANGE_LOGO;
		}

		if (colLogoLate.a >= 0.0f)
		{
			// ���S�̃��l������
			colLogoLate.a -= 0.06f;
		}

		// �T�C�Y�̔�剻
		m_fSizeX += ADD_WIDTH;
		m_fSizeY += ADD_HEIGHT;

		// �ݒ菈��
		m_pStart->SetCol(colStart);
		m_pStart->SetVtx();
		m_pLogo->SetCol(colLogo);
		m_pLogo->SetVtx();
		m_pLogoLate->SetSize(m_fSizeX, m_fSizeY);
		m_pLogoLate->SetCol(colLogoLate);
		m_pLogoLate->SetVtx();
	}
}

//=====================================================
// �t�F�[�h�̍X�V����
//=====================================================
void CTitle::UpdateFade(void)
{
	// �X�^�[�g�\���̊Ǘ�
	ManageStart();

	// �t�F�[�h�J�E���g�i�߂�
	m_nFadeCnt++;

	// �v���C���[�̐ݒ菈��
	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (m_apModelPlayer[nCount] != nullptr)
		{
			D3DXVECTOR3 posPlayer = m_apModelPlayer[nCount]->GetPosition();
			m_apModelPlayer[nCount]->SetPosition(posPlayer + PLAYER_ESC_MOVE);
			m_apModelPlayer[nCount]->SetRot(PLAYER_ESC_ROT);
		}
	}

	// �G�̐ݒ菈��
	for (int nCount = 0; nCount < ENEMY::NUM_ENEMY; nCount++)
	{
		if (m_apModelEnemy[nCount] != nullptr)
		{
			D3DXVECTOR3 posEnemy = m_apModelEnemy[nCount]->GetPosition();
			m_apModelEnemy[nCount]->SetPosition(posEnemy + ENEMY_MOVE);
		}
	}

	if (m_nFadeCnt == FADE_COUNT)
	{
		// �t�F�[�h�̎擾
		CFade* pFade = CFade::GetInstance();

		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_SELECT);
		}
	}
}

//=====================================================
// �J�����̍X�V����
//=====================================================
void CTitle::UpdateCamera(void)
{
	CCamera* pCamera = CManager::GetCamera();

	if (m_state == STATE_NONE)
	{
		if (pCamera != nullptr)
		{
			// �^�C�g���̃J�����X�V
			pCamera->UpdateTitle(CAMERA_MOVE[m_cameraInfo.num]);
		}
	}
	else if (m_state == STATE_OUT)
	{
		if (pCamera != nullptr)
		{
			// �^�C�g���̓�����Ƃ��̃J�����X�V
			pCamera->UpdateTitleEsc(CAMERA_ESC_MOVE);
		}
	}

	if (m_cameraInfo.nCount >= CAMERA_SWITCH_COUNT[m_cameraInfo.num])
	{
		m_cameraInfo.nCount = 0;

		// �J�����ԍ���i�߂�
		m_cameraInfo.num = (CAMERA)(m_cameraInfo.num + 1);

		if (m_cameraInfo.num >= CAMERA_MAX)
		{
			// �J�����ԍ�������
			m_cameraInfo.num = (CAMERA)0;
		}

		D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (m_cameraInfo.num >= CAMERA_PLAYER_ONE &&
			m_cameraInfo.num <= CAMERA_PLAYER_FOUR)
		{
			int nPlayerNum = m_cameraInfo.num + ((CAMERA_PLAYER_ONE - 1) - 1);

			if (m_apModelPlayer[nPlayerNum] != nullptr)
			{
				// �v���C���[�̈ʒu���擾
				posPlayer = m_apModelPlayer[nPlayerNum]->GetPosition();	
			}
		}

		if (pCamera != nullptr)
		{
			if (m_cameraInfo.num <= CAMERA_MAX)
			{
				// �J�����̖ړI�l�ݒ�
				pCamera->SetTitleDest(
					CAMERA_POSV[m_cameraInfo.num],
					posPlayer + CAMERA_POSR[m_cameraInfo.num]);
			}
		}
	}
	else
	{
		m_cameraInfo.nCount++;
	}
}

//=====================================================
// �t�F�[�h�ݒ�
//=====================================================
void CTitle::SetFadeIn(void)
{
	// ���擾
	CCamera* pCamera = CManager::GetCamera();
	
	if (m_pLogoLate == nullptr)
	{
		m_pLogoLate = CObject2D::Create(7);

		if (m_pLogoLate != nullptr)
		{
			m_pLogoLate->SetSize(LOGO_WIDTH, LOGO_HEIGHT);
			m_pLogoLate->SetPosition(LOGO_POS);
			int nIdx = CTexture::GetInstance()->Regist(LOGO_PATH);
			m_pLogoLate->SetIdxTexture(nIdx);
			m_pLogoLate->SetVtx();
		}
	}

	if (pCamera != nullptr)
	{
		// �����鎞�̃J�����̖ړI�l�ݒ�
		pCamera->SetTitleDest(
			CAMERA_ESC_POS_V, 
			CAMERA_ESC_POS_R);
	}

	// �t�F�[�h�A�E�g�ɐݒ�
	m_state = STATE_OUT;

	// �v���C���[�̃��[�V�����ݒ菈���i�ړ��j
	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (m_apModelPlayer[nCount] != nullptr)
		{
			m_apModelPlayer[nCount]->SetMotion(TITLE_MOTION_PLAYER_MOVE);
		}
	}

	// �G�̐�������
	for (int nCount = 0; nCount < ENEMY::NUM_ENEMY; nCount++)
	{
		m_apModelEnemy[nCount] = CMotion::Create((char*)ENEMY_BODY_PATH[nCount]);

		if (m_apModelEnemy[nCount] != nullptr)
		{
			m_apModelEnemy[nCount]->SetPosition(ENEMY_POS[nCount]);
			m_apModelEnemy[nCount]->SetRot(ENEMY_ROT);
			m_apModelEnemy[nCount]->SetMotion(1);
		}
	}
}