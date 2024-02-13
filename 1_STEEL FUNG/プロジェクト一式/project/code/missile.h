//*****************************************************
//
// �~�T�C���̏���[enemymissile.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMissile : public CEnemy
{// �v���C���[�Ɍ������G
public:
	CMissile();	// �R���X�g���N�^
	~CMissile();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMissile *Create(D3DXVECTOR3 pos);
	void Hit(float fDamage) { Death(); }
	void HitField(void) override;

private:
	void ChasePlayer(void);	// �ǐՏ���
	void Tilt(void);	// �X������
	void Death(void);	// ���S����

	int m_nDeathTimer;	// ���S�J�E���^�[
};

#endif
