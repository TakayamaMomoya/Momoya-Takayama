//*****************************************************
//
// �J�����r�w�C�r�A[cameraBehavior.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "cameraBehavior.h"
#include "camera.h"
#include "enemyManager.h"
#include "player.h"
#include "effect3D.h"
#include "inputmouse.h"
#include "inputkeyboard.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float DIST_CYLINDER = 1000.0f;
const float DIST_LOOK = 1500.0f;
const float MOVE_SPEED = 3.0f;						//�ړ��X�s�[�h
const float ROLL_SPEED = 0.02f;						//��]�X�s�[�h
}

//=====================================================
// �o�����̃J�����̓���
//=====================================================
void CApperPlayer::Update(CCamera *pCamera)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(1);

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posRDest = posPlayer;

	pInfoCamera->posV = posPlayer;
	pInfoCamera->posV.y = 10.0f;
	pInfoCamera->posV.x += 100.0f;
}

//=====================================================
// �v���C���[�̒Ǐ]
//=====================================================
void CFollowPlayer::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	CPlayer *pPlayer = CPlayer::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pPlayer == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	bool bLock = pPlayer->IsTargetLock();
	CEnemy *pEnemyLock = pEnemyManager->GetLockon();

	D3DXVECTOR3 pos = pPlayer->GetMtxPos(2);

	pInfoCamera->posRDest = pos;

	//�ڕW�̎��_�ݒ�
	pInfoCamera->posVDest =
	{
		pInfoCamera->posRDest.x - sinf(pInfoCamera->rot.x) * sinf(pInfoCamera->rot.y - D3DX_PI * 0.02f) * pInfoCamera->fLength,
		pInfoCamera->posRDest.y - cosf(pInfoCamera->rot.x) * pInfoCamera->fLength,
		pInfoCamera->posRDest.z - sinf(pInfoCamera->rot.x) * cosf(pInfoCamera->rot.y - D3DX_PI * 0.02f) * pInfoCamera->fLength
	};

	if (pEnemyLock != nullptr && bLock)
	{
		pCamera->ChangeBehavior(new CLookEnemy);
	}

#ifdef _DEBUG
	CEffect3D::Create(pInfoCamera->posRDest, 20.0f, 1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	CDebugProc::GetInstance()->Print("\n�J�����̓v���C���[�Ǐ]�ł�");
#endif
}

//=====================================================
// �G�����鏈��
//=====================================================
void CLookEnemy::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	CPlayer *pPlayer = CPlayer::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pPlayer == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	CEnemy *pEnemyLock = pEnemyManager->GetLockon();
	D3DXVECTOR3 posEnemy = { 0.0f,0.0f,0.0f };

	if (pEnemyLock != nullptr)
	{
		posEnemy = pEnemyLock->GetMtxPos(0);

		// �ʒu�̍������擾
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(2);
		D3DXVECTOR3 vecDiff = posEnemy - posPlayer;
		D3DXVECTOR3 vecDiffFlat = { vecDiff.x,0.0f,vecDiff.z };
		D3DXVECTOR3 rotDest;
		D3DXVECTOR3 *pRot = &pInfoCamera->rot;

		// �����̌������v�Z
		float fLegnthDiff = D3DXVec3Length(&vecDiff);
		float fLegnthFlat = D3DXVec3Length(&vecDiffFlat);

		rotDest.x = atan2f(fLegnthFlat, vecDiff.y) + D3DX_PI * 0.01f;
		rotDest.y = atan2f(vecDiff.x, vecDiff.z);

		float fLengthView = pInfoCamera->fLength;

		// �����_�ʒu�̐ݒ�
		pInfoCamera->posRDest = posPlayer + vecDiff * 0.5f;

		if (fLegnthDiff > 7000.0f)
		{// �G�������Ƃ��̐���
			posPlayer = pPlayer->GetMtxPos(0);

			fLengthView = pInfoCamera->fLength * 0.4f;

			pInfoCamera->posRDest.y -= fLegnthDiff * 0.1f;

			rotDest.x += 0.3f;
		}

		universal::FactingRot(&pRot->x, rotDest.x, 0.1f);
		universal::FactingRot(&pRot->y, rotDest.y, 0.1f);

		CDebugProc::GetInstance()->Print("\n����[%f]", fLegnthDiff);

		// ���_�ʒu�̐ݒ�
		pInfoCamera->posVDest =
		{
			posPlayer.x - sinf(pRot->x) * sinf(pRot->y) * fLengthView,
			posPlayer.y - cosf(pRot->x) * fLengthView,
			posPlayer.z - sinf(pRot->x) * cosf(pRot->y) * fLengthView
		};

		//universal::LimitDistSphereInSide(1000.0f, &pInfoCamera->posVDest, posPlayer);

		float fDiff = universal::LimitDistCylinder(1000.0f, &pInfoCamera->posV, posEnemy);

		if (fDiff < 1000.0f)
		{// �J������������������Ă鎞�J����������ݒ�
			D3DXVECTOR3 vecDiffCamera = pInfoCamera->posR - pInfoCamera->posV;

			float fLengthCameraFlat = sqrtf(vecDiffCamera.x * vecDiffCamera.x + vecDiffCamera.z * vecDiffCamera.z);

			rotDest.x = atan2f(fLengthCameraFlat, vecDiffCamera.y);
			rotDest.y = atan2f(vecDiffCamera.x, vecDiffCamera.z);
		}
	}

	bool bLock = pPlayer->IsTargetLock();

