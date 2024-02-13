//*****************************************************
//
// �S�[���̏���[goal.h]
// Author:���R����
//
//*****************************************************

#ifndef _GOAL_H_
#define _GOAL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"
#include <stdio.h>

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CObject3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGoal : public CObjectX
{
public:
	CGoal(int nPriority = 3);	// �R���X�g���N�^
	~CGoal();	// �f�X�g���N�^

	static CGoal *Create(void);
	static CGoal *GetInstance(void) { return m_pGoal; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DeadLine(void);
	bool IsFinish(void) { return m_bFinish; }
	void SetResult(void);
	float GetRadius(void) { return m_fRadius; }

private:
	void Load(void);
	void ApplyInfo(FILE *pFile,char *pTemp);
	void DetectPlayer(void);

	bool m_bFinish;	// �J�E���g�_�E�����I��������ǂ���
	float m_fRadius;	// ����̔��a
	CCollisionSphere *m_pCollisionGoal;	// �S�[������
	CObject3D *m_pArea;	// �͈͂̕\��
	static CGoal *m_pGoal;	// ���g�̃|�C���^
};

#endif

