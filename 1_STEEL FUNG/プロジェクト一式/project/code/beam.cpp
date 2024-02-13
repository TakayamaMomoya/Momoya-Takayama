//*****************************************************
//
// �r�[���̏���[beam.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "beam.h"
#include "anim3D.h"
#include "animEffect3D.h"
#include "universal.h"
#include "collision.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_VTX = 4;	// �����蔻��̒��_��
const float INITIAL_WIDTH_ANIM = 50.0f;	// �A�j���[�V�����̕�
const float INITIAL_WIDTH_LENGTH = 300.0f;	// �A�j���[�V�����̒���
const float INITIAL_SPEED_EXPAND = 40.f;	// �c��ޑ��x
const float INITIAL_SPEED_SHRINK = 5.0f;	// �k�ޑ��x
const float INITIAL_SPEED_EXTEND = 200.0f;	// �L�т鑬�x
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CBeam::CBeam(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBeam::~CBeam()
{

}

//=====================================================
// ��������
//=====================================================
CBeam *CBeam::Create(void)
{
	CBeam *pBeam = nullptr;

	if (pBeam == nullptr)
	{
		pBeam = new CBeam;

		if (pBeam != nullptr)
		{
			// ������
			pBeam->Init();
		}
	}

	return pBeam;
}

//=====================================================
// ����������
//=====================================================
HRESULT CBeam::Init(void)
{
	if (m_info.pAnim == nullptr)
	{// �A�j���[�V�����G�t�F�N�g�̐���
		CAnimEffect3D *pAnimEffect = CAnimEffect3D::GetInstance();

		if (pAnimEffect != nullptr)
		{
			m_info.pAnim = pAnimEffect->CreateEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAnimEffect3D::TYPE::TYPE_BEAM);

			if (m_info.pAnim != nullptr)
			{
				m_info.pAnim->EnableZtest(false);
				m_info.pAnim->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
			}
		}
	}

	// �l�̏�����
	m_info.fWidthAnimDest = INITIAL_WIDTH_ANIM;
	m_info.fLengthAnimDest = INITIAL_WIDTH_LENGTH;
	m_info.fSpeedExpand = INITIAL_SPEED_EXPAND;
	m_info.fSpeedShrink = INITIAL_SPEED_SHRINK;
	m_info.fSpeedExtend = INITIAL_SPEED_EXTEND;

	// �����蔻��̐���
	if (m_info.pCollision == nullptr)
	{
		m_info.pCollision = CCollisionSphere::Create(CCollision::TAG_ENEMYBULLET, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollision != nullptr)
		{
			m_info.pCollision->SetRadius(500.0f);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBeam::Uninit(void)
{
	if (m_info.pAnim != nullptr)
	{
		m_info.pAnim->Uninit();
		m_info.pAnim = nullptr;
	}

	if (m_info.pCollision != nullptr)
	{
		m_info.pCollision->Uninit();
		m_info.pCollision = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CBeam::Update(void)
{
	// �T�C�Y�̊Ǘ�
	ManageSize();

	// �A�j���[�V�����̒��_�ʒu�ݒ�
	SetAnimVtx();
}

//=====================================================
// �T�C�Y�̊Ǘ�
//=====================================================
void CBeam::ManageSize(void)
{
	if (m_info.fLengthAnimDest > m_info.fLengthAnim)
	{
		// �L�т鏈��
		m_info.fLengthAnim += m_info.fSpeedExtend;

		if (m_info.fLengthAnimDest <= m_info.fLengthAnim)
		{// ��������
			m_info.fLengthAnim = m_info.fLengthAnimDest;
		}
	}

	if (m_info.bFinish == false)
	{
		if (m_info.fWidthAnimDest > m_info.fWidthAnim)
		{
			// �c��ޏ���
			m_info.fWidthAnim += m_info.fSpeedExpand;

			if (m_info.fWidthAnimDest <= m_info.fWidthAnim)
			{// ���̐���
				m_info.fWidthAnim = m_info.fWidthAnimDest;

				m_info.bFinish = true;
			}
		}
	}
	else
	{
		// �k�ޏ���
		m_info.fWidthAnim -= m_info.fSpeedShrink;
		
		if (m_info.fWidthAnim <= 0.0f)
		{// �k�݂�������I��
			m_info.fWidthAnim = 0.0f;

			Uninit();
		}
	}
}

//=====================================================
// �A�j���[�V�����̒��_�ʒu�ݒ�
//=====================================================
void CBeam::SetAnimVtx(void)
{
	if (m_info.pAnim == nullptr)
	{
		return;
	}

	CAnim3D *pAnim = m_info.pAnim;

	D3DXVECTOR3 pos =
	{
		sinf(m_info.rot.x - D3DX_PI * 0.5f) * sinf(m_info.rot.y) * m_info.fLengthAnim,
		cosf(m_info.rot.x - D3DX_PI * 0.5f) * m_info.fLengthAnim,
		sinf(m_info.rot.x - D3DX_PI * 0.5f) * cosf(m_info.rot.y) * m_info.fLengthAnim
	}; 

	D3DXVECTOR3 posMiddle = m_info.pos + pos;

	pAnim->SetSize(m_info.fWidthAnim, m_info.fLengthAnim);
	pAnim->SetPosition(posMiddle);
	pAnim->SetVtx();

	if (m_info.pCollision != nullptr)
	{
		m_info.pCollision->SetPositionOld(m_info.pCollision->GetPosition());
		m_info.pCollision->SetPosition(m_info.pos + pos * 2);

		if (m_info.pCollision->IsTriggerEnter(CCollision::TAG_PLAYER))
		{
			CObject *pObj = m_info.pCollision->GetOther();

			if (pObj != nullptr)
			{
				// ���������I�u�W�F�N�g�̃q�b�g����
				pObj->Hit(10.0f);
			}
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CBeam::Draw(void)
{

}

//=====================================================
// �ʒu�ݒ�
//=====================================================
void CBeam::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;

	if (m_info.pAnim != nullptr)
	{
		m_info.pAnim->SetPosition(pos);
	}
}

//=====================================================
// �����ݒ�
//=====================================================
void CBeam::SetRotation(D3DXVECTOR3 rot)
{
	m_info.rot = rot;

	if (m_info.pAnim != nullptr)
	{
		m_info.pAnim->SetRotation(rot);
	}
}

//=====================================================
// �F�ݒ�
//=====================================================
void CBeam::SetCol(D3DXCOLOR col)
{
	if (m_info.pAnim != nullptr)
	{
		m_info.pAnim->SetColor(col);
	}
}

//=====================================================
// �T�C�Y�ݒ�
//=====================================================
void CBeam::SetAnimSize(float fWidth, float fLength)
{
	m_info.fWidthAnimDest = fWidth; 
	m_info.fLengthAnimDest = fLength;

	if (m_info.pCollision != nullptr)
	{
		m_info.pCollision->SetRadius(fWidth * 0.5f);
	}
}