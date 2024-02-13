//*****************************************************
//
// �l���I������[select.cpp]
// Author:���}���V
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "select.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "manager.h"
#include "debugproc.h"
#include "motion.h"
#include "model.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "renderer.h"
#include "sound.h"
#include "object2D.h"
#include "objectX.h"
#include "billboard.h"
#include "object3D.h"
#include "playerManager.h"
#include "player.h"
#include "sound.h"
#include "debrisSpawner.h"
#include "debris.h"
#include "lift.h"
#include "container.h"
#include "weaponManager.h"
#include "edit.h"
#include "animEffect3D.h"
#include "particle.h"
#include "number.h"
#include "number3D.h"
#include <stdio.h>
#include "number3D.h"
#include "UIManager.h"
#include "number3D.h"
#include "UIManager.h"

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{
	const D3DXVECTOR3 UI_POS({ -150.0f, 30.0f, -470.0f });	// UI�̈ʒu
	const D3DXVECTOR3 UI_SPACE({ 140.0f, 0.0f, 0.0f });	// UI�Ԃ̊Ԋu
	const D3DXVECTOR2 SIZE_FONT(20.0f, 5.0f);	//UI�̑傫��(�Q���FA)
	const float BLINKING_SPEED(0.02f);	//UI�_�ł̑���(�Q���FA)

	const D3DXVECTOR3 LEAF_POS({ -150.0f, 20.0f, -430.0f });	// �|�e�g�̗t�̈ʒu

	const D3DXVECTOR3 SPOWN_POS({ LEAF_POS.x, -80.0f, LEAF_POS.z });	//�v���C���[�o���̍���(���o�����߃}�C�i�X�l)

	const float NOTSEEWALL_POS_Z(-470.0f);	

	const D3DXVECTOR3 CONTAINER_POS({ -150.0, 0.0, -100.0f });	// �R���e�i�̈ʒu
	const D3DXVECTOR3 CONTAINER_SPACE({ 400.0, 0.0, -70.0f });	// �R���e�i�Ԃ̍L��
	const float RESPAWN_TIME(10.0f);	// �R���e�i�����̎���

	const float GO_GAME_POSy(250.0f);	// �J�ڂ��鍂��

	const float POW_JUMP = 25.0f;	// �G���g���[���̃W�����v��
};

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelect::CSelect()
{
	ZeroMemory(&m_apPlayerData[0], sizeof(m_apPlayerData));
	ZeroMemory(&m_aContainerData[0], sizeof(m_aContainerData));
	ZeroMemory(&m_apNumber[0], sizeof(m_apNumber));
	ZeroMemory(&m_aJoinUiData[0], sizeof(m_aJoinUiData));
	m_pStartUI = nullptr;
	m_pLift = nullptr;
	m_pSlash = nullptr;
	m_bLiftCamera = false;
	m_bLiftUp = false;
	m_bSound = false;
	m_selectState = SELECT_STATE::STATE_BEFORE;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSelect::~CSelect()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CSelect::Init(void)
{
	// �J�������̎擾
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetSelect();
	}

	// ������
	JoinUiInit();
	StartInit();
	ContainerInit();

	// UI�}�l�[�W���[�̒ǉ�
	CUIManager::Create();

	// �v���C���[�}�l�[�W���[�̐���
	CPlayerManager::Create();

	// �u���b�N�̓ǂݍ���
	CBlock::Load("data\\MAP\\select_map00.bin");

	// ���̐���
	CObject3D* pObject = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pObject != nullptr)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\wood001.jpg");
		pObject->SetIdxTexture(nIdx);
		pObject->SetTex(D3DXVECTOR2(10.0f, 10.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	// �|�e�g�̐Q���̐���
	CObjectX* pObjectX = CObjectX::Create({ 58.0f, 0.5f, -430.5f });
	if (pObjectX != nullptr)
	{
		pObjectX->BindModel(CModel::Load("data\\MODEL\\select\\potato_bed.x"));
	}

	// �|�e�g�̗t�̏�����
	PotatoLeafInit();

	// �Q���l���̕\��
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_apNumber[nCnt] = CNumber3D::Create(1, 0);

		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->SetPosition({ 15.0f + (nCnt * 30), 20.0f, 140.0f });
			m_apNumber[nCnt]->SetSizeAll(10.0f, 20.0f);
		}
	}

	// �X���b�V���̕\��
	m_pSlash = CBillboard::Create({ m_apNumber[0]->GetPosition().x + 15.0f, m_apNumber[0]->GetPosition().y, m_apNumber[0]->GetPosition().z }, 10.0f, 20.0f);
	if (m_pSlash != nullptr)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\slash.png");
		m_pSlash->SetIdxTexture(nIdx);
	}

	// �J�n�ʒu
	m_pLift = CLift::Create(D3DXVECTOR3(30.0f, 0.5f, 100.0f));
	
	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_BGM_SELECT);
	}

	// �RD�A�j���[�V�����Ǘ��̐���
	CAnimEffect3D::Create();

	// �R���e�i�̃��X�|�[�����Ԃ�ݒ�
	for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
	{
		m_aContainerData[nCnt].fReSpawnTimer = RESPAWN_TIME;
	}

	return S_OK;
}

