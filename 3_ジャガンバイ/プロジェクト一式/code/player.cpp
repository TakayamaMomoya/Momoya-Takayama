//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "player.h"
#include "playerManager.h"
#include "motion.h"
#include "debugproc.h"
#include "inputjoypad.h"
#include "inputkeyboard.h"
#include "universal.h"
#include "collision.h"
#include "game.h"
#include "manager.h"
#include "arrow.h"
#include "effect3D.h"
#include "motionDiv.h"
#include "enemyManager.h"
#include "sound.h"
#include "UIManager.h"
#include "record.h"
#include "ghost.h"
#include "rocket.h"
#include "goal.h"
#include "animEffect3D.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* BODY_PATH_LOWER[NUM_PLAYER] =
{// �����g�̃p�X
	"data\\MOTION\\motionPotatoman01_lower.txt",
	"data\\MOTION\\motionPotatoman02_lower.txt",
	"data\\MOTION\\motionPotatoman03_lower.txt",
	"data\\MOTION\\motionPotatoman04_lower.txt",
};

const char* BODY_PATH_UPPER[NUM_PLAYER] =
{// �㔼�g�̃p�X
	"data\\MOTION\\motionPotatoman01_upper.txt",
	"data\\MOTION\\motionPotatoman02_upper.txt",
	"data\\MOTION\\motionPotatoman03_upper.txt",
	"data\\MOTION\\motionPotatoman04_upper.txt",
};

const D3DXCOLOR COL_ARROW[NUM_PLAYER] =
{// ���̐F
	D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,1.0f,1.0f),
	D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),
	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),
};	// ���̐F

const float MOVE_LINE = 0.2f;		// �����Ă��锻�f�̂������l
const float ARROW_POSY = 5.0f;	// ���̈ʒu�̍���
const float ARROW_WIDTH = 30.0f;	// ���̕�
const float ARROW_HEIGHT = 50.0f;	// ���̍���
const float GRAVITY = 1.58f;	// �d��
const float POW_PUNCH_UP = 15.0f;	// �p���`�Ŕ�яオ��W�����v��
const float SPEED_LIFE_FADE = 0.01f;	// ���C�t�\���̏����鑬�x
const float SIZE_LIFE = 50.0f;	// ���C�t�\���̃T�C�Y
const D3DXVECTOR3 POS_LIFE = { 50.0f,50.0f,100.0f };	// ���C�t�\���̈ʒu

const int HAND_PARTS_NUM = 6;				// ��̔ԍ�
const float MOTION_STICK_RUNAWAY = 0.1f;	// �X�e�B�b�N�̖\������

const float LIMID_RANGE_LEFT = -460.0f;
const float LIMID_RANGE_RIGHT = 460.0f;
const float LIMID_RANGE_UP = 460.0f;
const float LIMID_RANGE_DOWN = -460.0f;
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	ZeroMemory(&m_info, sizeof(CPlayer::SInfo));
	ZeroMemory(&m_param, sizeof(CPlayer::SParam));
	m_bSound = false;
	m_bGoalJump = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// ��������
