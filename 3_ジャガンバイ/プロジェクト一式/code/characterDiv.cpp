//*****************************************************
//
// �����L�����N�^�[�̏���[character.h]
// Author:����쏟
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "characterDiv.h"
#include "motionDiv.h"
#include "renderer.h"
#include "universal.h"

//*****************************************************
// �}�N����`
//*****************************************************

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CCharacterDiv::CCharacterDiv(int nPriority)
{
	ZeroMemory(&m_info, sizeof(CCharacterDiv::SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CCharacterDiv::~CCharacterDiv()
{

}

//=====================================================
// ��������
//=====================================================
CCharacterDiv* CCharacterDiv::Create(char* pPathLower, char* pPathUpper)
{
	CCharacterDiv* pCharacter = nullptr;

	pCharacter = new CCharacterDiv;

	if (pCharacter != nullptr)
	{
		pCharacter->m_info.apPath[PARTS_LOWER] = pPathLower;
		pCharacter->m_info.apPath[PARTS_UPPER] = pPathUpper;
		pCharacter->Init();
	}

	return pCharacter;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CCharacterDiv::Load(char* pPathLower, char* pPathUpper)
{
	if (m_info.pBody == nullptr && pPathLower != nullptr && pPathUpper != nullptr)
	{
		m_info.pBody = CMotionDiv::Create(pPathLower, pPathUpper);

		if (m_info.pBody != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 rot = GetRot();

			m_info.pBody->SetPosition(pos);
			m_info.pBody->SetRot(rot);
		}
	}
}

//=====================================================
// ����������
//=====================================================
HRESULT CCharacterDiv::Init(void)
{
	return S_OK;
}

//=====================================================
// �ʒu�̐ݒ�
//=====================================================
void CCharacterDiv::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;
	
	if (m_info.pBody != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_info.pBody->SetPosition(pos);
	}
}

//=====================================================
// �I������
//=====================================================
void CCharacterDiv::Uninit(void)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->Uninit();
		m_info.pBody = nullptr;
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CCharacterDiv::Update(void)
{
	if (m_info.pBody != nullptr)
	{// �̂̒Ǐ]
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 rot = GetRot();

		m_info.pBody->SetPosition(pos);
		m_info.pBody->SetRot(rot);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CCharacterDiv::Draw(void)
{
	//�ϐ��錾
	D3DXMATRIX mtxRotModel, mtxTransModel;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_info.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
		m_info.rot.y, m_info.rot.x, m_info.rot.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxRotModel);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel,
		m_info.pos.x, m_info.pos.y, m_info.pos.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxTransModel);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_info.mtxWorld);
}

//=====================================================
// ���[�V�����ݒ�
//=====================================================
void CCharacterDiv::SetMotion(int nNum, int nMotion)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetMotion(nNum,nMotion);
	}
}

//=====================================================
// ���[�V�����擾
//=====================================================
int CCharacterDiv::GetMotion(int nNum)
{
	int nMotion = 0;

	if (m_info.pBody != nullptr)
	{
		nMotion = m_info.pBody->GetMotion(nNum);
	}

	return nMotion;
}