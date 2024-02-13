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
#include "enemyManager.h"
#include "collision.h"
#include "motion.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "universal.h"
#include "slow.h"
#include "camera.h"
#include "manager.h"
#include "bullet.h"
#include "effect3D.h"
#include "meshfield.h"
#include "cameraBehavior.h"
#include "heat.h"
#include "particle.h"
#include "game.h"
#include "boostEffect.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* BODY_PATH = "data\\MOTION\\motionArms01.txt";	// �����ڂ̃p�X
const float INITIAL_BOOST = 200.0f;	// �u�[�X�g�c�ʂ̏����l
const float REGEN_BOOST = 2.5f;	// �u�[�X�g�񕜗�
const float GRAVITY = 0.50f;	// �d��
const float SPEED_ROLL_CAMERA = 0.03f;	// �J������]���x
const float SPEED_BULLET = 150.0f;	// �e��
const float POW_JUMP = 20.0f;	// �W�����v��
const float POW_STAMP = 30.0f;	// ���݂��̐��i��
const float SPEED_STAMP = 70.0f;	// ���݂��������i��
const float SPEED_MOVE = 1.6f;	// �ړ����x
const float FACT_MOVE = 0.04f;	// �ړ��̌����W��
const float SPEED_ASSAULT = 4.0f;	// �ːi�̈ړ����x
const float POW_ADDMELEE = 50.0f;	// �ǌ��̐��i��
const float SPEED_DODGE = 50.0f;	// ��𐄐i��
const float POW_GRAB = 50.0f;	// �݂͂̐��i��
const float RADIUS_GRAB = 500.0f;	// �݂͂̔���
const float POW_THROW = 200.0f;	// �����̗�
const float LENGTH_LOCKON = 5000.0f;	// ���b�N�I���̒���
const float ANGLE_LOCKON = D3DX_PI * 0.2f;	// ���b�N�I���̊p�x
const float MELEE_DIST = 800.0f;	// �i���Ɉڂ鋗��
const float MIN_ANGLE_CAMERA = D3DX_PI * 0.2f;	// �J�����̉������鐧��
const float MAX_ANGLE_CAMERA = D3DX_PI * 0.7f;	// �J�����̏�����鐧��
const float DAMAGE_BULLET = 1.0f;	// �e�̈З�
const float DECREASE_PARAM = 2.0f;	// �p�����[�^�S�񕜂ɂ����鎞��
const D3DXVECTOR3 POS_PARAM[CPlayer::PARAM_MAX] =
{// �p�����[�^�\���̈ʒu
	{SCREEN_WIDTH * 0.5f - 320.0f,SCREEN_HEIGHT * 0.5f - 100.0f,0.0f},// �e
	{SCREEN_WIDTH * 0.5f + 320.0f,SCREEN_HEIGHT * 0.5f - 100.0f,0.0f},// �ߐ�
	{SCREEN_WIDTH * 0.5f + 320.0f,SCREEN_HEIGHT * 0.5f + 100.0f,0.0f},// �͂�
};
const char* PATH_PARAM[CPlayer::PARAM_MAX] =
{// �p�����[�^UI�̃e�N�X�`���p�X
	"data\\TEXTURE\\UI\\frame00.png",
	"data\\TEXTURE\\UI\\frame01.png",
	"data\\TEXTURE\\UI\\frame02.png",
};
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPlayer *CPlayer::m_pPlayer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_pPlayer = this;

	ZeroMemory(&m_info, sizeof(CPlayer::SInfo));
	ZeroMemory(&m_param, sizeof(CPlayer::SParam));
	ZeroMemory(&m_fragMotion, sizeof(CPlayer::SFragMotion));
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
CPlayer *CPlayer::Create(void)
{
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init();
		}
	}

	return m_pPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayer::Init(void)
{
	// ���f���̐ݒ�
	CMotion::Load((char*)BODY_PATH);

	// �p���N���X�̏�����
	CMotion::Init();

	// �����蔻��̐���
	if (m_info.pCollisionSphere == nullptr)
	{
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(50.0f);
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

	// �p�����[�^�[�ɏ����l������
	m_param.fInitialLife = 300.0f;
	m_info.fLife = m_param.fInitialLife;
	m_param.fSpeedMove = SPEED_MOVE;
	m_param.fInitialBoost = INITIAL_BOOST;
	m_info.fBoost = m_param.fInitialBoost;
	m_info.state = STATE_NORMAL;
	m_info.stateBoost = STATEBOOST_NORMAL;
	m_info.bLand = true;

	// �e�̗L����
	SetPosShadow(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
	EnableShadow(true);

	SetMotion(MOTION_WALK_FRONT);

	// �J�����̍s���ݒ�
	Camera::ChangeBehavior(new CFollowPlayer);

	// UI�̐���
	for (int i = 0; i < PARAM_MAX; i++)
	{
		m_info.apHeatUI[i] = CHeat::Create();

		if (m_info.apHeatUI[i] != nullptr)
		{
			m_info.apHeatUI[i]->SetPosition(POS_PARAM[i]);
			m_info.apHeatUI[i]->BindTextureFrame(PATH_PARAM[i]);
		}
	}

	// �Ǎ�
	Load();

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CPlayer::Load(void)
{
	char cTemp[256];
	int nThruster = 0;

	FILE *pFile = fopen("data\\TEXT\\playerParam.txt", "r");

	if (pFile != nullptr)
	{
		while (true)
		{
			// �����œǂݍ��񂾕����ɂ���ĉ��L��IF�ɂ�����
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "NUM_THRUSTER") == 0)
			{// �X���X�^���Ǎ�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.nNumThruster);

				if (m_info.pThruster == nullptr)
				{
					m_info.pThruster = new SInfoThruster[m_info.nNumThruster];

					if (m_info.pThruster != nullptr)
					{
						ZeroMemory(m_info.pThruster, sizeof(SInfoThruster) * m_info.nNumThruster);
					}
				}
			}

			if (strcmp(cTemp, "THRUSTERSET") == 0)
			{
				while (true)
				{
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PARENT") == 0)
					{// �e�ԍ�
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_info.pThruster[nThruster].nIdxParent);
					}

					if (strcmp(cTemp, "OFFSET") == 0)
					{// �I�t�Z�b�g�ʒu
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.y);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.z);
					}

					if (strcmp(cTemp, "VECTOR") == 0)
					{// �x�N�g��
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.y);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.z);
					}

					if (strcmp(cTemp, "SIZE") == 0)
					{// �T�C�Y
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].size.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].size.y);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// ����
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_info.pThruster[nThruster].nRot);
					}

					if (strcmp(cTemp, "END_THRUSTER") == 0)
					{
						if (m_info.pThruster[nThruster].pFire == nullptr)
						{
							m_info.pThruster[nThruster].pFire = CBoostEffect::Create();
						
							if (m_info.pThruster[nThruster].pFire != nullptr)
							{
								m_info.pThruster[nThruster].pFire->SetRadius(m_info.pThruster[nThruster].size.x);
								m_info.pThruster[nThruster].pFire->SetHeight(m_info.pThruster[nThruster].size.y);
							}
						}

						nThruster++;

						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		fclose(pFile);
	}
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

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

	if (m_info.pClsnAttack != nullptr)
	{
		m_info.pClsnAttack->Uninit();
		m_info.pClsnAttack = nullptr;
	}

	for (int i = 0; i < PARAM_MAX; i++)
	{
		if (m_info.apHeatUI[i] != nullptr)
		{
			m_info.apHeatUI[i]->Uninit();
			m_info.apHeatUI[i] = nullptr;
		}
	}

	if (m_info.pThruster != nullptr)
	{
		for (int i = 0; i < m_info.nNumThruster; i++)
		{
			if (m_info.pThruster[i].pFire != nullptr)
			{
				m_info.pThruster[i].pFire->Uninit();
				m_info.pThruster[i].pFire = nullptr;
			}
		}
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
	CSlow *pSlow = CSlow::GetInstance();

	// �p���N���X�̍X�V
	CMotion::Update();

	// ���b�N�I��
	Lockon();

	// ����
	Input();

	// �v���C���[�̉�]
	Rotation();

	// �ʒu�̔��f
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();
	
	// �O��̈ʒu��ۑ�
	SetPositionOld(pos);

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		pos += move * fScale;
		SetPosition(pos);
	}
	else
	{
		pos += move;
		SetPosition(pos);
	}

	// �d��
	int nMotion = GetMotion();
	
	if (nMotion != MOTION_SHOT && 
		nMotion != MOTION_ASSAULT && 
		nMotion != MOTION_MELEE && 
		nMotion != MOTION_MELEE2 &&
		nMotion != MOTION_GRAB &&
		nMotion != MOTION_THROW &&
		m_fragMotion.bStamp == false)
	{
		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			move.x += (0 - move.x) * FACT_MOVE * fScale;
			move.z += (0 - move.z) * FACT_MOVE * fScale;

			move.y -= GRAVITY * fScale;
		}
		else
		{
			move.x *= FACT_MOVE;
			move.z *= FACT_MOVE;

			move.y -= GRAVITY;
		}
	}
	else
	{
		move.x += (0 - move.x) * 0.05f;
		move.y += (0 - move.y) * 0.5f;
		move.z += (0 - move.z) * 0.05f;
	}

	SetMove(move);

	// �����蔻��̊Ǘ�
	ManageCollision();

	// ��ԊǗ�
	ManageState();

	// ���[�V�����Ǘ�
	ManageMotion();

	// �p�����[�^�[�Ǘ�
	ManageParam();

	if (m_info.pEnemyGrab != nullptr)
	{// ��ɒǏ]
		D3DXVECTOR3 offset = { 0.0f,-100.0f,0.0f };
		D3DXMATRIX mtxParent;
		D3DXMATRIX mtxPart = *GetParts(5)->pParts->GetMatrix();

		universal::SetOffSet(&mtxParent, mtxPart, offset);

		CDebugProc::GetInstance()->Print("\n�݈͂ʒu[%f,%f,%f]", mtxParent._41, mtxParent._42, mtxParent._43);

		m_info.pEnemyGrab->SetMatrix(mtxParent);
	}

	if (m_info.bLand)
	{
		// �u�[�X�g��
		AddBoost(REGEN_BOOST);
	}
	else
	{
		// �u�[�X�g��
		AddBoost(REGEN_BOOST * 0.2f);
	}
	
	if (nMotion == MOTION_WALK_FRONT)
	{// �����ɍ������o��
		D3DXVECTOR3 posToe = GetMtxPos(11);
		CParticle::Create(posToe, CParticle::TYPE::TYPE_SAND_SMOKE);

		posToe = GetMtxPos(14);
		CParticle::Create(posToe, CParticle::TYPE::TYPE_SAND_SMOKE);
	}

	// �u�[�X�g�G�t�F�N�g����
	Boost();

