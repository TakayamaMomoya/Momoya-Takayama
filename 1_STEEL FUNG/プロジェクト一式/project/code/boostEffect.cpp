//*****************************************************
//
// �r�[���u���[�h�̏���[BoostEffect.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "boostEffect.h"
#include "meshcylinder.h"
#include "object3D.h"
#include "texture.h"
#include "anim3D.h"
#include "animEffect3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int MESH_V = 6;	// ���b�V���̏c�̕�����
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CBoostEffect::CBoostEffect(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBoostEffect::~CBoostEffect()
{

}

//=====================================================
// ��������
//=====================================================
CBoostEffect *CBoostEffect::Create(void)
{
	CBoostEffect *pBoostEffect = nullptr;

	if (pBoostEffect == nullptr)
	{
		pBoostEffect = new CBoostEffect;

		if (pBoostEffect != nullptr)
		{
			// ������
			pBoostEffect->Init();
		}
	}

	return pBoostEffect;
}

//=====================================================
// ����������
//=====================================================
HRESULT CBoostEffect::Init(void)
{
	m_info.fRadius = 100.0f;
	m_info.fHeight = 400.0f;
	m_info.col = { 1.0f,1.0f,1.0f,1.0f };

	if (m_info.pBoost == nullptr)
	{
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if(pAnim3D != nullptr)
			m_info.pBoost = pAnim3D->CreateEffect(D3DXVECTOR3(0.0f,0.0f,0.0f),CAnimEffect3D::TYPE::TYPE_BOOST);

		if (m_info.pBoost != nullptr)
		{
			m_info.pBoost->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
			m_info.pBoost->SetRotation(D3DXVECTOR3(-1.57f, 0.0f, 0.0f));
			m_info.pBoost->SetSize(m_info.fRadius, m_info.fHeight);
			m_info.pBoost->SetPosition(D3DXVECTOR3(0.0f, m_info.fHeight, 0.0f));
			m_info.pBoost->EnableAdd(true);
			m_info.pBoost->EnableZtest(false);
			m_info.pBoost->SetAlphaTest(100);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBoostEffect::Uninit(void)
{
	if (m_info.pBoost != nullptr)
	{
		m_info.pBoost->Uninit();
		m_info.pBoost = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CBoostEffect::Update(void)
{
	// �����ڂ̒Ǐ]
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	if (m_info.pBoost != nullptr)
	{
		pos +=
		{
			sinf(rot.x) * sinf(rot.y) * m_info.fHeight,
				cosf(rot.x) * m_info.fHeight,
				sinf(rot.x) * cosf(rot.y) * m_info.fHeight
		};

		rot.x -= 1.57f;

		m_info.pBoost->SetPosition(pos);
		m_info.pBoost->SetRotation(rot);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CBoostEffect::Draw(void)
{

}

// ���a�ݒ�
void CBoostEffect::SetRadius(float fRadius)
{
	m_info.fRadius = fRadius;

	if (m_info.pBoost != nullptr)
	{
		float fHeight = m_info.pBoost->GetHeight();

		m_info.pBoost->SetSize(fRadius, fHeight);
		m_info.pBoost->SetVtx();
	}
}

// �����ݒ�
void CBoostEffect::SetHeight(float fHeight)
{
	m_info.fHeight = fHeight;

	if (m_info.pBoost != nullptr)
	{
		float fWidth = m_info.pBoost->GetWidth();

		m_info.pBoost->SetSize(fWidth, fHeight);
	}
}

// �F�ݒ�
void CBoostEffect::SetColor(D3DXCOLOR col)
{
	m_info.col = col;
	
	if (m_info.pBoost != nullptr)
	{
		m_info.pBoost->SetColor(col);
	}
}

// �ʒu�ݒ�
void CBoostEffect::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;

	D3DXVECTOR3 rot = GetRotation();

	if (m_info.pBoost != nullptr)
	{
		pos +=
		{
			sinf(rot.x) * sinf(rot.y) * m_info.fHeight,
				cosf(rot.x) * m_info.fHeight,
				sinf(rot.x) * cosf(rot.y) * m_info.fHeight
		};

		rot.x -= 1.57f;

		m_info.pBoost->SetPosition(pos);
		m_info.pBoost->SetRotation(rot);
	}
}

// Z�e�X�g�����邩�ǂ���
void CBoostEffect::EnableZtestBoost(bool bZtest)
{
	if (m_info.pBoost != nullptr)
	{
		m_info.pBoost->EnableZtest(bZtest);
	}
}