//=====================================================
CPlayer *CPlayer::Create(int nID)
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->m_info.nID = nID;

		pPlayer->Init();
	}

	return pPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayer::Init(void)
{
	m_bGoalJump = false;
	
	// �p���N���X�̏�����
	CCharacterDiv::Init();

	// �p�����[�^�[�̎󂯎��
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{
		SParam param = pPlayerManager->GetPlayerParam();

		m_param = param;

		int nNumAttack = pPlayerManager->GetNumAttack();

		m_info.nNumAttack = nNumAttack;

		AttackInfo *pAttackInfo = pPlayerManager->GetAttackInfo();

		m_info.pAttackInfo = pAttackInfo;
	}
	else
	{
		assert(("�v���C���[�p�����[�^�[�̎擾�Ɏ��s",false));
	}

	// �p�����[�^�[�̎󂯎��
	CUIManager* pUIManager = CUIManager::GetInstance();

	if (pUIManager != nullptr)
	{
		pUIManager->SetPlayer(m_info.nID);
	}
	else
	{
		assert(("UI�Ǘ��i�v���C���[�j�̎擾�Ɏ��s", false));
	}


	// �����蔻��̐���
	if (m_info.pCollisionSphere == nullptr)
	{
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(20.0f);
		}
	}

	if (m_info.pClsnAttack == nullptr)
	{// ���̓����蔻�萶��
		m_info.pClsnAttack = CCollisionSphere::Create(CCollision::TAG_NONE, CCollision::TYPE_SPHERE, this);

		if (m_info.pClsnAttack != nullptr)
		{// ���̐ݒ�
			m_info.pClsnAttack->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_info.pClsnAttack->SetRadius(0.0f);
		}
	}

	if (m_info.pCollisionCube == nullptr)
	{// �����蔻�萶��
		m_info.pCollisionCube = CCollisionCube::Create(CCollision::TAG_PLAYER, this);

		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 vtxMax = { 20.0f,100.0f,20.0f };
			D3DXVECTOR3 vtxMin = { -20.0f,0.0f,-20.0f };

			m_info.pCollisionCube->SetPosition(GetPosition());

			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	if (m_info.pArrow == nullptr)
	{// ���̐���
		m_info.pArrow = CArrow::Create(GetPosition(), ARROW_WIDTH, ARROW_HEIGHT);

		if (m_info.pArrow != nullptr)
		{
			m_info.pArrow->SetColor(COL_ARROW[m_info.nID]);
		}
	}

	m_info.fLife = m_param.fInitialLife;
	m_info.state = STATE_NORMAL;

	// ID�ɑΉ��������f���̐ݒ�
	CCharacterDiv::Load((char*)BODY_PATH_LOWER[m_info.nID], (char*)BODY_PATH_UPPER[m_info.nID]);

	// �e�̗L����
	for (int nCutPath = 0; nCutPath < CCharacterDiv::PARTS_MAX; nCutPath++)
	{
		CMotionDiv* pBody = GetBody();

		if (pBody != nullptr)
		{
			pBody->SetPosShadow(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
			pBody->EnableShadow(true);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{// �ۊǂ��Ă��鎩�g�̃|�C���^���폜
		pPlayerManager->ReleasePlayer(m_info.nID);
	}

	if (m_info.pCollisionSphere != nullptr)
	{
		m_info.pCollisionSphere->Uninit();
		m_info.pCollisionSphere = nullptr;
	}

	if (m_info.pCollisionCube != nullptr)
	{
		m_info.pCollisionCube->Uninit();
		m_info.pCollisionCube = nullptr;
	}

	if (m_info.pWeapon != nullptr)
	{
		m_info.pWeapon->Uninit();
		m_info.pWeapon = nullptr;
	}
	
	if (m_info.pArrow != nullptr)
	{
		m_info.pArrow->Uninit();
		m_info.pArrow = nullptr;
	}

	if (m_info.pClsnAttack != nullptr)
	{
		m_info.pClsnAttack->Uninit();
		m_info.pClsnAttack = nullptr;
	}

	// ���C�t�\���̍폜
	DeleteLife();

	// �p���N���X�̏I��
	CCharacterDiv::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
	// �p���N���X�̍X�V
	CCharacterDiv::Update();

	// ���P�b�g�ɏ�荞��
	BoardingRocket();

	// ����
	Input();

	// �ʒu�̔��f
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();
	
	// �O��̈ʒu��ۑ�
	SetPositionOld(pos);

	pos += move;
	SetPosition(pos);

	// �����蔻��̒Ǐ]
	if (m_info.pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posWaist = GetBody()->GetMtxPos(CCharacterDiv::PARTS_UPPER,0);
		
		// �G�Ƃ̐ڐG����
		m_info.pCollisionSphere->SetPosition(posWaist);

		bool bHit = m_info.pCollisionSphere->OnEnter(CCollision::TAG_ENEMY);

		if (bHit)
		{
			Hit(5.0f);
		}

		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posCollision = m_info.pCollisionCube->GetPosition();

			// ����̒Ǐ]
			m_info.pCollisionCube->SetPositionOld(posCollision);
			m_info.pCollisionCube->SetPosition(pos);

			// �u���b�N�Ƃ̉����o������
			m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

			pos = GetPosition();
			D3DXVECTOR3 posOld = GetPositionOld();

			if (pos.y <= 0.0f && posOld.y >= 0.0f)
			{
				pos.y = 0.0f;
				move.y = 0.0f;

				SetPosition(pos);

				if (m_info.state == STATE_BLOW)
				{
					m_info.state = STATE_NORMAL;
				}
			}

			SetMove(move);
		}

		if (m_info.pCollisionSphere != nullptr)
		{
			CGame* pGame = CGame::GetInstance();
			CScene::MODE mode = CManager::GetMode();
			pos = GetPosition();

			// �G�Ƃ̉����o������
			m_info.pCollisionSphere->SetPosition(pos);

			if (pGame != nullptr)
			{
				CGame::STATE state = pGame->GetState();

				if (state != CGame::STATE_ESCAPE && state != CGame::STATE_RESULT)
				{// �E�o���
					m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_PLAYER);
				}
			}
			else if (mode != CScene::MODE_GAME)
			{
				m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_PLAYER);
			}
			m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_ENEMY);

			m_info.pCollisionSphere->SetPositionOld(m_info.pCollisionSphere->GetPosition());
			m_info.pCollisionSphere->SetPosition(pos);

			SetPosition(pos);
		}
	}

	// �ړ��ʂ̌���
	if (m_info.state == STATE_BLOW)
	{
		move.x *= 0.6f;
		move.z *= 0.6f;
	}
	else
	{
		move.x *= 0.1f;
		move.z *= 0.1f;
	}
	move.y -= GRAVITY;
	SetMove(move);

	// ��ԊǗ�
	ManageState();

	// ���[�V�����Ǘ�
	ManageMotion();

	// ����̒Ǐ]
	if (m_info.pWeapon != nullptr)
	{
		CMotionDiv *pBody = GetBody();

		if (pBody != nullptr)
		{
			pBody->MultiplyMtx();
		}

		m_info.pWeapon->FollowPlayerHand();
	}

	if (m_info.pArrow != nullptr)
	{// ���̒Ǐ]
		D3DXVECTOR3 rot = GetRot();
		D3DXVECTOR3 posArrow = pos;

		posArrow.y = ARROW_POSY;

		m_info.pArrow->SetPosition(posArrow);
		m_info.pArrow->SetRot(rot);
	}

	// �U������Ǘ�
	ManageAttack();

	// ���C�t�\���̊Ǘ�
	ManageLife();

	// �s���͈�
	LimidPostion();

// �f�o�b�O����
#if _DEBUG
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();

	// �v���C���[�����S�����܂�
	if (pPlayerManager != nullptr && pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_F6))
		{
			float fDamage = pPlayerManager->GetPlayerParam().fInitialLife;	// �v���C���[�̏����̗͂��擾

			// �_���[�W����
			Hit(fDamage);
		}
	}
#endif // _DEBUG
}

