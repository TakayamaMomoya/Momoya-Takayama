//*****************************************************
//
// �����L���O�̏���[Ranking.cpp]
// Author:����쏟
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************

#include "rank_acting.h"
#include "main.h"

#include <stdio.h>

#include "manager.h"
#include "camera.h"
#include "record.h"

#include "object2D.h"
#include "number.h"

#include "object3D.h"
#include "motion.h"

#include "texture.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputmouse.h"
#include "sound.h"

#include "fade.h"

//*****************************************************
// �}�N����`
//*****************************************************

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	// �t�B�[���h
	const float FIELD_WIDTH = 1000.0f;		// �t�B�[���h�̕�
	const float FIELD_HEIGHT = 500.0f;		// �t�B�[���h�̍���
	const char* FIELD_FILE_NAME = "data\\TEXTURE\\MATERIAL\\WodenWall_00.jpg";	// �t�B�[���h�̃p�X

	// �E�H�[��
	const float WALL_WIDTH = 800.0f;		// �E�H�[���̕�
	const float WALL_HEIGHT = 500.0f;		// �E�H�[���̍���
	const char* WALL_FILE_NAME = "data\\TEXTURE\\MATERIAL\\concrete_00.jpg";	// �E�H�[���̃p�X

	// �J����
	const D3DXVECTOR3 CAMERA_POS_V = D3DXVECTOR3(0.0f, 200.0f, -800.0f);
	const D3DXVECTOR3 CAMERA_POS_R = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	const D3DXVECTOR3 CAMERA_MOVE = D3DXVECTOR3(0.0f, 0.0f, 7.0f);

	// ����
	const char* PLAYER_BODY_PATH[CRankActing::ACTOR_TYPE_MAX] =
	{// ���҂̑̂̃p�X[���Ҕԍ�]
		"data\\MOTION\\motionRanking01.txt",
		"data\\MOTION\\motionRanking02.txt",
		"data\\MOTION\\motionRanking03.txt",
		"data\\MOTION\\motionRanking04.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
	};
	const D3DXVECTOR3 PLAYER_POS[CRankActing::ACTING_TYPE_MAX][CRankActing::ACTOR_TYPE_MAX] =
	{// ���҂̈ʒu[���Ҕԍ�][���Z�ԍ�]

		// �s�i
		D3DXVECTOR3(500.0f, 0.0f, 0.0f),
		D3DXVECTOR3(600.0f, 0.0f, 0.0f),
		D3DXVECTOR3(700.0f, 0.0f, 0.0f),
		D3DXVECTOR3(800.0f, 0.0f, 0.0f),
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�

		// ������
		D3DXVECTOR3(500.0f, 0.0f, 0.0f),
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(600.0f, 0.0f, 0.0f),
		D3DXVECTOR3(650.0f, 0.0f, 30.0f),
		D3DXVECTOR3(675.0f, 0.0f, -30.0f),

		// �ǂ�������
		D3DXVECTOR3(-600.0f, 0.0f, 0.0f),
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(1050.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(-500.0f, 0.0f,  0.0f),

	};
	const D3DXVECTOR3 PLAYER_ROT[CRankActing::ACTING_TYPE_MAX][CRankActing::ACTOR_TYPE_MAX] =
	{// ���҂̌���[���Ҕԍ�][���Z�ԍ�]

		// �s�i
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�

		// ������
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),

		// �ǂ�������
		D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f),

	};
	const D3DXVECTOR3 PLAYER_MOVE[CRankActing::ACTING_TYPE_MAX][CRankActing::ACTOR_TYPE_MAX] =
	{// ���҂̈ړ���[���Ҕԍ�][���Z�ԍ�]

		// �s�i
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�

		// ������
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),

		// �ǂ�������
		D3DXVECTOR3(4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �o�ԂȂ�
		D3DXVECTOR3(4.0f, 0.0f, 0.0f),
	};

	const int CHANGE_COUNT = 300;	// ���Z��ς���܂ł̎���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CRankActing::CRankActing()
{
	ZeroMemory(&m_infoVisualObj, sizeof(m_infoVisualObj));
	m_typeActing = (ACTING_TYPE)0;
	m_nActingCnt = 0;


}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRankActing::~CRankActing()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CRankActing::Init(void)
{
	// �I�u�W�F�N�g�̏���������
	if (InitObj() != S_OK)
	{
		return E_FAIL;
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CRankActing::Uninit(void)
{

}

//=====================================================
// �X�V����
//=====================================================
void CRankActing::Update(void)
{
	// ���Z�̍X�V
	for (int nCount = 0; nCount < ACTOR_TYPE_MAX; nCount++)
	{
		if (m_infoVisualObj.apModelActor[nCount] != nullptr)
		{
			D3DXVECTOR3 playerMove = m_infoVisualObj.apModelActor[nCount]->GetPosition() + PLAYER_MOVE[m_typeActing][nCount];

			m_infoVisualObj.apModelActor[nCount]->SetPosition(playerMove);
		}
	}

	// ���Z�؂�ւ��J�E���g����
	CountActing();
}

//=====================================================
// �`�揈��
//=====================================================
void CRankActing::Draw(void)
{

}

//=====================================================
// ��������
//=====================================================
CRankActing* CRankActing::Create(void)
{
	CRankActing* pRankActing = new CRankActing;

	if (pRankActing != nullptr)
	{
		// �����N�̏�����
		if (pRankActing->Init() != S_OK)
		{
			return nullptr;
		}
	}

	return pRankActing;
}

//=====================================================
// �I�u�W�F�N�g�̏���������
//=====================================================
HRESULT CRankActing::InitObj(void)
{
	// ���擾
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		// �����L���O�̐ݒ�
		pCamera->SetRanking();
	}
	else if (pCamera == nullptr)
	{
		return E_FAIL;
	}

	// �n�ʂ̐���
	m_infoVisualObj.pField = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_infoVisualObj.pField != nullptr)
	{
		m_infoVisualObj.pField->SetSize(FIELD_WIDTH, FIELD_HEIGHT);
		int nIdx = CTexture::GetInstance()->Regist(FIELD_FILE_NAME);
		m_infoVisualObj.pField->SetIdxTexture(nIdx);
	}
	else if (m_infoVisualObj.pField == nullptr)
	{
		return E_FAIL;
	}

	// �n�ʂ̐���
	m_infoVisualObj.pWall = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_infoVisualObj.pWall != nullptr)
	{
		m_infoVisualObj.pWall->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 200.0f));
		m_infoVisualObj.pWall->SetSize(WALL_WIDTH, WALL_HEIGHT);
		m_infoVisualObj.pWall->SetRot(D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f));
		int nIdx = CTexture::GetInstance()->Regist(WALL_FILE_NAME);
		m_infoVisualObj.pWall->SetIdxTexture(nIdx);
	}
	else if (m_infoVisualObj.pWall == nullptr)
	{
		return E_FAIL;
	}

	for (int nCount = 0; nCount < ACTOR_TYPE_MAX; nCount++)
	{
		// ���҂̐���
		m_infoVisualObj.apModelActor[nCount] = CMotion::Create((char*)PLAYER_BODY_PATH[nCount]);

		if (m_infoVisualObj.apModelActor[nCount] != nullptr)
		{
			m_infoVisualObj.apModelActor[nCount]->SetPosition(PLAYER_POS[m_typeActing][nCount]);
			m_infoVisualObj.apModelActor[nCount]->SetRot(PLAYER_ROT[m_typeActing][nCount]);
			m_infoVisualObj.apModelActor[nCount]->SetMotion(1);
		}
		else if (m_infoVisualObj.apModelActor[nCount] == nullptr)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=====================================================
// ���Z�̐ݒ菈��
//=====================================================
void CRankActing::SetObjActing(void)
{
	for (int nCount = 0; nCount < ACTOR_TYPE_MAX; nCount++)
	{
		if (m_infoVisualObj.apModelActor[nCount] != nullptr)
		{
			m_infoVisualObj.apModelActor[nCount]->SetPosition(PLAYER_POS[m_typeActing][nCount]);
			m_infoVisualObj.apModelActor[nCount]->SetRot(PLAYER_ROT[m_typeActing][nCount]);
			m_infoVisualObj.apModelActor[nCount]->SetMotion(1);
		}
	}
}

//=====================================================
// ���Z�̐؂�ւ��J�E���g����
//=====================================================
void CRankActing::CountActing(void)
{
	// ���Z�J�E���g���ݒ�l�ȉ�
	if (m_nActingCnt <= CHANGE_COUNT)
	{
		// �J�E���g�����Z
		m_nActingCnt++;
	}
	else
	{
		// �J�E���g�̏�����
		m_nActingCnt = 0;

		// ���Z�ԍ��𑝂₷
		m_typeActing = (ACTING_TYPE)(m_typeActing + 1);

		// ���Z���ő吔
		if (m_typeActing >= ACTING_TYPE_MAX)
		{
			// ���Z��������
			m_typeActing = (ACTING_TYPE)0;
		}

		// ���Z��ݒ�
		SetObjActing();
	}
}