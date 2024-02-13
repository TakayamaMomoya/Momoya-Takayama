//*****************************************************
//
// �J�����̏���[camera.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "main.h"
#include "camera.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "debugproc.h"
#include "game.h"
#include "playerManager.h"
#include "player.h"
#include "rocket.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MOVE_SPEED					(3.0f)						//�ړ��X�s�[�h
#define ROLL_SPEED					(0.02f)						//��]�X�s�[�h
#define MOVE_FACT					(0.1f)						//�ړ��W��
#define LIMIT_HEIGHT	(50)	// �ʒu�𐧌����鍂��
#define INITIAL_ANGLE	(45.0f)	// �����̎���p
#define ANGLE_GAME	(D3DX_PI * 0.4f)	// �Q�[�����̃J�����̊p�x
#define RATE_CAMERA_MOVE	(1.5f)	// �J�������ǂꂾ���v���C���[�̐�����邩�̔{��
#define POSR_BOSS	(D3DXVECTOR3(2742.27f,240.53f,-81.36f))	// �{�X��̒����_�ʒu
#define POSV_BOSS	(D3DXVECTOR3(2741.14f,280.15f,-261.20f))	// �{�X��̎��_�ʒu

#define MOVE_FACT_TITLE_ESC	(0.025f)	// �^�C�g���̓�����Ƃ��̕␳
//====================================================
// ����������
//====================================================
HRESULT CCamera::Init(void)
{
	ZeroMemory(&m_camera,sizeof(Camera));

	m_camera.posV = D3DXVECTOR3(0.0f, 1050.0f, -590.0f);
	m_camera.posVOld = D3DXVECTOR3(0.0f, 30.0f, 100.0f);
	m_camera.posR = D3DXVECTOR3(0.0f, 200.0f, -145.0f);
	m_camera.posVDest = D3DXVECTOR3(0.0f, 1050.0f, -590.0f);
	m_camera.posRDest = D3DXVECTOR3(0.0f, 200.0f, -145.0f);
	m_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.fLength = 100.0f;
	m_camera.fViewAngle = INITIAL_ANGLE;

	float fLength = sqrtf
	(//Y�ʂł̑Ίp��
		(m_camera.posV.x - m_camera.posR.x) * (m_camera.posV.x - m_camera.posR.x) + 
		(m_camera.posV.z - m_camera.posR.z) * (m_camera.posV.z - m_camera.posR.z)
	);

	m_camera.fLength = sqrtf
	(//�ڕW�܂ł̒�������
		fLength * fLength + (m_camera.posV.y - m_camera.posR.y) * (m_camera.posV.y - m_camera.posR.y)
	);

	m_camera.rot.x = atan2f(fLength, m_camera.posV.y - m_camera.posR.y);

	SetPosV();

	m_camera.fTimeEvent = 0.0f;

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CCamera::Uninit(void)
{

}

//====================================================
// �X�V����
//====================================================
void CCamera::Update(void)
{

}

//====================================================
// �ڕW�Ɍ���������
//====================================================
void CCamera::MoveDist(float fFact)
{
	// �ڕW�ʒu�ɕ␳
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * fFact;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * fFact;
	
	// �J�E���^�[�̕ύX
	if (m_camera.fTimeEvent > 0.0f)
	{
		float fTick = CManager::GetTick();

		m_camera.fTimeEvent -= fTick;

		if (m_camera.fTimeEvent <= 0.0f)
		{
			// �����l�ɖ߂�
			m_camera.fTimeEvent = 0.0f;

			m_camera.posVDest = D3DXVECTOR3(0.0f, 1050.0f, -590.0f);
			m_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// �Q�[���𓮂���
			CGame *pGame = CGame::GetInstance();

			if (pGame != nullptr)
			{
				pGame->EnableStop(false);
			}
		}
	}
}

//====================================================
// ���쏈��
//====================================================
void CCamera::Control(void)
{
	// ���͎擾
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	float fMove = MOVE_SPEED;

	//�}�E�X����======================================================
	if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == true)
	{//�E�N���b�N���A���_����
		D3DXVECTOR3 rot;

		//�}�E�X�̈ړ��ʑ��
		rot = { (float)pMouse->GetMoveIX() * ROLL_SPEED, (float)pMouse->GetMoveIY() * ROLL_SPEED, 0.0f };

		D3DXVec3Normalize(&rot, &rot);

		//���_�̐���
		m_camera.rot.y += rot.x * ROLL_SPEED;
		m_camera.rot.x -= rot.y * ROLL_SPEED;

		//�����_�𑊑Έʒu�ɐݒ�
		SetPosR();

		if (pKeyboard->GetPress(DIK_LSHIFT) == true)
		{//����
			fMove *= 3;
		}

		//���_�ړ�===============================================
		if (pKeyboard->GetPress(DIK_A) == true)
		{//���ړ�
			m_camera.posV.x += sinf(m_camera.rot.y - D3DX_PI * 0.5f) * fMove;
			m_camera.posV.z += cosf(m_camera.rot.y - D3DX_PI * 0.5f) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_D) == true)
		{//�E�ړ�
			m_camera.posV.x += sinf(m_camera.rot.y - D3DX_PI * -0.5f) * fMove;
			m_camera.posV.z += cosf(m_camera.rot.y - D3DX_PI * -0.5f) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_W) == true)
		{//�O�ړ�
			m_camera.posV.x -= sinf(m_camera.rot.x + D3DX_PI) * sinf(m_camera.rot.y) * fMove;
			m_camera.posV.y += cosf(m_camera.rot.x + D3DX_PI) * MOVE_SPEED;
			m_camera.posV.z -= sinf(m_camera.rot.x + D3DX_PI) * cosf(m_camera.rot.y) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_S) == true)
		{//��ړ�
			m_camera.posV.x -= sinf(m_camera.rot.x) * sinf(m_camera.rot.y) * fMove;
			m_camera.posV.y += cosf(m_camera.rot.x) * MOVE_SPEED;
			m_camera.posV.z -= sinf(m_camera.rot.x) * cosf(m_camera.rot.y) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_E) == true)
		{//�㏸
			m_camera.posV.y += fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_Q) == true)
		{//���~
			m_camera.posV.y -= fMove;
			SetPosR();
		}
	}
}

