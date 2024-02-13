//*****************************************************
//
// �`�F�b�N�|�C���g�Ǘ��̏���[checkPointmanager.h]
// Author:���R����
//
//*****************************************************

#ifndef _CHECKPOINTMANAGER_H_
#define _CHECKPOINTMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CCheckPointManager : public CObject
{
public:
	CCheckPointManager();	// �R���X�g���N�^
	~CCheckPointManager();	// �f�X�g���N�^
	
	static CCheckPointManager *Create(void);
	static CCheckPointManager *GetInstance(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetNumCheckPoint(void) { return m_nNumCheckPoint; }

private:

	void Load(void);
	void LoadInfoCheckPoint(void);
	void TransBossBattle(void);

	int m_nProgress;	// ���݂̐i�s��
	int m_nNumCheckPoint;	// �`�F�b�N�|�C���g�̐�
	D3DXVECTOR3 *m_pPosCheckPoint;	// �`�F�b�N�|�C���g�̍��W���
	CObject2D *m_pCursor;	// �J�[�\���̃|�C���^
	static CCheckPointManager *m_pCheckPointManager;	// ���g�̃|�C���^
};

#endif