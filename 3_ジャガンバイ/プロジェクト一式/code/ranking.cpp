//*****************************************************
//
// �����L���O�̏���[Ranking.cpp]
// Author:����쏟
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************

#include "ranking.h"
#include "main.h"

#include <stdio.h>

#include "manager.h"
#include "camera.h"
#include "record.h"

#include "object2D.h"
#include "number.h"

#include "rank.h"
#include "rank_acting.h"

#include "texture.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputmouse.h"
#include "sound.h"

#include "fade.h"

//*****************************************************
// �}�N����`
//*****************************************************

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	// ��ѕ�
	const D3DXVECTOR3 RECORD_TEXT_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.15f, 0.0f);	// ��ѕ��̈ʒu
	const float RECORD_TEXT_SIZE = 250.0f;										// ��ѕ��̑傫��
	const float RECORD_TEXT_WIDTH = 1.0f * RECORD_TEXT_SIZE;					// ��ѕ��̕�
	const float RECORD_TEXT_HEIGHT = 0.2f * RECORD_TEXT_SIZE;					// ��ѕ��̍���
	const char* RECORD_TEXT_TEX = "data\\TEXTURE\\UI\\renk_text_record.png";	// ��ѕ��̃p�X

	// ��ю�ރA�C�R��
	const D3DXVECTOR3 GENRE_ICON_POS[CRecord::GENRE_TYPE_MAX] =
	{// �ʒu[���]
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.55f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.55f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.55f, 0.0f),
	};
	const float GENRE_ICON_SIZE[CRecord::GENRE_TYPE_MAX] =
	{// �傫��[���]
		250.0f,
		250.0f,
		250.0f,
	};
	const float GENRE_ICON_WIDTH[CRecord::GENRE_TYPE_MAX] =
	{// ����[���]
		0.5f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.5f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.5f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
	};
	const float GENRE_ICON_HEIGHT[CRecord::GENRE_TYPE_MAX] =
	{// �c��[���]
		0.4f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.6f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.5f * GENRE_ICON_SIZE[CRecord::GENRE_TYPE_DESTROY],
	};
	const char* GENRE_ICON_TEX[CRecord::GENRE_TYPE_MAX] =
	{// �e�N�X�`���̃p�X[���]
		"data\\TEXTURE\\UI\\Icon_destroyer.png",
		"data\\TEXTURE\\UI\\Icon_madman.png",
		"data\\TEXTURE\\UI\\Icon_engineer.png",
	};

	// ��ю�ޕ�
	const D3DXVECTOR3 GENRE_TEXT_POS[CRecord::GENRE_TYPE_MAX] =
	{// �ʒu[���]
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.35f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.35f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.35f, 0.0f),
	};
	const float GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_MAX] =
	{// �傫��[���]
		175.0f,
		175.0f,
		175.0f,
	};
	const float GENRE_TEXT_WIDTH[CRecord::GENRE_TYPE_MAX] =
	{// ����[���]
		0.8f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.8f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.8f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
	};
	const float GENRE_TEXT_HEIGHT[CRecord::GENRE_TYPE_MAX] =
	{// �c��[���]
		0.3f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.3f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
		0.3f * GENRE_TEXT_SIZE[CRecord::GENRE_TYPE_DESTROY],
	};
	const char* GENRE_TEXT_TEX[CRecord::GENRE_TYPE_MAX] =
	{// �e�N�X�`���̃p�X[���]
		"data\\TEXTURE\\UI\\rank_text_destroyer.png",
		"data\\TEXTURE\\UI\\rank_text_madman.png",
		"data\\TEXTURE\\UI\\rank_text_speedy.png",
	};

	// ��\��
	const D3DXVECTOR3 FACE_POS[NUM_PLAYER] =
	{// �ʒu[�v���C���[�ԍ�]
		D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.3f,  0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.425f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.550f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.675f,0.0f),
	};
	const float FACE_SIZE = 70.0f;
	const float FACE_WIDTH = 0.5f * FACE_SIZE;
	const float FACE_HEIGHT = 0.6f * FACE_SIZE;
	const char* FACE_FILE_NAME[NUM_PLAYER] =
	{// �t�@�C���� [�v���C���[�ԍ�]
		"data\\TEXTURE\\UI\\player01.png",
		"data\\TEXTURE\\UI\\player02.png",
		"data\\TEXTURE\\UI\\player03.png",
		"data\\TEXTURE\\UI\\player04.png",
	};

	// ����
	const D3DXVECTOR3 NUMBER_POS[NUM_PLAYER] =
	{// �ʒu[�v���C���[�ԍ�]
		D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.3f,  0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.425f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.550f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.675f,0.0f),
	};
	const float NUMBER_SIZE = 25.0f;
	const float NUMBER_WIDTH = 1.0f * NUMBER_SIZE;
	const float NUMBER_HEIGHT = 1.0f * NUMBER_SIZE;
	const char* NUMBER_TEX = nullptr;

	const int GENRE_CHANGE_COUNT = 300;		// ��ނ�ς���܂ł̎���

	const char* RANKING_BIN_FILE[CRecord::GENRE_TYPE_MAX] =
	{// �����L���O�̃t�@�C����
		"data\\BINARY\\ranking_destroyer",
		"data\\BINARY\\ranking_madman",
		"data\\BINARY\\ranking_engineer",
	};

	// �����L���O�̐���
	const D3DXVECTOR3 RANK_POS_NUM = D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.5f, 0.0f);
	const D3DXVECTOR3 RANK_POS_NUM_WIDTH = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.1f, 0.0f);
	const D3DXVECTOR3 RANK_POS_WIDTH = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);

	const float RANK_NUM_SIZE = 25.0f;
	const float RANK_NUM_WIDTH = 1.0f * RANK_NUM_SIZE;
	const float RANK_NUM_HEIGHT = 1.0f * RANK_NUM_SIZE;

	const float RANK_SIZE = 50.0f;
	const float RANK_WIDTH = 0.5f * RANK_SIZE;
	const float RANK_HEIGHT = 0.4f * RANK_SIZE;

	const float ALPHA_UPPER = 1.0f;			// ���l�̏����
	const float ALPHA_LOWER = 0.25f;		// ���l�̉�����
	const float ALPHA_CHANGE = 0.1f;		// ���l�̕ω���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CRanking::CRanking()
{
	ZeroMemory(&m_infoVisualUi, sizeof(m_infoVisualUi));
	ZeroMemory(&m_apRank[0], sizeof(m_apRank));
	m_pRankActing = nullptr;
	m_typeGenre = (CRecord::GENRE_TYPE)0;
	m_nGenreCnt = 0;

	for (int nCount = 0; nCount < CRecord::GENRE_TYPE_MAX; nCount++)
	{
		ZeroMemory(&m_aIsAlphaChange[nCount][0], sizeof(m_aIsAlphaChange[nCount]));
	}
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRanking::~CRanking()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CRanking::Init(void)
{
	// ��т̃|�C���^���擾
	CRecord* pRecord = CRecord::GetInstance();

	if (pRecord != nullptr)
	{
		// ��т̃\�[�g
		pRecord->AllSort();
	}

	// UI�̏���������
	if (InitUi() != S_OK)
	{
		return E_FAIL;
	}
	
	// �����N�̏���������
	if (InitRank() != S_OK)
	{
		return E_FAIL;
	}

	if (m_apRank[m_typeGenre] != nullptr)
	{
		// �����N�̐ݒ菈��
		SetRank();
	}

	// �����L���O���Z�̐���
	m_pRankActing = CRankActing::Create();
	
	if (m_pRankActing == nullptr)
	{
		return E_FAIL;
	}

	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_BGM_RANKING);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CRanking::Uninit(void)
{
	// ��т̏I������
	CRecord* pRecord = CRecord::GetInstance();

	if (pRecord != nullptr)
	{
		pRecord->Uninit();
	}

	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CRanking::Update(void)
{
	// ���͏��擾
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

	CFade *pFade = CFade::GetInstance();

	// �V�[���̍X�V
	CScene::Update();

	// ���UI�̍X�V
	UpdateUiRecord();

	// ���Z�̍X�V����
	if (m_pRankActing != nullptr)
	{
		m_pRankActing->Update();
	}

	// ���l�̍X�V����
	UpdateNum();

	// ��ʑJ��==========================================
	if (pKeyboard->GetTrigger(DIK_RETURN) ||
		pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
	{//ENTER�L�[�������ꂽ��
		//�^�C�g���Ɉڍs
		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_TITLE);
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CRanking::Draw(void)
{

}

//=====================================================
// UI�̏���������
//=====================================================
HRESULT CRanking::InitUi(void)
{
	// �e�N�X�`���̃|�C���^���擾
	CTexture* pTexture = CTexture::GetInstance();

	// ��т̃|�C���^���擾
	CRecord* pRecord = CRecord::GetInstance();

	// ���R�[�h�e�L�X�g�̐���
	m_infoVisualUi.pRecordText = CObject2D::Create(7);

	if (m_infoVisualUi.pRecordText != nullptr)
	{
		m_infoVisualUi.pRecordText->SetPosition(RECORD_TEXT_POS);
		m_infoVisualUi.pRecordText->SetSize(RECORD_TEXT_WIDTH, RECORD_TEXT_HEIGHT);

		if (pTexture != nullptr)
		{
			int nIdx = pTexture->Regist(RECORD_TEXT_TEX);
			m_infoVisualUi.pRecordText->SetIdxTexture(nIdx);
		}

		m_infoVisualUi.pRecordText->SetVtx();
	}
	if (m_infoVisualUi.pRecordText == nullptr)
	{
		return E_FAIL;
	}

	// ��ރA�C�R���̐���
	m_infoVisualUi.pGenreIcon = CObject2D::Create(7);

	if (m_infoVisualUi.pGenreIcon == nullptr)
	{
		return E_FAIL;
	}

	// ��ނ̃e�L�X�g�̐���
	m_infoVisualUi.pGenreText = CObject2D::Create(7);

	if (m_infoVisualUi.pGenreText == nullptr)
	{
		return E_FAIL;
	}

	if (pRecord != nullptr)
	{
		int nMaxPlayer = pRecord->GetNumSuvived();

		for (int nCount = 0; nCount < nMaxPlayer; nCount++)
		{
			// �v���C���[��̐���
			m_infoVisualUi.apFace[nCount] = CObject2D::Create(7);

			if (m_infoVisualUi.apFace[nCount] == nullptr)
			{
				return E_FAIL;
			}

			// �����̐���
			m_infoVisualUi.apNumber[nCount] = CNumber::Create(4, 0);

			if (m_infoVisualUi.apNumber[nCount] == nullptr)
			{
				return E_FAIL;
			}
		}
	}

	// ��ю�ނ̐ݒ菈��
	SetUiRecord();

	return S_OK;
}

//=====================================================
// UI�̏���������
//=====================================================
HRESULT CRanking::InitRank(void)
{
	for (int nCount = 0; nCount < CRecord::GENRE_TYPE_MAX; nCount++)
	{
		if (m_apRank[nCount] == nullptr)
		{
			// �����N�̐���
			m_apRank[nCount] = CRank::Create((CRecord::GENRE_TYPE)nCount,RANKING_BIN_FILE[nCount]);

			if (m_apRank[nCount] == nullptr)
			{
				return E_FAIL;
			}
		}
	}
	
	return S_OK;
}

//=====================================================
// ��т̐ݒ菈��
//=====================================================
void CRanking::SetUiRecord(void)
{
	// �e�N�X�`���̃|�C���^���擾
	CTexture* pTexture = CTexture::GetInstance();

	// ��т̃|�C���^���擾
	CRecord* pRecord = CRecord::GetInstance();

	// ��уA�C�R���̐ݒ菈��
	if (m_infoVisualUi.pGenreIcon != nullptr)
	{
		m_infoVisualUi.pGenreIcon->SetPosition(GENRE_ICON_POS[m_typeGenre]);
		m_infoVisualUi.pGenreIcon->SetSize(GENRE_ICON_WIDTH[m_typeGenre], GENRE_ICON_HEIGHT[m_typeGenre]);

		if (pTexture != nullptr)
		{
			int nIdx = pTexture->Regist(GENRE_ICON_TEX[m_typeGenre]);
			m_infoVisualUi.pGenreIcon->SetIdxTexture(nIdx);
		}

		m_infoVisualUi.pGenreIcon->SetVtx();
	}

	// ��уe�L�X�g�̐ݒ菈��
	if (m_infoVisualUi.pGenreText != nullptr)
	{
		m_infoVisualUi.pGenreText->SetPosition(GENRE_TEXT_POS[m_typeGenre]);
		m_infoVisualUi.pGenreText->SetSize(GENRE_TEXT_WIDTH[m_typeGenre], GENRE_TEXT_HEIGHT[m_typeGenre]);

		if (pTexture != nullptr)
		{
			int nIdx = pTexture->Regist(GENRE_TEXT_TEX[m_typeGenre]);
			m_infoVisualUi.pGenreText->SetIdxTexture(nIdx);
		}

		m_infoVisualUi.pGenreText->SetVtx();
	}

	if (pRecord != nullptr)
	{
		int nMaxPlayer = pRecord->GetNumSuvived();

		for (int nCount = 0; nCount < nMaxPlayer; nCount++)
		{
			if (pRecord != nullptr)
			{
				int nRank = 0;		// ����
				int nNumRank = 0;	// ���ʂł̒l

				// ��ނ��Ƃɐ�т̏��ʂ��擾
				switch (m_typeGenre)
				{
				case CRecord::GENRE_TYPE_DESTROY:

					nRank = pRecord->GetDestroyRank(nCount);
					nNumRank = pRecord->GetDestroy(nCount);

					break;
				case CRecord::GENRE_TYPE_MADMAN:

					nRank = pRecord->GetMadmanRank(nCount);
					nNumRank = pRecord->GetMadman(nCount);

					break;
				case CRecord::GENRE_TYPE_ENGINEER:

					nRank = pRecord->GetEngineerRank();
					nNumRank = pRecord->GetEngineer();

					break;
				}

				// ��UI�̐ݒ菈��
				if (m_infoVisualUi.apFace[nCount] != nullptr)
				{
					// ��UI�����ʂ̈ʒu�ɕύX
					m_infoVisualUi.apFace[nCount]->SetPosition(FACE_POS[nRank]);
					m_infoVisualUi.apFace[nCount]->SetSize(FACE_WIDTH, FACE_HEIGHT);

					if (pTexture != nullptr)
					{
						int nIdx = pTexture->Regist(FACE_FILE_NAME[nCount]);
						m_infoVisualUi.apFace[nCount]->SetIdxTexture(nIdx);
					}

					m_infoVisualUi.apFace[nCount]->SetVtx();
				}

				// �����̐ݒ菈��
				if (m_infoVisualUi.apNumber[nCount] != nullptr)
				{
					m_infoVisualUi.apNumber[nCount]->SetPosition(NUMBER_POS[nRank]);
					m_infoVisualUi.apNumber[nCount]->SetSizeAll(NUMBER_WIDTH, NUMBER_WIDTH);
					m_infoVisualUi.apNumber[nCount]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					m_infoVisualUi.apNumber[nCount]->SetValue(nNumRank, 4);

					if (nRank == 0)
					{
						m_infoVisualUi.apNumber[nCount]->SetTex("data\\TEXTURE\\UI\\number002.png");
					}
				}
			}
		}
	}
}

//=====================================================
// �����N�̐ݒ菈��
//=====================================================
void CRanking::SetRank(void)
{
	if (m_apRank[m_typeGenre] != nullptr)
	{
		// �����NUI�̐���
		m_apRank[m_typeGenre]->SetUi();

		// �ݒ菈��
		m_apRank[m_typeGenre]->SetPos(RANK_POS_NUM, RANK_POS_NUM_WIDTH, RANK_POS_WIDTH);
		m_apRank[m_typeGenre]->SetSize(RANK_NUM_WIDTH, RANK_NUM_HEIGHT, RANK_WIDTH, RANK_HEIGHT);
	}
}

//=====================================================
// ���UI�̍X�V
//=====================================================
void CRanking::UpdateUiRecord(void)
{
	// ��ރJ�E���g���ݒ�l�ȉ�
	if (m_nGenreCnt <= GENRE_CHANGE_COUNT)
	{
		// �J�E���g�𑝂₷
		m_nGenreCnt++;
	}
	else
	{
		// �J�E���g��������
		m_nGenreCnt = 0;

		if (m_apRank[m_typeGenre] != nullptr)
		{
			// �����N�̐��l�����Z�b�g
			m_apRank[m_typeGenre]->ReSetUi();
		}

		// ��ю�ނ𑝂₷
		m_typeGenre = (CRecord::GENRE_TYPE)(m_typeGenre + 1);

		// ��ю�ނ��ő�l
		if (m_typeGenre >= CRecord::GENRE_TYPE_MAX)
		{
			// ��ю�ނ�������
			m_typeGenre = (CRecord::GENRE_TYPE)0;
		}

		// ��ю�ނ̐ݒ�
		SetUiRecord();

		// �����N�̐�����\��
		SetRank();
	}
}

//=====================================================
// ���l�̍X�V
//=====================================================
void CRanking::UpdateNum(void)
{
	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (m_apRank[m_typeGenre] != nullptr)
		{
			if (m_apRank[m_typeGenre]->GetIsUpdate(nCount))
			{
				if (m_infoVisualUi.apNumber[nCount] != nullptr &&
					m_apRank[m_typeGenre]->GetNum(m_apRank[m_typeGenre]->GetUpdateIdx(nCount)) != nullptr)
				{
					// �F�̏��擾
					D3DXCOLOR col = m_infoVisualUi.apNumber[nCount]->GetColor();

					// ���l�̑�������
					if (m_aIsAlphaChange[m_typeGenre][nCount] == false)
					{
						col.a -= ALPHA_CHANGE;
					}
					else if (m_aIsAlphaChange[m_typeGenre][nCount] == true)
					{
						col.a += ALPHA_CHANGE;
					}

					// ���l�̑��������؂�ւ�����
					if (col.a <= ALPHA_LOWER)
					{
						m_aIsAlphaChange[m_typeGenre][nCount] = true;
					}
					else if (col.a >= ALPHA_UPPER)
					{
						m_aIsAlphaChange[m_typeGenre][nCount] = false;
					}

					m_infoVisualUi.apNumber[nCount]->SetColor(col);
					m_apRank[m_typeGenre]->GetNum(m_apRank[m_typeGenre]->GetUpdateIdx(nCount))->SetColor(col);
				}
			}
		}
	}
}