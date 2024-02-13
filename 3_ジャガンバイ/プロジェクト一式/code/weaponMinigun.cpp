//*****************************************************
//
// �~�j�K���̏���[weaponMinigun.]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weaponMinigun.h"
#include "inputjoypad.h"
#include "bullet.h"
#include "player.h"
#include "universal.h"
#include "sound.h"
#include "animEffect3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float BULLET_SPEED = 10.0f;	// �e�̑��x
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CMinigun::CMinigun(int nPriority) : CWeapon(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMinigun::~CMinigun()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CMinigun::Init(void)
{
	// �p���N���X�̏�����
	CWeapon::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMinigun::Uninit(void)
{
	// �p���N���X�̏I��
	CWeapon::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CMinigun::Update(void)
{
	// �p���N���X�̍X�V
	CWeapon::Update();
}

//=====================================================
// �U������	Autor:�����V
//=====================================================
void CMinigun::Attack(void)
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (pJoypad == nullptr)
	{
		return;
	}

	int nBullet = GetBullet();
	int nID = GetID();

	if (pJoypad->GetPress(CInputJoypad::PADBUTTONS_RB, nID))
	{// �ˌ�
		int nCntShot = GetCntShot();

		if (nBullet > 0 && nCntShot == 0)
		{// �e�̔���
			D3DXMATRIX* pMtx = GetMatrix();

			// �}�Y���̈ʒu��ݒ�
			D3DXMATRIX mtxMuzzle;
			universal::SetOffSet(&mtxMuzzle, *pMtx, D3DXVECTOR3(-95.0f, -15.0f, 0.0f));

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
			CBullet* pBullet = CBullet::Create(posMuzzle, -move, info.fLifeBullet, CBullet::TYPE_PLAYER, false,6.0f, info.fDamage);

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
				// �}�O�i�����C��
				pSound->Play(pSound->LABEL_SE_GUNSHOT_04);
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
void CMinigun::Draw(void)
{
	// �p���N���X�̕`��
	CWeapon::Draw();
}