// �f�o�b�O����
#if _DEBUG

#endif // _DEBUG
}

//=====================================================
// ���b�N�I��
//=====================================================
void CPlayer::Lockon(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return;
	}

	// �G�����b�N�I��
	pEnemyManager->Lockon(m_info.pEnemyGrab);
}

//=====================================================
// ���͏���
//=====================================================
void CPlayer::Input(void)
{
	// �ړ�����
	InputMove();

	// �J��������
	InputCamera();

	// �U������
	InputAttack();
}

//=====================================================
// �ړ��̓���
//=====================================================
void CPlayer::InputMove(void)
{
	CSlow *pSlow = CSlow::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}

	// �J�����擾
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// �ڕW�����̐ݒ�
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// ���͕����̎擾
	D3DXVECTOR3 vecInput = { 0.0f,0.0f,0.0f };

	vecInput += {sinf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x, 0.0f, cosf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x};
	vecInput += {sinf(pInfoCamera->rot.y) * axis.axisMove.z, 0.0f, cosf(pInfoCamera->rot.y) * axis.axisMove.z};

	float fLengthAxis = D3DXVec3Length(&axisMove);

	int nMotion = GetMotion();

	if (fLengthAxis >= 0.3f && nMotion != MOTION_SHOT)
	{// �ʏ�ړ����̖ڕW�����ݒ�
		m_info.rotDest.y = atan2f(vecInput.x, vecInput.z);

		CDebugProc *pDebugProc = CDebugProc::GetInstance();

		pDebugProc->Print("\n�ʏ�ړ�");
	}

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT || nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2 || nMotion == MOTION_THROW || m_info.bLockTarget)
	{// �G�̕�������������
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

			if (pEnemyLockon != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();
				pos.y += 200.0f;

				D3DXVECTOR3 posEnemy = pEnemyLockon->GetMtxPos(0);

				D3DXVECTOR3 vecDiff = posEnemy - pos;

				m_info.rotDest = universal::VecToRot(vecDiff);
				m_info.rotDest.x -= D3DX_PI * 0.5f;
			}
		}
	}

	if (nMotion != MOTION_ASSAULT &&
		nMotion != MOTION_MELEE &&
		nMotion != MOTION_MELEE2 &&
		nMotion != MOTION_GRAB &&
		nMotion != MOTION_THROW &&
		m_fragMotion.bStamp == false)
	{
		// �������͂̎擾
		CInputManager::SAxis axis = pInputManager->GetAxis();
		D3DXVECTOR3 axisMove = axis.axisMove;

		D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 rot = GetRotation();

		float fLengthAxis = D3DXVec3Length(&axisMove);

		if (m_fragMotion.bMove && fLengthAxis <= 0.3f)
		{// �}��~�t���O
			m_fragMotion.bStop = true;
		}

		fLengthAxis *= SPEED_MOVE;

		if (nMotion == MOTION_SHOT || m_info.bLockTarget)
		{// ���_�Œ莞
			D3DXVECTOR3 rotCamera = pInfoCamera->rot;

			vecMove += {sinf(rotCamera.y + D3DX_PI * 0.5f) * axisMove.x, 0.0f, cosf(rotCamera.y + D3DX_PI * 0.5f) * axisMove.x};
			vecMove += {sinf(rotCamera.y) * axisMove.z, 0.0f, cosf(rotCamera.y) * axisMove.z};
		}
		else
		{// ���������R�Ȏ�
			vecMove -= {sinf(rot.y) * fLengthAxis, 0.0f, cosf(rot.y) * fLengthAxis};
		}

		// �ړ����x�̐ݒ�
		D3DXVECTOR3 move = GetMove();

		D3DXVec3Normalize(&vecMove, &vecMove);
		vecMove *= m_param.fSpeedMove;

		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			vecMove *= fScale;
		}

		if (m_info.bLand)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_JUMP))
			{// �W�����v����
				m_fragMotion.bJump = true;
				m_fragMotion.bMove = false;
			};
		}
		else
		{
			if (m_info.stateBoost != STATEBOOST_OVERHEAT)
			{
				if (pInputManager->GetPress(CInputManager::BUTTON_JUMP))
				{// �u�[�X�g�㏸
					vecMove.y += 1.0f;

					AddBoost(-3.0f);
				};
			}

			Stamp();
		}
		
		float fAngleInput = atan2f(axisMove.x, axisMove.z);

		if (m_info.stateBoost != STATEBOOST_OVERHEAT)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_DODGE))
			{// �u�[�X�g���
				vecMove +=
				{
					sinf(pInfoCamera->rot.y + fAngleInput) * SPEED_DODGE,
					0.0f,
					cosf(pInfoCamera->rot.y + fAngleInput) * SPEED_DODGE,
				};

				AddBoost(-50.0f);
			}
		}

		move += vecMove;

		SetMove(move);
	}
	else if (nMotion == MOTION_ASSAULT)
	{
		AddMoveForward(SPEED_ASSAULT);
	}

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// �X���[�ɂ���
			if (pSlow != nullptr)
			{
				pSlow->SetScale(0.25f);
			}
		}

		if (pKeyboard->GetRelease(DIK_RETURN))
		{// �X���[����
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}

		if (pKeyboard->GetTrigger(DIK_O))
		{// ���p�[�c���Z�b�g
			ResetEnableMotion();
		}
	}