//=====================================================
// ���͏���
//=====================================================
void CPlayer::Input(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state == CGame::STATE_ESCAPE || state == CGame::STATE_RESULT || state == CGame::STATE_END)
		{
			return;
		}
	}

	if (m_info.motionInfo.bEmote == false)
	{// �G���[�g���͑���ł��Ȃ�
		// �ړ�����
		InputMove();

		// �U������
		InputAttack();

		// �G���[�g�̑���
		InputEmote();
	}

	// �G�C�~���O�̏���
	Aim();
}

//=====================================================
// �ړ��̓���
//=====================================================
void CPlayer::InputMove(void)
{
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pJoyPad == nullptr || pKeyboard == nullptr)
	{
		return;
	}

	// �v���C���[ID�擾
	int nId = m_info.nIDJoypad;

	D3DXVECTOR3 vecStickL = 
	{// �X�e�B�b�N�̃x�N�g���擾
		pJoyPad->GetJoyStickLX(nId),
		pJoyPad->GetJoyStickLY(nId),
		0.0f,
	};

	if (pKeyboard->GetPress(DIK_W))
	{
		vecStickL.y += m_param.fSpeedMove;
	}
	if (pKeyboard->GetPress(DIK_S))
	{
		vecStickL.y -= m_param.fSpeedMove;
	}
	if (pKeyboard->GetPress(DIK_A))
	{
		vecStickL.x -= m_param.fSpeedMove;
	}
	if (pKeyboard->GetPress(DIK_D))
	{
		vecStickL.x += m_param.fSpeedMove;
	}

	D3DXVECTOR3 vecMove =
	{// �ړ��x�N�g���̑��
		vecStickL.x,
		0.0f,
		vecStickL.y,
	};

	// �ړ����x�̐ݒ�
	D3DXVECTOR3 move = GetMove();

	D3DXVec3Normalize(&vecMove, &vecMove);
	vecMove *= m_param.fSpeedMove;

	move += vecMove;

	SetMove(move);
}

//=====================================================
// �U���̓���
//=====================================================
void CPlayer::InputAttack(void)
{
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (pJoyPad == nullptr)
	{
		return;
	}

	// �p���`�̓���
	m_info.motionInfo.bPunch = pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_LB, GetIDJoypad());

	int nMotionUpper = GetMotion(CCharacterDiv::PARTS_UPPER);

	if (nMotionUpper != MOTION_PUNCH)
	{
		if (m_info.pWeapon != nullptr)
		{// ����̍U��
			bool bEnable = m_info.pWeapon->IsEnable();

			if (m_info.pItemRepair != nullptr)
			{// �C�����i�������
				if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_RB, GetIDJoypad()))
				{
					m_info.pItemRepair->Detatch();

					// �����L����
					m_info.pWeapon->SetEnable(true);
				}
			}

			if (bEnable)
			{
				m_info.pWeapon->Attack();
			}

			
		}
	}
}

//=====================================================
// �G���[�g�̑���
//=====================================================
void CPlayer::InputEmote(void)
{
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (pJoyPad == nullptr)
	{
		return;
	}
	int nID = m_info.nIDJoypad;

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_UP, nID))
	{// ��{�^���̃��[�V����
		SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_EMOTE00);
		SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_EMOTE00);

		m_info.motionInfo.bEmote = true;
		m_bSound = false;

		if (m_info.pWeapon != nullptr)
		{
			m_info.pWeapon->SetEnable(false);
		}
	}

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_LEFT, nID))
	{// ���{�^���̃��[�V����
		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_EMOTE_LEFT);

			m_bSound = true;
		}

		SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_EMOTE01);
		SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_EMOTE01);

		m_info.motionInfo.bEmote = true;
		m_bSound = false;

		if (m_info.pWeapon != nullptr)
		{
			m_info.pWeapon->SetEnable(false);
		}
	}

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_RIGHT, nID))
	{// �E�{�^���̃��[�V����
		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_EMOTE_RIGHT);

			m_bSound = true;
		}

		SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_EMOTE02);
		SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_EMOTE02);

		m_info.motionInfo.bEmote = true;
		m_bSound = false;

		if (m_info.pWeapon != nullptr)
		{
			m_info.pWeapon->SetEnable(false);
		}
	}

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, nID))
	{// ���{�^���̌ŗL�G���[�g
		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_EMOTE_DOWN);

			m_bSound = true;
		}

		SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_EMOTE_UNIQUE);
		SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_EMOTE_UNIQUE);

		m_info.motionInfo.bEmote = true;
		m_bSound = false;

		if (m_info.pWeapon != nullptr)
		{
			m_info.pWeapon->SetEnable(false);
		}
	}
}

//=====================================================
// �C���^���N�g�̓���
//=====================================================
bool CPlayer::InputInteract(void)
{
	bool bTrigger = false;

	CInputJoypad* pJoyPad = CInputJoypad::GetInstance();
	int nID = GetIDJoypad();

	if (pJoyPad == nullptr)
	{
		return bTrigger;
	}

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_LB, nID))
	{
		bTrigger = true;
	}

	return bTrigger;
}

//=====================================================
// �C���^���N�g�̒�����
//=====================================================
bool CPlayer::InputInteractPress(void)
{
	bool bPress = false;

	CInputJoypad* pJoyPad = CInputJoypad::GetInstance();
	int nID = GetIDJoypad();

	if (pJoyPad == nullptr)
	{
		return bPress;
	}

	if (pJoyPad->GetPress(CInputJoypad::PADBUTTONS_LB, nID))
	{
		bPress = true;
	}

	return bPress;
}

//=====================================================
// ����̗L����
//=====================================================
void CPlayer::EnableWeapon(bool bEnable)
{
	if (m_info.pWeapon != nullptr)
	{
		m_info.pWeapon->SetEnable(bEnable);
	}
}

