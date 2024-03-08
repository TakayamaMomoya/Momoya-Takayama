//*****************************************************
//
// �ʒu�����̏���[limit.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "limit.h"
#include "object3D.h"
#include "player.h"
#include "texture.h"
#include "enemyManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR2 SIZE_GUIDE = { 200.0f,100.0f };	// �K�C�h�\���̃T�C�Y
const float DIST_DISP = 2000.0f;	// �ʒu�����̕\��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CLimit::CLimit(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CLimit::~CLimit()
{

}

//=====================================================
// ��������
//=====================================================
CLimit *CLimit::Create(void)
{
	CLimit *pLimit = nullptr;

	if (pLimit == nullptr)
	{
		pLimit = new CLimit;

		if (pLimit != nullptr)
		{
			// ������
			pLimit->Init();
		}
	}

	return pLimit;
}

//=====================================================
// ����������
//=====================================================
HRESULT CLimit::Init(void)
{
	return S_OK;
}

//=====================================================
// �K�C�h�\������
//=====================================================
void CLimit::CreateGuide(void)
{
	if (m_info.pGuide == nullptr)
	{
		m_info.pGuide = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (m_info.pGuide != nullptr)
		{
			D3DXVECTOR3 rot = m_info.rot;
			rot.x = D3DX_PI * -0.5f;

			m_info.pGuide->SetSize(SIZE_GUIDE.x, SIZE_GUIDE.y);
			m_info.pGuide->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
			m_info.pGuide->SetRotation(rot);
			m_info.pGuide->EnableCull(false);

			int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\limit00.png");
			m_info.pGuide->SetIdxTexture(nIdx);
		}
	}
}

//=====================================================
// �I������
//=====================================================
void CLimit::Uninit(void)
{
	DeleteGuide();

	Release();
}

//=====================================================
// �K�C�h�\���̍폜
//=====================================================
void CLimit::DeleteGuide(void)
{
	if (m_info.pGuide != nullptr)
	{
		m_info.pGuide->Uninit();
		m_info.pGuide = nullptr;
	}
}

//=====================================================
// �X�V����
//=====================================================
void CLimit::Update(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// �v���C���[�̈ʒu����
		float fDiff;

		D3DXVECTOR3 posGuide = CheckLimit((CObject*)pPlayer, &fDiff);

		if (fDiff * fDiff < DIST_DISP * DIST_DISP)
		{
			CreateGuide();
		}
		else
		{
			DeleteGuide();
		}

		if (m_info.pGuide != nullptr)
		{
			SetColGuide(fDiff);

			m_info.pGuide->SetPosition(posGuide);
		}
	}

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		std::list<CEnemy*> listEnemy = pEnemyManager->GetListRanking();

		for (auto pEnemy : listEnemy)
		{
			CheckLimit((CObject*)pEnemy);
		}
	}
}

//=====================================================
// �ʒu�����̊m�F
//=====================================================
D3DXVECTOR3 CLimit::CheckLimit(CObject *pObj, float *pDiff)
{
	D3DXVECTOR3 posObject = pObj->GetPosition();
	D3DXVECTOR3 posGuide = pObj->GetPosition();
	D3DXVECTOR3 pos = m_info.pos;
	D3DXVECTOR3 rot = m_info.rot;
	float fDiff = 0.0f;

	if (rot.y < D3DX_PI * 0.6f && rot.y > D3DX_PI * 0.4f)
	{// �E����
		posGuide.x = pos.x;
		fDiff = posObject.x - pos.x;

		if (posObject.x > pos.x)
		{
			posObject.x = pos.x;
		}
	}
	else if (rot.y > D3DX_PI * -0.6f && rot.y < D3DX_PI * -0.4f)
	{// ������
		posGuide.x = pos.x;
		fDiff = posObject.x - pos.x;

		if (posObject.x < pos.x)
		{
			posObject.x = pos.x;
		}
	}
	else if (rot.y > D3DX_PI * -0.1f && rot.y < D3DX_PI * 0.1f)
	{// �O
		posGuide.z = pos.z;
		fDiff = posObject.z - pos.z;

		if (posObject.z > pos.z)
		{
			posObject.z = pos.z;
		}
	}
	else
	{// ���
		posGuide.z = pos.z;
		fDiff = posObject.z - pos.z;

		if (posObject.z < pos.z)
		{
			posObject.z = pos.z;
		}
	}

	pObj->SetPosition(posObject);

	if (pDiff != nullptr)
		*pDiff = fDiff;

	return posGuide;
}

//=====================================================
// �K�C�h�\���̐F�ݒ�
//=====================================================
void CLimit::SetColGuide(float fDiff)
{
	if (m_info.pGuide == nullptr)
		return;

	D3DXCOLOR col = m_info.pGuide->GetColor();

	float fRate = fDiff / DIST_DISP;

	if (fRate < 0.0f)
	{
		fRate *= -1;
	}

	col.a = 1.0f - fRate;

	m_info.pGuide->SetColor(col);
}

//=====================================================
// �`�揈��
//=====================================================
void CLimit::Draw(void)
{

}