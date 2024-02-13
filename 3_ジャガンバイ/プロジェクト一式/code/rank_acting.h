
//*****************************************************
//
// �����L���O�̉��Z�̏���[rank_acting.h]
// Author:����쏟
//
//*****************************************************

#ifndef _RANK_ACTING_H_
#define _RANK_ACTING_H_

//*****************************************************
// �C���N���[�h
//*****************************************************

#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************

class CObject3D;
class CMotion;

//*****************************************************
// �萔��`
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CRankActing
{
public:

	// ���o�̎��
	enum ACTING_TYPE
	{
		ACTING_TYPE_MARCH = 0,	// �s�i
		ACTING_TYPE_RUN_AWAY,	// ������
		ACTING_TYPE_CHASE,		// �ǂ�������
		ACTING_TYPE_MAX
	};

	// ���҂̎��
	enum ACTOR_TYPE
	{
		ACTOR_TYPE_PLAYER_ONE = 0,	// �v���C���[01
		ACTOR_TYPE_PLAYER_TWO,		// �v���C���[02
		ACTOR_TYPE_PLAYER_THREE,	// �v���C���[03
		ACTOR_TYPE_PLAYER_FOUR,		// �v���C���[04
		ACTOR_TYPE_ENEMY_ONE,		// �G01
		ACTOR_TYPE_ENEMY_TWO,		// �G02
		ACTOR_TYPE_ENEMY_THREE,		// �G03
		ACTOR_TYPE_MAX
	};

	CRankActing();	// �R���X�g���N�^
	~CRankActing();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRankActing* Create(void);

	HRESULT InitObj(void);

	void SetObjActing(void);

	void CountActing(void);

private:

	struct SInfoVisualObj
	{
		CObject3D* pField;				// �n��
		CObject3D* pWall;				// ��
		CMotion* apModelActor[ACTOR_TYPE_MAX];	// �v���C���[
	};

	SInfoVisualObj m_infoVisualObj;		// �I�u�W�F�N�g�̌����ڊ֘A�̏���

	ACTING_TYPE m_typeActing;			// ���Z�̎��

	int m_nActingCnt;					// ���Z�J�E���g
};

#endif