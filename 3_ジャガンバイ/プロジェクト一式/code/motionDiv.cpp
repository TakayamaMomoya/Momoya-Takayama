//*****************************************************
//
// �������[�V�����̏���[motionDiv.cpp]
// Author:����쏟
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motionDiv.h"
#include "model.h"
#include <stdio.h>
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "particle.h"
#include "sound.h"
#include "universal.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_STRING	(256)	// ������̍ő吔

//=====================================================
// �R���X�g���N�^
//=====================================================
CMotionDiv::CMotionDiv(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
	ZeroMemory(&m_aLoopMotion[0], sizeof(m_aLoopMotion));
	ZeroMemory(&m_aMotionType[0], sizeof(m_aMotionType));
	ZeroMemory(&m_aMotionTypeOld[0], sizeof(m_aMotionTypeOld));
	ZeroMemory(&m_aCounterMotion[0], sizeof(m_aCounterMotion));
	ZeroMemory(&m_aKey[0], sizeof(m_aKey));
	ZeroMemory(&m_aFrame[0], sizeof(m_aFrame));
	ZeroMemory(&m_aNumKey[0], sizeof(m_aNumKey));
	ZeroMemory(&m_aFrame[0], sizeof(m_aFrame));
	ZeroMemory(&m_aNumMotion[0], sizeof(m_aNumMotion));
	ZeroMemory(&m_aNumParts[0], sizeof(m_aNumParts));
	ZeroMemory(&m_aFinish[0], sizeof(m_aFinish));
	m_bShadow = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_posShadow = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_bInde = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMotionDiv::~CMotionDiv()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CMotionDiv::Init(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		SetMotion(nCntDiv, 0);
		InitPose(0);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMotionDiv::Uninit(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
		{// �p�[�e�B�N�����̔j��
			if (m_aMotionInfo[nCntDiv][nCntMotion].pParticle != nullptr)
			{
				delete m_aMotionInfo[nCntDiv][nCntMotion].pParticle;
				m_aMotionInfo[nCntDiv][nCntMotion].pParticle = nullptr;
			}
		}
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CMotionDiv::Update(void)
{
	// ���݂̈ʒu��ۑ�
	m_posOld = m_pos;

	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		//�ϐ��錾
		int nNextKey;

		// �p�[�c�̃g�����X�t�H�[��
		D3DXVECTOR3 pos, rot;

		if (m_apParts[nCntDiv][0] == nullptr)
		{
			return;
		}

		int nMotionType = m_aMotionType[nCntDiv];
		
		if (m_aMotionInfo[nCntDiv][nMotionType].pParticle != nullptr)
		{
			for (int nCntParticle = 0; nCntParticle < m_aMotionInfo[nCntDiv][nMotionType].nNumParticle; nCntParticle++)
			{// �S�Ẵp�[�e�B�N�����m�F
				if (m_aKey[nCntDiv] == m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].nKey &&
					m_aCounterMotion[nCntDiv] == m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].nFrame)
				{// �p�[�e�B�N������
					// �e�p�[�c�̈ʒu�擾
					D3DXMATRIX* pMtx = m_apParts[nCntDiv][m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].nIdxParent]->pParts->GetMatrix();
					pos = D3DXVECTOR3(pMtx->_41, pMtx->_42, pMtx->_43) + m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].offset;

					CParticle::Create(pos, (CParticle::TYPE)m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].nType);
				}
			}
		}

		for (int nCntParts = 0; nCntParts < m_aNumParts[nCntDiv]; nCntParts++)
		{
			if (m_aKey[nCntDiv] >= m_aMotionInfo[nCntDiv][nMotionType].nNumKey - 1)
			{
				break;
			}

			// �p�[�c�̃g�����X�t�H�[���擾
			pos = m_apParts[nCntDiv][nCntParts]->pParts->GetPosOrg();
			rot = m_apParts[nCntDiv][nCntParts]->pParts->GetRot();

			if (m_aKey[nCntDiv] < m_aMotionInfo[nCntDiv][nMotionType].nNumKey - 1)
			{
				nNextKey = m_aKey[nCntDiv] + 1;
			}
			else
			{
				nNextKey = m_aMotionInfo[nCntDiv][nMotionType].nNumKey - 1;
			}

			//�O��Ǝ��̃L�[�̍����擾
			float DiffPosX = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosX -
				m_aKeyOld[nCntDiv][nCntParts].fPosX;

			float DiffPosY = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosY -
				m_aKeyOld[nCntDiv][nCntParts].fPosY;

			float DiffPosZ = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosZ -
				m_aKeyOld[nCntDiv][nCntParts].fPosZ;

			float DiffRotX = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotX -
				m_aKeyOld[nCntDiv][nCntParts].fRotX;

			float DiffRotY = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotY -
				m_aKeyOld[nCntDiv][nCntParts].fRotY;

			float DiffRotZ = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotZ -
				m_aKeyOld[nCntDiv][nCntParts].fRotZ;

			universal::LimitRot(&DiffRotX);
			universal::LimitRot(&DiffRotY);
			universal::LimitRot(&DiffRotZ);

			//�ړI�̒l=======================================================================================================
			float DestPosX = pos.x + m_aKeyOld[nCntDiv][nCntParts].fPosX +
				DiffPosX * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestPosY = pos.y + m_aKeyOld[nCntDiv][nCntParts].fPosY +
				DiffPosY * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestPosZ = pos.z + m_aKeyOld[nCntDiv][nCntParts].fPosZ +
				DiffPosZ * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestRotX = m_aKeyOld[nCntDiv][nCntParts].fRotX +
				DiffRotX * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestRotY = m_aKeyOld[nCntDiv][nCntParts].fRotY +
				DiffRotY * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestRotZ = m_aKeyOld[nCntDiv][nCntParts].fRotZ +
				DiffRotZ * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			//�p�[�c�̌����E�ʒu�ݒ�
			m_apParts[nCntDiv][nCntParts]->pParts->SetPosition(D3DXVECTOR3(DestPosX, DestPosY, DestPosZ));

			universal::LimitRot(&DestRotX);
			universal::LimitRot(&DestRotY);
			universal::LimitRot(&DestRotZ);

			rot = D3DXVECTOR3(DestRotX, DestRotY, DestRotZ);

			m_apParts[nCntDiv][nCntParts]->pParts->SetRot(rot);
		}

		m_aCounterMotion[nCntDiv]++;

		if (m_aKey[nCntDiv] >= m_aMotionInfo[nCntDiv][nMotionType].nNumKey - 1)
		{// ���[�V�������I�����Ă�����
			m_aMotionTypeOld[nCntDiv] = nMotionType;

			// �I��������Ԃɂ���
			m_aFinish[nCntDiv] = true;

			if (m_aMotionInfo[nCntDiv][nMotionType].bLoop)
			{// ���[�v������͍̂Đݒ�
				SetMotion(nCntDiv,nMotionType);
			}
		}

		if (m_aCounterMotion[nCntDiv] > m_aFrame[nCntDiv])
		{//�L�[�̃t���[�����ɒB������
			if (m_aKey[nCntDiv] < m_aMotionInfo[nCntDiv][nMotionType].nNumKey)
			{
				m_aKey[nCntDiv]++;

				m_aFrame[nCntDiv] = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[m_aKey[nCntDiv]].nFrame;

				m_aCounterMotion[nCntDiv] = 0;

				SetKeyOld();
			}
		}
	}
}

