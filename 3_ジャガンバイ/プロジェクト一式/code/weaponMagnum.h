//*****************************************************
//
// �}�O�i���̏���[weaponMagnum.h]
// Author:���R����
//
//*****************************************************

#ifndef _MAGNUM_H_
#define _MAGNUM_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weapon.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMagnum : public CWeapon
{
public:
	CMagnum(int nPriority = 3);	// �R���X�g���N�^
	~CMagnum();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
};

#endif

