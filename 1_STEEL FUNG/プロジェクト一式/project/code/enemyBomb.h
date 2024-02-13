//*****************************************************
//
// ���e�G�̏���[enemyBomb.h]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYBOMB_H_
#define _ENEMYBOMB_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyBomb : public CEnemy
{
public:
	CEnemyBomb();	// �R���X�g���N�^
	~CEnemyBomb();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void HitField(void) override;
	void Death(void);

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// �ҋ@���[�V����
		MOTION_MAX
	};
};

#endif
