//*****************************************************
//
// �����L���O�̏���[Rank.cpp]
// Author:����쏟
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************

#include "rank.h"
#include "object.h"

#include <stdio.h>

#include "record.h"

#include "object2D.h"
#include "number.h"

#include "texture.h"

//*****************************************************
// �}�N����`
//*****************************************************

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* RANK_TEX[RANK::NUM_MAX] =
	{// �e�N�X�`���̃p�X[���]
		"data\\TEXTURE\\UI\\1st.png",
		"data\\TEXTURE\\UI\\2nd.png",
		"data\\TEXTURE\\UI\\3rd.png",
	};
}
//=====================================================
// �R���X�g���N�^
//=====================================================
CRank::CRank()
{
	ZeroMemory(&m_infoVisualUi,sizeof(m_infoVisualUi));

	m_typeGenre = (CRecord::GENRE_TYPE)0;
	m_pFileName = nullptr;

	ZeroMemory(&m_aRankScore, sizeof(m_aRankScore));
	ZeroMemory(&m_aRankFace, sizeof(m_aRankFace));
	ZeroMemory(&m_aUpdateIdx, sizeof(m_aUpdateIdx));
	ZeroMemory(&m_aIsUpdate, sizeof(m_aIsUpdate));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRank::~CRank()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CRank::Init(void)
{
	// �����ݒ�
	Reset();
	Sort();

	// �擾�����X�R�A�Ń����L���O�ݒ�
	Set();

	// �ۑ�
	Save();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CRank::Uninit(void)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apRank[nCount] != nullptr)
		{
			m_infoVisualUi.apRank[nCount]->Uninit();
			m_infoVisualUi.apRank[nCount] = nullptr;
		}

		if (m_infoVisualUi.apNum[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->Uninit();
			m_infoVisualUi.apNum[nCount] = nullptr;
		}
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CRank::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CRank::Draw(void)
{

}

//=====================================================
// ��������
//=====================================================
CRank* CRank::Create(CRecord::GENRE_TYPE typeGenre,const char* pFileName)
{
	CRank* pRank = new CRank;

	if (pRank != nullptr)
	{
		// ��ю�ނ�ݒ�
		pRank->m_typeGenre = typeGenre;

		// �����L���O�t�@�C��������
		pRank->m_pFileName = (char*)pFileName;

		// �����N�̏�����
		if (pRank->Init() != S_OK)
		{
			return nullptr;
		}
	}

	return pRank;
}

//====================================================
// UI�̐ݒ菈��
//=====================================================
void CRank::SetUi(void)
{
	// �����̐���
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		m_infoVisualUi.apNum[nCount] = CNumber::Create(4, 0);
	}

	// ���ʂ̐���
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apRank[nCount] == nullptr)
		{
			m_infoVisualUi.apRank[nCount] = CObject2D::Create();

			if (m_infoVisualUi.apNum[nCount] != nullptr)
			{
				CTexture* pTexture = CTexture::GetInstance();

				if (pTexture != nullptr)
				{
					int nIdx = pTexture->Regist(RANK_TEX[nCount]);
					m_infoVisualUi.apRank[nCount]->SetIdxTexture(nIdx);
					m_infoVisualUi.apRank[nCount]->SetVtx();
				}
			}
		}
	}
}

//====================================================
// UI�̃��Z�b�g����
//=====================================================
void CRank::ReSetUi(void)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apRank[nCount] != nullptr)
		{
			m_infoVisualUi.apRank[nCount]->Uninit();
			m_infoVisualUi.apRank[nCount] = nullptr;
		}

		if (m_infoVisualUi.apNum[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->Uninit();
			m_infoVisualUi.apNum[nCount] = nullptr;
		}
	}
}

//=====================================================
// �ݒ菈��
//=====================================================
void CRank::Set(void)
{
	// �ϐ��錾
	bool bNewRank = false;
	int aNewScore[NUM_PLAYER] = {};

	// ��т̎擾
	CRecord* pRecord = CRecord::GetInstance();

	// �\�[�g
	Sort();

	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (pRecord != nullptr)
		{
			switch (m_typeGenre)
			{
			case CRecord::GENRE_TYPE_DESTROY:

				aNewScore[nCount] = pRecord->GetDestroy(nCount);

				break;

			case CRecord::GENRE_TYPE_MADMAN:

				aNewScore[nCount] = pRecord->GetMadman(nCount);

				break;
			case CRecord::GENRE_TYPE_ENGINEER:

				aNewScore[nCount] = pRecord->GetEngineer();

				break;
			}
		}
	}

	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (m_typeGenre == CRecord::GENRE_TYPE_ENGINEER)
		{
			if (aNewScore[nCount] < m_aRankScore[RANK::NUM_MAX - 1])
			{// �ŏ��l���z��������
				m_aRankScore[RANK::NUM_MAX - 1] = aNewScore[nCount];

				// �ă\�[�g
				Sort();

				bNewRank = true;

				break;
			}
		}
		else
		{
			if (aNewScore[nCount] > m_aRankScore[RANK::NUM_MAX - 1])
			{// �ŏ��l���z��������
				m_aRankScore[RANK::NUM_MAX - 1] = aNewScore[nCount];

				// �ă\�[�g
				Sort();

				bNewRank = true;
			}
		}
	}

	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		if (bNewRank == true)
		{
			for (int nCnt = 0; nCnt < RANK::NUM_MAX; nCnt++)
			{// �������l�ƍ��v����L�^��T��
				if (aNewScore[nCount] == m_aRankScore[nCnt] &&
					m_aRankScore[nCnt] != 0)
				{// �j���[���R�[�h�ԍ����L�^
					m_aUpdateIdx[nCount] = nCnt;
					m_aIsUpdate[nCount] = true;
				}
			}
		}
	}

	// �ۑ�����
