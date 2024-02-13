//*****************************************************
//
// �Q�[������[game.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "game.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputManager.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "debugproc.h"
#include <stdio.h>
#include "UIManager.h"
#include "object3D.h"
#include "playerManager.h"
#include "texture.h"
#include "skybox.h"
#include "itemWeapon.h"
#include "itemRepair.h"
#include "weaponManager.h"
#include "enemyManager.h"
#include "rocket.h"
#include "edit.h"
#include "goal.h"
#include "block.h"
#include "gimmickManager.h"
#include "renderer.h"
#include "animEffect3D.h"
#include "pause.h"
#include "box.h"
#include "gameover.h"
#include "enemyThief.h"
#include "ghost.h"
#include "record.h"
#include "containerManager.h"
#include "enemyEvent.h"
#include "repairStatus.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define TRANS_TIME	(60)	// �I���܂ł̗]�C�̃t���[����

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CGame::STATE CGame::m_state = STATE_NONE;	// ���
CGame *CGame::m_pGame = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CGame::CGame()
{
	m_nCntState = 0;
	m_bStop = false;
	m_bGameover = false;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGame::Init(void)
{
	m_pGame = this;

	m_state = STATE_NORMAL;
	m_bStop = false;
	m_bGameover = false;

	// UI�}�l�[�W���[�̒ǉ�
	CUIManager::Create();

	// ���̐���
	CObject3D *pObject = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pObject != nullptr)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\wood001.jpg");
		pObject->SetIdxTexture(nIdx);
		pObject->SetTex(D3DXVECTOR2(10.0f,10.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	// �O�̒n�ʐ���
	CObject3D *pObjectOut = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pObjectOut != nullptr)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\field00.jpg");
		pObjectOut->SetPosition(D3DXVECTOR3(0.0f, -5.0f, 0.0f));
		pObjectOut->SetIdxTexture(nIdx);
		pObjectOut->SetTex(D3DXVECTOR2(10.0f, 10.0f), D3DXVECTOR2(0.0f, 0.0f));
		pObjectOut->SetSize(5000.0f, 5000.0f);
	}

	// �X�J�C�{�b�N�X�̐���
	CSkybox::Create();

	// �u���b�N�̓ǂݍ���
	CBlock::Load("data\\MAP\\map01.bin");

	// �v���C���[�̐���
	CPlayerManager *pPlayerManger = CPlayerManager::GetInstance();

	if (pPlayerManger != nullptr)
	{
		pPlayerManger->CreatePlayer();
	}

	// ����}�l�[�W���[�̐���
	CWeaponManager::Create();

	// �G�}�l�[�W���[�̐���
	CEnemyManager *pEnemyManager = CEnemyManager::Create();

	// ���P�b�g�̐���
	CRocket::Create();

	// �C���󋵂̐���
	CRepairStatus::Create();

	// �M�~�b�N�}�l�[�W���[�̐���
	CGimmickManager::Create();

	// �RD�A�j���[�V�����Ǘ��̐���
	CAnimEffect3D::Create();

	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_BGM_GAME);
	}

	CRecord* pRecord = CRecord::GetInstance();

	// ��т̐���
	if (pRecord == nullptr)
	{
		pRecord = CRecord::Create();
	}
	else if (pRecord != nullptr)
	{
		// ��т̏I�������i���g���C�̋L�^�ێ��h�~�j
		pRecord->Uninit();

		// ��т̐�������
		pRecord = CRecord::Create();
	}

	if (pRecord == nullptr)
	{
		return E_FAIL;
	}

#ifdef _DEBUG
	// �G�f�B�b�g�̐���
	//CEdit::Create();
#endif

	// �t�H�O��������
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
	}

	// �R���e�i�}�l�[�W���[�̐���
	CContainerManager::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGame::Uninit(void)
{
	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();

	// �u���b�N�̔j��
	CBlock::DeleteAll();

	// �Q�[���I�[�o�[�̏I��
	CGameover* pGameover = CGameover::GetInstance();

	if (pGameover != nullptr)
	{
		pGameover->Uninit();
	}

	m_pGame = nullptr;
}

