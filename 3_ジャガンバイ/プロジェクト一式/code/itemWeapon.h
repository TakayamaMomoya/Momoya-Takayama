//*****************************************************
//
// ����A�C�e���̏���[ItemWeapon.h]
// Author:���R����
//
//*****************************************************

#ifndef _ITEMWEAPON_H_
#define _ITEMWEAPON_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gimmick.h"
#include "weapon.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject3D;
class CMeshCylinder;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CItemWeapon : public CGimmick
{
public:
	CItemWeapon(int nPriority = 3);	// �R���X�g���N�^
	~CItemWeapon();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CItemWeapon *Create(CWeapon::TYPE type);

private:
	struct SInfo
	{
		D3DXVECTOR3 posDest;	// �ڕW�ʒu
		float fScaleDest;	// �ڕW�X�P�[��
		float fTimer;	// �^�C�}�[
		CObject3D *pLight;	// ��
		CMeshCylinder* pCylinder;	// �~��
	};

	void Load(void);
	void Interact(CObject* pObj);
	void ApplyEffect(CPlayer *pPlayer);
	void ManageScale(void);
	void ManageTransform(void);
	void EffectScale(void);

	CWeapon::TYPE m_type;
	SInfo m_info;
};

#endif

