//*****************************************************
//
// ���[���K���̏���[weaponRailgun.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weaponRailgun.h"
#include "inputjoypad.h"
#include "bullet.h"
#include "player.h"
#include "sound.h"
#include "animEffect3D.h"
#include "universal.h"
#include "debugproc.h"
#include "playerManager.h"
#include "effect3D.h"
#include "enemyManager.h"
#include "weaponManager.h"
#include "object3D.h"
#include "motion.h"
#include "beam.h"
#include "inpact.h"
#include "record.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_VTX = 4;	// �����蔻��̒��_��
const D3DXCOLOR COL_RANGE = { 1.0f,1.0f,1.0f,0.4f };	// �͈͕\���̐F
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CRailgun::CRailgun(int nPriority) : CWeapon(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfoRailgun));
	m_pRange = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRailgun::~CRailgun()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CRailgun::Init(void)
{
	// �p���N���X�̏�����
	CWeapon::Init();

	// �ŗL������
	CWeaponManager *pWeaponManager = CWeaponManager::GetInstance();

	if (pWeaponManager != nullptr)
	{
		CWeaponManager::SInfoRailgun info = pWeaponManager->GetRailgunInfo();

		m_info.fWidth = info.fWidth;
		m_info.fLength = info.fLength;
	}

	// �͈͕\���̐���
	if (m_pRange == nullptr)
	{
		m_pRange = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pRange->SetSize(0.0f, 0.0f);
		m_pRange->SetColor(COL_RANGE);
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\range.png");
		m_pRange->SetIdxTexture(nIdx);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CRailgun::Uninit(void)
{
	if (m_pRange != nullptr)
	{
		m_pRange->Uninit();
		m_pRange = nullptr;
	}

	// �p���N���X�̏I��
	CWeapon::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CRailgun::Update(void)
{
	// �p���N���X�̍X�V
	CWeapon::Update();
}

//=====================================================
// �U������
//=====================================================
void CRailgun::Attack(void)
{
	// �����蔻��̔���
	Shot();
}

//=====================================================
// �ˌ�����
//=====================================================
void CRailgun::Shot(void)
{
	// �}�Y���̈ʒu��ݒ�
	D3DXMATRIX* pMtxWeapon = GetPlayer()->GetMatrix();
	D3DXMATRIX mtxMuzzle;
	universal::SetOffSet(&mtxMuzzle, *pMtxWeapon, D3DXVECTOR3(-18.0f, 6.0f, 0.0f));

	D3DXVECTOR3 posMuzzle =
	{
		mtxMuzzle._41,
		mtxMuzzle._42,
		mtxMuzzle._43,
	};

	// ���_�̐ݒ�
	D3DXVECTOR3 aPosVtx[NUM_VTX];
	D3DXMATRIX aMtxVtx[NUM_VTX];
	D3DXVECTOR3 aOffset[NUM_VTX] =
	{
		{ m_info.fWidth,0.0f,-m_info.fLength },
		{ -m_info.fWidth,0.0f,-m_info.fLength },
		{ -m_info.fWidth,0.0f,0.0f },
		{ m_info.fWidth,0.0f,0.0f },

	};

	for (int i = 0; i < NUM_VTX; i++)
	{
		universal::SetOffSet(&aMtxVtx[i], mtxMuzzle, aOffset[i]);

		aPosVtx[i] =
		{
			aMtxVtx[i]._41,
			0.0f,
			aMtxVtx[i]._43,
		};

#ifdef _DEBUG
		CEffect3D::Create(aPosVtx[i], 10.0f, 60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
	}

	// ���_���ݒ�
	SetVtx(aPosVtx[0], aPosVtx[1], aPosVtx[2], aPosVtx[3]);

	// ����
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
	{
		return;
	}

	int nBullet = GetBullet();
	int nID = GetID();
	int nCntShot = GetCntShot();

	// �G�Ƃ̐ڐG�𔻒肷��
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		// �擪�I�u�W�F�N�g����
		CEnemy* pEnemy = pEnemyManager->GetHead();

		while (pEnemy != nullptr)
		{
			// ���̃A�h���X��ۑ�
			CEnemy* pEnemyNext = pEnemy->GetNext();

			if (pEnemy != nullptr)
			{
				D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

				bool bHit = universal::CubeCrossProduct(aPosVtx[0], aPosVtx[1], aPosVtx[2], aPosVtx[3], posEnemy);

				CWeapon::SInfo info = GetInfo();

				if (bHit)
				{
					if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB, nID))
					{// �ˌ�
						if (nBullet > 0 && nCntShot == 0)
						{// �e�̔���
							pEnemy->Hit(info.fDamage);

							// ���擾����
							CPlayer* pPlayer = GetPlayer();
							CRecord* pRecord = CRecord::GetInstance();

							// �j�󐔂̐�щ��Z����
							if (pPlayer != nullptr &&
								pRecord != nullptr)
							{
								pRecord->CheckDeathEnemy(pEnemy, pPlayer->GetID());
							}
						}
						else
						{// �e�؂�̏ꍇ

						}
					}

					CMotion *pBody = pEnemy->GetBody();

					if (pBody != nullptr)
					{
						pBody->SetAllCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
				}
				else
				{
					CMotion *pBody = pEnemy->GetBody();

					if (pBody != nullptr)
					{
						pBody->ResetAllCol();
					}
				}
			}

			// ���̃A�h���X����
			pEnemy = pEnemyNext;
		}
	}

	if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB, nID))
	{// �e�����炷
		// �e�����炷
		nBullet--;
		SetBullet(nBullet);

		// �A�˃J�E���^�[�̃��Z�b�g
		nCntShot = GetRapid();

		SetCntShot(nCntShot);

		// �r�[���̐���
		CBeam *pBeam = CBeam::Create();

		if (pBeam != nullptr)
		{
			D3DXMATRIX mtxBeam;

			universal::SetOffSet(&mtxBeam, mtxMuzzle, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

			pBeam->SetMtx(mtxBeam);
			pBeam->SetAnimSize(m_info.fWidth, m_info.fLength);
		}

		// �Ռ��g�̐���
		D3DXMATRIX mtxInpact;

		universal::SetOffSet(&mtxInpact, mtxMuzzle, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));

		CInpact *pInpact = CInpact::Create(0.2f, &mtxInpact);

		if (pInpact != nullptr)
		{
			pInpact->SetSpeedExpand(25.0f);
		}

		universal::SetOffSet(&mtxInpact, mtxMuzzle, D3DXVECTOR3(0.0f, 0.0f, m_info.fLength * -0.5f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
		pInpact = CInpact::Create(0.2f, &mtxInpact);

		if (pInpact != nullptr)
		{
			pInpact->SetSpeedExpand(25.0f);
		}

		// ���̍Đ�
		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_GUNSHOT_03);
		}

		// �p�����[�^�[�擾
		CWeapon::SInfo info = GetInfo();

		if (pJoypad != nullptr)
		{// �R���g���[���[�̐U��
			pJoypad->Vibration(nID, CInputJoypad::PADVIB::PADVIB_USE, info.fVibPower, info.nVibTime);
		}
	}
}

