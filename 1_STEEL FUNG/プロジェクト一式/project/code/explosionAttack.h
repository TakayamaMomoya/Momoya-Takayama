//*****************************************************
//
// �����U���̏���[explosionAttack.h]
// Author:���R����
//
//*****************************************************

#ifndef _ExplAttack_H_
#define _ExplAttack_H_

#include "Object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CAnim3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CExplAttack : public CObject
{
public:
	CExplAttack(int nPriority = 3);	// �R���X�g���N�^
	~CExplAttack();	// �f�X�g���N�^

	static CExplAttack *Create(D3DXVECTOR3 pos,float fRadius);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

private:
	float m_fRadius;	// ���a
	D3DXVECTOR3 m_pos;	// �ʒu
	CCollisionSphere *m_pCollision;	// �����蔻��
	CAnim3D *m_pAnim;	// �A�j���[�V�����G�t�F�N�g
};

#endif