//=====================================================
// �X�V����
//=====================================================
void CGame::Update(void)
{
	CFade *pFade = CFade::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (m_bStop == false)
	{
		// �V�[���̍X�V
		CScene::Update();

		if (pInputManager != nullptr)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
			{
				CPause::Create();
			}
		}
	}
	else
	{
		// ��~���Ȃ��I�u�W�F�N�g�̍X�V
		CObject::UpdateNotStop();

		// �G�f�B�b�g�̍X�V
		CEdit* pEdit = CEdit::GetInstatnce();

		if (pEdit != nullptr)
		{
			pEdit->Update();
		}

		CPause *pPause = CPause::GetInstance();

		if (pPause != nullptr)
		{
			pPause->Update();
		}
	}

	// ��т̎擾
	CRecord* pRecord = CRecord::Create();

	if (pRecord != nullptr)
	{
		pRecord->Update();
	}

	CGameover* pGameover = CGameover::GetInstance();

	if (pGameover != nullptr)
	{
		pGameover->Update();
	}

	// �J�����X�V
	UpdateCamera();

	// ��ԊǗ�
	ManageState();

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{
		int nNumPlayer = pPlayerManager->GetNumPlayer();

		if (nNumPlayer <= 0 && m_bGameover == false)
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				CGameover::Create();
				m_bGameover = true;

				if (pSound != nullptr)
				{
					pSound->Play(pSound->LABEL_SE_GAMEOVER);
				}

				//pFade->SetFade(CScene::MODE_RANKING);
			}
		}
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �J�����̍X�V
//=====================================================
void CGame::UpdateCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	if (m_bStop == false)
	{
		if (m_state == STATE_ESCAPE || m_state == STATE_RESULT)
		{
			pCamera->UpdateResult();
		}
	}
	else
	{
		//// ����
		//pCamera->Control();
	}

	pCamera->MoveDist(0.05f);
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CGame::ManageState(void)
{
	CFade *pFade = CFade::GetInstance();

	switch (m_state)
	{
	case CGame::STATE_NORMAL:
		break;
	case CGame::STATE_ESCAPE:

		m_nCntState++;

		if (m_nCntState >= TRANS_TIME)
		{
			m_nCntState = 0;
			SetState(STATE_RESULT);

			// �S�[���Ń��U���g�̕\��
			CGoal *pGoal = CGoal::GetInstance();

			if (pGoal != nullptr)
			{
				pGoal->SetResult();
			}
		}

		break;
	case CGame::STATE_END:

		m_nCntState++;

		if (m_nCntState >= TRANS_TIME && pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_RANKING);
		}

		break;
	default:
		break;
	}
}

//=====================================================
// �C�x���g�J�����̐ݒ�
//=====================================================
void CGame::SetEventCamera(float fTime, D3DXVECTOR3 posRDest, D3DXVECTOR3 posVDest)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetEventTimer(fTime);
		pCamera->SetPosRDest(posRDest);
		pCamera->SetPosVDest(posVDest);

		EnableStop(true);
	}
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CGame::Debug(void)
{
	// ���͎擾
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_F))
	{
		m_bStop = m_bStop ? false : true;
	}

	if (pKeyboard->GetTrigger(DIK_G))
	{
		CItemRepair *pRepair = CItemRepair::Create();
		pRepair->SetPosition(D3DXVECTOR3(40.0f, 0.0f, 300.0f));
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CGame::Draw(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	char *apString[STATE::STATE_MAX] =
	{
		"NONE",
		"NORMAL",
		"ESCAPE",
		"RESULT",
		"END",
	};

	pDebugProc->Print("\n�Q�[���̏��[%s]\n", apString[m_state]);
}