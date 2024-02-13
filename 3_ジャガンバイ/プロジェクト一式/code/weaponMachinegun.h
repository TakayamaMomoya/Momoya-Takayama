//*****************************************************
//
// �}�V���K���̏���[weaponMachinegun.h]
// Author:���R����
//
//*****************************************************

#ifndef _MACHINEGUN_H_
#define _MACHINEGUN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weapon.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMachinegun : public CWeapon
{
public:
	CMachinegun(int nPriority = 3);	// �R���X�g���N�^
	~CMachinegun();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
};

#endif

