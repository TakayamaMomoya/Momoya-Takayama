//*****************************************************
//
// �H��̏���[ghost.h]
// Author:���R����
//
//*****************************************************

#ifndef _GHOST_H_
#define _GHOST_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGhost : public CMotion
{
public:
	CGhost(int nPriority = 3);	// �R���X�g���N�^
	~CGhost();	// �f�X�g���N�^

	static CGhost *Create(int nIdxPlayer);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
};

#endif

