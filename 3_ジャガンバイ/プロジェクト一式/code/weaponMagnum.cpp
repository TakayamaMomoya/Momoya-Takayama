//*****************************************************
//
// �}�O�i���̏���[MagnumMagnum.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weaponMagnum.h"
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
CMagnum::CMagnum(int nPriority) : CWeapon(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMagnum::~CMagnum()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CMagnum::Init(void)
{
	// �p���N���X�̏�����
	CWeapon::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMagnum::Uninit(void)
{
	// �p���N���X�̏I��
	CWeapon::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CMagnum::Update(void)
{
	// �p���N���X�̍X�V
	CWeapon::Update();
}

//=====================================================
// �U������
//=====================================================
void CMagnum::Attack(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

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
			universal::SetOffSet(&mtxMuzzle, *pMtx, D3DXVECTOR3(-50.0f, 20.0f, 0.0f));

			D3DXVECTOR3 posMuzzle =
			{
				mtxMuzzle._41,
				mtxMuzzle._42,
				mtxMuzzle._43,
			};

			D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

			CPlayer* pPlayer = GetPlayer();

			if (pPlayer != nullptr)
			{// �v���C���[�̌����Ɉړ��ʂ�ݒ�
				D3DXVECTOR3 rot = pPlayer->GetRot();

				int nRange = universal::RandRange(10, -10);

				float fRand = nRange * 0.01f;

				rot.y += fRand;

				move =
				{
					sinf(rot.y) * BULLET_SPEED,
					0.0f,
					cosf(rot.y) * BULLET_SPEED,
				};
			}

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

			CSound* pSound = CSound::GetInstance();

			if (pSound != nullptr)
			{
				// �}�O�i�����C��
				pSound->Play(pSound->LABEL_SE_GUNSHOT_00);
			}

			if (pJoypad != nullptr)
			{// �R���g���[���[�̐U��
				pJoypad->Vibration(nID, CInputJoypad::PADVIB::PADVIB_USE, info.fVibPower, info.nVibTime);
			}

			// �e�����炷
			nBullet--;
			SetBullet(nBullet);

			// �A�˃J�E���^�[�̃��Z�b�g
			nCntShot = GetRapid();

			SetCntShot(nCntShot);

			// �G�t�F�N�g�̐���
			CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

			if (pAnim3D != nullptr)
			{
				pAnim3D->CreateEffect(posMuzzle, CAnimEffect3D::TYPE::TYPE_MUZZLEFLUSH);
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
void CMagnum::Draw(void)
{
	// �p���N���X�̕`��
	CWeapon::Draw();
}