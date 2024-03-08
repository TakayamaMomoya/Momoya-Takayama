//*****************************************************
//
// �`�F�b�N�|�C���g��Behavior[checkpointBehavior.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "checkPointBehavior.h"
#include "checkPointManager.h"
#include "UI.h"
#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "enemyManager.h"
#include "limit.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SIZE_CURSOR = 20.0f;	// �J�[�\���T�C�Y
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\checkPoint00.png";	// �J�[�\���̃e�N�X�`��
const float DIST_PROGRESS = 1000.0f;	// �i�s���鋗��
const float LIMIT_DIST = 200.0f;	// ��ʒ��S���痣��鐧������
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CCheckPointBehavior::CCheckPointBehavior()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CCheckPointBehavior::~CCheckPointBehavior()
{

}

//=====================================================
// �ړ��p�[�g
//=====================================================
CCheckPointMove::CCheckPointMove()
{// �R���X�g���N�^
	m_pCursor = nullptr;
}

CCheckPointMove::~CCheckPointMove()
{// �f�X�g���N�^

}

void CCheckPointMove::Init(CCheckPointManager *pCheckPoint)
{// ������
	if (m_pCursor == nullptr)
	{// �J�[�\������
		m_pCursor = CUI::Create();

		if (m_pCursor != nullptr)
		{
			m_pCursor->SetSize(SIZE_CURSOR * 3, SIZE_CURSOR * 3);

			m_pCursor->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
			m_pCursor->SetIdxTexture(nIdx);
			m_pCursor->SetVtx();
		}
	}
}

void CCheckPointMove::Uninit(CCheckPointManager *pCheckPoint)
{// �I��
	Object::DeleteObject((CObject**)&m_pCursor);
}

void CCheckPointMove::Update(CCheckPointManager *pCheckPoint)
{// �X�V
	int nProgress = pCheckPoint->GetProgress();

	D3DXVECTOR3 posNext = pCheckPoint->GetCheckPosition(nProgress + 1);

	// �J�[�\���̕\��
	if (m_pCursor != nullptr)
	{
		// �傫���̕ύX
		float fHeight = m_pCursor->GetHeight();
		float fWidth = m_pCursor->GetWidth();

		fWidth += (SIZE_CURSOR - fWidth) * 0.1f;
		fHeight += (SIZE_CURSOR - fHeight) * 0.1f;

		m_pCursor->SetSize(fWidth, fHeight);

		D3DXMATRIX mtx;
		D3DXVECTOR3 posScreen;

		bool bInScreen = universal::IsInScreen(posNext, mtx, &posScreen);

		D3DXVECTOR3 vecDiff = SCRN_MID - posScreen;
		float fDiff = D3DXVec3Length(&vecDiff);

		if (!bInScreen || fDiff > LIMIT_DIST || posScreen.z >= 1)
		{
			D3DXVec3Normalize(&vecDiff, &vecDiff);

			vecDiff *= LIMIT_DIST;

			if (posScreen.z >= 1)
			{
				posScreen = SCRN_MID + vecDiff;
			}
			else
			{
				posScreen = SCRN_MID - vecDiff;
			}
		}

		m_pCursor->SetPosition(posScreen);
		m_pCursor->SetVtx();
	}

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// ��������
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		if (posPlayer.x >= posNext.x)
		{// �i�s�󋵂̉��Z�A�퓬�ֈڍs
			pCheckPoint->ChangeBehavior(new CCheckPointBattle);
		}
	}
}

//=====================================================
// �퓬���
//=====================================================
CCheckPointBattle::CCheckPointBattle()
{// �R���X�g���N�^
	ZeroMemory(&m_apLimit[0], sizeof(m_apLimit));
}

CCheckPointBattle::~CCheckPointBattle()
{// �f�X�g���N�^

}

void CCheckPointBattle::Init(CCheckPointManager *pCheckPoint)
{// ������
	int nProgress = pCheckPoint->GetProgress();

	D3DXVECTOR3 pos = pCheckPoint->GetCheckPosition(nProgress + 1);

	D3DXVECTOR3 aPos[CheckPointBehavior::NUM_LIMIT] =
	{
		{6000.0f,0.0f,0.0f},
		{-6000.0f,0.0f,0.0f},
	};
	D3DXVECTOR3 aRot[CheckPointBehavior::NUM_LIMIT] =
	{
		{0.0f,D3DX_PI * 0.5f,0.0f},
		{0.0f,-D3DX_PI * 0.5f,0.0f},
	};

	for (int i = 0; i < CheckPointBehavior::NUM_LIMIT; i++)
	{
		m_apLimit[i] = CLimit::Create();

		if (m_apLimit[i] != nullptr)
		{
			m_apLimit[i]->SetPosition(pos + aPos[i]);
			m_apLimit[i]->SetRotation(aRot[i]);
		}
	}
}

void CCheckPointBattle::Uninit(CCheckPointManager *pCheckPoint)
{// �I��
	Object::DeleteObject((CObject**)&m_apLimit[0], CheckPointBehavior::NUM_LIMIT);
}

void CCheckPointBattle::Update(CCheckPointManager *pCheckPoint)
{// �X�V
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();
	int nNumEnemy = CEnemy::GetNumAll();
	int nProgress = pCheckPoint->GetProgress();

	if (pEnemyManager != nullptr)
	{
		bool bEndSpawn = pEnemyManager->IsEndSpawn();

		pEnemyManager->SpawnGroup(nProgress + 1);

		if (nNumEnemy == 0 && bEndSpawn)
		{
			pEnemyManager->EnableEndSpawn(false);
			pCheckPoint->AddProgress();
			pCheckPoint->ChangeBehavior(new CCheckPointMove);
		}
	}
}