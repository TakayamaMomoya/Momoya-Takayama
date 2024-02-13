//*****************************************************
//
// �r�[���̏���[beam.h]
// Author:���R����
//
//*****************************************************

#ifndef _BEAM_H_
#define _BEAM_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CAnim3D;
class CCollisionSphere;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBeam : public CObject
{
public:
	CBeam(int nPriority = 3);	// �R���X�g���N�^
	~CBeam();	// �f�X�g���N�^

	static CBeam *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot);
	void SetCol(D3DXCOLOR col);
	void SetMtx(D3DXMATRIX mtx) { m_info.mtxWorld = mtx; }
	void SetAnimSize(float fWidth, float fLength);
	// �c��ޑ��x�ݒ�
	void SetExpand(float fSpeed) { m_info.fSpeedExpand = fSpeed; }	
	// �k�ޑ��x�ݒ�
	void SetShrink(float fSpeed) { m_info.fSpeedShrink = fSpeed; }	
	// �L�т鑬�x�ݒ�
	void SetExtend(float fSpeed) { m_info.fSpeedExtend = fSpeed; }	

private:
	struct SInfo
	{
		CAnim3D *pAnim;	// �A�j���[�V�����G�t�F�N�g
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
		CCollisionSphere *pCollision;	// �����蔻��
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 pos;	// �ʒu
		float fWidthAnim;	// �A�j���[�V�����̕�
		float fLengthAnim;	// �A�j���[�V�����̒���
		float fWidthAnimDest;	// �A�j���[�V�����̖ڕW�̕�
		float fLengthAnimDest;	// �A�j���[�V�����̖ڕW�̒���
		float fSpeedExpand;	// �c��ޑ��x
		float fSpeedShrink;	// �k�ޑ��x
		float fSpeedExtend;	// �L�т鑬�x
		bool bFinish;	// �c��݂��������̔���
	};

	void ManageSize(void);
	void SetAnimVtx(void);

	SInfo m_info;
};

#endif
