//*****************************************************
//
// �����`���[�̏���[weaponLauncher.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weaponLauncher.h"
#include "inputjoypad.h"
#include "missile.h"
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
CLauncher::CLauncher(int nPriority) : CWeapon(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CLauncher::~CLauncher()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CLauncher::Init(void)
{
	// �p���N���X�̏�����
	CWeapon::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CLauncher::Uninit(void)
{
	// �p���N���X�̏I��
	CWeapon::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CLauncher::Update(void)
{
	// �p���N���X�̍X�V
	CWeapon::Update();
}

//=====================================================
// �U������
//=====================================================
void CLauncher::Attack(void)
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
			CWeapon::SInfo info = GetInfo();

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_GUNSHOT_04);
			}

			if (pJoypad != nullptr)
			{// �R���g���[���[�̐U��
				pJoypad->Vibration(nID, CInputJoypad::PADVIB::PADVIB_USE, info.fVibPower, info.nVibTime);
			}

			// �}�Y���̈ʒu��ݒ�
			D3DXMATRIX mtxMuzzle;
			universal::SetOffSet(&mtxMuzzle, *pMtx, D3DXVECTOR3(-50.0f, 20.0f, 0.0f));

			D3DXVECTOR3 posMuzzle =
			{
				mtxMuzzle._41,
				mtxMuzzle._42,
				mtxMuzzle._43,
			};

			CPlayer* pPlayer = GetPlayer();
			D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

			if (pPlayer != nullptr)
			{// �v���C���[�̌����Ɉړ��ʂ�ݒ�
				rot = pPlayer->GetRot();

				// �e�𔭎�
				CMissile *pMissile = CMissile::Create();

				if (pMissile != nullptr)
				{
					pMissile->SetPosition(posMuzzle);
					pMissile->SetRot(rot);
					pMissile->SetIdxPlayer(pPlayer->GetID());
				}
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
void CLauncher::Draw(void)
{
	// �p���N���X�̕`��
	CWeapon::Draw();
}