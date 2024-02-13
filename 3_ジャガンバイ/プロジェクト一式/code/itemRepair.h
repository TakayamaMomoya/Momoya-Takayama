//*****************************************************
//
// �C���A�C�e���̏���[ItemRepair.h]
// Author:���R����
//
//*****************************************************

#ifndef _ITEMREPIR_H_
#define _ITEMREPIR_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gimmick.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;
class CFan3D;
class CBillboard;
class CAnim3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CItemRepair : public CGimmick
{
public:
	CItemRepair(int nPriority = 5);	// �R���X�g���N�^
	~CItemRepair();	// �f�X�g���N�^

	static CItemRepair *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Detatch(void);

private:
	void Load(void);
	void CreateGuide(void);
	void Interact(CObject* pObj);
	void CollisionRocket(void);
	void CheckPlayerAlive(void);
	void FollowPlayerHand(void);

	bool m_bInRocket;
	bool m_bSound;
	float m_fCntRepair;
	float m_fRepairCounter;
	CPlayer *m_pPlayer;
	CFan3D *m_pGauge;
	CBillboard *m_pInteract;
	CAnim3D *m_pGuide;
};

#endif

