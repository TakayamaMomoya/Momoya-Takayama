//*****************************************************
//
// �~�T�C������[missile.h]
// Author:���R����
//
//*****************************************************

#ifndef _MISSILE_H_
#define _MISSILE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "collision.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_HIT	(54)	// �ő�Ŋo�����閽�������I�u�W�F�N�g

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class COrbit;
class CCollisionSphere;
class CObjectX;
class CObject3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMissile : public CObject
{
public:
	CMissile(int nPriority = 6);	// �R���X�g���N�^
	~CMissile();	// �f�X�g���N�^

	static CMissile *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_info.posOld; }
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetMove(D3DXVECTOR3 move) { m_info.move = move; }
	void SetRot(D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetDamage(float fDamage) { m_info.fDamage = fDamage; }

	void SetIdxPlayer(int nIdx) { m_info.nIdxPlayer = nIdx; }

private:
	struct SInfoVisual
	{
		CObjectX *pMissile;	// �~�T�C���{�̂̌�����
		CObject3D *pBackLight;	// ����̌�
	};
	struct SInfo
	{
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
		D3DXVECTOR3 move;	// �ړ���
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posOld;	// �O��̈ʒu
		float fLife;	// ����
		float fSpeed;	// ���x
		CCollisionSphere *pCollisionSphere;	// ���̓����蔻��
		float fDamage;	// �^�_���[�W
		float fRadiusExplosion;	// �������a
		int nIdxPlayer;			// �v���C���[�ԍ�
	};
	void CreateVisual(void);
	void ManageMove(void);
	void FollowVisual(void);
	void Death(void);
	void DeleteVisual(void);

	SInfo m_info;	// ���
	SInfoVisual m_infoVisual;	// �����ڂ̏��
	static int m_nNumAll;	// ����
};

#endif