#ifdef _DEBUG
	CEffect3D::Create(pInfoCamera->posRDest, 20.0f, 1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	CDebugProc::GetInstance()->Print("\n�J�����̓��b�N�I����Ԃł�");
#endif

	if (pEnemyLock == nullptr || bLock== false)
	{// �ʏ�̒���
		pCamera->ChangeBehavior(new CFollowPlayer);

		return;
	}
}

//=====================================================
// �V�����_�[��̓���������
//=====================================================
void CMoveCylinder::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	CPlayer *pPlayer = CPlayer::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pPlayer == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	CEnemy *pEnemyLock = pEnemyManager->GetLockon();
	D3DXVECTOR3 posEnemy = { 0.0f,0.0f,0.0f };

	if (pEnemyLock != nullptr)
	{
		posEnemy = pEnemyLock->GetMtxPos(0);
	}

	// �ʒu�̍������擾
	D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(2);
	D3DXVECTOR3 vecDiff = posEnemy - posPlayer;
	D3DXVECTOR3 vecDiffFlat = { vecDiff.x,0.0f,vecDiff.z };

	// �����̌������v�Z
	D3DXVECTOR3 rot;
	float fLegnthDiff = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	float fLegnthFlat = D3DXVec3Length(&vecDiffFlat);

	rot.x = pInfoCamera->rot.x;
	rot.y = pInfoCamera->rot.y;

	// �����_�ʒu�̐ݒ�
	pInfoCamera->posRDest = posPlayer + vecDiff * 0.5f;

	// ���̋����ƍ��������𑫂�
	float fLengthView = fLegnthDiff + pInfoCamera->fLength;

	// ���_�ʒu�̐ݒ�
	pInfoCamera->posVDest =
	{
		posEnemy.x - sinf(rot.x) * sinf(rot.y) * fLengthView,
		posEnemy.y - cosf(rot.x) * fLengthView,
		posEnemy.z - sinf(rot.x) * cosf(rot.y) * fLengthView
	};

	pInfoCamera->rot = rot;

	bool bLock = pPlayer->IsTargetLock();

	int nMotion = pPlayer->GetMotion();

	if (pEnemyLock == nullptr || bLock == false)
	{// �v���C���[�Ǐ]�Ɉڍs
		Camera::ChangeBehavior(new CLookEnemy);
	}

	if (fLegnthFlat > DIST_LOOK)
	{// ���ʂ̒����Ɉڍs
		Camera::ChangeBehavior(new CLookEnemy);
	}
}

//=====================================================
// ���삷��
//=====================================================
void CMoveControl::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

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
		pInfoCamera->rot.y += rot.x * ROLL_SPEED;
		pInfoCamera->rot.x -= rot.y * ROLL_SPEED;

		//�����_�𑊑Έʒu�ɐݒ�
		pCamera->SetPosR();

		if (pKeyboard->GetPress(DIK_LSHIFT) == true)
		{//����
			fMove *= 3;
		}

		//���_�ړ�===============================================
		if (pKeyboard->GetPress(DIK_A) == true)
		{//���ړ�
			pInfoCamera->posVDest.x += sinf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
			pInfoCamera->posVDest.z += cosf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_D) == true)
		{//�E�ړ�
			pInfoCamera->posVDest.x += sinf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
			pInfoCamera->posVDest.z += cosf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_W) == true)
		{//�O�ړ�
			pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.x + D3DX_PI) * sinf(pInfoCamera->rot.y) * fMove;
			pInfoCamera->posVDest.y += cosf(pInfoCamera->rot.x + D3DX_PI) * MOVE_SPEED;
			pInfoCamera->posVDest.z -= sinf(pInfoCamera->rot.x + D3DX_PI) * cosf(pInfoCamera->rot.y) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_S) == true)
		{//��ړ�
			pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.x) * sinf(pInfoCamera->rot.y) * fMove;
			pInfoCamera->posVDest.y += cosf(pInfoCamera->rot.x) * MOVE_SPEED;
			pInfoCamera->posVDest.z -= sinf(pInfoCamera->rot.x) * cosf(pInfoCamera->rot.y) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_E) == true)
		{//�㏸
			pInfoCamera->posVDest.y += fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_Q) == true)
		{//���~
			pInfoCamera->posVDest.y -= fMove;
			pCamera->SetPosR();
		}
	}
}