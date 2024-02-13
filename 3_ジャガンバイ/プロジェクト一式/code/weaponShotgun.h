//*****************************************************
//
// �V���b�g�K���̏���[weaponShotgun.h]
// Author:���R����
//
//*****************************************************

#ifndef _SHOTGUN_H_
#define _SHOTGUN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weapon.h"
#include "weaponManager.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CShotgun : public CWeapon
{
public:
	CShotgun(int nPriority = 3);	// �R���X�g���N�^
	~CShotgun();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
	float m_fAngleDiffuse;	// �g�U����p�x
	int m_nNumPellet;	// ��x�ɔ��˂���e�̐�
};

#endif

