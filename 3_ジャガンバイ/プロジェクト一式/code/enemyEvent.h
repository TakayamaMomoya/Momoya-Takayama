//*****************************************************
//
// �G�C�x���g�̏���[enemyEvent.h]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYEVENT_H_
#define _ENEMYEVENT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyEvent : public CObject
{
public:
	CEnemyEvent(int nPriority = 6);	// �R���X�g���N�^
	~CEnemyEvent();	// �f�X�g���N�^

	static CEnemyEvent *Create(float fLife = 10.0f,float fDelay = 10.0f);
	static CEnemyEvent *GetInstace(void) { return m_pEnemyEvent; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScale(float fScale) { m_fScaleTime = fScale; }

private:
	void StartEvent(void);

	float m_fScaleTime;	// �^�C���X�P�[��
	float m_fDelay;	// �C�x���g�����܂ł̃f�B���C
	float m_fLife;	// ����

	static CEnemyEvent *m_pEnemyEvent;	// ���g�̃|�C���^
};

#endif