//=====================================================
// �|���S���̒��_�ݒ�
//=====================================================
void CRailgun::SetVtx(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4)
{
	if (m_pRange == nullptr)
	{
		return;
	}

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRange->GetVtxBuff();

	if (pVtxBuff == nullptr)
	{
		return;
	}

	//���_���̃|�C���^
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = vtx1;
	pVtx[1].pos = vtx2;
	pVtx[2].pos = vtx4;
	pVtx[3].pos = vtx3;

	pVtx[0].pos.y = 2.0f;
	pVtx[1].pos.y = 2.0f;
	pVtx[2].pos.y = 2.0f;
	pVtx[3].pos.y = 2.0f;

	//���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();
}

//=====================================================
// �L�����؂�ւ�
//=====================================================
void CRailgun::ToggleEnable(bool bEnable)
{
	if (bEnable)
	{
		if (m_pRange == nullptr)
		{// �͈͕\���̐���
			m_pRange = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_pRange->SetSize(0.0f, 0.0f);
			m_pRange->SetColor(COL_RANGE);

			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\range.png");
			m_pRange->SetIdxTexture(nIdx);
		}
	}
	else
	{
		if (m_pRange != nullptr)
		{
			m_pRange->Uninit();
			m_pRange = nullptr;
		}
	}

	CWeapon::ToggleEnable(bEnable);
}

//=====================================================
// �`�揈��
//=====================================================
void CRailgun::Draw(void)
{
	// �p���N���X�̕`��
	CWeapon::Draw();
}