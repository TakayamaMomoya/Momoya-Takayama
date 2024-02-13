//*****************************************************
//
// �����L���O�̏���[Ranking.h]
// Author:����쏟
//
//*****************************************************

#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************
// �C���N���[�h
//*****************************************************

#include "scene.h"

#include "record.h"

//*****************************************************
// �O���錾
//*****************************************************

class CObject2D;
class CNumber;

class CRank;
class CRankActing;

//*****************************************************
// �萔��`
//*****************************************************
namespace RANK
{
	const int NUM_MAX = 3;	// �ő吔
}

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CRanking : public CScene
{
public:
	
	CRanking();	// �R���X�g���N�^
	~CRanking();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT InitUi(void);
	HRESULT InitRank(void);

	void SetUiRecord(void);
	void SetRank(void);

	void UpdateUiRecord(void);
	void UpdateNum(void);

private:

	struct SInfoVisualUi
	{
		CObject2D* pRecordText;		// ���R�[�h�̃e�L�X�g
		CObject2D* pGenreText;		// ��ނ̃e�L�X�g
		CObject2D* pGenreIcon;		// ��ނ̃A�C�R�� 

		CObject2D* apFace[NUM_PLAYER];	// �e�v���C���[�̊�
		CNumber* apNumber[NUM_PLAYER];	// �e�v���C���[�̐��
	};

	SInfoVisualUi m_infoVisualUi;				// UI�̌����ڊ֘A�̏��
	CRank* m_apRank[CRecord::GENRE_TYPE_MAX];	// �����N�̏��
	CRankActing* m_pRankActing;					// ���Z�̏��

	CRecord::GENRE_TYPE m_typeGenre;	// ��т̎��

	int m_nGenreCnt;					// ��ރJ�E���g

	bool m_aIsAlphaChange[CRecord::GENRE_TYPE_MAX][NUM_PLAYER];	// ���l�̕ω�
};

#endif