#endif
}

//=====================================================
// ���݂�
//=====================================================
void CPlayer::Stamp(void)
{
	if (m_info.pClsnAttack == nullptr || m_fragMotion.bStamp)
	{// ����̃G���[
		return;
	}

	D3DXVECTOR3 pos = GetPosition();

	m_info.pClsnAttack->SetRadius(100.0f);
	m_info.pClsnAttack->SetPosition(pos);

	if (m_info.pClsnAttack->IsTriggerEnter(CCollision::TAG::TAG_ENEMY))
	{// �ΏۂƂ̓����蔻��
		CObject *pObj = m_info.pClsnAttack->GetOther();

		if (pObj != nullptr)
		{// ���߂�G���`�F�b�N
			CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

			if (pEnemyManager == nullptr)
			{
				return;
			}

			CEnemy *pEnemy = pEnemyManager->GetHead();

			while (pEnemy != nullptr)
			{
				CEnemy *pEnemyNext = pEnemy->GetNext();

				if ((CObject*)pEnemy == pObj)
				{
					bool bStamp = pEnemy->IsStamp();

					if (bStamp)
					{
						m_fragMotion.bStamp = true;

						D3DXVECTOR3 move = GetMove();

						move *= 0.0;

						SetMove(move);

						// ���������I�u�W�F�N�g�̃q�b�g����
						pObj->Hit(5.0f);
					}
				}

				pEnemy = pEnemyNext;
			}
		}
	}
}

