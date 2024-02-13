//*****************************************************
//
// �ؔ��̏���[box.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "box.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "playerManager.h"
#include "weapon.h"
#include "player.h"
#include "billboard.h"
#include "texture.h"
#include "debrisSpawner.h"
#include "itemRepair.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace 
{
	const char* MODEL_PATH = "data\\MODEL\\item\\WoodBox.x";	// ���f���̃p�X
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CBox::CBox(int nPriority) : CObjectX(nPriority)
{
	m_pCollisionSphere = nullptr;
	m_pCollisionCube = nullptr;
	m_type = TYPE_NONE;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBox::~CBox()
{

}

//=====================================================
// ��������
//=====================================================
CBox *CBox::Create(TYPE type)
{
	CBox *pBox = nullptr;

	if (pBox == nullptr)
	{
		pBox = new CBox;

		if (pBox != nullptr)
		{
			pBox->SetType(type);

			// ������
			pBox->Init();
		}
	}

	return pBox;
}

//=====================================================
// ����������
//=====================================================
HRESULT CBox::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	// ���f���̓Ǎ�
	int nIdx = CModel::Load((char*)MODEL_PATH);
	SetIdxModel(nIdx);
	BindModel(nIdx);

	if (m_pCollisionSphere == nullptr)
	{// �����蔻��̐���
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_BOX, CCollision::TYPE_SPHERE, this);

		if (m_pCollisionSphere != nullptr)
		{
			m_pCollisionSphere->SetRadius(50.0f);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// �����蔻�萶��
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 vtxMax = GetVtxMax();
		D3DXVECTOR3 vtxMin = GetVtxMin();

		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_BLOCK, this);

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetPosition(pos);

			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// �e�̗L����
	EnableShadow(true);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBox::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{
		m_pCollisionSphere->Uninit();
		m_pCollisionSphere = nullptr;
	}

	if (m_pCollisionCube != nullptr)
	{
		m_pCollisionCube->Uninit();
		m_pCollisionCube = nullptr;
	}

	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CBox::Update(void)
{
	// �p���N���X�̍X�V
	CObjectX::Update();

	if (m_pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionSphere->SetPosition(pos);
	}

	if (m_pCollisionCube != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionCube->SetPosition(pos);
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CBox::Hit(float fDamage)
{
	D3DXVECTOR3 pos = GetPosition();

	// �j�Ђ̕��o
	CDebrisSpawner::Create(pos,CDebrisSpawner::TYPE::TYPE_WALL);
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_SE_BOX_BROKEN);
	}

	switch (m_type)
	{
	case CBox::TYPE_REPAIR:
	{
		CItemRepair *pRepair = CItemRepair::Create();

		if (pRepair != nullptr)
		{
			pRepair->SetPosition(pos);
		}
	}
		break;
	case CBox::TYPE_RANDOM:
		break;
	default:
		assert(("�ؔ��̎�ނɕs���Ȓl�������Ă��邩�ݒ肵�Ă��Ȃ��������Ă�ł��܂�", false));
		break;
	}

	Uninit();
}

//=====================================================
// ��ނ̐ݒ�
//=====================================================
void CBox::SetType(TYPE type)
{
	if (type <= TYPE_NONE || type >= TYPE_MAX)
	{
		assert(("�ؔ��̎�ނɕs���Ȓl������Ă��܂�", false));
	}
	else
	{
		m_type = type;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CBox::Draw(void)
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}