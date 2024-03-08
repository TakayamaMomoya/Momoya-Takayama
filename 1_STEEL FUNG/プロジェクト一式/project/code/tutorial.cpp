//*****************************************************
//
// �`���[�g���A������[tutorial.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "tutorial.h"
#include "object.h"
#include "player.h"
#include "slow.h"
#include "manager.h"
#include "camera.h"
#include "cameraBehavior.h"
#include "UIManager.h"
#include "UI.h"
#include "enemyManager.h"
#include "meshfield.h"
#include "limit.h"
#include "object3D.h"
#include "renderer.h"
#include "texture.h"
#include "animEffect3D.h"
#include "guideInput.h"
#include "inputManager.h"
#include "fade.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_SPAWN = 3.0f;	// �X�|�[���܂ł̊Ԋu
const D3DXVECTOR3 POS_SKIP = { SCREEN_WIDTH * 0.9f,SCREEN_HEIGHT * 0.85f, 0.0f };	// �X�L�b�v�\���̈ʒu
const D3DXVECTOR3 POS_GAUGE = { POS_SKIP.x + 2.0f,POS_SKIP.y + 28.0f, 0.0f };	// �X�L�b�v�\���̈ʒu
const D3DXVECTOR2 SIZE_SKIP = { 80.0f,60.0f };	// �X�L�b�v�\���̃T�C�Y
const D3DXVECTOR2 SIZE_GAUGE = { 70.0f,26.0f };	// �X�L�b�v�Q�[�W�̃T�C�Y
const char* PATH_SKIP = "data\\TEXTURE\\UI\\tutorial06.png";	// �X�L�b�v�\���̃p�X
const float TIME_SKIP = 2.0f;	// �X�L�b�v�ɂ����鎞��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorial::CTutorial()
{
	m_fSpawnCnt = 0.0f;
	m_fCntSkip = 0.0f;
	m_pSkipGauge = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTutorial::~CTutorial()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CTutorial::Init(void)
{
	CAnimEffect3D::Create();

	CSlow::Create();

	CPlayer::Create();

	CUIManager::Create();

	CGuideInput::Create();

	CEnemyManager::Create();

	Camera::ChangeBehavior(new CFollowPlayer);

	CMeshField *pMesh = CMeshField::Create();

	if (pMesh != nullptr)
	{
		pMesh->Reset();
		int nIdx = Texture::GetIdx("data\\TEXTURE\\MATERIAL\\concrete.jpg");
		pMesh->SetIdxTexture(nIdx);
	}

	D3DXVECTOR3 aPos[4] =
	{
		{0.0f,0.0f,10000.0f},
		{15000.0f,0.0f,0.0f},
		{0.0f,0.0f,-10000.0f},
		{-3000.0f,0.0f,0.0f},
	};
	D3DXVECTOR3 aRot[4] =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,D3DX_PI * 0.5f,0.0f},
		{0.0f,D3DX_PI,0.0f},
		{0.0f,-D3DX_PI * 0.5f,0.0f},
	};

	for (int i = 0; i < 4; i++)
	{
		CLimit *pLimit = CLimit::Create();

		if (pLimit != nullptr)
		{
			pLimit->SetPosition(aPos[i]);
			pLimit->SetRotation(aRot[i]);

			CObject3D *pWall = CObject3D::Create(aPos[i]);

			if (pWall != nullptr)
			{
				aRot[i].x = -D3DX_PI * 0.5f;
				pWall->SetPosition(aPos[i] * 1.001f);
				pWall->SetRotation(aRot[i]);
				pWall->SetSize(30000.0f, 30000.0f);
				pWall->SetVtx();

				D3DXVECTOR2 lu = { 0.0f,0.0f };
				D3DXVECTOR2 rd = { 20.0f,20.0f };
				pWall->SetTex(rd, lu);

				int nIdx = Texture::GetIdx("data\\TEXTURE\\MATERIAL\\metal000.jpg");
				pWall->SetIdxTexture(nIdx);
			}
		}
	}

	// �t�H�O��������
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(false);
	}

	// �����G�X�|�[��
	SpawnEnemy();

	// �X�L�b�v�̃Q�[�W
	if (m_pSkipGauge == nullptr)
	{
		m_pSkipGauge = CUI::Create();

		if (m_pSkipGauge != nullptr)
		{
			D3DXVECTOR3 pos = POS_GAUGE;

			m_pSkipGauge->SetSize(SIZE_GAUGE.x, SIZE_GAUGE.y);
			m_pSkipGauge->SetPosition(pos);
			m_pSkipGauge->SetVtx();
		}
	}

	// �X�L�b�v�̃L���v�V����
	CUI *pCaption = CUI::Create();

	if (pCaption != nullptr)
	{
		D3DXVECTOR3 pos = POS_SKIP;

		pCaption->SetSize(SIZE_SKIP.x, SIZE_SKIP.y);
		pCaption->SetPosition(pos);
		pCaption->SetVtx();

		int nIdx = Texture::GetIdx(PATH_SKIP);
		pCaption->SetIdxTexture(nIdx);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorial::Uninit(void)
{
	if (m_pSkipGauge != nullptr)
	{
		m_pSkipGauge->Uninit();
		m_pSkipGauge = nullptr;
	}

	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorial::Update(void)
{
	// �V�[���̍X�V
	CScene::Update();

	// �J�����X�V
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->Update();
		pCamera->Quake();

		pCamera->MoveDist(0.3f);
	}

	// �G�̃X�|�[��
	int nNumEnemy = CEnemy::GetNumAll();

	if (nNumEnemy == 0)
	{// �S�ł����莞�ԂŃX�|�[��
		float fDeltaTime = CManager::GetDeltaTime();

		m_fSpawnCnt += fDeltaTime;

		if (m_fSpawnCnt >= TIME_SPAWN)
		{
			SpawnEnemy();

			m_fSpawnCnt = 0.0f;
		}
	}

	// �J�[�\���̌Œ�
	SetCursorPos((int)SCRN_MID.x, (int)SCRN_MID.y);

	// �X�L�b�v�̓���
	InputSkip();
}

//=====================================================
// �G�̃X�|�[��
//=====================================================
void CTutorial::SpawnEnemy(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
		return;

	D3DXVECTOR3 aPosEnemy[] =
	{
		// ���e���K
		{ 3000.0f, 1000.0f, 0.0f },
		{ 6500.0f, 1000.0f, 500.0f },
		{ 6500.0f, 1000.0f, 0.0f },
		{ 6500.0f, 1000.0f, -500.0f },
		{ 6500.0f, 1500.0f, -250.0f },
		{ 6500.0f, 1500.0f, 250.0f },

		// �ʏ�G
		{ 10000.0f, 1000.0f, 4000.0f },
		{ 10000.0f, 1000.0f, 3000.0f },
		{ 10000.0f, 1000.0f, 2000.0f },
		{ 10000.0f, 1000.0f, 1000.0f },
		{ 10000.0f, 1000.0f, 0.0f },
		{ 10000.0f, 1000.0f, -4000.0f },
		{ 10000.0f, 1000.0f, -3000.0f },
		{ 10000.0f, 1000.0f, -2000.0f },
		{ 10000.0f, 1000.0f, -1000.0f },
	};

	for (int i = 0; i < sizeof(aPosEnemy) / sizeof(D3DXVECTOR3); i++)
	{
		CEnemy::TYPE type = CEnemy::TYPE::TYPE_TUTORIAL;

		if (i % 10 == 0)
		{
			type = CEnemy::TYPE::TYPE_BOMB;
		}

		CEnemy *pEnemy = pEnemyManager->CreateEnemy(aPosEnemy[i], type);

		if (pEnemy != nullptr)
		{
			pEnemy->SetPosDest(aPosEnemy[i]);
		}
	}
}

//=====================================================
// �X�L�b�v�̓���
//=====================================================
void CTutorial::InputSkip(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetPress(CInputManager::BUTTON_SKIP))
	{
		float fDeltaTime = CManager::GetDeltaTime();

		m_fCntSkip += fDeltaTime;

		if (m_fCntSkip >= TIME_SKIP)
		{// ��莞�Ԓ������Ń^�C�g���ɑJ��
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}
	}
	else
	{
		if (m_fCntSkip < TIME_SKIP)
		{
			m_fCntSkip = 0;
		}
	}

	if (m_pSkipGauge != nullptr)
	{
		float fRate = m_fCntSkip / TIME_SKIP;

		universal::LimitValue(&fRate, 1.0f, 0.0f);

		float fWidthDest = SIZE_GAUGE.x * fRate;
		float fWidth = m_pSkipGauge->GetWidth();

		D3DXVECTOR3 pos = POS_GAUGE;

		fWidth += (fWidthDest - fWidth) * 0.2f;

		pos.x += fWidth - SIZE_GAUGE.x;

		m_pSkipGauge->SetPosition(pos);
		m_pSkipGauge->SetSize(fWidth, SIZE_GAUGE.y);
		m_pSkipGauge->SetVtx();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CTutorial::Draw(void)
{

}