//=====================================================
// ���j���[�̏���������
//=====================================================
void CSelect::JoinUiInit(void)
{
	char* apPath[NUM_PLAYER] =
	{
		"data\\TEXTURE\\UI\\join.png",
		"data\\TEXTURE\\UI\\join.png",
		"data\\TEXTURE\\UI\\join.png",
		"data\\TEXTURE\\UI\\join.png",
	};

	for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
	{
		// �����̐���
		m_aJoinUiData[nCnt].pUi2D = CBillboard::Create
		(
			{ // �ʒu
				UI_POS.x + (nCnt * UI_SPACE.x),
				UI_POS.y,
				UI_POS.z
			},
			// �T�C�Y
			SIZE_FONT.x, SIZE_FONT.y
		);

		if (m_aJoinUiData[nCnt].pUi2D != nullptr)
		{
			m_aJoinUiData[nCnt].pUi2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_aJoinUiData[nCnt].pUi2D->SetIdxTexture(CTexture::GetInstance()->Regist(apPath[nCnt]));
		}

		m_aJoinUiData[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_aJoinUiData[nCnt].state = FADE_OUT;
	}
}

//=====================================================
// �X�^�[�g�̏���������
//=====================================================
void CSelect::StartInit(void)
{
	m_pStartUI = CObject2D::Create(6);

	if (m_pStartUI != nullptr)
	{
		m_pStartUI->SetPosition(D3DXVECTOR3(1030.0f, 660.0f, 0.0f));
		m_pStartUI->SetSize(150.0f, 50.0f);
		m_pStartUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\start_game.png");
		m_pStartUI->SetIdxTexture(nIdx);

		m_pStartUI->SetVtx();
	}
}

//=====================================================
// �|�e�g�̗t�̏���������
//=====================================================
void CSelect::PotatoLeafInit(void)
{
	for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
	{
		// �����̐���
		m_apPlayerData[nCnt].pLeaf = CObjectX::Create
		(
			{ // �ʒu
				LEAF_POS.x + (nCnt * UI_SPACE.x),
				0.0f,
				LEAF_POS.z
			}
		);

		if (m_apPlayerData[nCnt].pLeaf != nullptr)
		{
			// �I�u�W�F�N�g���f���̐ݒ�
			int nIdx = CModel::Load("data\\MODEL\\select\\potatoLeaf.x");
			m_apPlayerData[nCnt].pLeaf->BindModel(nIdx);
		}
	}
}

//=====================================================
// �R���e�i�̏���������
//=====================================================
void CSelect::ContainerInit(void)
{
	// ����}�l�[�W���̐���
	CWeaponManager::Create();

	for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
	{
		// �R���e�i�̐���
		m_aContainerData[nCnt].pContainer = CContainer::Create();

		// �ʒu�ݒ�
		if (nCnt % 2 == 0)
		{
			m_aContainerData[nCnt].pContainer->SetPosition(D3DXVECTOR3
			(
				CONTAINER_POS.x,
				0.0f, 
				CONTAINER_POS.z + (nCnt * CONTAINER_SPACE.z))
			);
		}
		else
		{
			m_aContainerData[nCnt].pContainer->SetPosition(D3DXVECTOR3
			(
				CONTAINER_POS.x + CONTAINER_SPACE.x,
				0.0f + +CONTAINER_SPACE.y,
				CONTAINER_POS.z + (nCnt * CONTAINER_SPACE.z))
			);
		}
	}
}

//=====================================================
// �I������
//=====================================================
void CSelect::Uninit(void)
{
	// �u���b�N�̔j��
	CBlock::DeleteAll();

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CSelect::Update(void)
{
	// ���擾
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	bool bGameStart = false;
	int nJoinPlayer = 0;

	// �V�[���̍X�V
	CScene::Update();

	if (m_selectState == SELECT_STATE::STATE_BEFORE)
	{
		if (pKeyboard != nullptr && pMouse != nullptr)
		{
			if (CLift::GetIsIn() == true)
			{// �Q�����̑S�����͈͓��ɓ������Ƃ��������������

				for (int i = 0; i < NUM_PLAYER; i++)
				{
					if (m_abEntry[i] == true && pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, i))
					{// ���t�g���オ�鏈��
						m_pSlash->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						m_selectState = STATE_GO;
						m_bLiftUp = true;
					}
				}

				if (pKeyboard->GetTrigger(DIK_RETURN) ||
					pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
				{// �t�F�[�h
					m_pSlash->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_selectState = STATE_GO;
					m_bLiftUp = true;
				}

				bGameStart = true;
			}
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			if (m_apNumber[nCnt] != nullptr)
			{
				m_apNumber[nCnt]->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
			}
		}

		Lift();
	}

	// StartUI��������悤��
	if (m_pStartUI != nullptr)
	{
		if (bGameStart == true)
		{
			m_pStartUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (bGameStart == false)
		{
			m_pStartUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		m_pStartUI->SetVtx();
	}

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++)
	{
		if (m_aJoinUiData[nCntPlayer].pUi2D != nullptr && m_selectState == SELECT_STATE::STATE_BEFORE)
		{
			// �F�̕ύX
			MenuColorChange(nCntPlayer);
			// �Q������
			EntryInput(nCntPlayer);
		}

		if (m_abEntry[nCntPlayer] == true)
		{
			// �v���C���[�Q��̏���
			PlayerShowUp(nCntPlayer);

			// �s������
			MoveLimit(nCntPlayer);

			nJoinPlayer++;
		}
	}

	// �Q���l���̐ݒ�
	CLift::SetjoinPlayer(nJoinPlayer);

	// ���t�g���l����UI
	LiftInNumberUi(nJoinPlayer);
	
	// �R���e�i�̍Đݒu
	ReSetContainer();

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\n�Q���l��[%d]\n", nJoinPlayer);
#endif
}

//=====================================================
// �Đݒ�
//=====================================================
void CSelect::ReSetContainer(void)
{
	for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
	{
		if (m_aContainerData[nCnt].pContainer != nullptr)
		{
			if (m_aContainerData[nCnt].pContainer->GetState() == CContainer::STATE_END)
			{
				m_aContainerData[nCnt].pContainer = nullptr;
				m_aContainerData[nCnt].fReSpawnTimer = RESPAWN_TIME;
			}
		}

		if (m_aContainerData[nCnt].pContainer == nullptr)
		{
			// �o�ߎ��Ԏ擾
			float fTick = CManager::GetTick();

			m_aContainerData[nCnt].fReSpawnTimer -= fTick;

			if (m_aContainerData[nCnt].fReSpawnTimer > 0)
			{
				continue;
			}
			
			// �R���e�i�̐���
			m_aContainerData[nCnt].pContainer = CContainer::Create();

			if (m_aContainerData[nCnt].pContainer == nullptr)
			{
				continue;
			}

			// �ʒu�ݒ�
			if (nCnt % 2 == 0)
			{
				m_aContainerData[nCnt].pContainer->SetPosition(D3DXVECTOR3
				(
					CONTAINER_POS.x,
					0.0f,
					CONTAINER_POS.z + (nCnt * CONTAINER_SPACE.z))
				);
			}
			else
			{
				m_aContainerData[nCnt].pContainer->SetPosition(D3DXVECTOR3
				(
					CONTAINER_POS.x + CONTAINER_SPACE.x,
					0.0f + +CONTAINER_SPACE.y,
					CONTAINER_POS.z + (nCnt * CONTAINER_SPACE.z))
				);
			}
		}
	}
}

//=====================================================
// UI�_��
//=====================================================
void CSelect::MenuColorChange(int nPlayer)
{
	if (m_aJoinUiData[nPlayer].state == FADE_IN)
	{//�t�F�[�h�C�����
		m_aJoinUiData[nPlayer].col.a -= BLINKING_SPEED;			//�|���S���𓧖��ɂ��Ă���

		if (m_aJoinUiData[nPlayer].col.a <= 0.0f)
		{
			m_aJoinUiData[nPlayer].col.a = 0.0f;

			m_aJoinUiData[nPlayer].state = FADE_OUT;
		}
	}
	else if (m_aJoinUiData[nPlayer].state == FADE_OUT)
	{//�t�F�[�h�A�E�g���
		m_aJoinUiData[nPlayer].col.a += BLINKING_SPEED;			//�|���S����s�����ɂ��Ă���

		if (m_aJoinUiData[nPlayer].col.a >= 1.0f)
		{
			m_aJoinUiData[nPlayer].col.a = 1.0f;

			m_aJoinUiData[nPlayer].state = FADE_IN;
		}
	}

	// �F�̐ݒ�
	m_aJoinUiData[nPlayer].pUi2D->SetColor(m_aJoinUiData[nPlayer].col);
}

//=====================================================
// �Q������
//=====================================================
void CSelect::EntryInput(int nPlayer)
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, nPlayer))
	{
		if (m_abEntry[nPlayer] == true || m_apPlayerData[nPlayer].pPlayer != nullptr || pPlayerManager == nullptr)
		{
			return;
		}

		// �T�E���h�C���X�^���X�̎擾
		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_APPEARE);
		}

		// �Q����Ԃ�
		m_abEntry[nPlayer] = true;
		m_apPlayerData[nPlayer].state = PLAYER_ENTRY;

		// �v���C���[�𐶐�
		m_apPlayerData[nPlayer].pPlayer = pPlayerManager->BindPlayer(nPlayer);

		// �ʒu��UI�̏ꏊ��
		m_apPlayerData[nPlayer].pPlayer->SetPosition(D3DXVECTOR3(
			SPOWN_POS.x + (nPlayer * UI_SPACE.x),
			SPOWN_POS.y,
			SPOWN_POS.z));

		// �W�����v������
		D3DXVECTOR3 move = { 0.0f,POW_JUMP,0.0f };
		m_apPlayerData[nPlayer].pPlayer->SetMove(move);

		// �y�̐���
		CDebrisSpawner::Create(D3DXVECTOR3(
			m_aJoinUiData[nPlayer].pUi2D->GetPosition().x,
			50.0f,
			m_aJoinUiData[nPlayer].pUi2D->GetPosition().z), CDebrisSpawner::TYPE::TYPE_SOIL, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// UI�폜
		ObjDelete(nPlayer);
	}