#ifndef _DEBUG
	Save();
#endif
}

//=====================================================
// �����L���O��񃊃Z�b�g
//=====================================================
void CRank::Reset(void)
{
	//�O���t�@�C���ǂݍ���
	Load();
}

//=====================================================
// �����L���O�\�[�g
//=====================================================
void CRank::Sort(void)
{
	for (int nCntRanking = 0; nCntRanking < RANK::NUM_MAX - 1; nCntRanking++)
	{//�����L���O���\�[�g
		// ���[�̒l���ő�l�Ƃ���
		int nTop = nCntRanking;

		for (int nCount2 = nCntRanking + 1; nCount2 < RANK::NUM_MAX; nCount2++)
		{//���̒l�ƑΏۂ̒l���r
			if (m_typeGenre == CRecord::GENRE_TYPE_ENGINEER)
			{
				if (m_aRankScore[nTop] > m_aRankScore[nCount2])
				{//������r����������������������
					nTop = nCount2;
				}
			}
			else
			{
				if (m_aRankScore[nTop] < m_aRankScore[nCount2])
				{//������r����������������������
					nTop = nCount2;
				}
			}
		}

		//�v�f�̓���ւ�
		int nTemp = m_aRankScore[nCntRanking];
		m_aRankScore[nCntRanking] = m_aRankScore[nTop];
		m_aRankScore[nTop] = nTemp;
	}
}

//=====================================================
// �����L���O���ۑ�
//=====================================================
void CRank::Save(void)
{
#ifndef _DEBUG

	//�|�C���^�錾
	FILE* pFile;

	//�t�@�C�����J��
	pFile = fopen(m_pFileName, "wb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		//�o�C�i���t�@�C���ɏ�������
		fwrite(&m_aRankScore[0], sizeof(int), RANK::NUM_MAX, pFile);
		fwrite(&m_aRankFace[0], sizeof(int), RANK::NUM_MAX, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		assert(("�����L���O�ۑ��Ɏ��s", false));
	}
#endif
}

//=====================================================
//�����L���O���ǂݍ���
//=====================================================
void CRank::Load(void)
{
	//�|�C���^�錾
	FILE* pFile;

	//�t�@�C�����J��
	pFile = fopen(m_pFileName, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		//�o�C�i���t�@�C������ǂݍ���
		fread(&m_aRankScore[0], sizeof(int), RANK::NUM_MAX, pFile);
		fread(&m_aRankFace[0], sizeof(int), RANK::NUM_MAX, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		assert(("�����L���O�ǂݍ��݂Ɏ��s", false));
	}
}

//=====================================================
// �ʒu���ݒ菈��
//=====================================================
void CRank::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 posWhile, D3DXVECTOR3 posWhileRank)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apNum[nCount] != nullptr &&
			m_infoVisualUi.apRank[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->SetPosition(pos + (posWhile * (float)nCount));
			m_infoVisualUi.apRank[nCount]->SetPosition((pos + (posWhile * (float)nCount)) + posWhileRank);

			m_infoVisualUi.apNum[nCount]->SetValue(m_aRankScore[nCount], 4);
			m_infoVisualUi.apRank[nCount]->SetVtx();
		}
	}
}

//=====================================================
// �T�C�Y���ݒ菈��
//=====================================================
void CRank::SetSize(float width, float height, float widthRank, float heightRank)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apNum[nCount] != nullptr &&
			m_infoVisualUi.apRank[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->SetSizeAll(width, height);
			m_infoVisualUi.apRank[nCount]->SetSize(widthRank, heightRank);

			m_infoVisualUi.apNum[nCount]->SetValue(m_aRankScore[nCount], 4);
			m_infoVisualUi.apRank[nCount]->SetVtx();
		}
	}
}

//=====================================================
// �F���ݒ菈��
//=====================================================
void CRank::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apNum[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->SetColor(col);
		}
	}
}

//=====================================================
// �e�N�X�`�����ݒ菈��
//=====================================================
void CRank::SetTex(char* fileNameTex)
{
	for (int nCount = 0; nCount < RANK::NUM_MAX; nCount++)
	{
		if (m_infoVisualUi.apNum[nCount] != nullptr)
		{
			m_infoVisualUi.apNum[nCount]->SetTex(fileNameTex);
		}
	}
}