//=====================================================
// �G�C�~���O�̏���
//=====================================================
void CPlayer::Aim(void)
{
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();
	
	if (pJoyPad == nullptr)
	{
		return;
	}

	// �v���C���[ID�擾
	int nId = m_info.nIDJoypad;

	D3DXVECTOR3 vecStickR =
	{// �X�e�B�b�N�̃x�N�g���擾
		pJoyPad->GetJoyStickRX(nId),
		pJoyPad->GetJoyStickRY(nId),
		0.0f,
	};

	float fLength = D3DXVec3Length(&vecStickR);

	float fAngleDest = 0.0f;

	if (fLength >= 0.3f)
	{// �E�X�e�B�b�N�𓮂����Ă���ꍇ
		// �p�x�̎擾
		fAngleDest = atan2f(vecStickR.x, vecStickR.y);
	}
	else
	{// �ړ��������Ă���ꍇ
		D3DXVECTOR3 move = GetMove();

		float fLengthMove = D3DXVec3Length(&move);

		if (fLengthMove >= 2.0f)
		{
			// �p�x�̎擾
			fAngleDest = atan2f(move.x, move.z);
		}
		else
		{
			return;
		}
	}

	// �p�x�␳
	D3DXVECTOR3 rot = GetRot();

	universal::LimitRot(&fAngleDest);

	universal::FactingRot(&rot.y, fAngleDest + D3DX_PI, m_param.fSpeedRot);

	SetRot(rot);
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CPlayer::ManageState(void)
{
	switch (m_info.state)
	{
	case STATE_NORMAL:
	{// �ʏ���

	}
		break;
	case STATE_DAMAGE:
	{// �_���[�W���
		float fTick = CManager::GetTick();

		m_info.fTimerState -= fTick;

		if (m_info.fTimerState <= 0.0f)
		{
			m_info.state = STATE_NORMAL;

			for (int nCutPath = 0; nCutPath < CCharacterDiv::PARTS_MAX; nCutPath++)
			{
				CMotionDiv* pBody = GetBody();

				if (pBody != nullptr)
				{
					pBody->ResetAllCol();
				}
			}
		}
	}
		break;
	case STATE_DEATH:
	{// ���S���

	}
		break;
	default:
		break;
	}
}

//=====================================================
// ���[�V�����Ǘ�
//=====================================================
void CPlayer::ManageMotion(void)
{
	if (GetBody() == nullptr)
	{
		return;
	}
	
	CInputJoypad* pJoyPad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (pJoyPad == nullptr)
	{
		return;
	}

	int nId = m_info.nIDJoypad;	// �v���C���[ID�擾

	// �ړ���
	D3DXVECTOR3 move = GetMove();
	float fSpeed = sqrtf(move.x * move.x + move.z * move.z);

	// ����
	float fRotPlayer = GetRot().y;
	float fRotMove = atan2f(-move.x, -move.z);
	float fRot = fRotMove - fRotPlayer;
	universal::LimitRot(&fRot);

	// �Q�[���p�b�h�̃X�e�B�b�N
	D3DXVECTOR3 vecStickL =
	{// �X�e�B�b�N�̃x�N�g���擾
		pJoyPad->GetJoyStickLX(nId),
		pJoyPad->GetJoyStickLY(nId),
		0.0f,
	};
	float fLength = D3DXVec3Length(&vecStickL);
	if (fLength >= 0.1f)
	{
		// �\�����͖h�~��ݒ�
		m_info.motionInfo.bRunawayProtect = true;
	}

	// ���[�V�����̔ԍ����擾
	int nMotionLower = GetMotion(CCharacterDiv::PARTS_LOWER);
	int nMotionUpper = GetMotion(CCharacterDiv::PARTS_UPPER);

	//CDebugProc::GetInstance()->Print("\n�X�s�[�h[%f]",fSpeed);

	// �����g�̃��[�V����
	{
		bool bFinish = GetBody()->IsFinish(CCharacterDiv::PARTS_LOWER);

		if (m_info.motionInfo.bEmote)
		{// �G���[�g�̑��ɒʂ�Ȃ�

		}
		else if (m_info.motionInfo.bDoorPress)
		{// �h�A�J���郂�[�V����
			if (nMotionLower != MOTION_OPEN_DOOR)
			{
				SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_OPEN_DOOR);
			}
		}
		// �������[�V����
		else
		{
			// �e�����w��ړ�
			if (fSpeed > MOVE_LINE)
			{
				if (fRot >= 0.0f && fRot <= (D3DX_PI * 0.25f) ||
					fRot <= 0.0f && fRot >= -(D3DX_PI * 0.25f))
				{// ��
					if (nMotionLower != MOTION_MAGNUM_WALK_FRONT)
					{
						SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_WALK_FRONT);
					}
				}
				else if (fRot >= (D3DX_PI * 0.75f) && fRot <= D3DX_PI ||
					fRot <= -(D3DX_PI * 0.75f) && fRot >= -D3DX_PI)
				{// ��
					if (nMotionLower != MOTION_MAGNUM_WALK_BACK)
					{
						SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_WALK_BACK);
					}
				}
				else if (fRot >= (D3DX_PI * 0.25f) && fRot <= (D3DX_PI * 0.75f))
				{// �E
					if (nMotionLower != MOTION_MAGNUM_WALK_RIGHT)
					{
						SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_WALK_RIGHT);
					}
				}
				else if (fRot <= -(D3DX_PI * 0.25f) && fRot >= -(D3DX_PI * 0.75f))
				{// ��
					if (nMotionLower != MOTION_MAGNUM_WALK_LEFT)
					{
						SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_WALK_LEFT);
					}
				}
			}
			else
			{
				if (nMotionLower != MOTION_MAGNUM_NEUTRAL)
				{// �ҋ@
					SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_NEUTRAL);
				}
			}
		}
	}

	// �㔼�g�̃��[�V����
	{
		bool bFinish = GetBody()->IsFinish(CCharacterDiv::PARTS_UPPER);
		int nKey = GetBody()->GetKey(CCharacterDiv::PARTS_UPPER);
		int nNumKey = GetBody()->GetMotionInfo(CCharacterDiv::PARTS_UPPER, nMotionUpper).nNumKey;

		if (m_info.motionInfo.bEmote)
		{// �G���[�g�̑��ɒʂ�Ȃ�
			if (bFinish)
			{
				m_info.motionInfo.bEmote = false;

				if (m_info.pWeapon != nullptr)
				{
					m_info.pWeapon->SetEnable(true);
				}
			}
		}
		// �h�A�J���郂�[�V����
		else if (m_info.motionInfo.bDoorPress)
		{
			if (nMotionUpper != MOTION_OPEN_DOOR)
			{
				SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_OPEN_DOOR);
			}
		}
		// ���E��
		else if (m_info.motionInfo.bItemTrigger ||
				 nMotionUpper == MOTION_ITEM_PICK_UP && bFinish == false && nKey != nNumKey)
		{
			if (nMotionUpper != MOTION_ITEM_PICK_UP)
			{
				SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_PICK_UP);
			}
		}
		// ���L�C���A�C�e��
		else if (m_info.pItemRepair != nullptr)
		{
			// �e�����w��ړ�
			if (fSpeed > MOVE_LINE)
			{
				if (fRot >= 0.0f && fRot <= (D3DX_PI * 0.25f) ||
					fRot <= 0.0f && fRot >= -(D3DX_PI * 0.25f) ||
					m_info.motionInfo.bRunawayProtect)
				{// ��
					if (nMotionLower != MOTION_ITEM_WALK_FRONT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_WALK_FRONT);
					}
				}
				else if (fRot >= (D3DX_PI * 0.75f) && fRot <= D3DX_PI ||
					fRot <= -(D3DX_PI * 0.75f) && fRot >= -D3DX_PI)
				{// ��
					if (nMotionLower != MOTION_ITEM_WALK_BACK)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_WALK_BACK);
					}
				}
				else if (fRot >= (D3DX_PI * 0.25f) && fRot <= (D3DX_PI * 0.75f))
				{// �E
					if (nMotionLower != MOTION_ITEM_WALK_RIGHT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_WALK_RIGHT);
					}
				}
				else if (fRot <= -(D3DX_PI * 0.25f) && fRot >= -(D3DX_PI * 0.75f))
				{// ��
					if (nMotionLower != MOTION_ITEM_WALK_LEFT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_WALK_LEFT);
					}
				}
			}
			else
			{
				// �ҋ@
				if (nMotionUpper != MOTION_ITEM_NEUTRAL)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_NEUTRAL);
				}
			}
		}
		else if (m_info.motionInfo.bPunch || (nMotionUpper == MOTION_PUNCH && bFinish == false))
		{// �p���`�̃��[�V����
			if (nMotionUpper != MOTION_PUNCH)
			{
				SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_PUNCH);

				if (pSound != nullptr)
				{
					pSound->Play(pSound->LABEL_SE_SWING);
				}
			}
		}
		// ���L����
		else if (m_info.pWeapon != nullptr)
		{
			if (m_info.motionInfo.bEmote == false)
			{
				m_info.pWeapon->SetEnable(true);
			}

			switch (m_info.pWeapon->GetType())
			{
				// �}�O�i��
			case CWeapon::TYPE_MAGNUM:

				if (nMotionUpper != MOTION_MAGNUM_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_MAGNUM_ATTACK);
				}

				break;

				// �}�V���K��
			case CWeapon::TYPE_MACHINEGUN:

				if (nMotionUpper != MOTION_MAGNUM_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_MAGNUM_ATTACK);
				}

				break;

				// �V���b�g�K��
			case CWeapon::TYPE_SHOTGUN:

				if (nMotionUpper != MOTION_RIFLE_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_RIFLE_ATTACK);
				}

				break;
				// ���[���K��
			case CWeapon::TYPE_RAILGUN:

				if (nMotionUpper != MOTION_RIFLE_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_RIFLE_ATTACK);
				}

				break;
				// �~�j�K��
			case CWeapon::TYPE_MINIGUN:

				if (nMotionUpper != MOTION_RIFLE_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_RIFLE_ATTACK);
				}

				break;
				// �����`���[
			case CWeapon::TYPE_LAUNCHER:

				if (nMotionUpper != MOTION_RIFLE_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_RIFLE_ATTACK);
				}

				break;
			}
		}
		// �����Ȃ�
		else
		{
			// �e�����w��ړ�
			if (fSpeed > MOVE_LINE)
			{
				if (fRot >= 0.0f && fRot <= (D3DX_PI * 0.25f) ||
					fRot <= 0.0f && fRot >= -(D3DX_PI * 0.25f))
				{// ��
					if (nMotionUpper != MOTION_WALK_FRONT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_WALK_FRONT);
					}
				}
				else if (fRot >= (D3DX_PI * 0.75f) && fRot <= D3DX_PI ||
						 fRot <= -(D3DX_PI * 0.75f) && fRot >= -D3DX_PI)
				{// ��
					if (nMotionUpper != MOTION_WALK_BACK)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_WALK_BACK);
					}
				}
				else if (fRot >= (D3DX_PI * 0.25f) && fRot <= (D3DX_PI * 0.75f))
				{// �E
					if (nMotionUpper != MOTION_WALK_RIGHT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_WALK_RIGHT);
					}
				}
				else if (fRot <= -(D3DX_PI * 0.25f) && fRot >= -(D3DX_PI * 0.75f))
				{// ��
					if (nMotionUpper != MOTION_WALK_LEFT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_WALK_LEFT);
					}
				}
			}
			else
			{
				// �ҋ@
				if (nMotionUpper != MOTION_NEUTRAL)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_NEUTRAL);
				}
			}
		}
	}

	// ���[�V�������̏�����
	m_info.motionInfo.bDoorPress = false;			// �h�A�ւ̓��͏��
	m_info.motionInfo.bItemTrigger = false;			// ���ւ̓��͏��
	m_info.motionInfo.bRunawayProtect = false;		// �\�����͏��
}