//=====================================================
// �J��������
//=====================================================
void CPlayer::InputCamera(void)
{
	// ���̓}�l�[�W���[�擾
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}

	// �J�����擾
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	
	if (pInputManager->GetTrigger(CInputManager::BUTTON_LOCK))
	{// �^�[�Q�b�g���b�N�؂�ւ�
		m_info.bLockTarget = m_info.bLockTarget ? false : true;

		ToggleLockTarget();
	}
	
	if (m_info.bLockTarget)
	{// ���b�N���Ă�G�̐؂�ւ�
		SwitchLockEnemy();
	}

	// �������͂̎擾
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisCamera = axis.axisCamera;

	// �J�����̉�]
	pInfoCamera->rot.x += axisCamera.y * SPEED_ROLL_CAMERA;
	pInfoCamera->rot.y += axisCamera.x * SPEED_ROLL_CAMERA;

	universal::LimitValue(&pInfoCamera->rot.x, MAX_ANGLE_CAMERA, MIN_ANGLE_CAMERA);

	universal::LimitRot(&pInfoCamera->rot.x);
	universal::LimitRot(&pInfoCamera->rot.y);
}

//=====================================================
// ���b�N���Ă�G�̐؂�ւ�
//=====================================================
void CPlayer::SwitchLockEnemy(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pInputManager == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_RIGHT))
	{// �E����
		pEnemyManager->SwitchTarget(1, 0,m_info.pEnemyGrab);
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_LEFT))
	{// ������
		pEnemyManager->SwitchTarget(-1, 0, m_info.pEnemyGrab);
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_UP))
	{// �����
		pEnemyManager->SwitchTarget(0, 1, m_info.pEnemyGrab);
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_DOWN))
	{// ������
		pEnemyManager->SwitchTarget(0, -1, m_info.pEnemyGrab);
	}
}

//=====================================================
// �U���̓���
//=====================================================
void CPlayer::InputAttack(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	int nMotion = GetMotion();

	if (pInputManager == nullptr)
	{
		return;
	}

	if (pInputManager->GetPress(CInputManager::BUTTON_SHOT) && 
		m_info.aHeat[PARAM_GUN] == false)
	{// �ˌ�����
		m_fragMotion.bShot = true;
	}
	else
	{
		m_fragMotion.bShot = false;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_MELEE) &&
		m_info.aHeat[PARAM_MELEE] == false)
	{// �ߐڍU������
		m_fragMotion.bMelee = true;

		if (nMotion == MOTION_MELEE)
		{
			m_fragMotion.bAddAttack = true;
		}
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_GRAB))
	{// �͂ݏ���

		if (nMotion != MOTION_THROW && nMotion != MOTION_GRAB)
		{
			m_fragMotion.bGrab = true;
		}

		if (nMotion == MOTION_THROW)
		{
			// �X���[���L�����Z��
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}
	}
}

