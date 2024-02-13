//*****************************************************
//
// �������[�V�����̏���[motion_div.h]
// Author:����쏟
//
//*****************************************************

#ifndef _MOTION_DIV_H_
#define _MOTION_DIV_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "parts.h"
#include "object.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_PARTS	(20)	// �p�[�c�̍ő吔
#define MAX_MOTION	(40)	// ���[�V�����̍ő吔

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMotionDiv : public CObject
{
public:

	enum DIV_BODY
	{// ����������
		DIV_BODY_LOWER = 0,	// �����g
		DIV_BODY_UPPER,		// �㔼�g
		DIV_BODY_MAX
	};

	//�L�[�̍\����
	typedef struct
	{
		float fPosX;				//�ʒuX
		float fPosY;				//�ʒuY
		float fPosZ;				//�ʒuZ
		float fRotX;				//����X
		float fRotY;				//����Y
		float fRotZ;				//����Z
	}KEY;

	//�L�[���\����
	typedef struct
	{
		int nFrame;					//�Đ��t���[��
		KEY aKey[MAX_PARTS];			//�e���f���̃L�[�v�f
	}KEY_INFO;

	typedef struct
	{// �p�[�e�B�N�����\����
		int nKey;	//��������L�[
		int nFrame;	//��������t���[��
		int nType;	// ���
		D3DXVECTOR3 offset;	// �I�t�Z�b�g�ʒu
		int nIdxParent;	// �e�ƂȂ�p�[�c�̔ԍ�
	}PARTICLE_INFO;

	typedef struct
	{// �����蔻����\����
		int nKey;	//��������L�[
		int nFrame;	//��������t���[��
		D3DXVECTOR3 offset;	// �I�t�Z�b�g�ʒu
		int nIdxParent;	// �e�ƂȂ�p�[�c�̔ԍ�
	}COLLISION_INFO;

	//���[�V�������̍\����
	typedef struct
	{
		bool bLoop;						//���[�v���邩�ǂ���
		int nNumKey;					//�L�[�̑���
		KEY_INFO aKeyInfo[MAX_PARTS];	//�L�[���
		int nNumParticle;	// �p�[�e�B�N���̐�
		int nNumCollision;	// �����蔻��̐�
		PARTICLE_INFO* pParticle;	// �p�[�e�B�N���̃|�C���^
		COLLISION_INFO* pCollision;	// �����蔻��̃|�C���^
	}MOTION_INFO;

	// �p�[�c�̍\����
	typedef struct
	{
		int nIdx;
		int nIdxParent;
		CParts* pParts;
	}Parts;

	// �����o�֐�
	CMotionDiv(int nPriority = 5);	// �R���X�g���N�^
	~CMotionDiv();	// �f�X�g���N�^
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(char* pPathLower, char* pPathUpper);
	void MultiplyMtx(void);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// �ݒ菈��
	void SetPosShadow(D3DXVECTOR3 pos) { m_posShadow = pos; }	// �ݒ菈��
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }	// �ݒ菈��
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// �擾����
	D3DXVECTOR3* GetPosAddress(void) { return &m_pos; }	// �擾����
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	D3DXVECTOR3 GetMtxPos(int nNum, int nIdx);
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }	// �擾����
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMotion(int nNum, int nMotionType);
	int GetMotion(int nNum) { return m_aMotionType[nNum]; }
	void SetKeyOld(void);
	static CMotionDiv* Create(char* pPathLower, char* pPathUpper);
	Parts* GetParts(int nNum,int nIdx) { return m_apParts[nNum][nIdx]; }
	MOTION_INFO GetMotionInfo(int nNum, int nIdx) { return m_aMotionInfo[nNum][nIdx]; }
	D3DXMATRIX* GetMatrix(void) { return &m_mtxWorld; }
	bool IsFinish(int nNum) { return m_aFinish[nNum]; }
	void SetMatrix(void);
	float GetRadiusMax(int nNum);
	void SetAfterImage(int nNum, D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f }, int m_nLife = 10);
	int GetKey(int nNum) { return m_aKey[nNum]; }
	int GetFrame(int nNum) { return m_aCounterMotion[nNum]; }
	void SetAllCol(D3DXCOLOR col);
	void ResetAllCol(void);
	void InitPose(int nMotion);
	void EnableShadow(bool bShadow) { m_bShadow = bShadow; }
	void EnableIndependent(bool bInde) { m_bInde = bInde; }

private:
	Parts* m_apParts[DIV_BODY_MAX][MAX_PARTS];				// �p�[�c�̍\����
	MOTION_INFO m_aMotionInfo[DIV_BODY_MAX][MAX_MOTION];	// ���[�V�������̍\����
	KEY m_aKeyOld[DIV_BODY_MAX][MAX_PARTS];					// �O��̃L�[���̍\����
	int m_aNumMotion[DIV_BODY_MAX];			// ���[�V�����̑���
	int m_aMotionType[DIV_BODY_MAX];		// ���[�V�����̎��
	int m_aMotionTypeOld[DIV_BODY_MAX];		// �O��̃��[�V�����̎��
	int m_aFrame[DIV_BODY_MAX];				// �t���[����
	bool m_aLoopMotion[DIV_BODY_MAX];		// ���[�v���邩�ǂ���
	int m_aNumKey[DIV_BODY_MAX];			// �L�[�̑���
	int m_aKey[DIV_BODY_MAX];				// ���݂̃L�[
	int m_aCounterMotion[DIV_BODY_MAX];		// ���[�V�����J�E���^�[
	int m_aNumParts[DIV_BODY_MAX];			// �p�[�c�̐�
	bool m_aFinish[DIV_BODY_MAX];			// ���[�V�������I��������ǂ���

	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_posShadow;	// �e�̈ʒu
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXVECTOR3 m_rot;	//����
	D3DXMATRIX m_mtxWorld;	// �}�g���b�N�X
	bool m_bShadow;	// �e��`�悷�邩�ǂ���
	bool m_bInde;	// �������Ă��邩�ǂ���
};

#endif