//*****************************************************
//
// �R���e�i�̏���[container.h]
// Author:���R����
//
//*****************************************************

#ifndef _CONTAINER_H_
#define _CONTAINER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gimmick.h"

//*****************************************************
// �O���錾
//*****************************************************
class CItemWeapon;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CContainer : public CGimmick
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_OPEN,	// �J���Ă�����
		STATE_END,	// �I��
		STATE_MAX
	};
public:
	CContainer(int nPriority = 3);	// �R���X�g���N�^
	~CContainer();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	static CContainer *Create(void);

	STATE GetState(void) { return m_info.state; }

private:
	
	struct SInfo
	{// ���
		CItemWeapon* pWeapon;	// ����A�C�e���̃|�C���^
		CObjectX* pCap;	// �W�̃I�u�W�F�N�g
		STATE state;	// ���
		float fTimerDeath;	// ���S�܂ł̃^�C�}�[
	};

	void Load(void);
	void UpdateOpen(void);
	void Interact(CObject* pObj);
	void Open(void);
	int WeaponRand(void);

	SInfo m_info;
};

#endif

