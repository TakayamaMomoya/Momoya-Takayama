//*****************************************************
//
// ���[���K���̏���[weaponRailgun.h]
// Author:���R����
//
//*****************************************************

#ifndef _RAILGUN_H_
#define _RAILGUN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "weapon.h"

//*****************************************************
// �O����`
//*****************************************************
class CObject3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CRailgun : public CWeapon
{
public:
	struct SInfoRailgun
	{
		float fWidth;	// ����̕�
		float fLength;	// ����̒���
	};

	CRailgun(int nPriority = 3);	// �R���X�g���N�^
	~CRailgun();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Attack(void);
	void Draw(void);

private:
	void Shot(void);
	void SetVtx(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4);
	void ToggleEnable(bool bEnable);

	SInfoRailgun m_info;
	CObject3D *m_pRange;	// �͈͕\���̃|���S��
};

#endif