//=====================================================
// ���[�V�����ݒ�
//=====================================================
void CMotionDiv::SetMotion(int nNum, int nMotionType)
{
	// �I����Ԃ̉���
	m_aFinish[nNum] = false;

	// �I�����郂�[�V������ۑ�
	m_aMotionTypeOld[nNum] = m_aMotionType[nNum];
	SetKeyOld();

	if (m_aKey[nNum] != -1)
	{
		m_aFrame[nNum] = m_aMotionInfo[nNum][m_aMotionType[nNum]].aKeyInfo[m_aKey[nNum]].nFrame;
	}

	if (m_aMotionType[nNum] != nMotionType)
	{
		m_aFrame[nNum] = 10;
	}

	// ���[�V�������̐ݒ�
	m_aMotionType[nNum] = nMotionType;
	m_aKey[nNum] = -1;
	m_aCounterMotion[nNum] = 0;
}

//=====================================================
// �I������L�[�̏��ۑ�
//=====================================================
void CMotionDiv::SetKeyOld(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int nCntPart = 0; nCntPart < m_aNumParts[nCntDiv]; nCntPart++)
		{
			m_aKeyOld[nCntDiv][nCntPart].fPosX = m_apParts[nCntDiv][nCntPart]->pParts->GetPosition().x - m_apParts[nCntDiv][nCntPart]->pParts->GetPosOrg().x;
			m_aKeyOld[nCntDiv][nCntPart].fPosY = m_apParts[nCntDiv][nCntPart]->pParts->GetPosition().y - m_apParts[nCntDiv][nCntPart]->pParts->GetPosOrg().y;
			m_aKeyOld[nCntDiv][nCntPart].fPosZ = m_apParts[nCntDiv][nCntPart]->pParts->GetPosition().z - m_apParts[nCntDiv][nCntPart]->pParts->GetPosOrg().z;

			m_aKeyOld[nCntDiv][nCntPart].fRotX = m_apParts[nCntDiv][nCntPart]->pParts->GetRot().x;
			m_aKeyOld[nCntDiv][nCntPart].fRotY = m_apParts[nCntDiv][nCntPart]->pParts->GetRot().y;
			m_aKeyOld[nCntDiv][nCntPart].fRotZ = m_apParts[nCntDiv][nCntPart]->pParts->GetRot().z;
		}
	}
}