#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_F4))
	{
		if (m_abEntry[nPlayer] == true || m_apPlayerData[nPlayer].pPlayer != nullptr || pPlayerManager == nullptr)
		{
			return;
		}

		// �T�E���h�C���X�^���X�̎擾
		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_APPEARE);
		}

		// �Q����Ԃ�
		m_abEntry[nPlayer] = true;
		m_apPlayerData[nPlayer].state = PLAYER_ENTRY;

		// �v���C���[�𐶐�
		m_apPlayerData[nPlayer].pPlayer = pPlayerManager->BindPlayer(nPlayer);

		// �ʒu��UI�̏ꏊ��
		m_apPlayerData[nPlayer].pPlayer->SetPosition(D3DXVECTOR3(
			SPOWN_POS.x + (nPlayer * UI_SPACE.x),
			SPOWN_POS.y,
			SPOWN_POS.z));

		// �W�����v������
		D3DXVECTOR3 move = { 0.0f,POW_JUMP,0.0f };
		m_apPlayerData[nPlayer].pPlayer->SetMove(move);

		CDebrisSpawner::Create(D3DXVECTOR3(
			m_aJoinUiData[nPlayer].pUi2D->GetPosition().x,
			50.0f,
			m_aJoinUiData[nPlayer].pUi2D->GetPosition().z), CDebrisSpawner::TYPE::TYPE_SOIL, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// UI�폜
		ObjDelete(nPlayer);
	}
