//*****************************************************
//
// �c�e���\���̏���[UIMagazine.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "UIMagazine.h"
#include "UI.h"
#include "number.h"

#include "texture.h"

#include "player.h"
#include "playerManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int DIG_MAG_NUM = 3;		// ���e���̌���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIMagazine::CUIMagazine(int nPriority) : CObject(nPriority)
{
	m_pNumDig = nullptr;

	ZeroMemory(&m_info, sizeof(m_info));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUIMagazine::~CUIMagazine()
{

}

//=====================================================
// ��������
//=====================================================
CUIMagazine* CUIMagazine::Create(int nIdx)
{
	CUIMagazine *pMagazine = nullptr;

	pMagazine = new CUIMagazine;

	if (pMagazine != nullptr)
	{
		// �v���C���[�ԍ���ݒ�
		pMagazine->m_info.nIdxPlayer = nIdx;

		// ���e���g�̐�������
		CUI* pUIMagazineFrame = CUI::Create();

		if (pUIMagazineFrame != nullptr)
		{
			pMagazine->m_pUIMagazineFrame = pUIMagazineFrame;
		}

		// ���e���̐�������
		CUI* pUIMagazine = CUI::Create();

		if (pUIMagazine != nullptr)
		{
			pMagazine->m_pUIMagazine = pUIMagazine;
		}

		// ���e���i�����j�̐�������
		CNumber* pDigMag = CNumber::Create(DIG_MAG_NUM, 0);

		if (pDigMag != nullptr)
		{
			pMagazine->m_pNumDig = pDigMag;
		}

		// ���e��UI�̏�����
		pMagazine->Init();
	}

	return pMagazine;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUIMagazine::Init(void)
{
	if (m_pNumDig != nullptr &&
		m_pUIMagazine != nullptr &&
		m_pUIMagazineFrame != nullptr)
	{
		// �����l�ݒ菈���i�ݒ肵�ĂȂ������炱��j
		m_pUIMagazineFrame->SetSize(50.0f, 100.0f);
		m_pUIMagazineFrame->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pUIMagazine->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pUIMagazine->SetSize(50.0f, 100.0f);
		m_pNumDig->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pNumDig->SetSizeAll(50.0f, 100.0f);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUIMagazine::Uninit(void)
{
	if (m_pNumDig != nullptr &&
		m_pUIMagazine != nullptr &&
		m_pUIMagazineFrame != nullptr)
	{
		m_pUIMagazineFrame->Uninit();
		m_pUIMagazine->Uninit();
		m_pNumDig->Uninit();
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CUIMagazine::Update(void)
{
	if (m_pNumDig != nullptr)
	{
		SetNumMagazine();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CUIMagazine::Draw(void)
{

}

//=====================================================
// �ʒu�ݒ菈��
//=====================================================
void CUIMagazine::SetPosition(D3DXVECTOR3 posUIMagazine,D3DXVECTOR3 posUIMagazineFrame,D3DXVECTOR3 posNum)
{
	if (m_pNumDig != nullptr &&
		m_pUIMagazine != nullptr &&
		m_pUIMagazineFrame != nullptr)
	{
		m_info.posUIMagazine = posUIMagazine;
		m_info.posUIMagazineFrame = posUIMagazineFrame;
		m_info.posNum = posNum;

		m_pUIMagazine->SetPosition(posUIMagazine);
		m_pUIMagazineFrame->SetPosition(posUIMagazineFrame);
		m_pNumDig->SetPosition(posNum);

		SetVtxGage();
		m_pUIMagazineFrame->SetVtx();
	}
}

//=====================================================
// �傫���ݒ菈��
//=====================================================
void CUIMagazine::SetSize(float width, float height, float widthNum, float heightNum)
{
	if (m_pNumDig != nullptr)
	{
		m_info.fWidth = width;
		m_info.fHeight = height;

		m_info.fWidthNum = widthNum;
		m_info.fHeightNum = heightNum;

		m_pUIMagazine->SetSize(width, height);
		m_pUIMagazineFrame->SetSize(width, height);
		m_pNumDig->SetSizeAll(widthNum, heightNum);

		SetVtxGage();
		m_pUIMagazineFrame->SetVtx();
	}
}

//=====================================================
// �F�ݒ菈��
//=====================================================
void CUIMagazine::SetCol(D3DXCOLOR colMagazine, D3DXCOLOR colMagazineFrame, D3DXCOLOR colNum)
{
	if (m_pNumDig != nullptr)
	{
		m_info.colMagazine = colMagazine;
		m_info.colMagazineFrame = colMagazineFrame;
		m_info.colNum = colNum;

		m_pUIMagazine->SetCol(colMagazine);
		m_pUIMagazineFrame->SetCol(colMagazineFrame);
		m_pNumDig->SetColor(colNum);

		SetVtxGage();
	}
}

//=====================================================
// �e�N�X�`���ݒ菈��
//=====================================================
void CUIMagazine::SetTexture(const char* pFileName)
{
	if (m_pUIMagazine != nullptr &&
		m_pUIMagazineFrame != nullptr)
	{
		CTexture* pTexture = CTexture::GetInstance();

		if (pTexture != nullptr)
		{
			m_info.nIdxTexture = pTexture->Regist(pFileName);
		}

		m_pUIMagazine->SetIdxTexture(m_info.nIdxTexture);
		m_pUIMagazineFrame->SetIdxTexture(m_info.nIdxTexture);

		SetVtxGage();
		m_pUIMagazineFrame->SetVtx();
	}
}

//=====================================================
// ���e���ݒ菈��
//=====================================================
void CUIMagazine::SetNumMagazine(void)
{
	// �|�C���^�̎擾
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CPlayer* pPlayer = nullptr;
	CWeapon* pWeapon = nullptr;

	int nMaxBullet = 0;

	if (pPlayerManager != nullptr)
	{
		// �v���C���[�Ǘ��̎擾
		pPlayer = pPlayerManager->GetPlayer(m_info.nIdxPlayer);
	}

	if (pPlayer != nullptr)
	{
		// �v���C���[�̎擾
		pWeapon = pPlayer->GetWeapon();
	}

	if (pWeapon != nullptr)
	{
		// ���݂̑��e���ƑO��̑��e�����Ⴄ
		if (m_info.nBullet != pWeapon->GetBullet())
		{
			// �v���C���[�̒e���擾
			m_info.nBullet = pWeapon->GetBullet();
			nMaxBullet = pWeapon->GetMaxBullet();

			// �F�ύX�t���O���I��
			m_info.bColorChange = true;
			
			// ���݂̑��e���̊������Z�o
			float fLifeRatio = (1.0f - ((float)m_info.nBullet / (float)nMaxBullet));

			// �c���̌��Z�ʂ��Z�o
			m_info.fHeightSub = fLifeRatio * m_info.fHeight;

			// �ݒ菈��
			m_pUIMagazine->SetTex(D3DXVECTOR2(0.0f, fLifeRatio), D3DXVECTOR2(1.0f, 1.0f));
			SetVtxGage();
			m_pUIMagazineFrame->SetVtx();
		}
	}
	else
	{
		// �e��0
		m_info.nBullet = 0;

		// ���݂̑��e���̊������Z�o
		float fLifeRatio = (1.0f - ((float)m_info.nBullet / (float)nMaxBullet));

		// �c���̌��Z�ʂ��Z�o
		m_info.fHeightSub = fLifeRatio * m_info.fHeight;

		// �ݒ菈��
		m_pUIMagazine->SetTex(D3DXVECTOR2(0.0f, fLifeRatio), D3DXVECTOR2(1.0f, 1.0f));
		SetVtxGage();
		m_pUIMagazineFrame->SetVtx();
	}

	if (m_pNumDig != nullptr)
	{
		// �e�𔽉f
		m_pNumDig->SetValue(m_info.nBullet, DIG_MAG_NUM);
	}
}

//=====================================================
// �Q�[�W�p�̒��_���ݒ菈��
//=====================================================
void CUIMagazine::SetVtxGage(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	if (m_pUIMagazine != nullptr)
	{
		if (m_pUIMagazine->GetVtxBuff() != nullptr)
		{
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			m_pUIMagazine->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

			// �Ίp���̊p�x�擾
			float fAngleUp = atan2f(m_info.fWidth, m_info.fHeight);
			// �����̎擾
			float fLengthUp = sqrtf(m_info.fWidth * m_info.fWidth + m_info.fHeight * m_info.fHeight);

			// ���Z�ʂ̒������擾
			float fLengthSub = sqrtf(m_info.fWidthSub * m_info.fWidthSub + m_info.fHeightSub * m_info.fHeightSub);

			// �덷�͈͂�����
			if (fLengthUp - fLengthSub <= 1.0f)
			{
				fLengthUp = 0.0f;
				fLengthSub = 0.0f;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3
			(
				m_info.posUIMagazine.x - m_info.fWidth,
				m_info.posUIMagazine.y + cosf(0.0f - D3DX_PI + fAngleUp) * (fLengthUp * 2.0f) + (fLengthSub * 2.0f),
				0.0f
			);
			pVtx[1].pos = D3DXVECTOR3
			(
				m_info.posUIMagazine.x + m_info.fWidth,
				m_info.posUIMagazine.y + cosf(0.0f + D3DX_PI - fAngleUp) * (fLengthUp * 2.0f) + (fLengthSub * 2.0f),
				0.0f
			);
			pVtx[2].pos = D3DXVECTOR3
			(
				m_info.posUIMagazine.x - m_info.fWidth,
				m_info.posUIMagazine.y + cosf(0.0f - fAngleUp),
				0.0f
			);
			pVtx[3].pos = D3DXVECTOR3
			(
				m_info.posUIMagazine.x + m_info.fWidth,
				m_info.posUIMagazine.y + cosf(0.0f + fAngleUp),
				0.0f
			);


			// ���_�o�b�t�@�̃A�����b�N
			m_pUIMagazine->GetVtxBuff()->Unlock();
		}
	}
}