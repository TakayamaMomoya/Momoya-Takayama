//*****************************************************
//
// �S�[���^�C�}�[�̏���[goalGoalTimer.h]
// Author:���R����
//
//*****************************************************

#ifndef _GOALTIMER_H_
#define _GOALTIMER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CNumber;
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGoalTimer : public CObject
{
public:
	CGoalTimer();	// �R���X�g���N�^
	~CGoalTimer();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGoalTimer *Create(void);
	void AddGoalTimer(int nValue);
	float GetWidth(void) { return 0.0f; }	// �T�C�Y�擾
	float GetHeight(void) { return 0.0f; }	// �T�C�Y�擾
	float GetSecond(void) { return m_fSecond; }	// ���Ԏ擾
	void EnableStop(bool bStop) { m_bStop = bStop; }
	static CGoalTimer *GetInstance(void) { return m_pGoalTimer; }

private:
	CNumber *m_pObjSecond;	// �b�\���̃|�C���^
	CNumber *m_pObjDecimal;	// �����_�ȉ��\���̃|�C���^
	CUI *m_pPoint;	// �����_
	float m_fSecond;	// ���݂̎���(�b)
	static CGoalTimer *m_pGoalTimer;	// ���g�̃|�C���^
	bool m_bStop;	// �~�߂邩�ǂ���
	bool m_bSound;
};

#endif