//=====================================================
// �|�[�Y�̏����ݒ�
//=====================================================
void CMotionDiv::InitPose(int nMotion)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int i = 0; i < m_aMotionInfo[nCntDiv][nMotion].nNumKey; i++)
		{
			for (int nCntPart = 0; nCntPart < m_aNumParts[nCntDiv]; nCntPart++)
			{
				m_aKeyOld[nCntDiv][nCntPart] = m_aMotionInfo[nCntDiv][nMotion].aKeyInfo[i].aKey[nCntPart];
			}
		}
	}
}

//=====================================================
// �����̃}�g���b�N�X�̐ݒ�
//=====================================================
void CMotionDiv::SetMatrix(void)
{
	//�ϐ��錾
	D3DXMATRIX mtxRotModel, mtxTransModel;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// �}�g���b�N�X���������킹�鏈��
//=====================================================
void CMotionDiv::MultiplyMtx(void)
{
	if (m_bInde == false)
	{
		// �e�̃}�g���b�N�X
		SetMatrix();
	}

	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		D3DXMATRIX mtxRotModel, mtxTransModel;
		D3DXMATRIX* pMtxParent;
		D3DXMATRIX* pMtx;

		D3DXMATRIX mtxShadow;
		D3DLIGHT9 light;
		D3DXVECTOR4 posLight;
		D3DXVECTOR3 normal;
		D3DXPLANE plane;

		// ���C�g�̈ʒu�ݒ�
		pDevice->GetLight(2, &light);
		posLight = { -light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f };

		// ���ʏ��̐���
		normal = { 0.0f,1.0f,0.0f };
		D3DXPlaneFromPointNormal(&plane, &m_posShadow, &normal);

		for (int nCntParts = 0; nCntParts < m_aNumParts[nCntDiv]; nCntParts++)
		{
			// �}�g���b�N�X�̎擾
			pMtx = m_apParts[nCntDiv][nCntParts]->pParts->GetMatrix();

			//���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(pMtx);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
				m_apParts[nCntDiv][nCntParts]->pParts->GetRot().y, m_apParts[nCntDiv][nCntParts]->pParts->GetRot().x, m_apParts[nCntDiv][nCntParts]->pParts->GetRot().z);
			D3DXMatrixMultiply(pMtx, pMtx, &mtxRotModel);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTransModel,
				m_apParts[nCntDiv][nCntParts]->pParts->GetPosition().x, m_apParts[nCntDiv][nCntParts]->pParts->GetPosition().y, m_apParts[nCntDiv][nCntParts]->pParts->GetPosition().z);
			D3DXMatrixMultiply(pMtx, pMtx, &mtxTransModel);

			if (m_apParts[nCntDiv][nCntParts]->nIdxParent != -1)
			{//�e�p�[�c������ꍇ
				// �e�}�g���b�N�X�̎擾
				pMtxParent = m_apParts[nCntDiv][m_apParts[nCntDiv][nCntParts]->nIdxParent]->pParts->GetMatrix();
			}
			else
			{
				if (nCntDiv == 0)
				{
					pMtxParent = &m_mtxWorld;
				}
				else
				{
					pMtxParent = m_apParts[0][0]->pParts->GetMatrix();
				}
				
			}

			//�e�p�[�c�ƃp�[�c�̃��[���h�}�g���b�N�X���������킹��
			D3DXMatrixMultiply(pMtx, pMtx, pMtxParent);

			if (m_bShadow)
			{
				// �V���h�E�}�g���b�N�X������
				D3DXMatrixIdentity(&mtxShadow);

				// �V���h�E�}�g���b�N�X�̍쐬
				D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, pMtx, &mtxShadow);

				// �V���h�E�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				m_apParts[nCntDiv][nCntParts]->pParts->DrawShadow();
			}

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, pMtx);

			m_apParts[nCntDiv][nCntParts]->pParts->Draw();
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CMotionDiv::Draw(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		// �}�g���b�N�X���������킹�鏈��
		MultiplyMtx();

		CDebugProc::GetInstance()->Print("\n�t���[��[%d]", m_aFrame[nCntDiv]);
		CDebugProc::GetInstance()->Print("\n�L�[[%d]", m_aKey[nCntDiv]);
	}
}

