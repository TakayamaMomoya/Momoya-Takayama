//*****************************************************
//
// �V���b�g�K���̏���[weaponShotgun.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weaponShotgun.h"
#include "inputjoypad.h"
#include "bullet.h"
#include "player.h"
#include "sound.h"
#include "animEffect3D.h"
#include "universal.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define BULLET_SPEED	(10.0f)	// �e�̑��x

//=====================================================
// �R���X�g���N�^
//=====================================================
CShotgun::CShotgun(int nPriority) : CWeapon(nPriority)
{
	m_fAngleDiffuse = 0.0f;
	m_nNumPellet = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CShotgun::~CShotgun()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CShotgun::Init(void)
{
	// �p���N���X�̏�����
	CWeapon::Init();

	// �V���b�g�K���̏�����
	CWeaponManager *pWeaponManager = CWeaponManager::GetInstance();

	if (pWeaponManager != nullptr)
	{
		CWeaponManager::SInfoShotgun info = pWeaponManager->GetShotgunInfo();

		m_nNumPellet = info.nNumPellet;
		m_fAngleDiffuse = info.fAngleDiffuse;
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CShotgun::Uninit(void)
{
	// �p���N���X�̏I��
	CWeapon::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CShotgun::Update(void)
{
	// �p���N���X�̍X�V
	CWeapon::Update();
}

//=====================================================
// �U������
//=====================================================
void CShotgun::Attack(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();
	
	if (pJoypad == nullptr)
	{
		return;
	}

	int nBullet = GetBullet();
	int nID = GetID();

	if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB,nID))
	{// �ˌ�
		int nCntShot = GetCntShot();

		if (nBullet > 0 && nCntShot == 0)
		{// �e�̔���
			D3DXMATRIX* pMtx = GetMatrix();

			// �}�Y���̈ʒu��ݒ�
			D3DXMATRIX mtxMuzzle;
			universal::SetOffSet(&mtxMuzzle, *pMtx, D3DXVECTOR3(-60.0f, 5.0f, 0.0f));

			D3DXVECTOR3 posMuzzle =
			{
				mtxMuzzle._41,
				mtxMuzzle._42,
				mtxMuzzle._43,
			};

			D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

			CPlayer* pPlayer = GetPlayer();

			if (pPlayer != nullptr)
			{
				for (int i = 0; i < m_nNumPellet; i++)
				{// ��x�ɐݒ肳�ꂽ���̒e�𔭎�
					D3DXVECTOR3 rot = pPlayer->GetRot();

					// �ړ��p�x�����炷
					int nRange = universal::RandRange((int)(m_fAngleDiffuse * 0.5f), (int)(-m_fAngleDiffuse * 0.5f));

					float fRand = D3DXToRadian((float)nRange);

					rot.y += fRand;

					move =
					{
						sinf(rot.y) * BULLET_SPEED,
						0.0f,
						cosf(rot.y) * BULLET_SPEED,
					};

					// �p�����[�^�[�擾
					CWeapon::SInfo info = GetInfo();

					// �e�𔭎�
					CBullet* pBullet = CBullet::Create(posMuzzle, -move, info.fLifeBullet, CBullet::TYPE_PLAYER, false, 2.0f, info.fDamage);

					// �e�̃v���C���[�ԍ��ݒ菈��
					if (pBullet != nullptr)
					{
						CPlayer* pPlayer = GetPlayer();

						if (pPlayer != nullptr)
						{
							pBullet->SetIdxPlayer(pPlayer->GetID());
						}
					}

					if (pSound != nullptr)
					{
						// �V���b�g�K�����C��
						pSound->Play(pSound->LABEL_SE_GUNSHOT_02);
					}

					if (pJoypad != nullptr)
					{// �R���g���[���[�̐U��
						pJoypad->Vibration(nID, CInputJoypad::PADVIB::PADVIB_USE, info.fVibPower, info.nVibTime);
					}

					// �A�˃J�E���^�[�̃��Z�b�g
					nCntShot = GetRapid();

					SetCntShot(nCntShot);
				}

				// �G�t�F�N�g�̐���
				CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

				if (pAnim3D != nullptr)
				{
					pAnim3D->CreateEffect(posMuzzle, CAnimEffect3D::TYPE::TYPE_MUZZLEFLUSH);
				}

				// �e�����炷
				nBullet--;
				SetBullet(nBullet);
			}
		}
		else
		{// �e�؂�̏ꍇ

		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CShotgun::Draw(void)
{
	// �p���N���X�̕`��
	CWeapon::Draw();
}