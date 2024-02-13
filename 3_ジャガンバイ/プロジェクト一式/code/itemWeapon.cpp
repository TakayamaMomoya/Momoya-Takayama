//*****************************************************
//
// ����A�C�e���̏���[ItemWeaponWeapon.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "ItemWeapon.h"
#include "manager.h"
#include "renderer.h"
#include "playerManager.h"
#include "weapon.h"
#include "player.h"
#include "debugproc.h"
#include "object3D.h"
#include "texture.h"
#include "meshcylinder.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float GRAVITY = 0.3f;	// �d��

	D3DXCOLOR WEPONCOL[CWeapon::TYPE_MAX] =
	{// ���킲�Ƃ̃G�t�F�N�g�̐F
		{1.0f, 1.0f, 1.0f, 0.35f},	// �}�O�i��
		{1.0f, 1.0f, 1.0f, 0.35f},	// �}�V���K��
		{1.0f, 0.8f, 0.0f, 0.35f},	// �V���b�g�K��
		{1.0f, 0.0f, 0.0f, 0.35f},	// ���[���K��
		{1.0f, 0.0f, 0.0f, 0.35f},	// �~�j�K��
		{1.0f, 0.0f, 0.0f, 0.35f},	// ���P����
	};

	const float SPEED_SCALING = 0.1f;	// �X�P�[�����傫���Ȃ鑬�x
	const float INITIAL_DESTSCALE = 1.5f;	// �����̖ڕW�X�P�[��
	const float INITIAL_HEIGHT_BASE = 150.0f;	// �����̊����
	const float RANGE_FLOAT = 50.0f;	// �������݂���͈�
	const float SPEED_ROTATE = 0.01f;	// ��]���x
	const float SPEED_MOVE = 0.2f;	// �Ǐ]���x
	const float SPEED_FLOAT = 0.03f;	// �������݂��鑬�x
	const float SIZE_LIGHT = 60.0f;	// ���̃T�C�Y
	const float SIZE_RADIUS = 60;	// �V�����_�[�T�C�Y(��)
	const float SIZE_HEIGHT = 50.0f;	// �V�����_�[�T�C�Y(��)
	const float SPEED_SCALIN = 2.0f;	// �X�P�[�����傫���Ȃ鑬�x

}