//=====================================================
// �Ǎ�����
//=====================================================
void CMotionDiv::Load(char* pPathLower, char* pPathUpper)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		//�ϐ��錾
		char cTemp[MAX_STRING];
		int nCntMotion = 0;
		int nCntModel = 0;

		FILE* pFile = NULL;

		if (nCntDiv == DIV_BODY_LOWER)
		{
			//�t�@�C������ǂݍ���
			pFile = fopen(pPathLower, "r");
		}
		else if (nCntDiv == DIV_BODY_UPPER)
		{
			//�t�@�C������ǂݍ���
			pFile = fopen(pPathUpper, "r");
		}

		if (pFile != nullptr)
		{//�t�@�C�����J�����ꍇ
			while (true)
			{
				//�����ǂݍ���
				(void)fscanf(pFile, "%s", &cTemp[0]);

				//�t�@�C�����ǂݍ���=========================================
				if (strcmp(cTemp, "NUM_MODEL") == 0)
				{
					//"="�ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					//���f�����ǂݍ���
					(void)fscanf(pFile, "%d", &m_aNumParts[nCntDiv]);

					for (int nCntFile = 0; nCntFile < m_aNumParts[nCntDiv];)
					{//�t�@�C�����ǂݍ���

						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "MODEL_FILENAME") == 0)
						{//�t�@�C�����ǂݍ���
							(void)fscanf(pFile, "%s", &cTemp[0]);

							char aPath[MAX_STRING];

							// ���f���p�X�Ǎ�
							(void)fscanf(pFile, "%s", &aPath[0]);

							m_apParts[nCntDiv][nCntFile] = new Parts;

							m_apParts[nCntDiv][nCntFile]->pParts = CParts::Create();

							int nIdx = CModel::Load(&aPath[0]);

							// ���f���Ǎ�
							m_apParts[nCntDiv][nCntFile]->pParts->SetIdxModel(nIdx);
							m_apParts[nCntDiv][nCntFile]->pParts->BindModel(m_apParts[nCntDiv][nCntFile]->pParts->GetIdxModel());

							nCntFile++;
						}
					}
				}
				//===========================================================

				//�L�����N�^�[�ݒ�===========================================
				if (strcmp(cTemp, "CHARACTERSET") == 0)
				{//�L�����N�^�[�X�^�[�g
					nCntModel = 0;

					while (strcmp(cTemp, "END_CHARACTERSET") != 0)
					{//�I���܂ŃL�����N�^�[�ݒ�
						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "PARTSSET") == 0)
						{//�p�[�c�X�^�[�g
							while (strcmp(cTemp, "END_PARTSSET") != 0)
							{//�I���܂Ńp�[�c�ݒ�
								(void)fscanf(pFile, "%s", &cTemp[0]);

								if (strcmp(cTemp, "INDEX") == 0)
								{//�ԍ��ǂݍ���
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_apParts[nCntDiv][nCntModel]->nIdx);
								}

								if (strcmp(cTemp, "PARENT") == 0)
								{//�e�ԍ��ǂݍ���
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_apParts[nCntDiv][nCntModel]->nIdxParent);
								}

								if (strcmp(cTemp, "POS") == 0)
								{//�ʒu�ǂݍ���
									D3DXVECTOR3 pos;

									(void)fscanf(pFile, "%s", &cTemp[0]);

									for (int nCntPos = 0; nCntPos < 3; nCntPos++)
									{
										(void)fscanf(pFile, "%f", &pos[nCntPos]);
									}

									m_apParts[nCntDiv][nCntModel]->pParts->SetPosition(pos);

									m_apParts[nCntDiv][nCntModel]->pParts->SetPosOrg(pos);
								}

								if (strcmp(cTemp, "ROT") == 0)
								{//�����ǂݍ���
									D3DXVECTOR3 rot;

									(void)fscanf(pFile, "%s", &cTemp[0]);

									for (int nCntRot = 0; nCntRot < 3; nCntRot++)
									{
										(void)fscanf(pFile, "%f", &rot[nCntRot]);
									}

									m_apParts[nCntDiv][nCntModel]->pParts->SetRot(rot);
								}

							}//END_PART

							nCntModel++;
						}
					}//END_CHAR
				}
				//===========================================================

				//�ϐ��錾
				int nCntKey = 0;
				int nCntPart = 0;
				int nCntParticle = 0;

				//���[�V�����ݒ�===========================================
				if (strcmp(cTemp, "MOTIONSET") == 0)
				{//���[�V�����X�^�[�g
					while (strcmp(cTemp, "END_MOTIONSET") != 0)
					{//�I���܂ŃL�����N�^�[�ݒ�

						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "LOOP") == 0)
						{//���[�v���f
							(void)fscanf(pFile, "%s", &cTemp[0]);

							(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntDiv][nCntMotion].bLoop);
						}

						if (strcmp(cTemp, "NUM_KEY") == 0)
						{//�L�[�����f
							(void)fscanf(pFile, "%s", &cTemp[0]);

							(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][nCntMotion].nNumKey);
						}

						if (strcmp(cTemp, "NUM_PARTICLE") == 0)
						{//�p�[�e�B�N�������f
							(void)fscanf(pFile, "%s", &cTemp[0]);

							(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].nNumParticle);

							if (m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].nNumParticle != 0)
							{
								// �p�[�e�B�N�����𐶐�
								m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle = new PARTICLE_INFO[m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].nNumParticle];

								// �p�[�e�B�N����񏉊���
								ZeroMemory(m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle, sizeof(PARTICLE_INFO) * m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].nNumParticle);
							}
						}

						if (strcmp(cTemp, "PARTICLESET") == 0 && m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle != 0)
						{// �p�[�e�B�N�����ݒ�
							while (strcmp(cTemp, "END_PARTICLESET") != 0)
							{//�I���܂Ńp�[�e�B�N���ݒ�
								(void)fscanf(pFile, "%s", &cTemp[0]);

								if (strcmp(cTemp, "KEY") == 0)
								{// �Đ��L�[�擾
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].nKey);
								}

								if (strcmp(cTemp, "FRAME") == 0)
								{// �Đ��t���[���擾
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].nFrame);
								}

								if (strcmp(cTemp, "TYPE") == 0)
								{// ��ގ擾
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].nType);
								}

								if (strcmp(cTemp, "POS") == 0)
								{//�ʒu�ǂݍ���
									D3DXVECTOR3 pos;

									(void)fscanf(pFile, "%s", &cTemp[0]);

									for (int nCntPos = 0; nCntPos < 3; nCntPos++)
									{
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].offset[nCntPos]);
									}
								}

								if (strcmp(cTemp, "PARENT") == 0)
								{// �e�p�[�c�ԍ��擾
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].nIdxParent);
								}
							}

							nCntParticle++;
						}

						//if (strcmp(cTemp, "NUM_COLLISION") == 0)
						//{// �����蔻�萔���f
						//	(void)fscanf(pFile, "%s", &cTemp[0]);

						//	(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumCollision);

						//	if (m_aMotionInfo[m_nNumMotion].nNumCollision != 0)
						//	{
						//		// �����蔻����𐶐�
						//		m_aMotionInfo[m_nNumMotion].pCollision = new COLLISION_INFO[m_aMotionInfo[m_nNumMotion].nNumCollision];

						//		// �����蔻���񏉊���
						//		ZeroMemory(m_aMotionInfo[m_nNumMotion].pCollision, sizeof(COLLISION_INFO) * m_aMotionInfo[m_nNumMotion].nNumCollision);
						//	}
						//}

						//if (strcmp(cTemp, "COLLISIONSET") == 0 && m_aMotionInfo[m_nNumMotion].pCollision != 0)
						//{// �����蔻����ݒ�
						//	while (strcmp(cTemp, "END_COLLISIONSET") != 0)
						//	{//�I���܂œ����蔻��ݒ�
						//		(void)fscanf(pFile, "%s", &cTemp[0]);

						//		if (strcmp(cTemp, "KEY") == 0)
						//		{// �Đ��L�[�擾
						//			(void)fscanf(pFile, "%s", &cTemp[0]);

						//			(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nKey);
						//		}

						//		if (strcmp(cTemp, "FRAME") == 0)
						//		{// �Đ��t���[���擾
						//			(void)fscanf(pFile, "%s", &cTemp[0]);

						//			(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nFrame);
						//		}

						//		if (strcmp(cTemp, "POS") == 0)
						//		{//�ʒu�ǂݍ���
						//			D3DXVECTOR3 pos;

						//			(void)fscanf(pFile, "%s", &cTemp[0]);

						//			for (int nCntPos = 0; nCntPos < 3; nCntPos++)
						//			{
						//				(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].offset[nCntPos]);
						//			}
						//		}

						//		if (strcmp(cTemp, "PARENT") == 0)
						//		{// �e�p�[�c�ԍ��擾
						//			(void)fscanf(pFile, "%s", &cTemp[0]);

						//			(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nIdxParent);
						//		}
						//	}

						//	nCntParticle++;
						//}

						if (strcmp(cTemp, "KEYSET") == 0)
						{//�L�[�X�^�[�g
							while (strcmp(cTemp, "END_KEYSET") != 0)
							{//�I���܂ŃL�[�ݒ�

								(void)fscanf(pFile, "%s", &cTemp[0]);

								if (strcmp(cTemp, "FRAME") == 0)
								{//�t���[�����擾
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].nFrame);
								}

								if (strcmp(cTemp, "KEY") == 0)
								{//�t���[�����擾
									while (strcmp(cTemp, "END_KEY") != 0)
									{//�I���܂ŃL�[�ݒ�
										(void)fscanf(pFile, "%s", &cTemp[0]);

										if (strcmp(cTemp, "POS") == 0)
										{//�ʒu�擾
											(void)fscanf(pFile, "%s", &cTemp[0]);

											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
										}

										if (strcmp(cTemp, "ROT") == 0)
										{//�����擾
											(void)fscanf(pFile, "%s", &cTemp[0]);

											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
										}
									}

									nCntPart++;

								}//KEYif
							}//ENDKEYwhile

							nCntKey++;
							nCntPart = 0;
						}
					}//MOTIONwhile

					nCntMotion++;
					m_aNumMotion[nCntDiv]++;
				}
				//===========================================================

				if (strcmp(cTemp, "END_SCRIPT") == 0)
				{
					break;
				}
			}//while��

			fclose(pFile);
		}
		else
		{
			assert(("���[�V�����f�[�^�ǂݍ��݂Ɏ��s", false));
		}
	}
}