//=====================================================
// �U������̊Ǘ�
//=====================================================
void CPlayer::ManageAttack(void)
{
	if (m_info.pClsnAttack == nullptr)
	{// ����̃G���[
		return;
	}

	CMotionDiv *pBody = GetBody();

	if (pBody == nullptr)
	{// ����̃G���[
		return;
	}

	// ��т̎擾����
	CRecord* pRecord = CRecord::GetInstance();

	for (int i = 0; i < m_info.nNumAttack; i++)
	{
		if (pBody->GetMotion(CCharacterDiv::PARTS_UPPER) == m_info.pAttackInfo[i].nIdxMotion)
		{// �U�����[�V�������̔���
			int nFrame = pBody->GetFrame(CCharacterDiv::PARTS_UPPER);
			int nKey = pBody->GetKey(CCharacterDiv::PARTS_UPPER);
			D3DXVECTOR3 pos;

			if (nFrame == m_info.pAttackInfo[i].nFrame && nKey == m_info.pAttackInfo[i].nKey)
			{// �����蔻��̐ݒ�
				bool bHit = false;
				D3DXMATRIX mtx;
				D3DXMATRIX mtxPart = *pBody->GetMatrix();

				universal::SetOffSet(&mtx, mtxPart, m_info.pAttackInfo[i].pos);

				pos =
				{
					mtx._41,
					mtx._42,
					mtx._43
				};

				// �ʒu�ݒ�
				m_info.pClsnAttack->SetPosition(pos);

				// ���a�̐ݒ�
				m_info.pClsnAttack->SetRadius(m_info.pAttackInfo[i].fRadius);

#ifdef _DEBUG
				CEffect3D::Create(pos, m_info.pAttackInfo[i].fRadius, 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
				// �G�Ƃ̔���
				bHit = m_info.pClsnAttack->OnEnter(CCollision::TAG_ENEMY);

				// ���������I�u�W�F�N�g�̎擾
				CObject *pObj = m_info.pClsnAttack->GetOther();

				if (bHit == true && pObj != nullptr)
				{// �G�̃q�b�g����
					// �A�j���[�V�����G�t�F�N�g�̐���
					CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

					if (pAnimManager != nullptr)
					{
						pAnimManager->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_HIT00);
					}

					// �G���m�b�N�o�b�N������
					BlowEnemy(pObj);

					// �q�b�g����
					pObj->Hit(m_param.fDamagePunch);

					// �j�󐔂̐�щ��Z����
					if (pRecord != nullptr)
					{
						pRecord->CheckDeathEnemy(pObj, GetID());
					}
				}

				// �ؔ��Ƃ̔���
				bHit = m_info.pClsnAttack->OnEnter(CCollision::TAG_BOX);

				pObj = m_info.pClsnAttack->GetOther();

				if (bHit == true && pObj != nullptr)
				{// �ؔ��̃q�b�g����
					pObj->Hit(0.0f);
				}

				// �v���C���[�Ƃ̔���
				bHit = m_info.pClsnAttack->OnEnter(CCollision::TAG_PLAYER);

				pObj = m_info.pClsnAttack->GetOther();

				if (bHit == true && pObj != nullptr)
				{// �v���C���[�𐁂���΂�
					// �A�j���[�V�����G�t�F�N�g�̐���
					CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

					// �j�󐔂̐�щ��Z����
					if (pRecord != nullptr)
					{
						pRecord->AddMedman(GetID());
					}

					if (pAnimManager != nullptr)
					{
						pAnimManager->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_HIT00);
					}

					BlowPlayer(pObj);
				}
			}
		}
	}
}