//=====================================================
// �v���C���[�̉�]
//=====================================================
void CPlayer::Rotation(void)
{
	// �ړ������̎擾
	D3DXVECTOR3 move = GetMove();

	float fAngleMove = atan2f(-move.x, -move.z);
	float fLenghtMove = sqrtf(move.x * move.x + move.z * move.z);

	int nMotion = GetMotion();

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT || nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2 || nMotion == MOTION_THROW)
	{
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager == nullptr)
		{
			return;
		}

		CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

		if (pEnemyLockon == nullptr)
		{
			// �J�����擾
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera == nullptr)
			{
				return;
			}

			CCamera::Camera *pInfoCamera = pCamera->GetCamera();

			// �����̕␳
			D3DXVECTOR3 rot = GetRotation();
			D3DXVECTOR3 rotCamera = pInfoCamera->rot;

			rotCamera.x -= D3DX_PI * 0.5f;
			rotCamera.y;

			universal::LimitRot(&rotCamera.x);
			universal::LimitRot(&rotCamera.y);

			universal::FactingRot(&m_info.rotDest.x, -rotCamera.x, 0.15f);
			universal::FactingRot(&m_info.rotDest.y, rotCamera.y, 0.15f);
		}
	}
	else
	{
		if (fLenghtMove >= 6.0f)
		{
			// �����̕␳
			D3DXVECTOR3 rot = GetRotation();

			SetRotation(rot);

			if (m_info.bLand)
			{
				m_fragMotion.bMove = true;
			}
		}
		else
		{
			m_fragMotion.bMove = false;
		}

		m_info.rotDest.x = 0.0f;
	}

	// �����̕␳
	D3DXVECTOR3 rot = GetRotation();

	float fFact = 0.1f;

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT)
	{
		fFact = 0.4f;
	}

	universal::FactingRot(&rot.y, m_info.rotDest.y + D3DX_PI, fFact);
	universal::FactingRot(&rot.x, m_info.rotDest.x, fFact);

	SetRotation(rot);
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
// �����蔻��Ǘ�
//=====================================================
void CPlayer::ManageCollision(void)
{
	// �����蔻��̒Ǐ]
	if (m_info.pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posWaist = GetMtxPos(0);
		D3DXVECTOR3 move = GetMove();

		// �G�Ƃ̐ڐG����
		m_info.pCollisionSphere->SetPosition(posWaist + move);

		if (m_info.pCollisionCube != nullptr)
		{
			pos = GetPosition();
			D3DXVECTOR3 posCollision = m_info.pCollisionCube->GetPosition();

			// ����̒Ǐ]
			m_info.pCollisionCube->SetPositionOld(posCollision);
			m_info.pCollisionCube->SetPosition(pos);

			// �u���b�N�Ƃ̉����o������
			m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

			// ���b�V���t�B�[���h�Ƃ̓����蔻��
			pos = GetPosition();
			D3DXVECTOR3 posOld = GetPositionOld();

			CMeshField *pMesh = CMeshField::GetInstance();

			if (pMesh != nullptr)
			{
				float fHeight = pMesh->GetHeight(pos,&move);

				if (pos.y <= fHeight)
				{
					pos.y = fHeight;
					move.y = 0.0f;

					SetPosition(pos);

					int nMotion = GetMotion();
					bool bFinish = IsFinish();

					if (nMotion == MOTION_AIR)
					{
						m_info.bLand = true;
						m_fragMotion.bAir = false;
						m_fragMotion.bJump = false;
					}
				}
				else
				{
					m_info.bLand = false;

					m_fragMotion.bAir = true;
				}
			}

			SetMove(move);
		}
	}
}

//=====================================================
// ���[�V�����Ǘ�
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bStamp || nMotion == MOTION_STAMP)
	{// ���݂����[�V����
		if (nMotion != MOTION_STAMP)
		{
			SetMotion(MOTION_STAMP);
		}
		else
		{
			if (bFinish)
			{
				SetMotion(MOTION_AIR);
			}
		}
	}
	else if (nMotion == MOTION_THROW)
	{// �������[�V����
		if (bFinish)
		{
			SetMotion(MOTION_AIR);
		}
	}
	else if (m_fragMotion.bGrab)
	{// �͂݃��[�V����
		if (nMotion != MOTION_GRAB)
		{
			SetMotion(MOTION_GRAB);

			AddMoveForward(POW_GRAB);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bGrab = false;
			}
		}
	}
	else if (nMotion == MOTION_MELEE2)
	{
		if (bFinish)
		{
			SetMotion(MOTION_AIR);

			EndMelee();
		}
	}
	else if (nMotion == MOTION_MELEE)
	{
		if (bFinish)
		{
			if (m_fragMotion.bAddAttack)
			{
				SetMotion(MOTION_MELEE2);
				m_fragMotion.bAddAttack = false;
				m_fragMotion.bMelee = false;

				CEnemy *pEnemyLockon = GetLockOn();

				if (pEnemyLockon != nullptr)
				{
					D3DXVECTOR3 pos = GetPosition();
					D3DXVECTOR3 posEnemy = pEnemyLockon->GetPosition();

					if (universal::DistCmp(pos, posEnemy, MELEE_DIST, nullptr) == false)
					{
						Camera::ChangeBehavior(new CMoveCylinder);

						AddMoveForward(POW_ADDMELEE);
					}
				}

				m_info.aParam[PARAM_MELEE] += 0.9f;
			}
			else
			{
				SetMotion(MOTION_AIR);

				EndMelee();
			}
		}
	}
	else if (m_fragMotion.bMelee)
	{// �ߐڍU�����[�V����
		if (nMotion != MOTION_ASSAULT)
		{
			SetMotion(MOTION_ASSAULT);
		}
		else
		{
			StartMelee();
		}
	}
	else if (m_fragMotion.bShot)
	{// �ˌ����[�V����
		if (nMotion != MOTION_SHOT || (nMotion == MOTION_SHOT && bFinish))
		{
			SetMotion(MOTION_SHOT);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bShot = false;
			}
		}
	}
	else if (m_fragMotion.bAir)
	{// �؋󃂁[�V����
		if (nMotion != MOTION_AIR)
		{
			SetMotion(MOTION_AIR);
		}
	}
	else if (m_fragMotion.bJump)
	{// �W�����v���[�V����
		if (nMotion != MOTION_JUMP)
		{
			SetMotion(MOTION_JUMP);
		}
		else
		{

		}
	}
	else if (m_fragMotion.bStop)
	{// �}��~���[�V����
		if (nMotion != MOTION_STOP)
		{
			SetMotion(MOTION_STOP);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bStop = false;
			}
		}
	}
	else if (m_fragMotion.bMove)
	{// �������[�V����
		if (nMotion != MOTION_WALK_FRONT)
		{
			SetMotion(MOTION_WALK_FRONT);
		}
	}
	else
	{// �ҋ@���[�V����
		if (nMotion != MOTION_NEUTRAL)
		{
			SetMotion(MOTION_NEUTRAL);
		}
	}
}

