//*****************************************************
//
// �p�[�e�B�N������[particle.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "explosionspawner.h"
#include "animEffect3D.h"
#include "anim3D.h"

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CExplSpawner::CExplSpawner()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_fRadius = 0.0f;
	m_nLife = 0;
	m_nCntSpawn = 0;
	m_nTimerSpawn = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CExplSpawner::~CExplSpawner()
{

}

//=====================================================
// ��������
//=====================================================
CExplSpawner *CExplSpawner::Create(D3DXVECTOR3 pos, float fRadius, int nLife)
{
	CExplSpawner *pSpawner = nullptr;

	if (pSpawner == nullptr)
	{// �C���X�^���X����
		pSpawner = new CExplSpawner;

		pSpawner->m_pos = pos;

		pSpawner->m_nLife = nLife;

		pSpawner->m_fRadius = fRadius;
	}

	return pSpawner;
}

//=====================================================
// ����������
//=====================================================
HRESULT CExplSpawner::Init(void)
{
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CExplSpawner::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CExplSpawner::Update(void)
{
	D3DXVECTOR3 pos;
	float fSize;

	m_nCntSpawn++;

	if (m_nCntSpawn >= m_nTimerSpawn)
	{
		CAnimEffect3D *pAnim = CAnimEffect3D::GetInstance();

		if (pAnim != nullptr)
		{
			CAnim3D *pExplosion;

			pos.x = (float)universal::RandRange((int)m_fRadius, (int)-m_fRadius);
			pos.y = (float)universal::RandRange((int)m_fRadius, (int)-m_fRadius);
			pos.z = (float)universal::RandRange((int)m_fRadius, (int)-m_fRadius);

			fSize = (float)(rand() % 200) + 50;

			pExplosion = pAnim->CreateEffect(m_pos + pos,CAnimEffect3D::TYPE_EXPLOSION);

			pExplosion->SetSize(fSize, fSize);

			// �J�E���^�[���Z�b�g
			m_nCntSpawn = 0;

			if (m_nLife > 0)
			{
				// �^�C�}�[�Đݒ�
				m_nTimerSpawn = rand() % 30 / 2;
			}
		}

	}

	m_nLife--;

	if (m_nLife < 0)
	{
		CAnimEffect3D *pAnim = CAnimEffect3D::GetInstance();

		if (pAnim != nullptr)
		{
			CAnim3D *pExplosion;

			pExplosion = pAnim->CreateEffect(m_pos, CAnimEffect3D::TYPE_EXPLOSION);

			fSize = 1000.0f;

			pExplosion->SetSize(fSize, fSize);

			// �J�E���^�[���Z�b�g
			m_nCntSpawn = 0;

			if (m_nLife > 0)
			{
				// �^�C�}�[�Đݒ�
				m_nTimerSpawn = rand() % m_nLife / 2;
			}
		}

		// ���g�̔j��
		Uninit();
	}
}

//=====================================================
// �ʒu�ݒ菈��
//=====================================================
void CExplSpawner::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}