//=====================================================
// ���C�t�\���̊Ǘ�
//=====================================================
void CPlayer::ManageLife(void)
{
	if (m_info.pLife == nullptr || m_info.pLifeFrame == nullptr)
	{
		return;
	}

	// �ʒu�̐ݒ�
	D3DXVECTOR3 pos = GetPosition();
	pos += POS_LIFE;

	m_info.pLife->SetPosition(pos);
	m_info.pLifeFrame->SetPosition(pos);

	// �F�̐ݒ�
	D3DXCOLOR col = m_info.pLife->GetColor();

	col.a -= SPEED_LIFE_FADE;

	m_info.pLife->SetColor(col);
	m_info.pLifeFrame->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, col.a));

	// �T�C�Y����
	ResizeLife();

	if (col.a <= 0.0f)
	{// �����ɂȂ�����폜
		DeleteLife();
	}
}

//=====================================================
// ���C�t�\���̃T�C�Y����
//=====================================================
void CPlayer::ResizeLife(void)
{
	if (m_info.pLife == nullptr)
	{
		return;
	}

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr)
	{
		return;
	}

	float fInitialLife = pPlayerManager->GetPlayerParam().fInitialLife;

	float fRate = m_info.fLife / fInitialLife;

	m_info.pLife->SetSize(SIZE_LIFE * fRate, SIZE_LIFE * fRate);
}

