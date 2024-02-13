//*****************************************************
//
// ��я����̃w�b�^�[[record.h]
// Author:����쏟
//
//*****************************************************

#ifndef _RECORD_H_
#define _RECORD_H_

//*****************************************************
// �C���N���[�h
//*****************************************************

#include "object.h"
#include "collision.h"
#include "number.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CRecord
{
public:

	enum GENRE_TYPE
	{
		GENRE_TYPE_DESTROY = 0,	// �j��i�G��|�������j
		GENRE_TYPE_MADMAN,		// ���l�i���������������j
		GENRE_TYPE_ENGINEER,	// �Z�p�ҁi�N���A���ԁj
		GENRE_TYPE_MAX
	};

	CRecord();	// �R���X�g���N�^
	~CRecord();	// �f�X�g���N�^

	static CRecord* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void ReSetData(void);

	void AddDestroy(int nIdx);
	void CheckDeathEnemy(CObject* pObj, int nIdx);
	void CheckDeathEnemyAll(CCollision** ppCollsionMissile, D3DXVECTOR3 posMissile, float fRadiusMissile, int nIdx);

	void AddMedman(int nIdx);

	void AllSort(void);

	static CRecord* GetInstance(void) { return m_pRecord; }

	int GetNumSuvived(void) { return m_nNumSuvived; }

	int GetDestroy(int nIdx) { return m_aInfo[nIdx].nDestroy; }
	int GetDestroyRank(int nIdx) { return m_aInfo[nIdx].nDestroyRank; }

	int GetMadman(int nIdx) { return m_aInfo[nIdx].nMadman; }
	int GetMadmanRank(int nIdx) { return m_aInfo[nIdx].nMadmanRank; }

	float GetEngineer(void) { return m_fGameTime; }
	int GetEngineerRank(void) { return m_nTimeRank; }

private:

	void Debug(void);
	void SetNumPlayer(void);

	struct SInfo
	{
		int nDestroy;		// �G�̔j��
		int nDestroyRank;	// �G�̔j�󐔂̏���
		int nMadman;		// ��������������
		int nMadmanRank;	// ���������������̏���
	};

	static CRecord* m_pRecord;	// ���g�̃|�C���^
	SInfo m_aInfo[NUM_PLAYER];	// ��я��
	int m_nNumSuvived;			// �����Ґ�
	float m_fGameTime;			// �Q�[���̃v���C����
	int m_nTimeRank;	// �^�C���̏���
};

#endif