#endif
}

//=====================================================
// �s������
//=====================================================
void CSelect::MoveLimit(int nPlayer)
{
	if (m_apPlayerData[nPlayer].pPlayer == nullptr)
	{
		return;
	}

	// ���̎擾
	D3DXVECTOR3 pos = m_apPlayerData[nPlayer].pPlayer->GetPosition();
	D3DXVECTOR3 move = m_apPlayerData[nPlayer].pPlayer->GetMove();
	CCamera* pCamera = CManager::GetCamera();

	// ��l�̕ǔ���
	if (m_selectState == STATE_BEFORE)
	{
		if (pos.z < NOTSEEWALL_POS_Z)
		{
			pos.z = NOTSEEWALL_POS_Z;
		}

		if (pos.z > -24.0f)
		{
			if (pos.x <= -65.0f || pos.x >= 125.0f)
			{
				pos.z = -24.0f;
			}
			else
			{// ���t�g�ɏ���Ă���
				if (pos.x < -59.0f)
				{
					pos.x = -59.0f;
				}
				else if (pos.x > 119.0f)
				{
					pos.x = 119.0f;
				}
			}
		}

		if (pos.z > 185.0f)
		{// ���t�g��
			pos.z = 185.0f;
		}

		if (pos.x < -215.0f)
		{// ��ʍ�
			pos.x = -215.0f;
		}
		else if (pos.x > 329.0f)
		{// ��ʉE
			pos.x = 329.0f;
		}
	}
	else
	{
		if (pos.z < 20.0f)
		{
			pos.z = 20.0f;
		}
	}

	// ���̔��f
	m_apPlayerData[nPlayer].pPlayer->SetPosition(pos);
}