//=====================================================
// �G�𐁂���΂�����
//=====================================================
void CPlayer::BlowEnemy(CObject *pObj)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr || pObj == nullptr)
	{
		return;
	}

	CEnemy *pEnemy = pEnemyManager->GetHead();

	while (pEnemy != nullptr)
	{
		CEnemy *pEnemyNext = pEnemy->GetNext();

		if ((CObject*)pEnemy == pObj)
		{
			// �G�ƃv���C���[�̈ʒu�̍������擾
			D3DXVECTOR3 posEnemy = pEnemy->GetPosition();
			D3DXVECTOR3 moveEnemy = pEnemy->GetMove();
			D3DXVECTOR3 posPlayer = GetPosition();
			D3DXVECTOR3 vecDiff = posEnemy - posPlayer;

			// �����x�N�g�����ӂ��Ƃ΂��͂ɕϊ�
			D3DXVECTOR3 vecBlow;

			D3DXVec3Normalize(&vecBlow, &vecDiff);

			vecBlow *= m_param.fPowBlow;

			// ������΂��x�N�g�����ړ��ʂɉ��Z
			moveEnemy += vecBlow;

			pEnemy->SetMove(moveEnemy);
		}

		pEnemy = pEnemyNext;
	}
}

//=====================================================
// �v���C���[�𐁂���΂�����
//=====================================================
void CPlayer::BlowPlayer(CObject *pObj)
{
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr || pObj == nullptr)
	{
		return;
	}

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			if ((CObject*)pPlayer == pObj)
			{
				// ����Ǝ����̈ʒu�̍������擾
				D3DXVECTOR3 posOther = pPlayer->GetPosition();
				D3DXVECTOR3 moveOther = pPlayer->GetMove();
				D3DXVECTOR3 posOwn = GetPosition();
				D3DXVECTOR3 vecDiff = posOther - posOwn;

				// �����x�N�g�����ӂ��Ƃ΂��͂ɕϊ�
				D3DXVECTOR3 vecBlow;

				D3DXVec3Normalize(&vecBlow, &vecDiff);

				vecBlow *= m_param.fPowBlow;
				vecBlow.y = POW_PUNCH_UP;

				// ������΂��x�N�g�����ړ��ʂɉ��Z
				moveOther += vecBlow;

				pPlayer->SetMove(moveOther);

				pPlayer->SetState(STATE_BLOW);
			}
		}
	}
}

//=====================================================
// ���C�t�̕\��
//=====================================================
void CPlayer::DispLife(void)
{
	D3DXVECTOR3 pos = GetPosition();
	pos += POS_LIFE;

	if (m_info.pLifeFrame == nullptr)
	{
		m_info.pLifeFrame = CObject3D::Create(pos);

		if (m_info.pLifeFrame != nullptr)
		{
			m_info.pLifeFrame->EnableZtest(true);
			m_info.pLifeFrame->EnableBillboard(true);
			m_info.pLifeFrame->SetSize(SIZE_LIFE, SIZE_LIFE);
			m_info.pLifeFrame->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			m_info.pLifeFrame->SetPosition(pos);

			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\life000.png");
			m_info.pLifeFrame->SetIdxTexture(nIdx);
		}
	}

	if (m_info.pLife == nullptr)
	{
		m_info.pLife = CObject3D::Create(pos);

		if (m_info.pLife != nullptr)
		{
			m_info.pLife->EnableZtest(true);
			m_info.pLife->EnableBillboard(true);
			m_info.pLife->SetSize(SIZE_LIFE, SIZE_LIFE);
			m_info.pLife->SetPosition(pos);

			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\life000.png");
			m_info.pLife->SetIdxTexture(nIdx);

			// �����T�C�Y����
			ResizeLife();
		}
	}
}

