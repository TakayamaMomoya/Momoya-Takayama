//*****************************************************
//
// �~�j�K���̏���[weaponMinigun.h]
// Author:���R����
//
//*****************************************************

#ifndef _MINIGUN_H_
#define _MINIGUN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weapon.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMinigun : public CWeapon
{
public:
	CMinigun(int nPriority = 3);	// �R���X�g���N�^
	~CMinigun();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
};

#endif

