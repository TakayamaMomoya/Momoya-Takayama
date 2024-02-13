//*****************************************************
//
// ���t�g����[lift.cpp]
// Author:���}���V
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "lift.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "playerManager.h"
#include "player.h"
#include "fade.h"
#include "object2D.h"

#include "debugproc.h"

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{
	const float UP = 2.0f;
	const float RANGE(100.0f);
}

//=====================================================
//	�ÓI�����o�ϐ��錾
//=====================================================
int CLift::m_nNumJoinPlayer = 0;
int CLift::m_nInCnt = 0;
bool CLift::m_abJoin[NUM_PLAYER] = {};

//=====================================================
// �R���X�g���N�^
//=====================================================
CLift::CLift(int nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_nInCnt = 0;
	m_state = STATE_NONE;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CLift::~CLift()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CLift::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	int nIdx = CModel::Load("data\\MODEL\\select\\lift.x");
	// ���f���Ǎ�
	BindModel(nIdx);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CLift::Uninit(void)
{
	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CLift::Update(void)
{
	// �p���N���X�̍X�V
	CObjectX::Update();

	// ���擾
	CFade* pFade = CFade::GetInstance();
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CPlayer* pPlayer = nullptr;

	// �ϐ��錾
	int nInPlayer = 0;	// �͈͓��v���C���[�J�E���g�p

	// ���t�g�ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	if (m_state == STATE_UP)
	{
		pos.y += UP;
	}

	SetPosition(pos);

	for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
	{
		// �v���C���[�}�l�[�W����������擾
		pPlayer = pPlayerManager->GetPlayer(nCnt);

		if (pPlayer == nullptr)
		{
			continue;
		}

		// ���t�g�͈͂̐ݒ�
		if (GetPosition().x + RANGE >= pPlayer->GetPosition().x &&
			GetPosition().x - RANGE <= pPlayer->GetPosition().x &&
			GetPosition().z + RANGE >= pPlayer->GetPosition().z &&
			GetPosition().z - RANGE <= pPlayer->GetPosition().z)
		{
			// �v���C���[�ʒu�̎擾
			D3DXVECTOR3 playerPos = pPlayer->GetPosition();
			D3DXVECTOR3 playerRot = pPlayer->GetRot();

			// �ړ��ʂ̐ݒ�
			D3DXVECTOR3 movePlayer = pPlayer->GetMove();
			movePlayer.y = 0.0f;
			pPlayer->SetMove(movePlayer);

			// �ʒu�̐ݒ�
			pPlayer->SetPosition(D3DXVECTOR3(playerPos.x, pos.y, playerPos.z));
			pPlayer->SetRot(D3DXVECTOR3(playerRot.x, playerRot.y, playerRot.z));

			// �Q��
			m_abJoin[nCnt] = true;	

			nInPlayer++;	// �͈͓��v���C���[�̃J�E���g�A�b�v
		}
		else
		{
			// �s�Q��
			m_abJoin[nCnt] = false;
		}
	}

	// �͈͓��v���C���[�̃J�E���g
	m_nInCnt = nInPlayer;

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\n�J�n�ꏊ\n");
	CDebugProc::GetInstance()->Print("�͈͓��v���C���[[%d]\n", nInPlayer);
#endif
}

//=====================================================
// �`�揈��
//=====================================================
void CLift::Draw(void)
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}

//=====================================================
// ��������
//=====================================================
CLift* CLift::Create(D3DXVECTOR3 pos,int nPriority)
{
	CLift* pLift = nullptr;

	if (pLift == nullptr)
	{// �C���X�^���X����
		pLift = new CLift(nPriority);

		if (pLift != nullptr)
		{
			pLift->SetPosition(pos);

			// ����������
			pLift->Init();
		}
	}

	return pLift;
}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CLift::Load(void)
{
	return S_OK;
}

//=====================================================
// �͈͓��ɂ���v���C���[�̃J�E���g
//=====================================================
int CLift::GetInPlayer(void)
{
	return m_nInCnt;
}

//=====================================================
// �͈͓��ɓ������l���̔���
//=====================================================
bool CLift::GetIsIn(void)
{
	if (m_nNumJoinPlayer <= 0 && m_nInCnt <= 0)
	{
		return false;
	}
	else if (m_nNumJoinPlayer == m_nInCnt)
	{
		return true;
	}

	return false;
}