//=====================================================
// ���C�t�\���̍폜
//=====================================================
void CPlayer::DeleteLife(void)
{
	if (m_info.pLife != nullptr)
	{
		m_info.pLife->Uninit();
		m_info.pLife = nullptr;
	}

	if (m_info.pLifeFrame != nullptr)
	{
		m_info.pLifeFrame->Uninit();
		m_info.pLifeFrame = nullptr;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �p���N���X�̕`��
	CCharacterDiv::Draw();

	// �f�o�b�O�\��
	Debug();
}

//=====================================================
// �ʒu�ݒ�
//=====================================================
void CPlayer::SetPosition(D3DXVECTOR3 pos)
{
	CCharacterDiv::SetPosition(pos);

	if (m_info.pArrow != nullptr)
	{// ���̒Ǐ]
		D3DXVECTOR3 rot = GetRot();
		D3DXVECTOR3 posArrow = pos;

		posArrow.y = ARROW_POSY;

		m_info.pArrow->SetPosition(posArrow);
		m_info.pArrow->SetRot(rot);
	}
}

//=====================================================
// ����ݒ�
//=====================================================
void CPlayer::SetWeapon(CWeapon::TYPE type)
{
	if (m_info.pWeapon != nullptr)
	{
		m_info.pWeapon->Uninit();
		m_info.pWeapon = nullptr;
	}

	m_info.pWeapon = CWeapon::Create(type, HAND_PARTS_NUM);

	if (m_info.pWeapon != nullptr)
	{
		m_info.pWeapon->SetPlayer(this);
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CPlayer::Hit(float fDamage)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL)
		{
			return;
		}
	}

	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();

	if (m_info.state == STATE_NORMAL)
	{
		m_info.fLife -= fDamage;

		if (m_info.fLife <= 0.0f)
		{// ���S����
			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			// ���S�����v���C���[�̔ԍ���n��
			if (pPlayerManager != nullptr)
			{
				pPlayerManager->SetDeathPlayer(m_info.nID, true);
			}

			Uninit();

			int nIdx = GetID();

			// �H��̐���
			CGhost *pGhost = CGhost::Create(nIdx);

			if (pGhost != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();

				pGhost->SetPosition(pos);
			}
		}
		else
		{// �_���[�W����
			m_info.state = STATE_DAMAGE;

			CSound* pSound = CSound::GetInstance();

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_DAMAGE);
			}

			m_info.fTimerState = m_param.fTimeDamage;

			for (int nCutPath = 0; nCutPath < CCharacterDiv::PARTS_MAX; nCutPath++)
			{
				CMotionDiv* pBody = GetBody();

				if (pBody != nullptr)
				{
					pBody->SetAllCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}

			// �R���g���[���[�̐U��
			CInputJoypad *pJoypad = CInputJoypad::GetInstance();

			if (pJoypad != nullptr)
			{
				int nIdxJoypad = GetIDJoypad();

				pJoypad->Vibration(nIdxJoypad,CInputJoypad::PADVIB::PADVIB_USE,1.0f,20);
			}

			// �c�胉�C�t�̕\��
			DispLife();
		}
	}
}

//=====================================================
// �v���C���[�̈ړ�����
//=====================================================
void CPlayer::LimidPostion(void)
{
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		CPlayer* pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			D3DXVECTOR3 pos = pPlayer->GetPosition();

			if (pos.x <= LIMID_RANGE_LEFT)
			{
				pos.x = LIMID_RANGE_LEFT;
			}
			else if (pos.x >= LIMID_RANGE_RIGHT)
			{
				pos.x = LIMID_RANGE_RIGHT;
			}
			else if (pos.z >= LIMID_RANGE_UP)
			{
				pos.z = LIMID_RANGE_UP;
			}
			else if (pos.z <= LIMID_RANGE_DOWN)
			{
				pos.z = LIMID_RANGE_DOWN;
			}

			pPlayer->SetPosition(pos);
		}
	}
}

//=====================================================
// ���P�b�g�ɏ�荞��
//=====================================================
void CPlayer::BoardingRocket(void)
{
	// �C���X�^���X���擾
	CGame* pGame = CGame::GetInstance();
	CRocket* pRocket = CRocket::GetInstance();
	CGoal* pGoal = CGoal::GetInstance();

	if (pGoal != nullptr)
	{// �S�[�����Ƀv���C���[�����邩
		// ���W�̍������狗�����v�Z
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posGoal = pGoal->GetPosition();
		D3DXVECTOR3 vecDiff = pos - posGoal;
		float fDiff = D3DXVec3Length(&vecDiff);
		float fRadius = pGoal->GetRadius();

		if (fDiff < fRadius)
		{
			if (pGame != nullptr)
			{
				CGame::STATE state = pGame->GetState();

				if (state == CGame::STATE_ESCAPE)
				{// �E�o���
					if (pRocket != nullptr)
					{
						// �ړ��ʂ̐ݒ�
						D3DXVECTOR3 posRocket = pRocket->GetPosition();
						D3DXVECTOR3 vecDiff = posRocket - pos;

						pos.x += vecDiff.x * 0.2f;
						pos.z += vecDiff.z * 0.2f;

						D3DXVec3Normalize(&vecDiff, &vecDiff);

						vecDiff *= 0.08f;

						SetPosition(pos);

						// ������ڕW�����ɕ␳
						float fAngleDist = atan2f(vecDiff.x, vecDiff.z);
						D3DXVECTOR3 rot = GetRot();

						fAngleDist += D3DX_PI;

						universal::FactingRot(&rot.y, fAngleDist, 0.1f);

						SetRot(rot);

						D3DXVECTOR3 vecDiffDelete = pos - posRocket;
						float fLenth = D3DXVec3Length(&vecDiffDelete);

						if (fLenth < 20.0f)
						{// ���P�b�g�ɋ߂�
							SetPosition({ pos.x, -100.0f, pos.z });		// ���߂�
						}

						if(m_bGoalJump == false)
						{// �W�����v������
							pos.y = posRocket.y + 250.0f - pos.y;
							SetPosition(pos);
							D3DXVECTOR3 move = GetMove();
							SetMove({ move.x, 0.0f, move.z });
							m_bGoalJump = true;
						}
					}

					if (m_info.pArrow != nullptr)
					{// ���̍폜
						m_info.pArrow->Uninit();
						m_info.pArrow = nullptr;
					}
				}
			}
		}
	}
}

//=====================================================
// �f�o�b�O�\��
//=====================================================
void CPlayer::Debug(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\n�v���C���[�ԍ�[%d]", m_info.nID);
	pDebugProc->Print("\n�v���C���[��ԃJ�E���^[%f]", m_info.fTimerState);
	pDebugProc->Print("\n�v���C���[�̈ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\n�v���C���[�̈ړ���[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);

	if (GetBody() != nullptr)
	{
		int nMotion = GetBody()->GetMotion(CCharacterDiv::PARTS_LOWER);

		pDebugProc->Print("\n�����g�̃��[�V����[%d]", nMotion);
	}

	if (GetBody() != nullptr)
	{
		int nMotion = GetBody()->GetMotion(CCharacterDiv::PARTS_UPPER);

		pDebugProc->Print("\n�㔼�g�̃��[�V����[%d]", nMotion);
	}
}