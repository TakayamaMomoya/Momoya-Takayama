//*****************************************************
//
// �u�[�X�g�̏���[boost.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "boost.h"
#include "texture.h"
#include "fan2D.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR3 GAUGE_POS = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };	// �Q�[�W�̈ʒu
const char* TEXTURE_PATH = "data\\TEXTURE\\UI\\boost00.png";	// �e�N�X�`���p�X
const float RADIUS_GAUGE = 100.0f;	// �Q�[�W�̔��a
const float INITIAL_ROT = D3DX_PI * 0.35f;
const float ANGLE_MAX = D3DX_PI * 0.3f;
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CBoost::CBoost(int nPriority) : CObject(nPriority)
{
	m_pObjectGauge = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBoost::~CBoost()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CBoost::Init(void)
{
	if (m_pObjectGauge == nullptr)
	{// �u�[�X�g�Q�[�W�̐���
		m_pObjectGauge = CFan2D::Create();

		if (m_pObjectGauge != nullptr)
		{
			m_pObjectGauge->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectGauge->SetAngleMax(ANGLE_MAX);
			m_pObjectGauge->SetRotation(INITIAL_ROT);
			m_pObjectGauge->SetRadius(RADIUS_GAUGE);
			m_pObjectGauge->SetVtx();

			int nIdx = CTexture::GetInstance()->Regist(TEXTURE_PATH);
			m_pObjectGauge->SetIdxTexture(nIdx);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBoost::Uninit(void)
{
	if (m_pObjectGauge != nullptr)
	{// �Q�[�W�̔j��
		m_pObjectGauge->Uninit();
		m_pObjectGauge = nullptr;
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CBoost::Update(void)
{
	// �ϐ��錾
	float fBoost = 0.0f;
	float fRate;
	D3DXVECTOR3 pos;

	// �v���C���[�̎擾
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	// �v���C���[���̎擾
	fBoost = pPlayer->GetBoost();
	CPlayer::SParam param = pPlayer->GetParam();

	// �u�[�X�g�̊������Z�o
	fRate = fBoost / param.fInitialBoost;

	if (m_pObjectGauge != nullptr)
	{// �Q�[�W�̐ݒ�
		// �����̐ݒ�
		float fRot = INITIAL_ROT + ANGLE_MAX * (1.0f - fRate);

		m_pObjectGauge->SetRotation(fRot);

		// �T�C�Y�ݒ�
		m_pObjectGauge->SetRateAngle(fRate);
		m_pObjectGauge->SetVtx();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CBoost::Draw(void)
{

}

//=====================================================
// ��������
//=====================================================
CBoost *CBoost::Create(void)
{
	CBoost *pBoost = nullptr;

	if (pBoost == nullptr)
	{
		pBoost = new CBoost;

		if (pBoost != nullptr)
		{
			pBoost->Init();
		}
	}

	return pBoost;
}