//====================================================
// �^�C�g���̐ݒ�
//====================================================
void CCamera::SetTitle(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// �^�C�g���̖ړI�n�ݒ�
//====================================================
void CCamera::SetTitleDest(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest)
{
	m_camera.posVDest = posVDest;
	m_camera.posRDest = posRDest;
}

//====================================================
// �Z���N�g�̐ݒ�
//====================================================
void CCamera::SetSelect(void)
{
	D3DXVECTOR3 posV = { 54.0f,320.0f,-760.0f };
	D3DXVECTOR3 posR = { 54.0f,-122.0f,60.0f };

	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// �����L���O�̐ݒ�
//====================================================
void CCamera::SetRanking(void)
{
	D3DXVECTOR3 posV = { 0.0f,175.0f,-500.0f };
	D3DXVECTOR3 posR = { 0.0f,175.0f,0.0f };

	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// ���t�g�̏����ʒu
//====================================================
void CCamera::SetLift(void)
{
	D3DXVECTOR3 posV = { 25.0f,80.0f,-130.0f };
	D3DXVECTOR3 posR = { 25.0f,-100.0f,820.0f };

	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// �㏸���t�g
//====================================================
void CCamera::SetUpLift(void)
{
	if (m_camera.posV.y < 200.0f)
	{
		m_camera.posV.y += 2.0f;
		m_camera.posVDest.y += 2.0f;

		m_camera.posR.y += 2.0f;
		m_camera.posRDest.y += 2.0f;

		return;
	}
	
	m_camera.posV.y += 1.5f;
	m_camera.posVDest.y += 1.5f;
	m_camera.posR.y += 4.5f;
	m_camera.posRDest.y += 4.5f;
}

//====================================================
// �Ǐ]����
//====================================================
void CCamera::FollowPlayer(void)
{
	// �v���C���[�̎擾
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posPlayer = { 0.0f,0.0f,0.0f };

	for (int i = 0;i < NUM_PLAYER;i++)
	{// ��ԎႢ�ԍ��̃v���C���[�̈ʒu���Q��
		CPlayer* pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			posPlayer = pPlayer->GetPosition();

			break;
		}
	}

	// �ړI���W�ݒ�
	m_camera.posRDest = posPlayer;
	m_camera.posVDest = 
	{
		m_camera.posRDest.x,
		m_camera.posRDest.y + 70.0f,
		m_camera.posRDest.z - 100.0f,
	};

	// �ړI���W�ɕ␳
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT;
}

//====================================================
// �^�C�g���̃J��������
//====================================================
void CCamera::UpdateTitle(D3DXVECTOR3 move)
{
	// �J�����̈ړ�����
	m_camera.posVDest = m_camera.posVDest + move;

	// �ړI���W�ɕ␳
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT_TITLE_ESC;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT_TITLE_ESC;
}

//====================================================
// �^�C�g���̓�����Ƃ��̃J��������
//====================================================
void CCamera::UpdateTitleEsc(D3DXVECTOR3 move)
{
	// �J�����̈ړ�����
	m_camera.posVDest = m_camera.posVDest + move;
	m_camera.posRDest = m_camera.posRDest + move;

	// �ړI���W�ɕ␳
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT_TITLE_ESC;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT_TITLE_ESC;
}

//====================================================
// ���U���g�̃J��������
//====================================================
void CCamera::UpdateResult(void)
{
	// ���P�b�g�̍��W�擾
	CRocket *pRocket = CRocket::GetInstance();

	D3DXVECTOR3 posTarget = { 0.0f,0.0f,0.0f };

	if (pRocket != nullptr)
	{
		posTarget = pRocket->GetPosition();
	}
	else
	{
		return;
	}

	// �ړI���W�ݒ�
	m_camera.posRDest = posTarget;
	m_camera.posVDest =
	{
		0.0f,
		500.0f,
		-300.0f,
	};

	// �ړI���W�ɕ␳
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT;
}

//====================================================
// �Q�[���I�[�o�[�̃J��������
//====================================================
void CCamera::UpdateGameover(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest)
{
	// �ړI���W�ݒ�
	m_camera.posRDest = posRDest;
	m_camera.posVDest = posVDest;

	// �ړI���W�ɕ␳
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT;
}

//====================================================
// �U������
//====================================================
void CCamera::Quake(void)
{
	//��ʐU��
	if (m_camera.nTimerQuake > 0)
	{
		//���Ԃ����炷
		m_camera.nTimerQuake--;

		//���_�����_�𓮂����ăJ������h�炷
		m_camera.posV.x += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * sinf(m_camera.rot.y);
		m_camera.posV.y += (float)(rand() % 101 - 50) * m_camera.fQuakeSizeV;
		m_camera.posV.z += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * cosf(m_camera.rot.y);

		m_camera.posR.x += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * sinf(m_camera.rot.y);
		m_camera.posR.y += (float)(rand() % 101 - 50) * m_camera.fQuakeSizeV;
		m_camera.posR.z += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * cosf(m_camera.rot.y);

		//���X�ɗh��̃T�C�Y������������
		m_camera.fQuakeSizeH += (0.0f - m_camera.fQuakeSizeH) * 0.03f;
		m_camera.fQuakeSizeV += (0.0f - m_camera.fQuakeSizeV) * 0.03f;
	}
}

//====================================================
// �U���ݒ�
//====================================================
void CCamera::SetQuake(float fQuakeSizeH, float fQuakeSizeV, int nTime)
{
	if (fQuakeSizeH > 0.0f && fQuakeSizeV > 0.0f)
	{
		//�h��鎞�ԑ��
		m_camera.nTimerQuake = nTime;

		//�h��̑傫�����
		m_camera.fQuakeSizeH = fQuakeSizeH;
		m_camera.fQuakeSizeV = fQuakeSizeV;
	}
}

//====================================================
// ���_�ݒ�
//====================================================
void CCamera::SetPosV(void)
{
	m_camera.posV =
	{
		m_camera.posR.x + sinf(m_camera.rot.x) * sinf(m_camera.rot.y + D3DX_PI) * m_camera.fLength,
		m_camera.posR.y + cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posR.z + sinf(m_camera.rot.x) * cosf(m_camera.rot.y + D3DX_PI) * m_camera.fLength
	};
}

//====================================================
// �����_�ݒ�
//====================================================
void CCamera::SetPosR(void)
{
	m_camera.posR =
	{
		m_camera.posV.x - sinf(m_camera.rot.x) * sinf(m_camera.rot.y + D3DX_PI) * m_camera.fLength,
		m_camera.posV.y - cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posV.z - sinf(m_camera.rot.x) * cosf(m_camera.rot.y + D3DX_PI) * m_camera.fLength
	};
}

//====================================================
// �ݒ菈��
//====================================================
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X==================================================
	//�v���W�F�N�V�����}�g���b�N�X������
	D3DXMatrixIdentity(&m_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�쐬
	D3DXMatrixPerspectiveFovLH(&m_camera.mtxProjection,
		D3DXToRadian(m_camera.fViewAngle),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		MIN_DRAW,
		MAX_DRAW);

	/*D3DXMatrixOrthoLH(&m_camera.mtxProjection,	// ���s���e�̂���
		720,	// ����
		1280,	// ��
		MIN_DRAW,
		MAX_DRAW);*/

	//�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION,&m_camera.mtxProjection);

	//�r���[�}�g���b�N�X============================================================
	//�r���[�}�g���b�N�X������
	D3DXMatrixIdentity(&m_camera.mtxView);

	//�r���[�}�g���b�N�X�쐬
	D3DXMatrixLookAtLH(&m_camera.mtxView,
		&m_camera.posV,
		&m_camera.posR,
		&m_camera.vecU);

	//�r���[�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\n���_�̈ʒu�F[%f,%f,%f]", m_camera.posV.x, m_camera.posV.y, m_camera.posV.z);
	CDebugProc::GetInstance()->Print("\n���_�̖ڕW�ʒu�F[%f,%f,%f]", m_camera.posVDest.x, m_camera.posVDest.y, m_camera.posVDest.z);
	CDebugProc::GetInstance()->Print("\n�����_�̈ʒu�F[%f,%f,%f]", m_camera.posR.x, m_camera.posR.y, m_camera.posR.z);
	CDebugProc::GetInstance()->Print("\n�����_�̖ڕW�ʒu�F[%f,%f,%f]", m_camera.posRDest.x, m_camera.posRDest.y, m_camera.posRDest.z);
	CDebugProc::GetInstance()->Print("\n�J���������F[%f]", m_camera.fLength);
#endif
}

//====================================================
// ���擾
//====================================================
CCamera::Camera *CCamera::GetCamera(void)
{
	return &m_camera;
}