//=====================================================
// �p�[�c�̍ő唼�a�擾
//=====================================================
float CMotionDiv::GetRadiusMax(int nNum)
{
	float fRadiusMax = 0.0f;
	float fTemp;

	for (int nCntParts = 0; nCntParts < m_aNumParts[nNum]; nCntParts++)
	{
		if (m_apParts[nCntParts] != nullptr)
		{// �p�[�c�̔��a�擾
			fTemp = m_apParts[nNum][nCntParts]->pParts->GetRadius();

			if (fTemp > fRadiusMax)
			{// �ő唼�a�ݒ�
				fRadiusMax = fTemp;
			}
		}
	}

	return fRadiusMax;
}

//=====================================================
// �p�[�c�̐�Έʒu�擾
//=====================================================
D3DXVECTOR3 CMotionDiv::GetMtxPos(int nNum,int nIdx)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (nIdx < 0 &&
		nIdx >= m_aNumParts[nNum])
	{// �͈͊O����
		return pos;
	}

	if (m_apParts[nNum][nIdx] != nullptr)
	{
		if (m_apParts[nNum][nIdx]->pParts != nullptr)
		{
			D3DXMATRIX mtx = *m_apParts[nNum][nIdx]->pParts->GetMatrix();

			pos =
			{
				mtx._41,
				mtx._42,
				mtx._43,
			};
		}
	}

	return pos;
}

