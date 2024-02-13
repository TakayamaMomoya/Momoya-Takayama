//*****************************************************
//
// �A�C�e���̏���[item.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gimmick.h"
#include "gimmickManager.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "playerManager.h"
#include "weapon.h"
#include "player.h"
#include "billboard.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SIZE_INTERACT = 30.0f;	// �C���^���N�g�\���̃T�C�Y
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CGimmick::CGimmick(int nPriority) : CObjectX(nPriority)
{
	m_pCollisionSphere = nullptr;
	m_pInteract = nullptr;
	m_bEnable = true;

	// �擪�A�Ō���A�h���X�擾
	CGimmickManager *pManager = CGimmickManager::GetInstance();
	CGimmick *pHead = nullptr;
	CGimmick *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}
	else
	{
		return;
	}

	// �l�̃N���A
	m_pPrev = nullptr;
	m_pNext = nullptr;

	if (pHead == nullptr)
	{// �擪�ƍŌ���A�h���X�̑��
		pManager->SetHead(this);
		pManager->SetTail(this);

		return;
	}

	// �O�̃A�h���X�ɍŌ���̃A�h���X��������
	m_pPrev = pTail;

	// �Ō���̃A�h���X�������ɂ���
	pManager->SetTail(this);

	if (m_pPrev != nullptr)
	{
		// �O�̃I�u�W�F�N�g�̎��̃A�h���X�������ɂ���
		m_pPrev->m_pNext = this;
	}
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CGimmick::~CGimmick()
{
	// �擪�A�Ō���A�h���X�擾
	CGimmickManager *pManager = CGimmickManager::GetInstance();
	CGimmick *pHead = nullptr;
	CGimmick *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

	if (pTail != this && pHead != this)
	{// �^�񒆂̃A�h���X�̔j��
		if (m_pPrev != nullptr)
		{
			// �O�̃A�h���X���玟�̃A�h���X���Ȃ�
			m_pPrev->m_pNext = m_pNext;
		}

		if (m_pNext != nullptr)
		{
			// ���̃A�h���X����O�̃A�h���X���Ȃ�
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	if (pHead == this)
	{// �擪�A�h���X�̔j��
		//if (m_pNext != nullptr)
		{// �擪�A�h���X�����̃A�h���X�Ɉ����p��
			pManager->SetHead(m_pNext);

			if (m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
			}
		}
	}

	if (pTail == this)
	{// �Ō���A�h���X�̔j��
		if (m_pPrev != nullptr)
		{// �Ō���A�h���X��O�̃A�h���X�Ɉ����p��
			pManager->SetTail(m_pPrev);

			m_pPrev->m_pNext = nullptr;
		}
	}
}

//=====================================================
// ��������
//=====================================================
CGimmick *CGimmick::Create(void)
{
	CGimmick *pItem = nullptr;

	if (pItem == nullptr)
	{
		pItem = new CGimmick;

		if (pItem != nullptr)
		{
			// ������
			pItem->Init();
		}
	}

	return pItem;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGimmick::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	if (m_pCollisionSphere == nullptr)
	{// �����蔻��̐���
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ITEM, CCollision::TYPE_SPHERE, this);

		if (m_pCollisionSphere != nullptr)
		{
			m_pCollisionSphere->SetRadius(50.0f);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGimmick::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{
		m_pCollisionSphere->Uninit();
		m_pCollisionSphere = nullptr;
	}

	if (m_pInteract != nullptr)
	{
		m_pInteract->Uninit();
		m_pInteract = nullptr;
	}

	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CGimmick::Update(void)
{
	// �p���N���X�̍X�V
	CObjectX::Update();

	if (m_pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionSphere->SetPosition(pos);

		// �v���C���[�Ƃ̓����蔻��
		if (m_pCollisionSphere->OnEnter(CCollision::TAG_PLAYER))
		{
			if (m_pInteract == nullptr && m_bEnable == true)
			{// �C���^���N�g�\������
				D3DXVECTOR3 pos = GetPosition();

				pos.y += 50.0f;

				m_pInteract = CBillboard::Create(pos, SIZE_INTERACT, SIZE_INTERACT);

				if (m_pInteract != nullptr)
				{
					int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\interact.png");
					m_pInteract->SetIdxTexture(nIdx);
					m_pInteract->SetZTest(true);
				}
			}
			// ���������I�u�W�F�N�g�̎擾
			CObject *pObj = m_pCollisionSphere->GetOther();

			// �A�C�e�����菈��
			Interact(pObj);
		}
		else
		{
			if (m_pInteract != nullptr)
			{// �C���^���N�g�\���폜
				m_pInteract->Uninit();
				m_pInteract = nullptr;
			}
		}

		if (m_pCollisionSphere != nullptr)
		{
			if (m_pCollisionSphere->IsTriggerExit(CCollision::TAG_PLAYER))
			{
				// ���������I�u�W�F�N�g�̎擾
				CObject *pObj = m_pCollisionSphere->GetOther();

				Exit(pObj);
			}
		}
	}

	if (m_pInteract != nullptr && m_bEnable == false)
	{// �C���^���N�g�폜
		m_pInteract->Uninit();
		m_pInteract = nullptr;
	}
}

//=====================================================
// �A�C�e�����莞�̏���
//=====================================================
void CGimmick::Interact(CObject *pObj)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CGimmick::Draw(void)
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}