//*****************************************************
//
// �ؔ��̏���[box.h]
// Author:���R����
//
//*****************************************************

#ifndef _BOX_H_
#define _BOX_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CCollisionCube;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBox : public CObjectX
{
public:
	enum TYPE
	{
		TYPE_NONE = 0,	// ���ł��Ȃ����
		TYPE_REPAIR,	// �C���A�C�e�����o�Ă���
		TYPE_RANDOM,	// �����_���ɕ����o�Ă���
		TYPE_MAX
	};

	CBox(int nPriority = 3);	// �R���X�g���N�^
	~CBox();	// �f�X�g���N�^

	static CBox *Create(TYPE type = TYPE_RANDOM);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);
	void SetType(TYPE type);
	CCollisionSphere *GetCollisionSphere(void) { return m_pCollisionSphere; }
	CCollisionCube *GetCollisionCube(void) { return m_pCollisionCube; }

private:
	CCollisionSphere *m_pCollisionSphere;	// �����蔻��
	CCollisionCube *m_pCollisionCube;	// ��`�̓����蔻��
	TYPE m_type;	// ���
};

#endif

