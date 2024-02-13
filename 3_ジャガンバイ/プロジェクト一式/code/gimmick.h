//*****************************************************
//
// �A�C�e���̏���[item.h]
// Author:���R����
//
//*****************************************************

#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CBillboard;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGimmick : public CObjectX
{
public:
	CGimmick(int nPriority = 3);	// �R���X�g���N�^
	~CGimmick();	// �f�X�g���N�^

	static CGimmick *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void Interact(CObject* pObj);
	CCollisionSphere *GetCollisionSphere(void) { return m_pCollisionSphere; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	CBillboard *GetGuide(void) { return m_pInteract; }
	CGimmick *GetNext(void) { return m_pNext; }

protected:
	virtual void Exit(CObject* pObj) {};

private:
	CCollisionSphere *m_pCollisionSphere;
	CBillboard *m_pInteract;
	bool m_bEnable;	// �C���^���N�g�ł��邩�ǂ���

	CGimmick *m_pPrev;	// �O�̃A�h���X
	CGimmick *m_pNext;	// ���̃A�h���X
};

#endif