//=====================================================
// �p�����[�^�Ǘ�
//=====================================================
void CPlayer::ManageParam(void)
{
	for (int i = 0; i < PARAM_MAX; i++)
	{
		if (m_info.aParam[i] > 1.0f)
		{
			m_info.aParam[i] = 1.0f;
			m_info.aHeat[i] = true;
		}

		float fDeltaTime = CManager::GetDeltaTime();

		m_info.aParam[i] -= 1.0f * (fDeltaTime / DECREASE_PARAM);

		if (m_info.aParam[i] < 0.0f)
		{
			m_info.aParam[i] = 0.0f;
			m_info.aHeat[i] = false;
		}

		if (m_info.apHeatUI[i] != nullptr)
		{
			m_info.apHeatUI[i]->SetParam(m_info.aParam[i]);
		}
	}
}

//=====================================================
// �u�[�X�g�G�t�F�N�g�̐���
//=====================================================
void CPlayer::Boost(void)
{
	MultiplyMtx();

	if (m_info.pThruster == nullptr)
		return;

	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// �ڕW�����̐ݒ�
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	D3DXVECTOR3 move = GetMove();
	float fSpeed = sqrtf(axisMove.x * axisMove.x + axisMove.z * axisMove.z);

	universal::LimitValue(&fSpeed, 1.0f, 0.0f);

	for (int i = 0; i < m_info.nNumThruster; i++)
	{
		if (m_info.pThruster[i].pFire != nullptr)
		{
			int nParent = m_info.pThruster[i].nIdxParent;
			D3DXVECTOR3 offset = m_info.pThruster[i].offset;
			D3DXVECTOR3 vector = m_info.pThruster[i].vector;

			D3DXVECTOR3 posBoost;
			D3DXVECTOR3 vecBoost;

			D3DXMATRIX mtxParent = *GetParts(nParent)->pParts->GetMatrix();
			D3DXMATRIX mtx;
			D3DXMATRIX mtxVec;

			universal::SetOffSet(&mtx, mtxParent, offset);
			universal::SetOffSet(&mtxVec, mtx, vector);

			posBoost = { mtx._41, mtx._42 ,mtx._43 };
			vecBoost = { mtxVec._41 - posBoost.x,mtxVec._42 - posBoost.y,mtxVec._43 - posBoost.z };

			CDebugProc::GetInstance()->Print("\n�x�N�g��[%f,%f,%f]", vecBoost.x, vecBoost.y, vecBoost.z);

			D3DXVECTOR3 rot = universal::VecToRot(vecBoost);
			rot.x *= -1;
			rot.x += D3DX_PI;

			float fHeight = m_info.pThruster[i].pFire->GetHeight();

			float fDest = m_info.pThruster[i].size.y * fSpeed;

			float fDiff = fDest - fHeight;

			float fFact = 0.2f;

			if (fDiff < 0.0f)
			{
				fFact = 0.8f;
			}

			fHeight += fDiff * fFact;

			m_info.pThruster[i].pFire->SetHeight(fHeight);
			m_info.pThruster[i].pFire->SetRotation(rot);
			m_info.pThruster[i].pFire->SetPosition(posBoost);
		}

	}
}

