//*****************************************************
//
// �����N�̏���[Rank.h]
// Author:����쏟
//
//*****************************************************

#ifndef _RANK_H_
#define _RANK_H_

//*****************************************************
// �C���N���[�h
//*****************************************************

#include "ranking.h"
#include "scene.h"
#include "object.h"
#include "record.h"

//*****************************************************
// �O���錾
//*****************************************************

class CObject2D;
class CNumber;

//*****************************************************
// �萔��`
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CRank : public CObject
{
public:

	CRank();	// �R���X�g���N�^
	~CRank();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRank* Create(CRecord::GENRE_TYPE typeGenre, const char* pFileName);

	void SetUi(void);
	void ReSetUi(void);

	void Set(void);
	void Reset(void);
	void Sort(void);
	void Save(void);
	void Load(void);

	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 posWhile, D3DXVECTOR3 posWhileRank);
	void SetSize(float width, float height, float widthRank, float heightRank);
	void SetColor(D3DXCOLOR col);
	void SetTex(char* fileNameTex);

	int GetUpdateIdx(int nIdx) { return m_aUpdateIdx[nIdx]; }
	bool GetIsUpdate(int nIdx) { return m_aIsUpdate[nIdx]; }
	CNumber* GetNum(int nIdx) { return m_infoVisualUi.apNum[nIdx]; }

private:

	struct SInfoVisualUi
	{
		CNumber* apNum[RANK::NUM_MAX];		// �����L���O�̐���
		CObject2D* apRank[RANK::NUM_MAX];	// �����L���O�̏���UI
	};

	SInfoVisualUi m_infoVisualUi;		// UI�̌����ڊ֘A�̏��

	CRecord::GENRE_TYPE m_typeGenre;	// ��ю�ނ̎��
	char* m_pFileName;					// �����L���O�t�@�C���̖��O

	int m_aRankScore[RANK::NUM_MAX];	// �����L���O�̒l
	int m_aRankFace[RANK::NUM_MAX];		// ��̔ԍ�
	int m_aUpdateIdx[NUM_PLAYER];		// �X�V���������L���O�̔ԍ�
	bool m_aIsUpdate[NUM_PLAYER];		// �����L���O�̍X�V�̗L��
};

#endif