//=====================================================
// �v���C���[�̓o��
//=====================================================
void CSelect::PlayerShowUp(int nPlayer)
{
	if (m_apPlayerData[nPlayer].pPlayer == nullptr)
	{
		return;
	}

	if (m_apPlayerData[nPlayer].state != PLAYER_ENTRY)
	{
		return;
	}

	// ���̎擾
	D3DXVECTOR3 pos = m_apPlayerData[nPlayer].pPlayer->GetPosition();
	D3DXVECTOR3 move = m_apPlayerData[nPlayer].pPlayer->GetMove();

	if (pos.y >= 100.0f)
	{
		m_apPlayerData[nPlayer].state = PLAYER_FREE;
	}
}

//=====================================================
// ���t�g
//=====================================================
void CSelect::Lift(void)
{
	if (m_bLiftUp == true)
	{
		CCamera* pCamera = CManager::GetCamera();

		D3DXVECTOR3 lift = m_pLift->GetPosition();

		// ��Ԃ̕ύX
		m_pLift->SetState(m_pLift->STATE_UP);

		if (m_bLiftCamera == false)
		{
			pCamera->SetLift();
			m_bLiftCamera = true;
		}
		else
		{
			// �㏸�J�����A���O��
			pCamera->SetUpLift();
		}

		for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
		{
			if (m_abEntry[nCnt] == true)
			{// ���t�g���オ�鏈��
				// �T�E���h�̍Đ�
				if (m_bSound == false)
				{
					CSound* pSound = CSound::GetInstance();

					if (pSound != nullptr)
					{
						pSound->Play(pSound->LABEL_SE_RIFT);
					}
					m_bSound = true;
				}
			}
		}

		CFade* pFade = CFade::GetInstance();

		if (lift.y > GO_GAME_POSy)
		{// ���t�g�����̍����ɍs���ƃQ�[����

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_GAME);
			}
		}
	}
}

//=====================================================
// ���t�g���ɓ������l����UI
//=====================================================
void CSelect::LiftInNumberUi(int nPlayer)
{
	if (m_apNumber[0] != nullptr)
	{
		m_apNumber[0]->SetValue(CLift::GetInPlayer(), 1);
	}

	if (m_apNumber[1] != nullptr)
	{
		m_apNumber[1]->SetValue(nPlayer, 1);
	}
}

//=====================================================
// �I�u�W�F�N�g����������
//=====================================================
void CSelect::ObjDelete(int nPlayer)
{
	if (m_aJoinUiData[nPlayer].pUi2D != nullptr)
	{
		m_aJoinUiData[nPlayer].pUi2D->Uninit();
		m_aJoinUiData[nPlayer].pUi2D = nullptr;
	}
	if (m_apPlayerData[nPlayer].pLeaf != nullptr)
	{
		m_apPlayerData[nPlayer].pLeaf->Uninit();
		m_apPlayerData[nPlayer].pLeaf = nullptr;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CSelect::Draw(void)
{

}