//=====================================================
// �ߐڍU���̎n�܂�^�C�~���O
//=====================================================
void CPlayer::StartMelee(void)
{
	bool bFinish = IsFinish();
	bool bNear = false;

	CEnemy *pEnemyLockon = GetLockOn();

	if (pEnemyLockon != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posEnemy = pEnemyLockon->GetPosition();

		if (universal::DistCmp(pos, posEnemy, MELEE_DIST, nullptr))
		{
			Camera::ChangeBehavior(new CMoveCylinder);

			bNear = true;

			// �ړ����~�߂�
			D3DXVECTOR3 move = GetMove();

			move *= 0.7f;

			SetMove(move);
		}
	}

	if (bFinish || bNear)
	{// �����𖞂�������i���U���Ɉڍs
		m_fragMotion.bMelee = false;

		SetMotion(MOTION_MELEE);

		m_info.rotDest.x = 0.0f;

		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			pEnemyManager->EnableLockTarget(true);
		}

		m_info.aParam[PARAM_MELEE] += 0.7f;
	}
}

//=====================================================
// �O���Ɉړ��ʂ�������
//=====================================================
void CPlayer::AddMoveForward(float fSpeed)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRotation();

	vecMove =
	{
		sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * fSpeed,
		cosf(rot.x - D3DX_PI * 0.5f) * fSpeed,
		sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * fSpeed
	};

	move += vecMove;

	SetMove(move);
}

//=====================================================
// ��Ɉړ��ʂ�������
//=====================================================
void CPlayer::AddMoveUp(float fSpeed)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };

	vecMove.y += fSpeed;

	move += vecMove;

	SetMove(move);
}

//=====================================================
// �C�x���g�̊Ǘ�
//=====================================================
void CPlayer::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = *GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

	if (nMotion == MOTION_SHOT)
	{// �e�𔭎�
		Shot(pos);
	}

	if (nMotion == MOTION_JUMP)
	{// �W�����v
		D3DXVECTOR3 move = GetMove();

		move.y += POW_JUMP;

		SetMove(move);
	}

	if (nMotion == MOTION_STAMP)
	{// ���݂�
		AddMoveUp(POW_STAMP);

		AddMoveStamp();

		m_fragMotion.bStamp = false;
	}

	if (nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2)
	{// �ߐڍU��
		ManageAttack(pos,300.0f);
	}

	if (nMotion == MOTION_GRAB)
	{// �͂�
		// �G��͂�
		if (m_info.pClsnAttack == nullptr)
		{// ����̃G���[
			return;
		}

		m_info.pClsnAttack->SetPosition(pos);
		m_info.pClsnAttack->SetRadius(RADIUS_GRAB);

		if (m_info.pClsnAttack->OnEnter(CCollision::TAG::TAG_ENEMY))
		{// �ΏۂƂ̓����蔻��
			CObject *pObj = m_info.pClsnAttack->GetOther();

			CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

			if (pEnemyManager != nullptr)
			{// �G�`�F�b�N
				// �͂�
				CEnemy *pEnemy = pEnemyManager->GetHead();
				CEnemy *pEnemyGrab = nullptr;

				while (pEnemy != nullptr)
				{
					CEnemy *pEnemyNext = pEnemy->GetNext();

					if ((CObject*)pEnemy == pObj)
					{
						pEnemyGrab = pEnemy;

						pEnemyGrab->DeleteCollision();
						pEnemyGrab->EnableIndependent(true);
						SetMotion(MOTION_THROW);

						// �X���[�ɂ���
						CSlow *pSlow = CSlow::GetInstance();

						if (pSlow != nullptr)
						{
							pSlow->SetScale(0.1f);
						}

						m_fragMotion.bGrab = false;
					}

					pEnemy = pEnemyNext;
				}

				// �͂ޓG�̌���
				m_info.pEnemyGrab = pEnemyGrab;

				if (m_info.pEnemyGrab != nullptr)
				{// �͂�ł���G���烍�b�N���O��
					CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

					if (pEnemyManager != nullptr)
					{
						pEnemyManager->SetEnemyLock(nullptr);
					}
				}
			}
		}
	}

	if (nMotion == MOTION_THROW)
	{// �͂񂾓G�𓊂���΂�
		if (m_info.pEnemyGrab != nullptr)
		{
			m_info.pEnemyGrab->EnableIndependent(false);

			D3DXVECTOR3 offset = { 0.0f,-100.0f,0.0f };
			D3DXMATRIX mtxParent;
			D3DXMATRIX mtxPart = *GetParts(5)->pParts->GetMatrix();

			universal::SetOffSet(&mtxParent, mtxPart, offset);

			D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42, mtxParent._43 };

			m_info.pEnemyGrab->SetPosition(pos);
			m_info.pEnemyGrab->CreateCollision();
			m_info.pEnemyGrab->SetState(CEnemy::STATE::STATE_THROWN);

			// ���������Ɉړ��ʂ�ݒ�
			D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
			D3DXVECTOR3 rot = GetRotation();

			vecMove =
			{
				sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * POW_THROW,
				cosf(rot.x - D3DX_PI * 0.5f) * POW_THROW,
				sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * POW_THROW
			};

			m_info.pEnemyGrab->SetMove(vecMove);

			m_info.pEnemyGrab = nullptr;

			// �X���[�ɂ���
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}
	}
}

//=====================================================
// �ˌ�
//=====================================================
void CPlayer::Shot(D3DXVECTOR3 posMazzle)
{
	D3DXVECTOR3 rot = GetRotation();

	D3DXVECTOR3 move =
	{
		sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * SPEED_BULLET,
		cosf(rot.x - D3DX_PI * 0.5f) * SPEED_BULLET,
		sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * SPEED_BULLET
	};

	CBullet *pBullet = CBullet::Create(posMazzle, move, 5, CBullet::TYPE_PLAYER, false, 40.0f, DAMAGE_BULLET,
		D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));

	// �M�ʂ����Z
	m_info.aParam[PARAM_GUN] += 0.3f;
}