//=====================================================
// �R���X�g���N�^
//=====================================================
CItemWeapon::CItemWeapon(int nPriority) : CGimmick(nPriority)
{
	m_type = CWeapon::TYPE_MAGNUM;
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CItemWeapon::~CItemWeapon()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CItemWeapon::Init(void)
{
	// �p���N���X�̏�����
	CGimmick::Init();

	// �ǂݍ���
	Load();

	// �X�P�[�������ݒ�
	SetScale(0.0f);

	// �������ɂ���
	D3DXVECTOR3 rot = GetRot();

	rot.x = D3DX_PI * 0.5f;

	SetRot(rot);

	m_info.fScaleDest = INITIAL_DESTSCALE;

	// ���̐���
	if (m_info.pLight == nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_info.pLight = CObject3D::Create(pos);

		if (m_info.pLight != nullptr)
		{
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\glow.png");
			m_info.pLight->SetIdxTexture(nIdx);
			m_info.pLight->SetColor(WEPONCOL[m_type]);
			m_info.pLight->SetSize(0.0f, 0.0f);
			m_info.pLight->EnableAdd(true);
		}
	
	}
	if (m_info.pCylinder == nullptr)
	{
		m_info.pCylinder = CMeshCylinder::Create();

		if (m_info.pCylinder != nullptr)
		{
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\ring00.png");
			m_info.pCylinder->SetIdxTexture(nIdx);
			m_info.pCylinder->SetCol(WEPONCOL[m_type]);
			m_info.pCylinder->SetRadius(SIZE_RADIUS);
			m_info.pCylinder->SetHeight(SIZE_HEIGHT);
			m_info.pCylinder->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// �ǂݍ��ݏ���
//=====================================================
void CItemWeapon::Load(void)
{
	char* apPath[CWeapon::TYPE_MAX] =
	{
		"data\\MODEL\\weapon\\magnum.x",
		"data\\MODEL\\weapon\\mac10.x",
		"data\\MODEL\\weapon\\shotgun.x",
		"data\\MODEL\\weapon\\railgun.x",
		"data\\MODEL\\weapon\\minigun_000.x",
		"data\\MODEL\\weapon\\launcher.x",
	};

	// ���f���̓Ǎ�
	int nIdx = CModel::Load(apPath[m_type]);
	SetIdxModel(nIdx);
	BindModel(nIdx);
}

//=====================================================
// �I������
//=====================================================
void CItemWeapon::Uninit(void)
{
	if (m_info.pLight != nullptr)
	{
		m_info.pLight->Uninit();
		m_info.pLight = nullptr;
	}
	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->Uninit();
		m_info.pCylinder = nullptr;
	}

	// �p���N���X�̏I��
	CGimmick::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CItemWeapon::Update(void)
{
	// �p���N���X�̍X�V
	CGimmick::Update();

	// �X�P�[���̊Ǘ�
	ManageScale();

	// �񂷏���
	ManageTransform();

	// �ʒu���̎擾
	D3DXVECTOR3 pos = GetPosition();
	pos.y = 1.0f;

	// ���̒Ǐ]
	if (m_info.pLight != nullptr)
	{
		m_info.pLight->SetPosition(pos);
	}
	// �V�����_�[�̒Ǐ]�E��]
	if (m_info.pCylinder != nullptr)
	{
		D3DXVECTOR3 rot = m_info.pCylinder->GetRot();
		rot.y += SPEED_ROTATE;
		universal::LimitRot(rot);

		m_info.pCylinder->SetPosition(pos);
		m_info.pCylinder->SetRot({ rot.x, rot.y, rot.z });
	}
}

//=====================================================
// �X�P�[���̊Ǘ�
//=====================================================
void CItemWeapon::ManageScale(void)
{
	// ����̃X�P�[���ݒ�
	float fScale = GetScale();

	fScale += (m_info.fScaleDest - fScale) * SPEED_SCALING;

	SetScale(fScale);

	// �G�t�F�N�g�̃X�P�[���ݒ�
	if (m_info.pLight != nullptr)
	{
		fScale = m_info.pLight->GetWidth();
		fScale += (SIZE_RADIUS - fScale) * SPEED_SCALING;

		m_info.pLight->SetSize(fScale, fScale);
	}
	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetRadius(fScale);
		m_info.pCylinder->SetVtx();
	}
}

//=====================================================
// �g�����X�t�H�[���̊Ǘ�
//=====================================================
void CItemWeapon::ManageTransform(void)
{
	// ��]
	D3DXVECTOR3 rot = GetRot();

	rot.y += SPEED_ROTATE;

	universal::LimitRot(&rot.y);

	SetRot(rot);

	// �ڕW�ʒu�Ɉړ�
	D3DXVECTOR3 pos = GetPosition();
	pos.y = 0.0f;
	D3DXVECTOR3 posDest = m_info.posDest + pos;

	pos += (posDest - pos) * SPEED_MOVE;

	SetPosition(pos);

	// �������݂�����
	m_info.fTimer += SPEED_FLOAT;

	universal::LimitRot(&m_info.fTimer);

	float fDiff = sinf(m_info.fTimer) * RANGE_FLOAT;

	m_info.posDest.y = INITIAL_HEIGHT_BASE + fDiff;
}

//=====================================================
// �A�C�e�����莞�̏���
//=====================================================
void CItemWeapon::Interact(CObject *pObj)
{
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pObj == nullptr || pPlayerManager == nullptr)
	{
		return;
	}

	// �v���C���[�擾
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			if ((CObject*)pPlayer == pObj)
			{// �v���C���[���o
				bool bGet = pPlayer->InputInteract();

				if (bGet)
				{
					// ���ʂ̕t�^
					ApplyEffect(pPlayer);

					Uninit();
				}
			}
		}
	}
}

//=====================================================
// ���ʂ�K�p���鏈��
//=====================================================
void CItemWeapon::ApplyEffect(CPlayer* pPlayer)
{
	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->SetWeapon(m_type);
}

//=====================================================
// �`�揈��
//=====================================================
void CItemWeapon::Draw(void)
{
	// �p���N���X�̕`��
	CGimmick::Draw();
}

//=====================================================
// ��������
//=====================================================
CItemWeapon *CItemWeapon::Create(CWeapon::TYPE type)
{
	if (type >= CWeapon::TYPE_MAX || type < 0)
	{
		assert(("�z��O�̕�����o�����Ƃ��Ă܂�",false));
	}

	CItemWeapon *pItemWeapon = nullptr;

	if (pItemWeapon == nullptr)
	{
		pItemWeapon = new CItemWeapon;

		if (pItemWeapon != nullptr)
		{
			pItemWeapon->m_type = type;

			// ������
			pItemWeapon->Init();
		}
	}

	return pItemWeapon;
}