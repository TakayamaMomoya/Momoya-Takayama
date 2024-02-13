//*****************************************************
//
// ���C�t�\���̏���[life.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "life.h"
#include "UI.h"

#include "texture.h"

#include "player.h"
#include "playerManager.h"

#include "UIManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float LIFE_CHANGE_RATE[CLife::LIFE_STATE_MAX]
	{
		1.00f,
		0.60f,
		0.25f,
		0.05f,
	};

	const D3DXCOLOR LIFE_COLOR[CLife::LIFE_STATE_MAX]
	{
		D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
		D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
	};
}
//=====================================================
// �R���X�g���N�^
//=====================================================
CLife::CLife(int nPriority) : CObject(nPriority)
{
	m_pUILife = nullptr;
	m_pUILifeFrame = nullptr;
	ZeroMemory(&m_info, sizeof(m_info));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CLife::~CLife()
{

}

//=====================================================
// ��������
//=====================================================
CLife *CLife::Create(int nIdx)
{
	CLife *pLife = new CLife;

	if (pLife != nullptr)
	{
		// �v���C���[�ԍ���ݒ�
		pLife->m_info.nIdxPlayer = nIdx;

		if (pLife->m_pUILifeFrame == nullptr)
		{
			// ���C�t�g�̐�������
			pLife->m_pUILifeFrame = CUI::Create();
		}

		if (pLife->m_pUILife == nullptr)
		{
			// ���C�t�̐�������
			pLife->m_pUILife = CUI::Create();
		}

		// ���C�t�̏�����
		pLife->Init();
	}

	return pLife;
}

//=====================================================
// ����������
//=====================================================
HRESULT CLife::Init(void)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		// �����l�ݒ菈���i�ݒ肵�ĂȂ������炱��j
		m_pUILife->SetSize(50.0f, 100.0f);
		m_pUILife->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pUILifeFrame->SetSize(50.0f, 100.0f);
		m_pUILifeFrame->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CLife::Uninit(void)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		m_pUILife->Uninit();
		m_pUILifeFrame->Uninit();
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CLife::Update(void)
{
	// �v���C���[�̗̑�
	SetLife();
}

//=====================================================
// �`�揈��
//=====================================================
void CLife::Draw(void)
{

}

//=====================================================
// �ʒu�ݒ菈��
//=====================================================
void CLife::SetLife(void)
{
	// �v���C���[�Ǘ��̎擾
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CPlayer* pPlayer = nullptr;

	if (pPlayerManager != nullptr)
	{
		// �v���C���[�̎擾
		pPlayer = pPlayerManager->GetPlayer(m_info.nIdxPlayer);
	}

	if (pPlayerManager != nullptr )
	{
		if (pPlayer != nullptr)
		{
			// ���݂̗̑͂ƑO�̗̑͂��Ⴄ
			if (m_info.fLife != pPlayer->GetLife())
			{
				// ���݂̗̑͂��擾
				m_info.fLife = pPlayer->GetLife();

				// �F�ύX�t���O
				m_info.bColorChange = true;

				// �̗͂̏����l���擾
				float fMaxLife = pPlayerManager->GetPlayerParam().fInitialLife;

				// �Q�[�W�̏���ʂ��v�Z
				float fLifeRatio = (m_info.fLife / fMaxLife);

				// �T�C�Y��ύX
				m_pUILife->SetSize(
					LIFE::WIDTH_MAX * fLifeRatio,
					LIFE::HEIGHT_MAX * fLifeRatio);

				// �F��ύX
				for (int nCount = 0; nCount < CLife::LIFE_STATE_MAX; nCount++)
				{
					// ���݂̗̑͏󋵂𔻒�
					if (m_info.fLife >= fMaxLife * LIFE_CHANGE_RATE[nCount])
					{
						// �F��ݒ�
						m_info.colLife = LIFE_COLOR[nCount];
						m_pUILife->SetCol(LIFE_COLOR[nCount]);

						break;
					}
				}

				m_pUILife->SetVtx();
				m_pUILifeFrame->SetVtx();
			}
		}
		else
		{
			// �̗͂͂Ȃ�
			m_info.fLife = 0.0f;

			// �̗͂̏����l���擾
			float fMaxLife = pPlayerManager->GetPlayerParam().fInitialLife;

			// �Q�[�W�̏���ʂ��v�Z
			float fLifeRatio = (m_info.fLife / fMaxLife);

			// �T�C�Y��ύX
			m_pUILife->SetSize(
				LIFE::WIDTH_MAX * fLifeRatio,
				LIFE::HEIGHT_MAX * fLifeRatio);

			m_pUILife->SetVtx();
			m_pUILifeFrame->SetVtx();
		}
	}
}

//=====================================================
// �ʒu�ݒ菈��
//=====================================================
void CLife::SetPosition(D3DXVECTOR3 pos)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		m_info.posLife = pos;
		
		m_pUILife->SetPosition(pos);
		m_pUILifeFrame->SetPosition(pos);

		m_pUILife->SetVtx();
		m_pUILifeFrame->SetVtx();
	}
}

//=====================================================
// �傫���ݒ菈��
//=====================================================
void CLife::SetSize(float width, float height)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		m_info.fWidth = width;
		m_info.fHeight = height;

		m_pUILife->SetSize(width, height);
		m_pUILifeFrame->SetSize(width, height);

		m_pUILife->SetVtx();
		m_pUILifeFrame->SetVtx();
	}
}

//=====================================================
// �F�ݒ菈��
//=====================================================
void CLife::SetCol(D3DXCOLOR colLife, D3DXCOLOR colLifeFrame)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		m_info.colLife = colLife;
		m_info.colLifeFrame = colLifeFrame;

		m_pUILife->SetCol(colLife);
		m_pUILifeFrame->SetCol(colLifeFrame);

		m_pUILife->SetVtx();
		m_pUILifeFrame->SetVtx();
	}
}

//=====================================================
// �e�N�X�`���ݒ菈��
//=====================================================
void CLife::SetTexture(const char* pFileName)
{
	if (m_pUILife != nullptr &&
		m_pUILifeFrame != nullptr)
	{
		CTexture* pTexture = CTexture::GetInstance();

		if (pTexture != nullptr)
		{
			m_info.nIdxTexture = pTexture->Regist(pFileName);
		}

		m_pUILife->SetIdxTexture(m_info.nIdxTexture);
		m_pUILifeFrame->SetIdxTexture(m_info.nIdxTexture);

		m_pUILife->SetVtx();
		m_pUILifeFrame->SetVtx();
	}
}