//=====================================================
// �U������̊Ǘ�
//=====================================================
void CPlayer::ManageAttack(D3DXVECTOR3 pos, float fRadius)
{
	if (m_info.pClsnAttack == nullptr)
	{// ����̃G���[
		return;
	}

	m_info.pClsnAttack->SetPosition(pos);
	m_info.pClsnAttack->SetRadius(fRadius);

	if (m_info.pClsnAttack->OnEnter(CCollision::TAG::TAG_ENEMY))
	{// �ΏۂƂ̓����蔻��
		CObject *pObj = m_info.pClsnAttack->GetOther();

		if (pObj != nullptr)
		{
			// �q�b�g�X�g�b�v
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetSlowTime(0.2f, 0.01f);
			}

			// �J�����h��
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera != nullptr)
			{
				pCamera->SetQuake(1.01f, 1.01f, 10);
			}

			m_info.pClsnAttack->DamageAll(CCollision::TAG::TAG_ENEMY, 5.0f);
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();

	// �f�o�b�O�\��
	Debug();
}

//=====================================================
// �q�b�g����
//=====================================================
void CPlayer::Hit(float fDamage)
{
	if (m_info.state != STATE_DEATH)
	{
		m_info.fLife -= fDamage;

		if (m_info.fLife <= 0.0f)
		{// ���S����
			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			Uninit();

			CGame::SetState(CGame::STATE::STATE_END);
		}
		else
		{// �_���[�W����
			m_info.state = STATE_DAMAGE;

			// �J�����h��
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera != nullptr)
			{
				if (fDamage < 0.5f)
				{
					pCamera->SetQuake(0.2f, 0.2f, 10);
				}
				else if (fDamage < 1.0f)
				{
					pCamera->SetQuake(0.4f, 0.4f, 10);
				}
				else
				{
					pCamera->SetQuake(1.5f, 1.5f, 20);
				}
			}
		}
	}
}

//=====================================================
// ���b�N�I���̓G�擾
//=====================================================
CEnemy *CPlayer::GetLockOn(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return nullptr;
	}

	CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

	return pEnemyLockon;
}

//=====================================================
// �i���̏I��
//=====================================================
void CPlayer::EndMelee(void)
{
	Camera::ChangeBehavior(new CFollowPlayer);

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{

	}
}

//=====================================================
// �^�[�Q�b�g���b�N�̐؂�ւ�
//=====================================================
void CPlayer::ToggleLockTarget(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return;
	}

	pEnemyManager->EnableLockTarget(m_info.bLockTarget);
}

//=====================================================
// �u�[�X�g���Z
//=====================================================
void CPlayer::AddBoost(float fValue)
{
	m_info.fBoost += fValue;

	if (m_info.fBoost < 0.0f)
	{// �I�[�o�[�q�[�g����
		m_info.stateBoost = STATEBOOST_OVERHEAT;
	}

	if (m_info.stateBoost == STATEBOOST_OVERHEAT)
	{// �I�[�o�[�q�[�g����̕��A
		float fRate = m_info.fBoost / m_param.fInitialBoost;

		if (fRate > 0.5f)
		{
			m_info.stateBoost = STATEBOOST_NORMAL;
		}
	}

	universal::LimitValue(&m_info.fBoost, m_param.fInitialBoost, 0.0f);
}

//=====================================================
// ���݂��̈ړ��ʉ��Z
//=====================================================
void CPlayer::AddMoveStamp(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}

	// �J�����擾
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	CInputManager::SAxis axis = pInputManager->GetAxis();

	axis.axisMove;

	float fLengthInput = sqrtf(axis.axisMove.x * axis.axisMove.x + axis.axisMove.y * axis.axisMove.y);

	if (fLengthInput >= 0.3f)
	{
		float fAngleInput = atan2f(axis.axisMove.x, axis.axisMove.y);

		D3DXVECTOR3 move = GetMove();

		move +=
		{
			sinf(pInfoCamera->rot.y + fAngleInput) * SPEED_STAMP,
			0.0f,
			cosf(pInfoCamera->rot.y + fAngleInput) * SPEED_STAMP,
		};

		SetMove(move);
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

	pDebugProc->Print("\n�v���C���[�̈ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\n�v���C���[�̈ړ���[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
	pDebugProc->Print("\n�ڕW�̌���[%f,%f,%f]", GetRotation().x, GetRotation().y, GetRotation().z);
	pDebugProc->Print("\n�u�[�X�g�c��[%f]", m_info.fBoost);
	pDebugProc->Print("\n�̗�[%f]", m_info.fLife);
	for (int i = 0; i < PARAM_MAX; i++)
		pDebugProc->Print("\n�M��[%f][%d]", m_info.aParam[i],m_info.aHeat[i]);

	int nMotion = GetMotion();

	pDebugProc->Print("\n���[�V����[%d]", nMotion);
	pDebugProc->Print("\n�^�[�Q�b�g���b�N[%d]", m_info.bLockTarget);
}