//=====================================================
// �c���̐ݒ�
//=====================================================
void CMotionDiv::SetAfterImage(int nNum,D3DXCOLOR col, int m_nLife)
{
	for (int nCntParts = 0; nCntParts < m_aNumParts[nNum]; nCntParts++)
	{
		if (m_apParts[nNum][nCntParts] != nullptr)
		{// �c���ݒ�
			D3DXMATRIX* pMtx = m_apParts[nNum][nCntParts]->pParts->GetMatrix();
			CModel::Model* model = m_apParts[nNum][nCntParts]->pParts->GetModel();

			//CAfterImage::Create(*m_apParts[nCntParts]->m_pParts->GetMatrix(), m_apParts[nCntParts]->m_pParts->GetIdxModel(),col,m_nLife);
		}
	}
}

//=====================================================
// �S�p�[�c�̐F�ݒ�
//=====================================================
void CMotionDiv::SetAllCol(D3DXCOLOR col)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int i = 0; i < m_aNumParts[nCntDiv]; i++)
		{
			if (m_apParts[nCntDiv][i] != nullptr)
			{
				if (m_apParts[nCntDiv][i]->pParts != nullptr)
				{
					m_apParts[nCntDiv][i]->pParts->SetEmissiveCol(col);
				}
			}
		}
	}
}

//=====================================================
// �S�p�[�c�̐F���Z�b�g
//=====================================================
void CMotionDiv::ResetAllCol(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int i = 0; i < m_aNumParts[nCntDiv]; i++)
		{
			if (m_apParts[nCntDiv][i] != nullptr)
			{
				if (m_apParts[nCntDiv][i]->pParts != nullptr)
				{
					m_apParts[nCntDiv][i]->pParts->ResetColor();
				}
			}
		}
	}

}

//=====================================================
// ��������
//=====================================================
CMotionDiv* CMotionDiv::Create(char* pPathLower, char* pPathUpper)
{
	CMotionDiv* pCMotionDiv = nullptr;

	if (pCMotionDiv == nullptr)
	{
		// �C���X�^���X����
		pCMotionDiv = new CMotionDiv;

		// �Ǎ�����
		pCMotionDiv->Load(pPathLower, pPathUpper);

		// ����������
		pCMotionDiv->Init();
	}

	return pCMotionDiv;
}