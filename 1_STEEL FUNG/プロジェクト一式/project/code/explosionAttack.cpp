//*****************************************************
//
// �����U���̏���[explosionAttack.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "explosionAttack.h"
#include "collision.h"
#include "animEffect3D.h"
#include "anim3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float DAMAGE = 5.0f;	// �_���[�W
const DWORD SPEED_ALPHA = 5;	// �A���t�@�e�X�g�l�̌��鑬�x
const DWORD LINE_DELETE = 255;	// �����邵�����l
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CExplAttack::CExplAttack(int nPriority) : CObject(nPriority)
{
	m_fRadius = 0.0f;
	m_pCollision = nullptr;
	m_pAnim = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CExplAttack::~CExplAttack()
{

}

//=====================================================
// ��������
//=====================================================
CExplAttack *CExplAttack::Create(D3DXVECTOR3 pos, float fRadius)
{
	CExplAttack *pExplAttack = nullptr;

	if (pExplAttack == nullptr)
	{
		pExplAttack = new CExplAttack;

		if (pExplAttack != nullptr)
		{
			pExplAttack->m_fRadius = fRadius;
			pExplAttack->m_pos = pos;
			pExplAttack->Init();
		}
	}

	return pExplAttack;
}

//=====================================================
// ����������
//=====================================================
HRESULT CExplAttack::Init(void)
{
	if (m_pCollision == nullptr)
	{// �����蔻��̐���
		m_pCollision = CCollisionSphere::Create(CCollision::TAG_ENEMYBULLET, CCollision::TYPE_SPHERE, this);

		if (m_pCollision != nullptr)
		{
			m_pCollision->SetRadius(m_fRadius * 2);
			m_pCollision->SetPosition(m_pos);
		}
	}

	// �A�j���[�V�����G�t�F�N�g����
	CAnimEffect3D *pAnimEffect = CAnimEffect3D::GetInstance();

	if (pAnimEffect != nullptr)
	{
		// �v���Y�}
		m_pAnim = pAnimEffect->CreateEffect(m_pos, CAnimEffect3D::TYPE::TYPE_PLASMA);

		if (m_pAnim != nullptr)
		{
			m_pAnim->SetSize(m_fRadius, m_fRadius);
			m_pAnim->RandStart();
			m_pAnim->SetAlphaTest(70);
			m_pAnim->EnableZtest(false);
		}

		// �Ռ��g
		CAnim3D *pInpact = pAnimEffect->CreateEffect(m_pos, CAnimEffect3D::TYPE::TYPE_HIT00);
		
		if (pInpact != nullptr)
		{
			pInpact->SetSize(m_fRadius * 3.0f, m_fRadius * 3.0f);
			pInpact->SetAlphaTest(70);
			pInpact->EnableZtest(false);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CExplAttack::Uninit(void)
{
	if (m_pCollision != nullptr)
	{
		m_pCollision->Uninit();
		m_pCollision = nullptr;
	}

	if (m_pAnim != nullptr)
	{
		m_pAnim->Uninit();
		m_pAnim = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CExplAttack::Update(void)
{
	if (m_pCollision != nullptr)
	{// �v���C���[�Ƃ̓����蔻��
		if (m_pCollision->OnEnter(CCollision::TAG::TAG_PLAYER))
		{
			CObject *pObj = m_pCollision->GetOther();

			if (pObj != nullptr)
			{
				pObj->Hit(DAMAGE);

				m_pCollision->Uninit();
				m_pCollision = nullptr;

				return;
			}
		}
	}

	if (m_pAnim != nullptr)
	{
		// �A���t�@�e�X�g�̂������l���グ�Ă���
		DWORD dwAlpha = m_pAnim->GetAlpha();

		dwAlpha += SPEED_ALPHA;

		if (dwAlpha > LINE_DELETE)
		{
			Uninit();

			return;
		}

		m_pAnim->SetAlphaTest(dwAlpha);
	}
	else
	{
		Uninit();

		return;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CExplAttack::Draw(void)
{

}